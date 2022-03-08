/*  03/02/2022
  METAR Reporting with LEDs and Local WEB SERVER
  In Memory of F. Hugh Magee, brother of John Magee author of poem HIGH FLIGHT.
  https://en.wikipedia.org/wiki/John_Gillespie_Magee_Jr.
  https://youtu.be/Yg61_kyG2zE    HomebuiltHELP; The video that started me on this project.
  https://youtu.be/xPlN_Tk3VLQ    Getting Started with ESP32 video from DroneBot Workshop.
  https://www.aviationweather.gov/docs/metar/Stations.txt    List of ALL Stations Codes.
  https://aeronav.faa.gov/visual/01-27-2022/PDFs/     FAA Wall Chart downloads and then edit to suit.

  REMARKS/COMMENTS:
  Created by John Pipe with help from David Bird.
  FULLY Configurable for YOUR application. (See the list of all Stations Codes, link above).
  No of Airports = 100 tested 6/24/20 (60+ possible with external power).
  When Error, successfully recovers. If not, press "RESET" button on ESP32.
  The code is stored on the ESP32 Development Board 2.4GHz WiFi+Bluetooth Dual Mode,
  once the code is downloaded, it will run by it's self and will automatically restart, even after a power off.
  ESP32 requires internet access (Network or Hot Spot) to download METARS.

  A computer, with FREE software https://www.arduino.cc/en/Main/Software , is needed to initially configure,
  download software and (optionally) display any program messages.
  (Watch Getting Started with ESP32 video from DroneBot Workshop, link above).

  Updates METARS approximately every six minutes, so nearly REAL TIME data, from AVIATIONWEATHER.GOV.
  A set of WS2812 LEDS show all station CATEGORIES (similar to the HomebuiltHELP video, link above).
  Then cycles through all the stations and flashes individually for:
  Wind Gusts(Cyan)[suspendable], Precipitation(Green/White), Ice(Blue), Other(Yellow) and Significatant Change(Orange).
  Then displays "RAINBOW" for all stations, for Visibility [Red Orange Pink/White], Wind Speed Gradient [Cyan],
  Temperature Gradient [Blue Green Yellow Orange Red] and Altimeter Pressure Gradient [Blue Purple].

  Recommended, VIEWABLE with a cell phone or computer connected to the SAME network:
  SUMMARY html gives a colorful overview and
  STATION html shows DECODED METAR information and much MORE.  (See Below for Improvements)

  NOTE: To view these, you need the http address which is shown at start up if the Serial Monitor is swiched on.
  or Click Test Link  http://metar.local/

  Makes a GREAT Christmas Tree Chain of Lights, TOO (and a Good Conversation Piece).

  MADE THINGS A LITTLE BETTER, BUG FIXES, IMPROVEMENTS, REPAIRS TO TIME-SPACE CONTINUUM, ETC, ETC.
  Includes: Decoded Metar, Current UTC Time, Temperature in Deg F, Elevation Ft, Estimated Density Altitude Ft

  Modified Significant Weather to include Cloud Cover, RVR & Weather 12/31
  Changed to a TIMED 6 Minute METAR read and update 01/07
  Added for Ice and Hail (Blue) 01/30
  Added Capability to select ANY Airport Code 02/02
  Added Summary to HTML 03/04
  Cleaned up Update_Time & loop 03/08
  Added User 03/10
  Added Cloud_base Change Arrows 03/13
  Added Alt Pressure Change Arrows 03/13
  Added Yellow Misc Weather 03/24
  Added Observation Time 04/01
  Added Orange Info Changes 04/05
  Cleaned up Parse_Metar 04/14
  Added Wind Changes 04/30
  Added Pressure Display 05/06
  Modified Visibility Display 05/15
  Modified Variable Types 05/15
  Tweaked Rainbow Displays 05/21
  Dual Core: Main_Loop Task1 Core0; Go_Server Task2 Core1 05/24
  Modified Server Update Time 05/30
  More little tweaks 06/01
  Messing with memory storage 06/15
  Modified to HTTPS 06/23
  REMOVED from remark "Welcome User" 07/11
  Modified the URL address 10/03
  Changed Temperature Display Colors 10/29
  Address to the server with http://metar.local/ 10/29
  Added Remarks 02/11/21
  Modified Dictinary 04/14/21
  Modified HTML 04/29/21
  Fixed rem pointer error 05/18/21
  Removed Flashing Sig Weather 07/12/21
  Added debug print of remarks 07/30/21
  More Readable, Modified Dictinary 08/27/21
  More Reliable, More Modified Dictinary 01/19/22
  Modified to Summary Page to Jump to a Station 03/2/22
*/

#include <Arduino.h>
#include <FastLED.h>
#include <WiFi.h>
#include <WiFiMulti.h>
WiFiMulti wifiMulti;
#include <HTTPClient.h>
#include "ESPmDNS.h"

WiFiServer server(80);   // Set web server port number to 80

//  Configure Network
//const char*      ssid = "xxxxxx";          // your network SSID (name)
//const char*  password = "xxxxxxxx";        // your network password

const char*        ssid = "name";           // your network SSID (name)
const char*    password = "password";       // your network password

// Setup from Time Server
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;                // UTC Time
const int   daylightOffset_sec = 0;           // UTC Time
struct tm timeinfo;                           // Time String "%A, %B %d %Y %H:%M:%S"


// To get Station DATA used for MetarData in "GetData" Routine
// Test link: https://www.aviationweather.gov/adds/dataserver_current/httpparam?datasource=metars&requestType=retrieve&format=xml&mostRecentForEachStation=constraint&hoursBeforeNow=1.25&stationString=KFLL,KFXE
String    host = "https://aviationweather.gov";
String    urlb = "/adds/dataserver_current/httpparam?datasource=metars&requestType=retrieve&format=xml&mostRecentForEachStation=constraint&hoursBeforeNow=1.25&stationString=";

// To get Station NAME and Information in "Go_Server" and "Decode_Name" Routines
// Test link: https://aviationweather.gov/adds/dataserver_current/httpparam?dataSource=Stations&requestType=retrieve&format=xml&stationString=KFLL
String    urls = "/adds/dataserver_current/httpparam?dataSource=Stations&requestType=retrieve&format=xml&stationString=";

// To Invoke HTML Display/Web Page after downloading code
// Test link: http://metar.local/summary/


// Set Up LEDS
#define No_Stations          52      // Number of Stations also Number of LEDs
#define NUM_LEDS    No_Stations      // Number of LEDs
#define DATA_PIN              5      // Connect to pin D5/P5 with 330 to 500 Ohm Resistor
#define LED_TYPE         WS2812      // WS2811 or WS2812 or NEOPIXEL
#define COLOR_ORDER         GRB      // WS2811 are RGB or WS2812 are GRB or NEOPIXEL are CRGB
#define BRIGHTNESS           20      // Master LED Brightness (<12=Dim 20=ok >20=Too Bright/Much Power)
#define FRAMES_PER_SECOND   120
CRGB leds[NUM_LEDS];                 // Color Order for LEDs


// Define STATIONS Variables
std::vector<String> PROGMEM Stations {  //   << Set Up   - Do NOT change this line
  "NULL, STATION NAME         ",        // 0 << Reserved - Do NOT change this line
  "KCHA, CHATTANOOGA, TN      ",        // 1 << START modifying from here
  "KRMG, ROME, GA             ",        // 2  Order of LEDs; NULL for no airport
  "KVPC, CARTERSVILLE, GA     ",        // 3
  "KATL, ATLANTA, GA          ",        // 4  First FIVE Characters are REQUIRED !!
  "KCTJ, CARROLTON, GA        ",        // 5
  "KLGC, LA GRANGE, GA        ",        // 6  Over type your Station Code and Station name
  "KCSG, COLUMBUS, GA         ",        // 7  and include the "quotes" and the "commas"
  "KMCN, MACON, GA            ",        // 8  Padding after station name are not necessary.
  "KCKF, CORDELLE, GA         ",        // 9
  "KABY, ALBANY, GA           ",        // 10 Note: SKYVECTOR.COM is good for locating METAR Reporting Stations
  "KTLH, TALLAHASSEE, FL      ",        // 11
  "KVLD, VALDOSTA, GA         ",        // 12
  "KAYS, WAYCROSS, GA         ",        // 13
  "KJAX, JACKSONVILLE, FL     ",        // 14
  "KBQK, BRUNSWICK, GA        ",        // 15
  "KSAV, SAVANNAH, GA         ",        // 16
  "KTBR, STATESBORO, GA       ",        // 17
  "KAGS, AUGUSTA, GA          ",        // 18
  "KAHN, ATHENS, GA           ",        // 19
  "KCEU, CLEMSON, GA          ",        // 20
  "KJES, JESUP (my home town), GA",     // 21
  "KBHC, BAXLEY, GA           ",        // 22
  "KAZE, HAZLEHURST, GA       ",        // 23
  "KCAE, COLUMBIA, SC         ",        // 24
  "KVDI, VIDALIA RGNL, GA     ",        // 25
  "KAFP, ANSON CO, NC         ",        // 26
  "KBDU, BOULDER, CO          ",        // 27
  "KCVG, CINCINNATI, OH       ",        // 28
  "KBWI, WASHINGTON, DC       ",        // 29
  "KORD, CHICAGO O'HARE, IL   ",        // 30
  "KMEM, MEMPHIS, TN          ",        // 31
  "KMSY, NEW ORLEANS, LA      ",        // 32
  "KSDF, LOUISVILLE, KY       ",        // 33
  "KBOS, BOSTON, MA           ",        // 34
  "KCLT, CHARLOTTE, NC        ",        // 35
  "KBNA, NASHVILLE, TN        ",        // 36
  "EGPN, DUNDEE, SCOTLAND     ",        // 37
  "EGPH, EDINBURGH, SCOTLAND  ",        // 38
  "EGPE, INVERNESS, SCOTLAND  ",        // 39
  "EGLL, LONDON HEATHROW, UK  ",        // 40
  "EGKK, LONDON GATWICK, UK   ",        // 41
  "EGCC, MANCHESTER, UK       ",        // 42
  "EGDM, BOSCOMBE DOWN, UK    ",        // 43
  "EGYP, MOUNT PLEASANT, FK   ",        // 44
  "EHAM, AMSTERDAM SCHIPHOL   ",        // 45
  "LFSB, BASEL, SWITZERLAND   ",        // 46
  "KORL, ORLANDO, FL          ",        // 47
  "KPBI, WEST PALM BEACH, FL  ",        // 48
  "KFXE, FT LAUDERDALE EXE, FL",        // 49
  "KFLL, FT LAUDERDALE INT, FL",        // 50
  "KHWO, NTH PERRY, FL        ",        // 51
  "KMIA, MIAMI, FL            ",        // 52 (last airport)
  "KEYW, KEY WEST, FL         ",        // 53 (extras)
  "KJAX, JACKSONVILLE, FL     ",        // 54
  "KTIX, TITUSVILLE, FL       ",        // 55
  "KDAB, DAYTONA BEACH, FL    ",        // 56
  "KTBR, STATESBORO, GA       ",        // 57
  "KAGS, AUGUSTA, GA          ",        // 58
  "KAHN, ATHENS, GA           ",        // 59
  "KCEU, CLEMSON, GA          ",        // 60
};                                      // << Do NOT change this line

PROGMEM String StationMetar[No_Stations + 1];  // Station Metar
PROGMEM String      Remark[No_Stations + 1];   // Remark
PROGMEM String Sig_Weather[No_Stations + 1];   // Significant Weather
PROGMEM float         temp[No_Stations + 1];   // Temperature deg C
PROGMEM float        dewpt[No_Stations + 1];   // Dew point deg C
PROGMEM String        Wind[No_Stations + 1];   // Wind speed
PROGMEM String        wDir[No_Stations + 1];   // Wind direction
PROGMEM int       old_wDir[No_Stations + 1];   // old Wind direction
PROGMEM float        visab[No_Stations + 1];   // Visibility
PROGMEM String         Sky[No_Stations + 1];   // Sky_cover
PROGMEM int new_cloud_base[No_Stations + 1];   // New Cloud Base
PROGMEM int old_cloud_base[No_Stations + 1];   // Old Cloud Base
PROGMEM float      seapres[No_Stations + 1];   // Sea Level Pressure
PROGMEM float        altim[No_Stations + 1];   // Altimeter setting
PROGMEM float    old_altim[No_Stations + 1];   // old altimeter setting
PROGMEM float    elevation[No_Stations + 1];   // Elevation setting
PROGMEM String    Category[No_Stations + 1];   // NULL   VFR    MVFR   IFR    LIFR
//..............................................Black  Green   Blue   Red    Magenta

#define LED_BUILTIN 2         // ON Board LED GPIO 2
PROGMEM String MetarData;     // Raw METAR data
String Clock;                 // Clock  "HH:MM"
byte Hour;                    // Latest Hour
byte Minute;                  // Latest Minute
String Last_Up_Time;          // Last Update Time  "HH:MM"
byte Last_Up_Min;             // Last Update Minute
byte Group_of_Stations = 18;  // Get a Group of <18 Stations at a time
byte Update_Interval = 6;     // Updates Data every 6 Minutes (Don't overload AVIATIONWEATHER.GOV)
byte Count_Down;              // Count to Next Update
byte Station_Num = 1;         // Station # for Server - flash button
int httpCode;                 // Error Code
byte Comms_Flag = 0;          // Communication Flag
String local_hwaddr;          // WiFi local hardware Address
String local_swaddr;          // WiFi local software Address
TaskHandle_t Task1;           // Main_Loop, Core 0
TaskHandle_t Task2;           // Go_Server, Core 1
const char* ServerName = "metar";      // Logical Address http://metar.local/summary/


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);     // the onboard LED
  Serial.begin(115200);
  delay(4000);         // Time to press "Clear output" in Serial Monitor
  Serial.println(F("\nMETAR Reporting with LEDs and Local WEB SERVER"));
  Serial.println("Program ~ " + Filename() + "\n");
  Init_LEDS();         // Initialize LEDs
  if (String(ssid) == "iPhone")  Serial.println(F("** If iPhone DOESN'T CONNECT:  Select HOT SPOT on iPhone **"));
  digitalWrite(LED_BUILTIN, HIGH);  //ON
  Serial.print("WiFi Connecting to " + String(ssid) + " ");
  // Initializes the WiFi library's network settings.
  WiFi.begin(ssid, password);
  // CONNECT to WiFi network:
  while (WiFi.status() != WL_CONNECTED)   {
    delay(300);    // Wait a little bit
    Serial.print(".");
  }
  Serial.println(F(" ~ Connected OK"));
  // Print the Signal Strength:
  long rssi = WiFi.RSSI() + 100;
  Serial.print("Signal Strength = " + String(rssi));
  if (rssi > 50)  Serial.println(F(" (>50 - Good)"));  else   Serial.println(F(" (Could be Better)"));
  digitalWrite(LED_BUILTIN, LOW);   //OFF
  Serial.println(F("*******************************************"));
  Serial.println("To View Decoded Station METARs from a Computer or \nCell Phone connected to the " + String(ssid) + " WiFi Network.");

  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(ssid, password);

  if (!MDNS.begin(ServerName)) {     // Start mDNS with ServerName
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);                   // Stay here
    }
  }
  local_hwaddr = WiFi.localIP().toString();
  Serial.println("MDNS started ~\tUse IP Address\t: " + local_hwaddr);
  local_swaddr = "http://" + String(ServerName) + ".local/summary/";
  Serial.println("\t\tOr Use Address\t: " + local_swaddr);

  digitalWrite(LED_BUILTIN, HIGH);  //ON
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);   // Get UTC Time from Server
  Update_Time();          // Set Up Time : Hour & Minute
  Serial.print(F("Date & Time :\t"));
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M - UTC");
  digitalWrite(LED_BUILTIN, LOW);   //OFF

  //Task1 = Main_Loop() function, with priority 1 and executed on core 0 for Getting Data and Display Leds
  xTaskCreatePinnedToCore( Main_Loop, "Task1", 10000, NULL, 1, &Task1, 0);
  delay(200);          // Wait a smidgen
  
  //Task2 = Go_Server() function, with priority 1 and executed on core 1 for Creating Web Pages and Handling Requests
  xTaskCreatePinnedToCore( Go_Server, "Task2", 10000, NULL, 1, &Task2, 1);
  delay(200);          // Wait a smidgen
  
  Serial.println(F("Dual Core Initialized"));
  server.begin();      // Start the web server
  Serial.println(F("Web Server Started"));
  Serial.println(F("Set Up Complete"));
  Serial.println(F("*******************************************"));
}

String Filename() {
  return String(__FILE__).substring(String(__FILE__).lastIndexOf("\\") + 1);
}

void loop() {
  // Nothing to do here
}


//  *********** Main_Loop  TASK1 for Getting Data and Display Leds
void Main_Loop( void * pvParameters ) {
  for (;;) {
    Last_Up_Time = Clock;
    Last_Up_Min = Minute;
    Update_Time();                   // Update CurrentTime : Hour & Minute

    if (Last_Up_Min + Update_Interval > 60)   Last_Up_Min = 60 - Update_Interval;
    Count_Down = Last_Up_Min + Update_Interval - Minute;

    if (Comms_Flag > 0)Serial.println(Clock + "   WAITING to Get Metar Update  -  Comms In Use");
    int count = 0;
    while (Comms_Flag > 0) {    // Checking Communication Flag 1=Active
      delay(2000);
      count++;                  // loop for 200 seconds
      if (count > 100)  return;
    }

    Comms_Flag = 1;                  // Communication Flag 1=Active
    GetAllMetars();                  // Get All Metars and Display Categories
    Comms_Flag = 0;                  // Communication Flag 0=Reset
    Serial.println(Clock + "\tMetar Data Updated" + "\tNext Update in " + String(Count_Down) + " Mins");

    while (Count_Down > 0)    {
      Display_Metar_LEDS ();          // Display Station Metar/Show Loops
      Update_Time();                  // Update CurrentTime : Hour & Minute
      Count_Down = Last_Up_Min + Update_Interval - Minute;
      if (Count_Down > Update_Interval)   Count_Down = 0;
    }
  }
}

// *********** Set/Get Current Time Clock, Hour, Minute
void Update_Time() {
  if (!getLocalTime(&timeinfo)) {
    Serial.println("\t\t\t**  FAILED to Get Time  **\n");
    Network_Status ();            // WiFi Network Error
  }
  char TimeChar[6];
  strftime(TimeChar, sizeof(TimeChar), "%H:%M", &timeinfo);
  Clock  = TimeChar;
  Hour   = Clock.substring(0, 2).toInt();
  Minute = Clock.substring(3, 5).toInt();
}


// *********** Initialize LEDs
void Init_LEDS() {
  // Set up the strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, No_Stations).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);        // For NEOPIXELS
  // Set master brightness control (<12=Dim 20=ok >20=Too Bright)
  FastLED.setBrightness(BRIGHTNESS);
  // Set all leds to Black
  fill_solid(leds, No_Stations, CRGB::Black);
  FastLED.show();
  delay(100);    // Wait a smidgen
  Serial.println("LEDs Initialized for No_Stations = " + String(No_Stations));
}


// *********** GET All Metars in Chunks
void GetAllMetars() {
  //                                     Get a Group of Stations <20 at a time
  for (byte j = 0; j < No_Stations; j = j + 1 + Group_of_Stations) {
    byte Start  = j;
    byte Finish = Start + Group_of_Stations;
    if (Finish > No_Stations)  Finish = No_Stations;
    String url = "";
    for (byte i = Start; i <= Finish; i++) {
      String station = Stations[i].substring(0, 5);
      url = url + String(station);
    }
    byte len = url.length();
    url = url.substring(0, len - 1);  // Remove last "comma"

    // For Troubleshooting:
    //Serial.println("In GetAllMetars url = " + url);

    GetData(url, Start);              // GET Some Metar Data (Group of Stations at a time) - GetData Routine
    for (byte i = Start; i <= Finish; i++) {
      ParseMetar(i);                  // One Station at a time - ParseMetar Routine
    }
  }
}

// *********** GET Some Metar Data/Name Group of Stations at a time
void GetData(String url, byte i) {
  MetarData = "";                     // Reset Raw Data for Group of Stations <20 at a time

  if (url == "NAME") url = host + urls + Stations[0];  else   url = host + urlb + url;

  if ((wifiMulti.run() == WL_CONNECTED)) {
    digitalWrite(LED_BUILTIN, HIGH);  // ON
    HTTPClient https;
    https.begin(url);
    //  Start connection and send HTTP header
    httpCode = https.GET();
    // httpCode will be negative on error so check
    if (httpCode > 0 ) {
      // HTTP header has been send and Server response header has been handled
      // and File found at server
      if (httpCode == HTTP_CODE_OK) {

        // For Troubleshooting : Check Max Allocated Heap
        //Serial.println("In GetData #" + String(i) + "+\t  Max Allocated Heap\t=" + String(ESP.getMaxAllocHeap()));

        MetarData = https.getString();
      } else {
        //  httpCode may still have positive Error so Double Check
        Serial.println("In GetData #" + String(i) + "+\tDouble Check for Error\t=" + String(httpCode) + " : " + https.errorToString(httpCode).c_str());
      }
      https.end();                     // Communication HTTP_CODE_OK
      digitalWrite(LED_BUILTIN, LOW);  //OFF

      // For Troubleshooting : Check What is the size of MetarData
      //Serial.println("In GetData #" + String(i) + "+\t      MetarData Size\t=" + String(MetarData.length()));

    } else {
      https.end();                     // Communication HTTP Error
      digitalWrite(LED_BUILTIN, LOW);  //OFF
      Serial.println(Clock + "\tNo:" + String(i) + "\tCommunication Error in GetData = " + String(httpCode) + " : " + https.errorToString(httpCode).c_str());
      //Communication Error httpCode = -11 : read Timeout DON'T Print
      if (httpCode != -11)  {
        Serial.println("In GetData : url = " + url);
        Network_Status ();              // WiFi Network Error
        Serial.println("In GetData : MetarData=\n" + String(MetarData));
        //StationMetar[i] = https.errorToString(httpCode).c_str();
      }
    }
  } else {
    Serial.print(F("In GetData : "));
    Network_Status ();                 // WiFi Network Error
  }
}


// ***********   WiFi Network Error
void Network_Status () {
  Serial.print(Clock + "\t\t\tWiFi Network : ");
  if (WiFi.status() == 0 ) Serial.println(F("= 0 Idle"));
  if (WiFi.status() == 1 ) Serial.println(F("= 1 Not Available, SSID can not be reached"));
  if (WiFi.status() == 2 ) Serial.println(F("= 2 Scan Completed"));
  if (WiFi.status() == 3 ) Serial.println(F("= 3 Connected"));
  if (WiFi.status() == 4 ) Serial.println(F("= 4 Failed, password incorrect"));
  if (WiFi.status() == 5 ) Serial.println(F("= 5 Connection Lost"));
  if (WiFi.status() == 6 ) Serial.println(F("= 6 Disconnected"));
}


// ***********   Parse Metar Data
void ParseMetar(byte i) {
  String Parsed_metar = "";
  String station = Stations[i].substring(0, 4);
  if (station == "NULL")   return;

  int Data_Start = MetarData.indexOf(station);                      // Search for station id
  int Data_End  = MetarData.indexOf("</METAR>", Data_Start + 1);    // Search for "data end"
  if (Data_Start > 0 && Data_End > 0)    {
    Parsed_metar = MetarData.substring(Data_Start, Data_End);       // Parse Metar Data
    // Remove found data from MetarData
    MetarData = MetarData.substring(0, Data_Start) + MetarData.substring(Data_End, MetarData.length());

    Decodedata(i, station, Parsed_metar);                           // DECODE the Station DATA

  } else {
    // Station NOT found or NOT reporting
    //Communication Error httpCode = -11 : read Timeout DON'T Print
    if (httpCode != -11)   {
      Serial.println(Clock + "\tNo:" + String(i) + "\t" + station + "\tStation Not Reporting, Skipping this one in ParseMetar");
      if (Data_End < 0 )
        Serial.println("\tIn ParseMetar:   Data_Start=" + String(Data_Start) +  "  Data_End=" + String(Data_End));
      if (Data_Start < 0 && Data_End < 0 )
        Serial.println("Likely Communication or Station Code Error");
      //Serial.println("MetarData=\n" + MetarData.substring(0, MetarData.length()));
    }

    //Reset All Parameters if Not Found
    Category[i] = "NF";             // Not Found
    Sky[i] = "NA";                  // Not Found
    StationMetar[i] = "Station Not Reporting";
    if (httpCode != HTTP_CODE_OK)  StationMetar[i] = "Connection Error";
    Sig_Weather[i] = "Not Found";   // Not Found
    visab[i] = 0;                   // Not Found
    wDir[i] = "NA";                 // Not Found
    Wind[i] = "NA";                 // Not Found
    temp[i] = 0;                    // Not Found
    dewpt[i] = 0;                   // Not Found
    altim[i] = 0;                   // Not Found
    old_altim[i] = 0;               // Not Found
    Remark[i] = "";                 // Not Found

    Display_LED (i, 20);            // Display Station LED
  }
}

// *********** DECODE the Station DATA
void Decodedata(byte i, String station, String Parsed_metar) {
  String old_obs_time;
  old_obs_time = StationMetar[i].substring(0, 4);                         // Previous Last Observation Time
  if (old_obs_time == "new ")  old_obs_time = StationMetar[i].substring(4, 8);
  // Otherwise old_obs_time = "" and will UPDATE later
  // Search for Metar End "</raw_text" in Parsed_metar
  int search_Strt = Parsed_metar.indexOf(station, 0) + 7;                  // Start of search
  int search_Raw_Text = Parsed_metar.indexOf("</raw_text", search_Strt);   // End of Remark - "</raw_text"
  if (search_Strt < 7)  {
    Serial.println(Clock + "\tNo:" + String(i) + "\t" + station + "\tFailed in Decodedata search_Strt= " + String(search_Strt));
    return;
  }

  // Search for Metar End in Parsed_metar
  int search_From = Parsed_metar.indexOf("Z ", search_Strt) + 6;  // Start search from timeZ

  int search_End_A = Parsed_metar.indexOf(" A", search_From);     // Search End for ALT in US
  int search_End_Q = Parsed_metar.indexOf(" Q", search_From);     // Search End for ALT in Non US
  int search_End_RMK = Parsed_metar.indexOf(" RMK", search_From); // Search RMK

  int search_End = search_Raw_Text;                                      // Default for End
  if (search_End_RMK > 0)  {
    search_End = search_End_RMK;                                         // Skip if no RMK
    if (search_End_A > 0)               search_End = search_End_A + 6;   // ALT in US
    if (search_End > search_End_RMK)    search_End = search_End_RMK;     // Found AO not ALT
  }
  if (search_End_Q > 0)                 search_End = search_End_Q + 6;   // ALT in Non US

  // Append Minutes ago updates on every cycle
  Update_Time();                                                // Get Time : Hour & Minute
  byte obsh = Parsed_metar.substring(7, 9).toInt();             // METAR Obs Time - Hour
  byte obsm = Parsed_metar.substring(9, 11).toInt();            // METAR Obs Time - Minute
  int ago = ((Hour - obsh) * 60) + Minute - obsm;               // Minutes ago
  if (ago < 0)    ago = ((24 - obsh) * 60) + Minute - obsm;

  // *** CREATE  Station Metar : Codes updates on every cycle
  StationMetar[i] = Parsed_metar.substring(search_Strt, search_End) + " (" + String(ago) + "m&nbspago)";  // Append ago

  // *** UPDATE/SKIP this Station *** : Check Last Observation Time with [Parsed_metar Observation Time] in StationMetar
  if (old_obs_time != StationMetar[i].substring(0, 4))   {        // If No Update : SKIP this Station

    // *** NEW DATA: UPDATING STATION
    StationMetar[i] = "new " + StationMetar[i];

    // *** Decoding after RMK > Remark[i]
    String mesg;
    String text;

    search_End = Parsed_metar.indexOf("RMK") + 3;                          // End for RMK in US : Start of Remark
    if (search_End < 3)  search_End = Parsed_metar.indexOf(" Q") + 6;      // End for Q  Non US : Start of Remark
    if (search_End < 6)  search_End = search_Raw_Text;                     // No RMK in US

    // *** CREATE  Remark : Codes
    Remark[i] = "[" + Parsed_metar.substring(search_End, search_Raw_Text) + " ]";    // Adds Brackets and a SPACE for easy viewing

    // For Troubleshooting : Print Parsed_metar
    //Serial.println("Station No " + String(i) + "\t" + station + "\t" + Parsed_metar.substring(search_Strt, search_End) + Remark[i]);

    // DECODING Remark  (Mainly REMOVE unwanted codes)
    //  **** REMOVE   AO   Automated Station
    int search2 = Remark[i].indexOf(" AO");
    int search3 = Remark[i].indexOf(" ", search2 + 1);
    search_End = search2;

    if (search2 >= 0 && search3 >= 0)   {
      text = Remark[i].substring(search2, search3);
      mesg = "";
      Remark[i].replace(Remark[i].substring(search2, search3), mesg);
    }


    //  ****  REMOVE   SLPnnn   Sea Level Pressure  12-hour mean
    search2 = Remark[i].indexOf(" SLP");
    search3 = Remark[i].indexOf(" ", search2 + 1);
    if (search3 - search2 == 6)  search3 = -1;                    // If SLPNO

    if (search2 >= 0 && search3 >= 0)  {
      text = Remark[i].substring(search2, search3);
      mesg = "";
      Remark[i].replace(Remark[i].substring(search2, search3), mesg);
    }


    //  ****  REPLACE   Ppppp   Hourly Precipition Amount
    // p=precip amount in Hundredths of Inch
    search2 = Remark[i].indexOf(" P0");
    search3 = Remark[i].indexOf(" ", search2 + 1);
    if (search3 - search2 != 6)  search3 = -1;

    if (search2 >= 0 && search3 >= 0)   {
      text = Remark[i].substring(search2, search3);
      if (search2 > 4)  mesg = " <br>";  else mesg = "";
      mesg = mesg + "Hourly Precip=" + text.substring(3, 4) + "." + text.substring(4, 6) + '"';
      mesg.replace("0.00", "Trace");
      Remark[i].replace(Remark[i].substring(search2, search3), mesg);
    }


    //  ****  REPLACE   ITnnn  Hourly Ice Accretion in Hundredths of Inch [Hourly]
    //  T = 1 or 3 or 6 hour   nnn= Icing amount
    search2 = Remark[i].indexOf(" I1");
    search3 = Remark[i].indexOf(" ", search2 + 1);
    if (search3 - search2 != 6)  search3 = -1;

    if (search2 >= 0 && search3 >= 0)   {
      text = Remark[i].substring(search2, search3);
      if (search2 > 4)  mesg = " <br>";  else mesg = "";
      if (text.substring(2, 3) == "1")  mesg = mesg + "Hourly Ice=";
      if (text.substring(2, 3) == "3")  mesg = mesg + "3 Hour Ice=";
      if (text.substring(2, 3) == "6")  mesg = mesg + "6 Hour Ice=";
      mesg = mesg + text.substring(3, 4) + "." + text.substring(4, 6) + '"';
      mesg.replace("0.00", "Trace");
      Remark[i].replace(Remark[i].substring(search2, search3), mesg);
    }


    //  ****  REPLACE   ITnnn  Ice Accretion in Hundredths of Inch [3 Hour]
    //  T = 1 or 3 or 6 hour   nnn= Icing amount
    search2 = Remark[i].indexOf(" I3");
    search3 = Remark[i].indexOf(" ", search2 + 1);
    if (search3 - search2 != 6)  search3 = -1;

    if (search2 >= 0 && search3 >= 0)   {
      text = Remark[i].substring(search2, search3);
      if (search2 > 4)  mesg = " <br>";  else mesg = "";
      if (text.substring(2, 3) == "1")  mesg = mesg + "Hourly Ice=";
      if (text.substring(2, 3) == "3")  mesg = mesg + "3 Hour Ice=";
      if (text.substring(2, 3) == "6")  mesg = mesg + "6 Hour Ice=";
      mesg = mesg + text.substring(3, 4) + "." + text.substring(4, 6) + '"';
      mesg.replace("0.00", "Trace");
      Remark[i].replace(Remark[i].substring(search2, search3), mesg);
    }


    //  ****  REPLACE   ITnnn  Ice Accretion in Hundredths of Inch [6 Hour]
    //  T = 1 or 3 or 6 hour   nnn= Icing amount
    search2 = Remark[i].indexOf(" I6");
    search3 = Remark[i].indexOf(" ", search2 + 1);
    if (search3 - search2 != 6)  search3 = -1;

    if (search2 >= 0 && search3 >= 0)   {
      text = Remark[i].substring(search2, search3);
      if (search2 > 4)  mesg = " <br>";  else mesg = "";
      if (text.substring(2, 3) == "1")  mesg = mesg + "Hourly Ice=";
      if (text.substring(2, 3) == "3")  mesg = mesg + "3 Hour Ice=";
      if (text.substring(2, 3) == "6")  mesg = mesg + "6 Hour Ice=";
      mesg = mesg + text.substring(3, 4) + "." + text.substring(4, 6) + '"';
      mesg.replace("0.00", "Trace");
      Remark[i].replace(Remark[i].substring(search2, search3), mesg);
    }


    //  ****  REMOVE   TsTTTTTTT   Current Temperature
    // s= 0 Positive s= 1 Negative  T= Temp in Deg C
    search2 = Remark[i].indexOf(" T0");                      //  Positive
    if (search2 < 0)    search2 = Remark[i].indexOf(" T1");  //  Negative
    search3 = Remark[i].indexOf(" ", search2 + 1);

    if (search2 >= 0 && search3 >= 0)  {
      text = Remark[i].substring(search2, search3);
      if (text.charAt(2) == '0')  text.setCharAt(2, '+');  else  text.setCharAt(2, '-');
      if (text.charAt(6) == '0')  text.setCharAt(6, '+');  else  text.setCharAt(6, '-');
      text = text.substring(2, 5) + "." + text.substring(5, 6) + " DewPt " + text.substring(6, 9) + "." + text.substring(9, 10);
      mesg = "";
      Remark[i].replace(Remark[i].substring(search2, search3), mesg);
    }


    //  ****  REMOVE  1sTTT  6 Hr MAX Temperature
    //3-hourly report: A METAR taken at 0300, 0900, 1500, or 2100 UTC.
    //6-hourly report: A METAR taken at 0000, 0600, 1200, or 1800 UTC
    // at 0000, 0600, 1200, and 1800 UTC observations
    // s= 0 Positive s= 1 Negative  T= Temp in Deg C
    search2 = Remark[i].indexOf(" 10");                      //  Positive
    if (search2 < 0)    search2 = Remark[i].indexOf(" 11");  //  Negative
    search3 = Remark[i].indexOf(" ", search2 + 1);

    if (search3 - search2 != 6)  search3 = -1;
    if (search_End < 0)  search3 = -1;                        // No Automated Station

    if (search2 >= 0 && search3 >= 0)  {
      text = Remark[i].substring(search2, search3);
      if (text.charAt(2) == '0')  text.setCharAt(2, '+');  else  text.setCharAt(2, '-');
      text = text.substring(2, 5) + "." + text.substring(5, 6);
      Remark[i].replace(Remark[i].substring(search2, search3), "");
    }


    //  ****  REMOVE  2sTTT  6 Hr MIN Temperature
    // s= 0 Positive s= 1 Negative  T=Temp in Deg C
    search2 = Remark[i].indexOf(" 20");                      //  Positive
    if (search2 < 0)    search2 = Remark[i].indexOf(" 21");  //  Negative
    search3 = Remark[i].indexOf(" ", search2 + 1);
    int search1 = Remark[i].indexOf("FT", search2 + 1);      // NOT 200FT Density Alt
    if (search1 > 0 && search1 < search3)  search2 = -1;     // NOT 200FT Density Alt
    if (search3 - search2 != 6)  search3 = -1;
    if (search_End < 0)  search3 = -1;                       // No Automated Station

    if (search2 >= 0 && search3 >= 0)    {
      text = Remark[i].substring(search2, search3);
      if (text.charAt(2) == '0')  text.setCharAt(2, '+');  else  text.setCharAt(2, '-');
      text = text.substring(2, 5) + "." + text.substring(5, 6);
      Remark[i].replace(Remark[i].substring(search2, search3), "");
    }


    //  ****  REMOVE   3PPPP  Air Pressure at station level in hPa
    search2 = Remark[i].indexOf(" 3");
    search3 = Remark[i].indexOf(" ", search2 + 1);
    if (search1 == -1)  search1 = Remark[i].indexOf("V", search2 + 1);    // NOT Visability
    search1 = Remark[i].indexOf("FT", search2 + 1);         // NOT 300FT Density Alt
    if (search1 > 0 && search1 < search3)  search2 = -1;    // NOT 300FT Density Alt
    if (search3 - search2 != 6)  search3 = -1;
    if (search_End < 0)  search3 = -1;                      // No Automated Station

    if (search2 >= 0 && search3 >= 0)  {
      text = Remark[i].substring(search2 + 2, search3);
      Remark[i].replace(Remark[i].substring(search2, search3), "");
    }


    //  ****  REMOVE    4sTTTsTTT  24 Hr MAX/MIN Temperature
    // 24-hour report at 0000 UTC
    // s= 0 Positive s= 1 Negative  T=Temp in Deg C
    search2 = Remark[i].indexOf(" 40");                      //  Positive
    if (search2 < 0)    search2 = Remark[i].indexOf(" 41");  //  Negative
    search3 = Remark[i].indexOf(" ", search2 + 1);
    if (search3 - search2 != 10)  search3 = -1;
    if (search_End < 0)  search3 = -1;                       // No Automated Station

    if (search2 >= 0 && search3 >= 0)    {
      text = Remark[i].substring(search2, search3);
      if (text.charAt(2) == '0')  text.setCharAt(2, '+');  else  text.setCharAt(2, '-');
      if (text.charAt(6) == '0')  text.setCharAt(6, '+');  else  text.setCharAt(6, '-');
      text = text.substring(2, 5) + "." + text.substring(5, 6) + " / " + text.substring(6, 9) + "." + text.substring(9, 10);
      Remark[i].replace(Remark[i].substring(search2, search3), "");
    }


    //  ****  REPLACE   6RRRR 3Hr   Precipitation Amount in Inches
    // R=precip amount in Hundredths of Inch ??
    search2 = Remark[i].indexOf(" 60");
    search3 = Remark[i].indexOf(" ", search2 + 1);
    //search1 = Remark[i].indexOf("FT", search2 + 1);         // NOT 600FT Density Alt
    //if (search1 > 0 && search1 < search3)  search2 = -1;    // NOT 600FT Density Alt
    if (search3 - search2 != 6)  search3 = -1;
    if (search_End < 0)  search3 = -1;                      // No Automated Station

    if (search2 >= 0 && search3 >= 0)  {
      text = Remark[i].substring(search2 + 1, search3);
      if (search2 > 4)  mesg = " <br>";  else mesg = "";
      mesg = mesg + "3 Hour Precip=" + text.substring(2, 3) + "." + text.substring(3, 6) + '"';
      mesg.replace("0.00", "Trace");
      Remark[i].replace(Remark[i].substring(search2, search3), mesg);
    }


    //  ****  REPLACE   7RRRR  24Hr Precipitation Amount in Inches
    // R=precip amount in Hundredths of Inch ??
    search2 = Remark[i].indexOf(" 70");
    search3 = Remark[i].indexOf(" ", search2 + 1);
    //search1 = Remark[i].indexOf("FT", search2 + 1);         // NOT 700FT Density Alt
    //if (search1 > 0 && search1 < search3)  search3 = -1;    // NOT 700FT Density Alt
    if (search3 - search2 != 6)  search3 = -1;
    if (search_End < 0)  search3 = -1;                        // No Automated Station

    if (search2 >= 0 && search3 >= 0)     {
      text = Remark[i].substring(search2 + 1, search3);
      mesg = "<br>24Hr Precip=" + text.substring(2, 3) + "." + text.substring(3, 5) + '"';
      Remark[i].replace(Remark[i].substring(search2, search3), mesg);
    }


    //  ****  REMOVE   8/LMH  Cloud Types
    // /=above overcast 3= dense cirrus 6=stratus 9-CB
    search2 = Remark[i].indexOf(" 8/");
    search3 = Remark[i].indexOf(" ", search2 + 1);
    if (search3 - search2 != 6)  search3 = -1;
    if (search_End < 0)  search3 = -1;                        // No Automated Station

    if (search2 >= 0 && search3 >= 0)   {
      text = Remark[i].substring(search2 + 1, search3);
      mesg = text.substring(2, 5);
      Remark[i].replace(Remark[i].substring(search2, search3), "");
    }


    //  ****  REMOVE   8/CLCMCH  Cloud Cover
    // 8/CLCMCH, shall be reported and coded in 3- and 6-hourly reports
    search2 = Remark[i].indexOf(" 8/");
    search3 = Remark[i].indexOf(" ", search2 + 1);
    if (search_End < 0)  search3 = -1;                        // No Automated Station

    if (search2 >= 0 && search3 >= 0)      {
      text = Remark[i].substring(search2 + 1, search3);
      mesg = text.substring(2, 6);
      Remark[i].replace(Remark[i].substring(search2, search3), "");
    }


    // ****  REMOVE  933RRR   Water Equivalent of Snow on Ground in tenths in the 0800 UTC report
    search2 = Remark[i].indexOf(" 933");
    search3 = Remark[i].indexOf(" ", search2 + 1);

    if (search2 >= 0 && search3 >= 0)    {
      text = Remark[i].substring(search2 + 1, search3);
      mesg = text.substring(3, 6);
      Remark[i].replace(Remark[i].substring(search2, search3), "");
    }


    //  ****  REPLACE   4/SSS  Snow Depth   0000, 0600, 1200, and 1800 UTC
    // SSS Inches Snow on the Ground
    search2 = Remark[i].indexOf(" 4/");
    search3 = Remark[i].indexOf(" ", search2 + 1);

    // For Troubleshooting
    //if (search2 > 0) Serial.println(station + "\t4/SSS  Snow Depth =" + Remark[i].substring(search2, search3) + ".  Search2 =" + search2 + "  Search3 =" + search3 + "  Length =" + (search3 - search2));

    if (search3 - search2 != 6)  search3 = -1;

    if (search2 >= 0 && search3 >= 0)   {
      text = Remark[i].substring(search2 + 4, search3);
      if (search2 > 4)   mesg = " <br>";  else    mesg = "";
      mesg = mesg + "Snow Depth on the Ground=" + text + '"';
      Remark[i].replace(Remark[i].substring(search2, search3), mesg);
    }


    //  ****  REPLACE   5appp   Pressure Tendency   3- and 6-hourly report
    // a=4 Steady   a=0123 Incr   a=5678 Decr   ppp=tenths hPa/mb
    search2 = Remark[i].indexOf(" 5");
    search3 = Remark[i].indexOf(" ", search2 + 1);
    if (search_End < 0)  search3 = -1;                       // No Automated Station
    if (search3 - search2 != 6)  search3 = -1;

    if (search2 >= 0 && search3 >= 0)   {
      text = Remark[i].substring(search2 + 1, search3);
      int press_change = 0;
      press_change = text.substring(1, 2).toInt();
      if (search2 > 4)   mesg = " <br>";  else    mesg = "";
      if (press_change > 4)  mesg = mesg + "3 Hour Pressure DEC " + Remark[i].substring(search2 + 4, search2 + 5) + "." + Remark[i].substring(search2 + 5, search2 + 6) + " mb";
      if (press_change < 4)  mesg = mesg + "3 Hour Pressure INC " + Remark[i].substring(search2 + 4, search2 + 5) + "." + Remark[i].substring(search2 + 5, search2 + 6) + " mb";
      if (press_change == 4) mesg = mesg + "3 Hour Pressure Steady";
      mesg.replace("0.0 mb", "Negligable");
      Remark[i].replace(Remark[i].substring(search2, search3), mesg);
    }


    //  ****  REPLACE  98MMM   Duration of Sunshine in Minutes
    search2 = Remark[i].indexOf(" 98");
    search3 = Remark[i].indexOf(" ", search2 + 1);
    if (search3 - search2 != 6)  search3 = -1;

    if (search2 >= 0 && search3 >= 0)    {
      text = Remark[i].substring(search2 + 1, search3);
      float sun = (float)text.substring(2, 5).toInt() / 60.0;
      if (search2 > 4)   mesg = " <br>";  else    mesg = " ";
      mesg = mesg + "Sunshine=" + String(sun) + " Hrs";
      Remark[i].replace(Remark[i].substring(search2, search3), mesg);
    }



    // ****  DECODE WEATHER in REMARKS and make readable in Dictionary Function
    String weather = Decode_Weather(Remark[i]);                    // Decode_Weather Routine
    Remark[i] = weather;                                           // Return with readable weather

    // Searching flight_category
    int search0 = Parsed_metar.indexOf("<flight_category") + 17;    // Search Between search0
    search1 = Parsed_metar.indexOf("</flight_category");            // Search Between search1
    if (search0 < 17) Category[i] = "NA";  else  Category[i] = Parsed_metar.substring(search0, search1);

    // Significant Weather
    Sig_Weather[i] = "";
    // Wind Gusts
    search0 = StationMetar[i].indexOf("G");
    search1 = StationMetar[i].indexOf("KT");
    if (search0 > 0 && search1 - search0 > 2)
      Sig_Weather[i] =  Sig_Weather[i] + "Gusts to " + StationMetar[i].substring(search1 - 2, search1) + " KTS ;  ";

    // Variable Wind Dir
    search0 = StationMetar[i].indexOf("0V");
    search1 = StationMetar[i].indexOf("FT");
    if (search0 > 0  && search1 < 0)
      Sig_Weather[i] = Sig_Weather[i] + " Wind Dir Varies: " + StationMetar[i].substring(search0 - 2, search0 + 5) + ";  ";

    // Significant Weather
    search0 = StationMetar[i].indexOf("SM R");
    search1 = StationMetar[i].indexOf("FT");
    if (search0 > 0 && search1 > 0)
      Sig_Weather[i] = Sig_Weather[i] + " Runway Vis: " + StationMetar[i].substring(search0 + 3, search1 + 2) + ";  ";

    // Significant Weather in metar and making readable Weather
    if (StationMetar[i].indexOf("DR") > 0)    Sig_Weather[i] = Sig_Weather[i] + " Drifting";
    if (StationMetar[i].indexOf("FZ") > 0)    Sig_Weather[i] = Sig_Weather[i] + " Freezing";
    if (StationMetar[i].indexOf("MI") > 0)    Sig_Weather[i] = Sig_Weather[i] + " Shallow";
    if (StationMetar[i].indexOf("PR") > 0)    Sig_Weather[i] = Sig_Weather[i] + " Partial";
    if (StationMetar[i].indexOf("BC") > 0)    Sig_Weather[i] = Sig_Weather[i] + " Patches of";
    if (StationMetar[i].indexOf("FG") > 0)    Sig_Weather[i] = Sig_Weather[i] + " Fog; ";
    if (StationMetar[i].indexOf("BR") > 0)    Sig_Weather[i] = Sig_Weather[i] + " Mist; ";
    if (StationMetar[i].indexOf("DU") > 0)    Sig_Weather[i] = Sig_Weather[i] + " Dust; ";
    if (StationMetar[i].indexOf("FU") > 0)    Sig_Weather[i] = Sig_Weather[i] + " Smoke; ";
    if (StationMetar[i].indexOf("HZ") > 0)    Sig_Weather[i] = Sig_Weather[i] + " Haze; ";
    if (StationMetar[i].indexOf("FY") > 0)    Sig_Weather[i] = Sig_Weather[i] + " Spray; ";
    if (StationMetar[i].indexOf("SA") > 0)    Sig_Weather[i] = Sig_Weather[i] + " Sand; ";
    if (StationMetar[i].indexOf(" PO") > 0)   Sig_Weather[i] = Sig_Weather[i] + " Dust/Sand; ";
    if (StationMetar[i].indexOf("SQ") > 0)    Sig_Weather[i] = Sig_Weather[i] + " Squalls; ";
    if (StationMetar[i].indexOf(" +") > 0)    Sig_Weather[i] = Sig_Weather[i] + " Heavy";
    if (StationMetar[i].indexOf(" -") > 0)    Sig_Weather[i] = Sig_Weather[i] + " Light";
    if (StationMetar[i].indexOf("RA") > 0)    Sig_Weather[i] = Sig_Weather[i] + " Rain ";
    if (StationMetar[i].indexOf("BL") > 0)    Sig_Weather[i] = Sig_Weather[i] + " Blowing";
    if (StationMetar[i].indexOf("SN") > 0)    Sig_Weather[i] = Sig_Weather[i] + " Snow ";
    if (StationMetar[i].indexOf("SH") > 0)    Sig_Weather[i] = Sig_Weather[i] + " Showers ";
    if (StationMetar[i].indexOf("SG") > 0)    Sig_Weather[i] = Sig_Weather[i] + " Snow Grains; ";
    if (StationMetar[i].indexOf("DZ") > 0)    Sig_Weather[i] = Sig_Weather[i] + " Drizzle; ";
    if (StationMetar[i].indexOf("GS") > 0)    Sig_Weather[i] = Sig_Weather[i] + " Small Hail; ";
    if (StationMetar[i].indexOf("GR") > 0)    Sig_Weather[i] = Sig_Weather[i] + " Large Hail; ";
    if (StationMetar[i].indexOf("IC") > 0)    Sig_Weather[i] = Sig_Weather[i] + " Ice Crystals ";
    if (StationMetar[i].indexOf("PL") > 0)    Sig_Weather[i] = Sig_Weather[i] + " Ice Pellets ";
    if (StationMetar[i].indexOf("VA") > 0)    Sig_Weather[i] = Sig_Weather[i] + " Volcanic Ash ";
    if (StationMetar[i].indexOf("TS") > 0)    Sig_Weather[i] = Sig_Weather[i] + " Thunderstorm ";
    if (StationMetar[i].indexOf(" VC") > 0)   Sig_Weather[i] = Sig_Weather[i] + "in Vicinity; ";
    if (StationMetar[i].indexOf("-VC") > 0)   Sig_Weather[i] = Sig_Weather[i] + "in Vicinity; ";
    if (StationMetar[i].indexOf("+VC") > 0)   Sig_Weather[i] = Sig_Weather[i] + "in Vicinity; ";
    if (StationMetar[i].indexOf("CBMAM") > 0) Sig_Weather[i] = Sig_Weather[i] + " Mammatus CB";
    if (StationMetar[i].indexOf("CB ") > 0)   Sig_Weather[i] = Sig_Weather[i] + " Cumulonimbus Clouds ";
    if (StationMetar[i].indexOf("TCU") > 0)   Sig_Weather[i] = Sig_Weather[i] + " Towering Cumulus Clouds ";
    if (StationMetar[i].indexOf("UP ") > 0)   Sig_Weather[i] = Sig_Weather[i] + " Unknown Precipitation; ";

    if (Sig_Weather[i] == "")  Sig_Weather[i] = "None";

    // Searching temp_c        temp[i] = deg C
    search0 = Parsed_metar.indexOf("<temp_c") + 8;
    search1 = Parsed_metar.indexOf("</temp");
    if (search0 < 8)   Serial.println(Clock + "\tNo:" + String(i) + "\t" + station + "\tTemperature Not Found, in Decodedata");
    if (search0 < 8) temp[i] = 0;  else  temp[i] = Parsed_metar.substring(search0, search1).toFloat() + 0.001;
    //float TempF = temp[i] * 1.8 + 32;            // deg F

    // Searching dewpoint_c
    search0 = Parsed_metar.indexOf("<dewpoint_c") + 12;
    search1 = Parsed_metar.indexOf("</dewpoint");
    if (search0 < 12) dewpt[i] = 0;  else  dewpt[i] = Parsed_metar.substring(search0, search1).toFloat() + 0.001;

    // Searching Wind_dir_degrees
    old_wDir[i] = wDir[i].toInt();
    search0 = Parsed_metar.indexOf("wind_dir_degrees") + 17;
    search1 = Parsed_metar.indexOf("</wind_dir_degree");
    if (search0 < 17) wDir[i] = "NA";  else  wDir[i] = Parsed_metar.substring(search0, search1);
    if (StationMetar[i].indexOf("VRB") > 0) wDir[i] = "VRB";
    if (wDir[i] == "NA")   Serial.println(Clock + "\tNo:" + String(i) + "\t" + station + "\tWind Dir Not Found, in Decodedata");

    // Searching Wind_speed_kt
    search0 = Parsed_metar.indexOf("wind_speed_kt") + 14;
    search1 = Parsed_metar.indexOf("</wind_speed");
    if (search0 < 14) Wind[i] = "NA";  else  Wind[i] = Parsed_metar.substring(search0, search1) + " KT";
    if (Wind[i] == "0 KT")  Wind[i] = "CALM";

    // Searching  visibility_statute_mi
    search0 = Parsed_metar.indexOf("visibility_statute_mi") + 22;
    search1 = Parsed_metar.indexOf("</visibility");
    if (search0 < 22)  visab[i] = 0;  else  visab[i] = Parsed_metar.substring(search0, search1).toFloat();

    // Searching  <sky_condition
    old_cloud_base[i] = new_cloud_base[i];
    search0 = Parsed_metar.indexOf("<sky_condition");            // Start Search from Here
    search1 = Parsed_metar.indexOf("sky_cover", search0) + 11;
    if (search1 < 11)  Sky[i] = "NA";  else  Sky[i] = Parsed_metar.substring(search1, search1 + 3);   {
      search1 = Parsed_metar.indexOf("_ft_agl=", search0) + 9;
      search2 = Parsed_metar.indexOf(" />", search0) - 1;
      if (Sky[i] == "OVX")  Sky[i] = "OBSECURED";
      if (Sky[i] == "CAV")  Sky[i] = "CLEAR";
      if (Sky[i] == "CLR")  Sky[i] = "CLEAR";
      if (Sky[i] == "SKC")  Sky[i] = "CLEAR";
      if (Sky[i] == "BKN")  Sky[i] = Sky[i] + " Clouds at " + Parsed_metar.substring(search1, search2) + " Ft";
      if (Sky[i] == "FEW")  Sky[i] = Sky[i] + " Clouds at " + Parsed_metar.substring(search1, search2) + " Ft";
      if (Sky[i] == "SCT")  Sky[i] = Sky[i] + " Clouds at " + Parsed_metar.substring(search1, search2) + " Ft";
      if (Sky[i] == "OVC")  Sky[i] = Sky[i] + " Clouds at " + Parsed_metar.substring(search1, search2) + " Ft";
      new_cloud_base[i] = Parsed_metar.substring(search1, search2).toInt();
      if (Sky[i] == "OBSECURED")   new_cloud_base[i] = 0;
      if (Sky[i] == "CLEAR")   new_cloud_base[i] = 99999;
    }
    if (Sky[i] == "NA")   new_cloud_base[i] = 0;

    // Vertical Visibility Searching <vert_vis_ft>
    search0 = Parsed_metar.indexOf("<vert_vis_ft>") + 13;
    if (search0 > 13)  {
      Sig_Weather[i] = Sig_Weather[i] + " Vertical Visibility = " + Parsed_metar.substring(search0, search0 + 3) + " Ft; ";
      new_cloud_base[i] = Parsed_metar.substring(search0, search0 + 3).toInt();
    }

    // Searching <altim_in_hg
    old_altim[i] = altim[i];
    search0 = Parsed_metar.indexOf("<altim_in_hg>") + 13;
    search1 = Parsed_metar.indexOf("</altim_in_hg>");
    if (search0 < 13)  Serial.println(Clock + "\tNo:" + String(i) + "\t" + station + "\tAltimeter Not Found, in Decodedata");
    if (search0 < 13) altim[i] = 0;  else  altim[i] = Parsed_metar.substring(search0, search1).toFloat();
    //    float Pressure = altim[i]; // in_hg

    // Searching <sea_level_pressure_mb
    search0 = Parsed_metar.indexOf("<sea_level_pressure_mb>") + 23;
    if (search0 < 23) seapres[i] = 0;  else  seapres[i] = Parsed_metar.substring(search0, search0 + 6).toFloat();

    // Searching elevation_m
    search0 = Parsed_metar.indexOf("<elevation") + 13;
    search1 = Parsed_metar.indexOf("</elevation");
    if (search0 < 13) elevation[i] = 0; else elevation[i] = Parsed_metar.substring(search0, search1).toFloat();

  }                               // UPDATE Station
  Display_LED (i, 20);            // Display This Station LED
}



//  *********** Decode Weather and make readable in Dictionary Function
String Decode_Weather(String weather) {
  // Dictionary of CODES

  // For Troubleshooting:
  //Serial.println("\tInital weather\t." + weather + ".");

  weather.replace("BECMG", "CMG");                       // Rename Later
  weather.replace("CAVOK", "cavok");                     // Rename Later
  weather.replace("RERA", "Recent Rain");
  weather.replace("REDZ", "Recent Drizzle");
  weather.replace("RTS", "Routes");
  weather.replace("TEMPO", " Temporary:");
  weather.replace("AWOS", "awos");                       // Rename Later
  weather.replace("OTS", "NA");

  weather.replace("NOSIG", "No Signifiant Weather");
  weather.replace("NSW", "No Signifiant Weather");
  weather.replace("NSC", "No Signifiant Clouds");
  weather.replace("DENSITY ALT", " Density Alt");
  weather.replace("DEWPT", " Dew Point");
  weather.replace("CONS ", " Continuous");
  weather.replace("PDMTLY", " Predominantly");
  weather.replace("TRANSPARENT", "Transparent");

  weather.replace("STNRY", "  Stationary");
  weather.replace("DRFTNG", " Drifting");
  weather.replace("DRFTG", "  Drifting");
  weather.replace("SLWLY", " Slow");
  weather.replace("BANK", "bank");               // Rename Later

  weather.replace("OBSC", "Obscured");
  weather.replace("FROPA", " Frontal Passage ");
  weather.replace("PRESFR", " Pressure Falling Rapidly ");
  weather.replace("PRESRR", " Pressure Rising Rapidly ");
  weather.replace("FROIN", " Frost On The Indicator ");
  weather.replace("ALSTG", " Alitmeter Setting");
  weather.replace("ALTSTG", " Alitmeter Setting");

  weather.replace("SLPNO", " Sea Level Pressure NA ");
  weather.replace("PNO", " Rain Gauge NA ");
  weather.replace("FZRANO", " Freezing Rain Sensor NA ");
  weather.replace("TSNO", " Thunderstorm Sensor NA ");
  weather.replace("RVRNO", " Runway visibility Sensor NA ");     // Rename Later
  weather.replace("CHINO", " Secondary Ceiling Height Sensor NA ");
  weather.replace("VISNO", " visibility Sensor NA ");    // Rename Later
  weather.replace("PWINO ", " Peak Wind Sensor NA ");
  weather.replace("CEILOMETER", "Ceilometer ");            // CEILOMETER OTS

  weather.replace("PK WND", " Peak wind");                // See below
  weather.replace("WSHFT ", "Wind Shift at ");
  weather.replace("WIND", "Wind");
  weather.replace("WND", "Wind");
  weather.replace("WS", "Wind Shear");

  weather.replace("ALF", "Aloft");
  weather.replace("GRADL", "Gradually");
  weather.replace("GRAD", "Gradual");
  weather.replace("LGR", "Longer");
  weather.replace("MISG", "Missing");
  weather.replace("ASOCTD", "Associated");
  weather.replace("CONTRAILS", "Contrails");

  weather.replace("TR", "Trace ");
  weather.replace("RE", "Recent ");

  weather.replace("SHELF", " Shelf");
  weather.replace("DATA", "Data");
  weather.replace("AND", "and");
  weather.replace("UTC", "utc");                    // Rename Later
  weather.replace("ESTD", "eastsimated");           // Rename Later
  weather.replace("ESTMD", "eastsimated");          // Rename Later

  weather.replace("PAST", "Past");
  weather.replace("PRSNT", "Present");

  weather.replace("THN", "Thin");
  weather.replace("THRU", "Thru");
  weather.replace("HR ", "Hour ");
  weather.replace("WSHFT", "Wind Shift");
  weather.replace("ICG", "Icing");
  weather.replace("PCPN", "Precip");
  weather.replace("MTS", "Mountains");
  weather.replace("ACC", "AC");
  weather.replace("SNINCR", "Snow Increasing Rapidily");
  weather.replace("ACFT MSHP", "Aircraft Mishap");
  weather.replace("STFD", "Staffed");
  weather.replace("FST", " First");
  weather.replace("LAST", " Last");
  weather.replace("OBS", "Observation");
  weather.replace("OB", "Observation");
  weather.replace("NXT", "Next");
  weather.replace("CNVTV", "Conv");
  weather.replace("CVCTV", "Conv");                  // Rename Later
  weather.replace("FUNNEL CLOUD ", "Funnel Cloud");
  weather.replace("PLUME", "Plume");

  weather.replace("BKN", "KN CLD");                  // Rename Later
  weather.replace("SCT", "Scattered CLD");
  weather.replace("FEW", "Few CLD");
  weather.replace("MDT CU", " ModerateCU");
  weather.replace("MDT", "Moderate");
  weather.replace("CLD", "Clouds");
  weather.replace("EMBD", "mbedded");                 // Rename Later
  weather.replace("EMBED", "mbedded");                // Rename Later

  weather.replace("BINOVC", " KN in OC");             // Rename Later

  weather.replace("OCNL LTG", "OCNLLTG");
  weather.replace("OCNL LT", "OCNLLTG");
  weather.replace("OCNL", " Occasional");
  weather.replace("FRQ LTG", "FRQLTG");
  weather.replace("FRQ", " Frequent");
  weather.replace("AND", "and");
  weather.replace("IN VICINTY", "VC");

  weather.replace("LTGICCCCA", "LTG LTinC CA");        // Lightning in Cloud and Air
  weather.replace("LTGICCACG", "LTG LTtoG CA");        // Lightning in Cloud Air and Ground
  weather.replace("LTGCGICCC", "LTG LTinC and LTtoG"); // Lightning in Cloud and Ground
  weather.replace("LTGICCCCG", "LTG LTinC and LTtoG"); // Lightning in Cloud and Ground
  weather.replace("LTGICIC", "LTG LTinC");             // Lightning in Cloud
  weather.replace("LTGICCC", "LTG LTinC");             // Lightning in Cloud
  weather.replace("LTGCGIC", "LTG LTinC and LTtoG");   // Lightning in Cloud and Ground
  weather.replace("LTGICCG", "LTG LTinC and LTtoG");   // Lightning in Cloud and Ground
  weather.replace("LTGCCCG", "LTG LTinC and LTtoG");   // Lightning in Cloud and Ground
  weather.replace("LTICGCG", "LTG LTinC and LTtoG");   // Lightning in Cloud and Ground
  weather.replace("LTGICCA", "LTG LTinC CA");          // Lightning in Cloud and Air
  weather.replace("LTGCG", "LTG LTtoG");               // Lightning in Ground
  weather.replace("LTCIC", "LTG LTinC");               // Lightning in Cloud
  weather.replace("LTGCC", "LTG LTinC");               // Lightning in Cloud
  weather.replace("LTGIC", "LTG LTinC");               // Lightning in Cloud
  weather.replace("CG", " and LTtoG");                 // Lightning to Ground (fix)
  weather.replace("LTG", " Lightning");
  weather.replace("LTinC", "in Clouds");
  weather.replace("LTtoG", "to Ground");
  weather.replace("CA", "and Cloud to Air");

  weather.replace("ALQDS", "All Quadrents");
  weather.replace("ALQS", "All Quadrents");
  weather.replace("DIST", "Distant");
  weather.replace("DSNT", "Distant");
  weather.replace("MOVG", "Movement");
  weather.replace("MOV", "Movement");
  weather.replace("STNRY", "Stationary");

  weather.replace("RAB", "Rain began");                // Rename Later
  weather.replace("RAE", "Rain ended");                // Rename Later
  weather.replace("SNB", "Snow began");                // Rename Later
  weather.replace("SNE", "Snow ended");                // Rename Later

  weather.replace(" E", " east");                       // Rename Later
  weather.replace("E ", "east ");                       // Rename Later
  weather.replace("NE", "northeast");                   // Rename Later
  weather.replace(" SE", " Seast");                     // Rename Later
  weather.replace("E-", "east-");                       // Rename Later
  weather.replace("-E", "-east");                       // Rename Later
  weather.replace("-SE", "-Seast");                     // Rename Later

  weather.replace("SFC VIS", " SurfaceVIS");
  weather.replace("SFC", "Surface");
  weather.replace("TWR VIS", " TowerVIS");
  weather.replace("TWR", " Tower ");
  weather.replace("VV", "visability variable");         // Rename Later
  weather.replace("VISBL", "visabile");                 // Rename Later
  weather.replace("VIS M", "visability Measured");      // Rename Later
  weather.replace("VIS", " visability");                // Rename Later
  weather.replace("INC", "Increasing");
  weather.replace("DEC", "Decreasing");

  weather.replace(" +", " Heavy ");
  weather.replace(" -", " Light ");
  weather.replace(" - ", " to ");

  weather.replace("SHRASN", "Rain and Snow Showers ");
  weather.replace("SHRA", "Rain Showers ");
  weather.replace("SHSN", "Snow Showers ");
  weather.replace("VCSH", "Showers VC");
  weather.replace("VCSH", "RA Shower VC");
  weather.replace("VCTS", "TS VC");
  weather.replace("OVC", "OC");                        // Rename Later
  weather.replace("VC", "In the vicinity");            // Rename Later

  weather.replace("HIER", "Higher");
  weather.replace("LWR", "Lower");

  weather.replace("DSIPTD", "Dissipated");
  weather.replace("DSIPT", "Dissipated");
  weather.replace("DSPTD", "Dissipated");
  weather.replace("DSPT", "Dissipated");
  weather.replace("HVY", "Heavy");
  weather.replace("LGT", "Light");
  weather.replace("BLACK", "black:  ");              // Rename Later

  weather.replace("OHD", "oerhead");                 // Rename Later
  weather.replace("OVD", "oerhead");                 // Rename Later
  weather.replace("VRB", "variable");                // Rename Later
  weather.replace("VRY", "very");                    // Rename Later
  weather.replace("VERY", "very");                   // Rename Later
  weather.replace("VSBY", "visibility");             // Rename Later
  weather.replace("VSB", "visibile");                // Rename Later
  weather.replace("VIRGA", "virga");                 // Rename Later
  weather.replace("V", "variable");                  // Rename Later

  weather.replace("CIG", " Ceiling");
  weather.replace("ACSL", " Standing Lenticular Altocumulus Clouds");
  weather.replace("SC", " Stratocumulus Clouds");
  weather.replace("CBMAM", " mammatusCB");

  weather.replace("TCU", " TC");                     // Rename Later
  weather.replace("TS", " Thunderstorm");

  weather.replace("BLU", "blu");                     // See Military Later
  weather.replace("WHT", "wht");                     // See Military Later
  weather.replace("GRN", "grn");                     // See Military Later
  weather.replace("YLO", "ylo");                     // See Military Later
  weather.replace("AMB", "amb");                     // See Military Later
  weather.replace("RED", "redd");                    // See Military Later

  weather.replace("BC", " Patches of ");
  weather.replace("BL", " blowing ");                // Rename Later
  weather.replace("DR", " Drifting ");
  weather.replace("FZ", " Freezing ");

  weather.replace("MI", "Shallow");
  weather.replace("PTCHY", "Patchy");
  weather.replace("MOD", "Moderate");

  weather.replace("DZ", "Drizzle");
  weather.replace("RA", "Rain");

  weather.replace("SN", "Snow");
  weather.replace("SG", "Snow Grains");

  weather.replace("BR", "Mist");
  weather.replace("IC", "Ice Cristals");
  weather.replace("PL", "Ice Pellets");
  weather.replace("GS", "Small Hail/Snow Pellets");
  weather.replace("GR", "Large Hail");
  weather.replace("FG", "Fog");
  weather.replace("FU", "Smoke");
  weather.replace("HZY", "Hazy");
  weather.replace("HZ", "Haze");
  weather.replace("DS", "Dust Storm");
  weather.replace("DU", "Dust");
  weather.replace("FC", "Funnel Cloud");
  weather.replace("UP", "Unknown Precip");
  weather.replace("SH", " Showers");

  weather.replace("RainG", "Ragged ");

  weather.replace("RF", "Rainfall");
  weather.replace("RMK", "Remark:");

  weather.replace("CB", " Cumulonimbus Clouds");
  weather.replace("CC", " Cirrocumulus Clouds");
  weather.replace("CI", " Cirrus Clouds");
  weather.replace("CS", " Cirostratus Clouds");
  weather.replace("CU", " Cumulus Clouds");
  weather.replace("CF", " Cumulusfractus Clouds");
  weather.replace("SF", " Stratus Fractus Clouds");
  weather.replace("ST", " Stratus Clouds");
  weather.replace("NS", " Nimbostratus Clouds");
  weather.replace("AC", " Altocumulus Clouds");
  weather.replace("AS", " Altostratus Clouds");

  weather.replace("B", " began");                      // Rename Later
  weather.replace("E", " ended");                      // Rename Later

  weather.replace("Clouds1", "Clouds=1 Oktas ");
  weather.replace("Clouds2", "Clouds=2 Oktas ");
  weather.replace("Clouds3", "Clouds=3 Oktas ");
  weather.replace("Clouds4", "Clouds=4 Oktas ");
  weather.replace("Clouds5", "Clouds=5 Oktas ");
  weather.replace("Clouds6", "Clouds=6 Oktas ");
  weather.replace("Clouds7", "Clouds=7 Oktas ");
  weather.replace("Clouds8", "Clouds=8 Oktas ");

  weather.replace("blu blu", "blu");                   // Military
  weather.replace("wht wht", "wht");                   // Military
  weather.replace("grn grn", "grn");                   // Military
  weather.replace("ylo ylo", "ylo");                   // Military
  weather.replace("amb amb", "amb");                   // Military
  weather.replace("redd redd", "redd");                // Military
  weather.replace("blu", "BLUE: Cloud Base>2500ft Visibility>8km ");
  weather.replace("wht", "WHITE: Cloud Base>1500ft Visibility>5km ");
  weather.replace("grn", "GREEN: Cloud Base>700ft Visibility>3.7km ");
  weather.replace("ylo", "YELLOW: Cloud Base>300ft Visibility>1600m ");
  weather.replace("amb", "AMBER: Cloud Base>200ft Visibility>800m ");
  weather.replace("redd", "RED: Cloud Base<200ft Visibility<800m ");

  weather.replace("OC", "Overcast");                   // Rename
  weather.replace("KN", "Broken");                     // Rename
  weather.replace("Conv", "Convective");               // Rename
  weather.replace("TC", "Towering Cumulus Clouds");    // Rename
  weather.replace("CMG", " Becoming:");                // Rename
  weather.replace("oerhead", "Overhead");              // Rename

  weather.replace("northeast", "NE");                  // Change it Back
  weather.replace("east", "E");                        // Change it Back
  weather.replace("var", "Var");                       // Change it Back
  weather.replace("very", "Very");                     // Change it Back
  weather.replace("vi", "Vi");                         // Change it Back
  weather.replace("bank", "Bank");                     // Change it Back
  weather.replace("blowing", "Blowing");               // Change it Back

  weather.replace("began", "Began");                   // Change it Back
  weather.replace("ended", "Ended");                   // Change it Back

  weather.replace("mammatus", "Mammatus");             // Change it Back
  weather.replace("mbedded", "Embedded");              // Change it Back

  weather.replace("awos", "AWOS");                     // Change it Back
  weather.replace("black", "BLACK");                   // Change it Back
  weather.replace("cavok", "CAVOK");                   // Change it Back
  weather.replace("utc", "UTC");                       // Change it Back

  weather.replace("SOG", " Snow on the Ground");

  // Peak Wind
  int search0 = weather.indexOf("Peak wind");
  if (search0 >= 0) {
    int search1 = weather.indexOf("/", search0 + 9);
    String Dir =  weather.substring(search1 - 5, search1 - 2) + "Deg ";
    String Wind = weather.substring(search1 - 2, search1) + "KT at ";
    String At = weather.substring(search1 + 1, search1 + 5) + " ";
    weather.replace("Peak wind", "Peak Wind");
    weather.replace(weather.substring(search1 - 5, search1 + 5), Dir + Wind + At);
  }

  // For Troubleshooting:
  //Serial.println("\t  Raw weather\t." + weather + ".");

  weather.replace(" $", "");                             // Clean Up
  weather.replace("[ ", "");                             // Clean Up
  weather.replace("[", "");                              // Clean Up
  weather.replace(" ]", "");                             // Clean Up
  weather.replace("]", "");                              // Clean Up
  weather.replace("  ", "<br>");                         // Making it readable
  weather.replace("<br><br>", "<br>");                   // Clean Up
  weather.replace("<br> ", "");                          // Clean Up

  // For Troubleshooting:
  //Serial.println("\tFinal weather\t." + weather + ".");

  return weather;                  // Return with Readable Weather
}


// *********** Display One Station LED
void Display_LED(byte index, int wait) {
  if (index == 0)  return;
  leds[index - 1] = 0xaf5800;    // Orange Decoding Data
  FastLED.show();
  delay(wait);
  Set_Cat_LED(index);           // Set Category for This Station LED
  FastLED.show();
}


// *********** DECODE the Station NAME
void Decode_Name(byte i) {
  String Station_name = Stations[i].substring(0, 4);
  int search0 = MetarData.indexOf(Station_name) + 1;      // Start Search from Here
  int search1 = MetarData.indexOf("<site", search0);
  int search2 = MetarData.indexOf("</site", search0);
  if (search1 > 0 && search2 > 0)   Station_name = Station_name + ", " + MetarData.substring(search1 + 6, search2) + ",";
  search1 = MetarData.indexOf("<country", search0);
  search2 = MetarData.indexOf("</country", search0);
  if (MetarData.substring(search1 + 9, search2) == "US") {
    search1 = MetarData.indexOf("<state", search0);
    search2 = MetarData.indexOf("</state", search0);
    if (search1 > 0)   Station_name = Station_name + " " + MetarData.substring(search1 + 7, search2);
  }
  if (search1 > 0 && search2 > 0)   Station_name = Station_name + " " + MetarData.substring(search1 + 9, search2);
  Stations[i] = Station_name.c_str();
  //Serial.println("\tStation[0] code in Decode_Name: " + String(Stations[i]) + ".");
}


// ***********   Display Metar/Show Loops
void Display_Metar_LEDS () {
  int Wait_Time = 5000;             //  Delay after Loop (Seconds x 1000)
  Display_Weather_LEDS (8000);      //  Display Weather

  // ***********   Comment these lines out to suspend a function

  Display_Vis_LEDS (Wait_Time);     //  Display Visibility [Red-Orange-White]
  //Display_Wind_LEDS (Wait_Time);    //  Display Wind Speed [Shades of Aqua]
  Display_Temp_LEDS (Wait_Time);    //  Display Temperatures [Blue-Green-Yellow-Red]
  Display_Alt_LEDS (Wait_Time);     //  Display Altimeter Pressure [Blue-Purple]
}


// ***********  Display Weather on LEDS
void Display_Weather_LEDS (int wait) {
  Display_Cat_LEDS ();             //  Display All Categories

  for (byte index = 1; index < (No_Stations + 1); index++)  {

    if (Sig_Weather[index] != "None")   {        // IF NOT "None" in Sig Wx,  Display Weather
      Station_Num = index;                       // Station # for Server - flash button

      //  Twinkle for Flashing Weather   (index,  red,green,blue, pulses, on, off time)
      if (Sig_Weather[index].indexOf("KT") > 0)     Twinkle(index, 0x00, 0xff, 0xff, 1, 500, 500); //Gusts   Aqua
      if (StationMetar[index].indexOf("FZ") > 0)    Twinkle(index, 0x00, 0x00, 0x70, 3, 300, 400); //Freezing Blue
      if (StationMetar[index].indexOf("FG") > 0)    Twinkle(index, 0x30, 0x40, 0x26, 3, 400, 500); //Fog   L Yellow
      if (StationMetar[index].indexOf("BR") > 0)    Twinkle(index, 0x20, 0x50, 0x00, 3, 500, 500); //Mist  L Green
      if (StationMetar[index].indexOf("DZ") > 0)    Twinkle(index, 0x22, 0x50, 0x22, 3, 300, 300); //Drizzle L Green
      if (StationMetar[index].indexOf("RA") > 0)    Twinkle(index, 0x22, 0xFC, 0x00, 5, 300, 300); //Rain    Green
      if (StationMetar[index].indexOf("HZ") > 0)    Twinkle(index, 0x20, 0x20, 0x20, 3, 400, 400); //Haze    White/purple
      if (StationMetar[index].indexOf("SN") > 0)    Twinkle(index, 0xaf, 0xaf, 0xaf, 4, 300, 600); //Snow    White
      if (StationMetar[index].indexOf("SG") > 0)    Twinkle(index, 0xaf, 0xaf, 0xaf, 4, 300, 600); //Snow    White
      if (StationMetar[index].indexOf("TS") > 0)    Twinkle(index, 0xff, 0xff, 0xff, 4,  10, 900); //Thunder White
      if (StationMetar[index].indexOf("GS") > 0)    Twinkle(index, 0x88, 0x88, 0x00, 4, 100, 800); //S Hail  Yellow
      if (StationMetar[index].indexOf("GR") > 0)    Twinkle(index, 0x88, 0x88, 0x00, 5, 100, 800); //L Hail  Yellow
      if (StationMetar[index].indexOf("IC") > 0)    Twinkle(index, 0x00, 0x00, 0x80, 3, 300, 400); //Ice C   Blue
      if (StationMetar[index].indexOf("PL") > 0)    Twinkle(index, 0x00, 0x00, 0xa0, 3, 300, 400); //Ice P   Blue
      if (StationMetar[index].indexOf("VCSH") > 0)  Twinkle(index, 0x22, 0xfc, 0x00, 2, 300, 300); //Showers Green
      if (StationMetar[index].indexOf("TCU") > 0)   Twinkle(index, 0x30, 0x30, 0x30, 2, 300, 300); //CU      Grey
      if (StationMetar[index].indexOf("CB") > 0)    Twinkle(index, 0x30, 0x30, 0x40, 2, 300, 300); //CB      Grey
      //  Rest of Weather = YELLOW
      if (StationMetar[index].indexOf("DU") > 0)    Twinkle(index, 0x88, 0x88, 0x00, 3, 500, 500); //Dust    Yellow
      if (StationMetar[index].indexOf("FU") > 0)    Twinkle(index, 0x88, 0x88, 0x00, 3, 500, 500); //Smoke   Yellow
      if (StationMetar[index].indexOf("FY") > 0)    Twinkle(index, 0x88, 0x88, 0x00, 3, 500, 500); //Spray   Yellow
      if (StationMetar[index].indexOf("SA") > 0)    Twinkle(index, 0x88, 0x88, 0x00, 3, 500, 500); //Sand    Yellow
      if (StationMetar[index].indexOf("PO") > 0)    Twinkle(index, 0x88, 0x88, 0x00, 3, 500, 500); //Dust&Sand Yellow
      if (StationMetar[index].indexOf("SQ") > 0)    Twinkle(index, 0x88, 0x88, 0x00, 3, 500, 500); //Squalls Yellow
      if (StationMetar[index].indexOf("VA") > 0)    Twinkle(index, 0x88, 0x88, 0x00, 3, 500, 500); //Volcanic Ash Yellow
      if (StationMetar[index].indexOf("UP") > 0)    Twinkle(index, 0x88, 0x88, 0x00, 3, 500, 500); //Unknown Yellow
    }
    Set_Cat_LED (index);                    // Set Category for This Station LED
    FastLED.show();
    delay(100);    // Wait a smidgen
  }
  delay(wait);
}


// ***********  Twinkle for Flashing Weather
void Twinkle (byte index, byte red, byte green, byte blue, byte pulses, int on_time, int off_time) {
  leds[index - 1].r = 0x00;    //  Red   Off
  leds[index - 1].g = 0x00;    //  Green Off
  leds[index - 1].b = 0x00;    //  Blue  Off
  FastLED.show();
  delay(100);    // Wait a smidgen
  for (byte i = 0; i < pulses; i++) {
    leds[index - 1].r = red;   //  Red   On
    leds[index - 1].g = green; //  Green On
    leds[index - 1].b = blue;  //  Blue  On
    FastLED.show();
    delay(on_time);
    leds[index - 1].r = 0x00;  //  Red   Off
    leds[index - 1].g = 0x00;  //  Green Off
    leds[index - 1].b = 0x00;  //  Blue  Off
    FastLED.show();
    delay(off_time);
  }
  delay(300);    // Wait a little bit for flash button
}


// *********** Display ALL Categories
void Display_Cat_LEDS () {
  for (byte i = 1; i < (No_Stations + 1); i++) {
    Set_Cat_LED (i);  // Set Category for This Station LED
  }
  FastLED.show();
  delay(200);        // Wait a smidgen
}


// *********** Set Category for One Station LED
void Set_Cat_LED (byte i)  {
  if (Category[i] == "VFR" ) leds[i - 1] = CRGB::DarkGreen;
  if (Category[i] == "MVFR") leds[i - 1] = CRGB::DarkBlue;
  if (Category[i] == "IFR" ) leds[i - 1] = CRGB::DarkRed;
  if (Category[i] == "LIFR") leds[i - 1] = CRGB::DarkMagenta;
  if (Category[i] == "NA" )  leds[i - 1] = CRGB(20, 20, 0); // DIM  Yellowish
  //if (Category[i] == "NF" )  leds[i - 1] = CRGB(12, 14, 14); // VERY DIM  Blueish
  if (Category[i] == "NF" )  leds[i - 1] = CRGB::Black;
}


// *********** Display Visibility [Red Orange Pink/White]
void Display_Vis_LEDS (int wait) {
  for (byte i = 1; i < (No_Stations + 1); i++) {
    byte hue     =       visab[i] * 5; // (red yellow white)
    byte sat     = 170 - visab[i] * 6;
    byte bright  = 200 - visab[i] * 4;
    leds[i - 1] = CHSV(hue, sat, bright);
    if (visab[i] == 0 || Category[i].substring(0, 1) == "NF")  leds[i - 1] = CHSV( 0, 0, 0);
  }
  FastLED.show();
  delay(wait);
}


// *********** Display Winds [Aqua]
void Display_Wind_LEDS (int wait) {
  for (byte i = 1; i < (No_Stations + 1); i++) {
    byte wind = Wind[i].toInt();
    leds[i - 1] = CRGB(0, wind * 7, wind * 7);
    if (wind == 0 || Category[i].substring(0, 1) == "NF")  leds[i - 1] = CHSV( 0, 0, 0);
  }
  FastLED.show();
  delay(wait);
}


// *********** Display Temperatures [Blue Green Yellow Orange Red]
void Display_Temp_LEDS (int wait) {
  for (byte i = 1; i < (No_Stations + 1); i++) {
    //int hue = 176 - temp[i] * 4.2;     //  purple blue green yellow orange red
    byte hue = 160 - temp[i] * 4;         //  purple blue green yellow orange red
    //Serial.println("Station=" + Stations[i].substring(0, 4) + "  Temp=" + String(temp[i]) + "  Hue=" + String(hue));
    leds[i - 1] = CHSV( hue, 180, 150);
    if (temp[i] == 0 || Category[i].substring(0, 1) == "NF")  leds[i - 1] = CHSV( 0, 0, 0);
  }
  FastLED.show();
  delay(wait);
}


// *********** Display Altimeter Pressure [Blue Purple]
void Display_Alt_LEDS (int wait) {
  for (byte i = 1; i < (No_Stations + 1); i++) {
    byte hue = (altim[i] - 27.92) * 100;     //  blue purple
    leds[i - 1] = CHSV( hue - 30, 130, 150);
    if (altim[i] == 0 || Category[i].substring(0, 1) == "NF")   leds[i - 1] = CHSV( 0, 0, 0);
  }
  FastLED.show();
  delay(wait);
}



//  *********** Go_Server HTML  TASK 2 for Creating Web Pages and Handling Requests
void Go_Server ( void * pvParameters ) {
  int diff_in_clouds = 2750;      // Significant CHANGE IN CLOUD BASE
  float diff_in_press  = 0.045;   // Significant CHANGE IN PRESSURE
  String header;                  // Header for Server
  int sta_n = 1;
  byte wx_flag;
  byte station_flag = 1;
  byte summary_flag = 1;
  float TempF;
  String html_code;

  for (;;) {
    WiFiClient client = server.available();   // Listen for incoming clients
    if (client) {                             // If a new client connects,
      String currentLine = "";                // make a String to hold incoming data from the client
      while (client.connected()) {            // loop while the client's connected
        if (client.available()) {             // if there's bytes to read from the client,
          char c = client.read();             // read a byte, then
          //Serial.write(c);                  // print it out the serial monitor
          header += c;

          if (header == "GET /favicon.ico HTTP/1.1")  break;
          // if the header (from a Computer) then ignore "GET /favicon.ico HTTP/1.1" reply : This causes double write.

          if (c == '\n') {                    // if the byte is a newline character
            // if the current line is blank, you got two newline characters in a row.
            // that's the end of the client HTTP request, so send a response:

            // For Troubleshooting:    Print GET Requests
            //int search0 = currentLine.indexOf("GET /");
            //if (search0 >= 0)   Serial.println("currentLine = " + currentLine);

            if (currentLine.length() == 0) {
              // HTTP headers always Start with a response code (e.g. HTTP/1.1 200 OK)
              // and a content-type so the client knows what's coming wih a return, then a blank line
              html_code = "HTTP/1.1 200 OK";
              html_code += "Content-type:text/html";
              html_code += "Connection: close";
              client.println(html_code);
              //client.print(F("Refresh: 60"));  // refresh the page automatically every 60 sec
              client.println();                  // Send  a blank line

              //Serial.println(header);

              // *******   CHECKING BUTTONS & Handling Requests    ******
              // Checking if AIRPORT CODE was Entered
              int search = header.indexOf("GET /get?Airport_Code=");
              if (search >= 0) {                // From Display Station ANY AIRPORT CODE Button  or  METAR Summary
                String Airport_Code = header.substring(search + 22, search + 26);
                Airport_Code.toUpperCase();    // Changes all letters to UPPER CASE
                sta_n = -1;
                for (int i = 0; i < (No_Stations + 1); i++) {    // Check if Airport_Code is in Data base
                  if (Airport_Code == Stations[i].substring(0, 4))    sta_n = i;
                }

                if (sta_n == -1)  {           // Airport_Code NOT in Data base, ADD as Station[0]
                  sta_n = 0;
                  Stations[sta_n] = Airport_Code.c_str();
                  Stations[sta_n] = Stations[sta_n] + ',';        // Add a Comma
                  StationMetar[sta_n] = "";
                  new_cloud_base[sta_n] = 0;
                  old_cloud_base[sta_n] = 0;
                  altim[sta_n] = 0;
                  old_altim[sta_n] = 0;
                  wDir[sta_n] = "";

                  if (Comms_Flag > 0)Serial.println(Clock + "   WAITING to Get Station Metar  -  Comms In Use");
                  int count = 0;
                  while (Comms_Flag > 0) {    // Checking Communication Flag 1=Active
                    delay(2000);
                    count++;                  // loop for 200 seconds
                    if (count > 100)  return;
                  }
                  // Communication Flag - GetData is NOT running Communication Flag 0
                  Comms_Flag = 1;           // Set Communication Flag 1=Active

                  //Serial.println(Clock + "   Getting Metar NAME for " + String(Airport_Code));
                  GetData("NAME", 0);       // GET Some Metar /NAME
                  //Serial.println(Clock + "   Decode NAME for " + String(Airport_Code));
                  Decode_Name(0);           // Decode Station NAME
                  //Serial.println(Clock + "   Get Metar DATA for " + String(Airport_Code));
                  GetData(Airport_Code, 0); // GET Some Metar /DATA
                  //Serial.println(Clock + "   ParseMetar for " + String(Airport_Code));
                  ParseMetar(0);            // Parse Metar DATA

                  Comms_Flag = 0;           // Set Communication Flag 0=Reset

                  if (Category[0] == "NF")   {   // Checking for Error in Station Name
                    StationMetar[0] = Airport_Code + " : Error in Station Name or NA";
                  }
                }
                summary_flag = 0;
                station_flag = 1;
              }

              // Checking which BUTTON was Pressed
              search = header.indexOf("GET /back HTTP/1.1");
              if (search >= 0) {             // From Display Station BACK Button
                sta_n = sta_n - 1;
                if (sta_n < 0)  sta_n = No_Stations;
                if (Stations[sta_n].substring(0, 4) == "NULL")   sta_n = sta_n - 1;
                if (sta_n < 0)  sta_n = No_Stations;
                summary_flag = 0;
                station_flag = 1;
              }
              search = header.indexOf("GET /next HTTP/1.1");
              if (search >= 0) {             // From Display Station NEXT Button
                sta_n = sta_n + 1;
                if (sta_n > No_Stations) sta_n = 0;
                if (Stations[sta_n].substring(0, 4) == "NULL")   sta_n = sta_n + 1;
                if (sta_n > No_Stations) sta_n = 0;
                summary_flag = 0;
                station_flag = 1;
              }
              search = header.indexOf("GET /flash HTTP/1.1");
              if (search >= 0) {
                sta_n = Station_Num;             // From Display Station FLASH Button
                if (sta_n < 1 || sta_n > No_Stations)  sta_n = 1;
                summary_flag = 0;
                station_flag = 1;
              }
              search = header.indexOf("GET /summary HTTP/1.1");
              if (search >= 0) {             // From Display Station SUMMARY Button
                summary_flag = 1;
                station_flag = 1;
              }


              if (summary_flag == 1)  {
                // *********** DISPLAY SUMMARY ***********
                // Display the HTML web page responsive in any web browser, Page Header, Title, Style & Page Body
                html_code = "<!DOCTYPE html><html><HEAD><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
                html_code += "<TITLE>METAR</TITLE>";
                html_code += "<STYLE> html { FONT-family: Helvetica; display: inline-block; margin: 0px auto; text-align: left;}</STYLE></HEAD>";
                html_code += "<BODY><h2>METAR Summary</h2>";

                if (Last_Up_Min + Update_Interval > 60)   Last_Up_Min = 60 - Update_Interval;
                Count_Down = Last_Up_Min + Update_Interval - Minute;
                if (Count_Down > Update_Interval)   Count_Down = 0;

                html_code += "Summary of Weather Conditions - Last Update : " + Last_Up_Time + " UTC &nbsp&nbsp Next Update in " + String(Count_Down) + " Minutes<BR>";
                html_code += "<FONT SIZE='-1'>Click on Station ID to View Station Details</FONT><BR>";
                client.print(html_code);

                // Display SUMMARY Table ***********
                String Deg = "Deg F";       // Set degrees F or C
                html_code = "<TABLE BORDER='2' CELLPADDING='5'>";
                html_code += "<TR><TD>No:</TD><TD>Station<BR><CENTER>ID</CENTER></TD><TD>CAT</TD><TD>SKY<BR>COVER</TD><TD>VIS<BR>Miles</TD><TD>WIND<BR>from</TD><TD>WIND<BR>Speed</TD><TD>TEMP<BR>";
                html_code += Deg;
                html_code += "</TD><TD>ALT<BR>in&nbspHg</TD><TD>REMARKS</TD></TR>";
                client.print(html_code);

                for (int i = 0; i < (No_Stations + 1); i++) {
                  String color = "<TD><FONT COLOR=";
                  if (Category[i] == "VFR" ) color = color + "'Green'>";
                  if (Category[i] == "MVFR") color = color + "'Blue'>";
                  if (Category[i] == "IFR" ) color = color + "'Red'>";
                  if (Category[i] == "LIFR") color = color + "'Magenta'>";
                  if (Category[i] == "NA")   color = color + "'Black'>";
                  if (Category[i] == "NF")   color = color + "'Orange'>";

                  // Display Station No & ID in SUMMARY
                  if (Stations[i].substring(0, 4) != "NULL")    {  // Skip Line
                    if (i == sta_n )   {
                      html_code = "<TR><TD BGCOLOR='Yellow'>" + String(i) + "</TD><TD BGCOLOR='Yellow'>" + color.substring(4, color.length()) + Stations[i].substring(0, 4) + "</FONT></TD>";
                    } else {
                      // Display LINK:  Makes a request in the header ("GET /get?Airport_Code=XXXX")
                      html_code = "<TR><TD>" + String(i) + "</TD><TD><a href='/get?Airport_Code=" + String(Stations[i].substring(0, 4)) + "'>" + color.substring(4, color.length()) + String(Stations[i].substring(0, 4)) + "</a></FONT></TD>";
                    }
                    client.print(html_code);

                    // Display Category in SUMMARY
                    client.print(color + Category[i] + "</FONT></TD>");

                    // Display Sky Cover in SUMMARY
                    if (Sky[i].length() < 11)   {
                      client.print(color + Sky[i]);
                    } else {
                      if (old_cloud_base[i] > 0 && new_cloud_base[i] <= old_cloud_base[i] - diff_in_clouds)     // Significant DECREASE in Cloud Base Triggers MistyRose
                        client.print(F("<TD BGCOLOR='MistyRose'><FONT COLOR='Purple'>"));   else  client.print(color);
                      client.print(Sky[i].substring(0, 3) + "&nbspat<BR>" + new_cloud_base[i] + "&nbspFt&nbsp");
                      if (old_cloud_base[i] > 0)    {
                        if (new_cloud_base[i]  > old_cloud_base[i])  client.print(F("&uArr;"));  //up arrow
                        if (new_cloud_base[i]  < old_cloud_base[i])  client.print(F("&dArr;"));  //down arrow
                        if (new_cloud_base[i] == old_cloud_base[i])  client.print(F("&rArr;"));  //right arrow
                      }
                    }
                    client.print(F("</FONT></TD>"));

                    // Display Visibility in SUMMARY
                    client.print(color + String(visab[i]) + "</FONT></TD>");

                    // Display Wind Direction in SUMMARY
                    int newDir = wDir[i].toInt();
                    int oldDir = old_wDir[i];
                    if (newDir != 0 && oldDir != 0) {
                      int diff = oldDir - newDir;
                      if (diff < 0)      diff = newDir - oldDir;
                      if (diff > 300)    diff = 360 - diff;
                      if (diff > 30)     client.print("<TD BGCOLOR='MistyRose'><FONT COLOR='Purple'>" + wDir[i]);
                      else   client.print(color + wDir[i]);
                    }  else   client.print(color + wDir[i]);
                    client.print(F("</FONT></TD>"));

                    // Display Wind Speed in SUMMARY
                    client.print(color + Wind[i] + "</FONT></TD>");

                    // Display Temperature in SUMMARY
                    TempF = temp[i] * 1.8 + 32;  // deg F
                    if (temp[i] == 0 && dewpt[i] == 0)  client.print(color + "NA </FONT></TD>"); else if (Deg == "Deg C")   client.print(color + String(temp[i], 1) + "</FONT></TD>"); else
                      client.print(color + String(TempF, 1) + "</FONT></TD>");

                    // Display Altimeter in SUMMARY
                    wx_flag = 0;
                    if (old_altim[i] > 0.1)   {
                      if (altim[i] >= old_altim[i] + diff_in_press)  wx_flag = 1;    // Significant INCREASE in Pressure
                      if (altim[i] <= old_altim[i] - diff_in_press)  wx_flag = 1;    // Significant DECREASE in Pressure
                      if (wx_flag == 1) client.print("<TD BGCOLOR = 'MistyRose'><FONT COLOR='Purple'>" + String(altim[i]));
                      else  client.print(color + String(altim[i]));
                      if (altim[i] > old_altim[i])   client.print(F("<BR>&uArr; ")); //up arrow
                      if (altim[i] < old_altim[i])   client.print(F("<BR>&dArr; ")); //down arrow
                      if (altim[i] == old_altim[i])  client.print(F("<BR>&rArr; ")); //right arrow
                    }  else  {
                      client.print(color + String(altim[i]));
                    }
                    client.print(F("</FONT></TD>"));

                    // Display Metar in SUMMARY
                    if (StationMetar[i].substring(0, 3) == "new")  client.print(F("<TD><FONT COLOR='Purple'>"));  else  client.print(color);
                    client.print(StationMetar[i]);

                    // Display Remark in SUMMARY
                    if (Remark[i].length() > 1)   client.print("<BR><FONT COLOR='Navy'>Remark Available</FONT></TD></TR>");  else  client.print("</FONT></TD></TR>");
                  }
                }
                client.print(F("</TABLE>"));
              }

              if (station_flag == 1)  {

                // *********** DISPLAY STATION  ***********
                html_code = "<!DOCTYPE html><html><HEAD><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
                html_code += "<TITLE>METAR</TITLE>";
                html_code += "<STYLE> html { FONT-family: Helvetica; display: inline-block; margin: 0px auto; text-align: left;}</STYLE></HEAD>";
                html_code += "<BODY><h2>METAR for Station:</h2>";

                if (station_flag == 1 || summary_flag == 1)  {
                  html_code += "<P>For&nbsp:&nbsp" +  Stations[sta_n] + "&nbsp&nbsp#&nbsp&nbsp" + String(sta_n) + "<BR>";

                  // Display BUTTONS: Makes a request in the header ("GET /back HTTP/1.1")
                  html_code += "<a href='/back'><INPUT TYPE='button' VALUE='Previous Station' onClick= sta_n></a>";
                  html_code += "<a href='/flash'><INPUT TYPE='button' VALUE='Flashing LED' onClick= sta_n></a>";
                  html_code += "<a href='/next'><INPUT TYPE='button' VALUE='Next Station' onClick= sta_n></a>";
                  html_code += "&nbsp&nbsp&nbsp&nbsp<a href=/summary><INPUT TYPE='button' VALUE='Summary of Stations' onClick= sta_n></a>";
                  html_code += "<BR>&nbsp&nbsp&nbsp&nbspPress <B>'BUTTON'</B> when <B>LED is Flashing</B><BR>";
                  client.print(html_code);

                  // Display TABLE/FORM: Makes a request in the header ("GET /get?Airport_Code=")
                  html_code = "<FORM METHOD='get' ACTION='get'>";
                  html_code += "<TABLE BORDER='0' CELLPADDING='1'>";
                  html_code += "<TR><TD>ENTER ANY AIRPORT ID CODE:</TD><TD>";
                  html_code += "<INPUT TYPE='text' NAME='Airport_Code' SIZE='5'>";
                  html_code += "</TD></TR></TABLE></FORM>";
                  client.print(html_code);

                  // Update LED and Current Time in STATION
                  Display_LED (sta_n, 300);  // Display One Station LED
                  Update_Time();             //  GET CurrentTime : Hour & Minute

                  if (Last_Up_Min + Update_Interval > 60)   Last_Up_Min = 60 - Update_Interval;
                  Count_Down = Last_Up_Min + Update_Interval - Minute;
                  if (Count_Down > Update_Interval)   Count_Down = 0;

                  String CountDown_Time = "Current Time : " + Clock + " UTC&nbsp&nbsp&nbsp&nbsp&nbsp&nbspNext Update in less than 0 Minutes<BR>";
                  CountDown_Time.replace("than 0", "than " + String(Count_Down));
                  if (Count_Down == 1)  CountDown_Time.replace("Minutes", "Minute");
                  client.print(CountDown_Time);

                  // Display STATION Table  ***********
                  String Bcol = "BORDERCOLOR=";
                  if (Category[sta_n] == "VFR" ) Bcol = Bcol + "'Green'";
                  if (Category[sta_n] == "MVFR") Bcol = Bcol + "'Blue'";
                  if (Category[sta_n] == "IFR" ) Bcol = Bcol + "'Red'";
                  if (Category[sta_n] == "LIFR") Bcol = Bcol + "'Magenta'";
                  if (Category[sta_n] == "NA")   Bcol = Bcol + "'Black'";
                  if (Category[sta_n] == "NF")   Bcol = Bcol + "'Orange'";
                  String color = "<TD><FONT " + Bcol.substring(6, Bcol.length()) + ">";
                  html_code = "<TABLE " + Bcol + " BORDER='3' CELLPADDING='5'>";

                  // Display Flight Category in STATION
                  html_code += "<TR><TD>Flight Category</TD>" + color + "<B>" + String(Category[sta_n]) + "</B></FONT>  for " + String(Stations[sta_n]) + "</TD></TR>";

                  // Display Metar in STATION
                  html_code += "<TR><TD>Station Metar</TD>";
                  if (StationMetar[sta_n].substring(0, 3) == "new" )  html_code += "<TD><FONT COLOR='Purple'>" + String(StationMetar[sta_n]) + "</FONT>"; else  html_code += "<TD>" + String(StationMetar[sta_n]);
                  html_code += "</TD></TR>";

                  // Display Significant Weather in STATION
                  html_code += "<TR>" + color + "Significant Weather<BR><FONT COLOR='Purple'>and Remarks</FONT></TD>" + color + String(Sig_Weather[sta_n]) + "</FONT>";

                  //  Comments for Weather and Cloud Base in STATION
                  if (Sky[sta_n].substring(0, 3) == "OVC")
                    html_code += "<BR><FONT " + Bcol.substring(6, Bcol.length()) + ">Overcast Cloud Layer</FONT>";
                  if (new_cloud_base[sta_n] > 0 && new_cloud_base[sta_n] <= 1200)
                    html_code += "<BR><FONT " + Bcol.substring(6, Bcol.length()) + ">Low Cloud Base</FONT>";
                  if (old_cloud_base[sta_n] > 0 && old_altim[sta_n] > 0)    {
                    if (new_cloud_base[sta_n] > old_cloud_base[sta_n] && altim[sta_n] > old_altim[sta_n])
                      html_code += "<BR><FONT COLOR='Navy'>Weather is Getting Better</FONT>";
                    if (new_cloud_base[sta_n] < old_cloud_base[sta_n] && altim[sta_n] < old_altim[sta_n])
                      html_code += "<BR><FONT COLOR='Navy'>Weather is Getting Worse</FONT>";
                  }

                  //  Remarks in STATION
                  if (Remark[sta_n].length() > 1)
                    html_code += "<BR><FONT COLOR='Purple'>" + String(Remark[sta_n]);
                  else html_code += "<BR><FONT COLOR='Purple'>None</FONT>";

                  //  Sky Cover in STATION
                  html_code += "</TD></TR><TR><TD>Sky Cover</TD>" + color;
                  html_code += String(Sky[sta_n]) + "&nbsp&nbsp&nbsp";
                  if (old_cloud_base[sta_n] > 0)   {
                    if (new_cloud_base[sta_n] > old_cloud_base[sta_n])  {
                      html_code += "&uArr; from ";   //up arrow
                      html_code += String(old_cloud_base[sta_n]);
                      html_code += " Ft";
                    }
                    if (new_cloud_base[sta_n] < old_cloud_base[sta_n])  {
                      html_code += "&dArr; from ";   //down arrow
                      if (old_cloud_base[sta_n] > 99998)   html_code += "CLEAR";
                      else {
                        html_code += String(old_cloud_base[sta_n]) + " Ft";
                      }
                    }
                    if (new_cloud_base[sta_n] == old_cloud_base[sta_n])  html_code += "&rArr; No change";   //right arrow
                  }
                  //  Significant Change in Cloud Base
                  if (old_cloud_base[sta_n] > 0 && old_altim[sta_n] > 0)    {
                    if (new_cloud_base[sta_n] >= old_cloud_base[sta_n] + diff_in_clouds) // INCREASE
                      html_code += "<BR><FONT COLOR='Navy'>Significant Increase in Cloud Base";
                    if (new_cloud_base[sta_n] <= old_cloud_base[sta_n] - diff_in_clouds) // DECREASE
                      html_code += "<BR><FONT COLOR='Orange'>Significant Decrease in Cloud Base";
                  }
                  html_code += "</FONT></TD></TR>";

                  //  Visibility in STATION
                  html_code += "<TR><TD>Visibility</TD>" + color + String(visab[sta_n]) + " Statute miles</FONT></TD></TR>";

                  //  Wind in STATION
                  html_code += "<TR><TD>Wind from</TD><TD>";
                  int newDir = wDir[sta_n].toInt();
                  int oldDir = old_wDir[sta_n];
                  if (Wind[sta_n] == "CALM")   html_code += String(Wind[sta_n]);
                  else  html_code += String(wDir[sta_n]) + " Deg at " + String(Wind[sta_n]);
                  if (newDir > 0 && oldDir > 0) {
                    int diff = oldDir - newDir;
                    if (diff < 0)      diff = newDir - oldDir;
                    if (diff > 300)    diff = 360 - diff;
                    if (diff > 30) {
                      html_code += "<FONT COLOR='Orange'> : Significant Change from " + String(old_wDir[sta_n]) + " Deg</FONT>";
                    }  else  {
                      if (newDir != oldDir)   html_code += " : Previously " + String(old_wDir[sta_n]) + " Deg";
                      if (newDir == oldDir)   html_code += " : No change";
                    }
                  }
                  html_code += "</TD></TR>";

                  //  Temperature in STATION
                  TempF = temp[sta_n] * 1.8 + 32;  // deg F
                  html_code += "<TR><TD>Temperature</TD><TD>";
                  if (temp[sta_n] <= 0)  html_code += "<FONT COLOR='Blue'>";  else   html_code += "<FONT COLOR='Black'>";
                  if (temp[sta_n] == 0 && dewpt[sta_n] == 0)   html_code += "NA</FONT>";  else
                    html_code += String(temp[sta_n], 1) + " Deg C&nbsp&nbsp&nbsp:&nbsp&nbsp&nbsp" + String(TempF, 1) + " Deg F</FONT>";
                  if (temp[sta_n] >= 35.0)   html_code += "<FONT SIZE='-1' FONT COLOR='Red'><I> and HOT</I></FONT>";

                  //  Dew Point in STATION
                  html_code += "</TD></TR><TR><TD>Dew Point</TD><TD>";
                  if (dewpt[sta_n] <= 0)  html_code += "<FONT COLOR='Blue'>";  else   html_code += "<FONT COLOR='Black'>";
                  if (dewpt[sta_n] == 0)   html_code += "NA"; else
                    html_code += String(dewpt[sta_n], 1) + " Deg C";
                  if (dewpt[sta_n] >= 24.0)   html_code += "<FONT SIZE='-1' FONT COLOR='Red'><I> and Muggy</I></FONT>";
                  html_code += "</TD></TR>";

                  //  Altimeter in STATION
                  html_code += "</FONT><TR><TD>Altimeter</TD><TD>" + String(altim[sta_n]) + "&nbspin&nbspHg&nbsp&nbsp";
                  if (old_altim[sta_n] > 0)  {
                    if (altim[sta_n] > old_altim[sta_n]) {
                      if (altim[sta_n] > old_altim[sta_n] + diff_in_press)  html_code += "<FONT COLOR='Orange'>Significant Change&nbsp</FONT>";
                      html_code += "&uArr; from ";   //up arrow
                      html_code += String(old_altim[sta_n], 2);
                    }
                    if (altim[sta_n] < old_altim[sta_n]) {
                      if (altim[sta_n] < old_altim[sta_n] - diff_in_press)   html_code += "<FONT COLOR='Orange'>Significant Change&nbsp</FONT>";
                      html_code += "&dArr; from ";   //down arrow
                      html_code += String(old_altim[sta_n], 2);
                    }
                    if (altim[sta_n] == old_altim[sta_n])   html_code += "&rArr; Steady";   //right arrow
                  }
                  html_code += "</TD></TR>";

                  //  Elevation & Density Altitude in STATION
                  float Elevation = elevation[sta_n] * 3.28; //feet
                  html_code += "<TR><TD>Elevation</TD><TD>" + String(elevation[sta_n], 1) + " m&nbsp&nbsp&nbsp:&nbsp&nbsp&nbsp" + String(Elevation, 1) + " Ft</TD></TR>";
                  float PressAlt = Elevation + (1000 * (29.92 - altim[sta_n]));                              // Ft
                  float DensityAlt = PressAlt + (120 * (temp[sta_n] - (15 - abs(2 * Elevation / 1000))));    // Ft
                  html_code += "<TR><TD>Estimated Density Altitude</TD><TD>";
                  if (temp[sta_n] == 0 || altim[sta_n] == 0)   html_code += "NA"; else  html_code += String(DensityAlt, 1) + " Ft</TD></TR>";

                  html_code += "</TABLE>";             //  End of Table in Station
                  client.print(html_code);

                  // Display Foooter and Close
                  html_code = "<BR><FONT SIZE='-1'>File Name: " + Filename();
                  html_code += "<BR>H/w Address : " + local_hwaddr;
                  html_code += "<BR>Url Address &nbsp: " + local_swaddr;
                  html_code += "<BR><B>Dedicated to: F. Hugh Magee</B>";
                  html_code += "</FONT></BODY></html>";
                  client.print(html_code);
                }
              }

              client.println();    // The HTTP response ends with another blank line
              break;               // Break out of the while loop
            } else {               // if you got a newline, then clear currentLine
              currentLine = "";
            }
          } else if (c != '\r') {  // if you got anything else but a carriage return character,
            currentLine += c;      // add it to the end of the currentLine
          }
        }                          // Client Available
      }                            // While Client Connected
    }                              // If new client Connects
    header = "";                   // Clear the header
    client.stop();                 // Close the Connection
  }
}