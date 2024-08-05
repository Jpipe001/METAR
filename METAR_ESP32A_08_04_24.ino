
/* 08/04/24  Latest Software on Github : https://github.com/Jpipe001/METAR    <<   Check for Updates

  METAR Reporting with LEDs and Local WEB SERVER
  In Memory of F. Hugh Magee, brother of John Magee author of poem HIGH FLIGHT.
  https://en.wikipedia.org/wiki/John_Gillespie_Magee_Jr.                         ~  GOOD READ
  https://joeclarksblog.com/wp-content/uploads/2012/11/high_flight-790x1024.jpg  ~  Poem HIGH FLIGHT

  https://youtu.be/Yg61_kyG2zE    HomebuiltHELP; The video that started me on this project.
  https://youtu.be/xPlN_Tk3VLQ    Getting Started with ESP32 video from DroneBot Workshop.

  https://aeronav.faa.gov/visual/            FAA Wall Chart downloads and then edit to suit.
  https://SkyVector.Com                      Good for locating METAR Reporting Stations.
  ###################################################################################################

  This software, the ideas and concepts is Copyright (c). All rights to this software are reserved.

  Any redistribution or reproduction of any part or all of the contents in any form is prohibited other than the following:
  1. You may print or download to a local hard disk extracts for your personal and non-commercial use only.
  2. You may copy the content to individual third parties for their personal use, but only if you acknowledge the author John Pipe as the source of the material.
  3. You may not, except with my express written permission, distribute or commercially exploit the content.
  4. You may not transmit it or store it in any other website or other form of electronic retrieval system for commercial purposes.

  The above copyright ('as annotated') notice and this permission notice shall be included in all copies or substantial portions of the Software and where the
  software use is visible to an end-user.

  THE SOFTWARE IS PROVIDED "AS IS" FOR PRIVATE USE ONLY, IT IS NOT FOR COMMERCIAL USE IN WHOLE OR PART OR CONCEPT. FOR PERSONAL USE IT IS SUPPLIED WITHOUT WARRANTY
  OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHOR OR COPYRIGHT HOLDER BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

  ###################################################################################################

  REMARKS/COMMENTS:
  Created by John Pipe with help from David Bird.
  FULLY Configurable for YOUR application. (See the list of all Stations Codes, link above).
  No of Airports = 100 tested 6/24/20 (60+ only possible with external power).
  When Error, successfully recovers. If not, press "RESET" button on ESP32.
  The code is stored on the ESP32 Development Board 2.4GHz WiFi+Bluetooth Dual Mode,
  once the code is downloaded, it will run by it's self and will automatically restart, even after a power off.
  ESP32 requires internet access (Network or Hot Spot) to download METARS.

  A computer, with FREE software https://www.arduino.cc/en/Main/Software , is needed to initially configure,
  download software and (optionally) display any program messages using the (built in) Serial Monitor.
  (Watch Getting Started with ESP32 video from DroneBot Workshop, link above).

  Updates METARS approximately every ten minutes, so nearly REAL TIME data, from AVIATIONWEATHER.GOV.
  A set of WS2811 or WS2812 LEDS show all station CATEGORIES (similar to the HomebuiltHELP video, link above).
  Then cycles through all the stations and flashes individually for:
  Wind Gusts(Cyan)[suspendable], Precipitation(Green/White), Ice(Blue), Other(Yellow).
  Then displays GRADIENTS [suspendable] for all stations, for Visibility [White to Red], Wind Direction [Yellow to Red] ~ Suspended,
  Temperature Gradient [Blue Green Yellow Orange Red] and Altimeter Pressure Gradient [Blue to Purple].

  DISPLAYS: Viewable with a computer or cell phone connected on the SAME network:
  SUMMARY html gives a colorful SUMMARY overview and STATION html shows DECODED METAR information and much MORE.

  NOTE: To view these, you need the http address which is shown at start up, if the Serial Monitor is switched on.
  or Click on this Logical Name Test Link  http://metar.local

  Makes a GREAT Christmas Tree Chain of Lights, TOO (and a Good Conversation Piece).

  OPTIONAL:
   In Main Loop: Turn LEDS OFF at NIGHT  5PM to 8AM ~ Local Time ~ Enabled
   In Main Loop: Reduce LED Power usage while getting Data ~ Disabled
   In Display_Metar_LEDS: Gradient Effect ~ Partially Enabled
   In Single_Metar: Print Stations that were Updated  ~ Disabled
   In Checking AIRPORT CODE: Airport Code can be assigned to any Station No ~ Disabled Set to 0
   In Display SUMMARY Table  ~  Change Units of Temperature "Deg F" & Altimeter "in Hg"

  MADE THINGS A LITTLE BETTER, BUG FIXES, IMPROVEMENTS, REPAIRS TO TIME-SPACE CONTINUUM, ETC, ETC.
  Includes: Decoded Metar, Current UTC Time, Elevation Ft, Estimated Density Altitude Ft, Temp & Pressure Units can be changed.
  ANY Airport code may be used in the Worldwide FAA Data Base(see above link), but optimized for US airports.

  Questions, Comments or Suggestions "Mail to" - Jpipe001@Gmail.com, Your Feedback is Appreciated and I can send you updates.

  RECENT CHANGES:
  Revised Link to Aviationweather.gov 10/17/23
  Added to Sky Cover 11/09/23
  Added Get FAA Station Name if Station Name = GETNAME 06/10/24
  Added Max / Min Temperature in Station Display  06/25/24
  Added Radar link in Station Display  07/25/24
  Swatted a few Bugs, Minor Improvements to make things Better 08/04/24
*/

// Include the following Libaries:
#include <Arduino.h>
#include "Over_The_Air.h" // Over The Air Software Updates *** Put This File in the SAME Folder as METAR ***
#include <FastLED.h>      // FastLED  by Daniel Garcia
#include <WiFiMulti.h>    // WifiMulti_Generic  by Khoi Hoang
WiFiMulti wifiMulti;
#include <HTTPClient.h>   // HttpClient  by Adrian McEwen
#include <ESPmDNS.h>      // MDNS_Generic  by Khoi Hoang
WiFiServer server(80);    // Set web server port number to 80


//  ################   CONFIGURE YOUR SETTINGS HERE  ################

// Please Modify the "Your Network Name" and "Your Network Password" To suit your Project
const char*      ssid = "Your Network Name";          // your network SSID (Name)
const char*  password = "Your Network Password";      // your network Password


// Please Modify the "No_Stations and LED_TYPE and COLOR_ORDER" To suit your Project
// Set Up LEDS   ***   Connect LEDS to Ground & 5 Volts on ESP32 pins (There are Varients, so Check)
#define No_Stations          52      // Number of LEDs / Number of Stations
#define LED_TYPE         WS2812      // WS2811 or WS2812 or NEOPIXEL
#define COLOR_ORDER         GRB      // WS2811 are RGB or WS2812 are GRB or NEOPIXEL are CRGB
#define DATA_PIN              5      // Connect LED Data Line to pin D5/P5/GPIO5  *** With CURRENT LIMITING 330 Ohm Resistor in Line ***
#define NUM_LEDS    No_Stations      // Number of LEDs
#define BRIGHTNESS           16      // Master LED Brightness (<12=Dim 16~20=ok >20=Too Bright) Set as required
#define FRAMES_PER_SECOND   120
CRGB leds[NUM_LEDS];                 // Color Order for LEDs ~ CRGB for Displaying Gradients


// Please Modify the "List of Stations" To suit your Project.
/*   This is the Order of LEDs; "NULL" is for NO airport or LED not used.
   Station ID in CAPITALS are REQUIRED !!
   First SIX Characters are REQUIRED !! (including the first Comma and the Space)

   Edit or Over type your Station ID and METAR Reporting Station (Airport) Name.
   and include the "quotes" and the "first and last comma" (SYNTAX is IMPORTANT).

   You can use the FAA Airport Name by using  "GETNAME"  or  Customise the Station Name. (See below)
   "GETNAME" is useful if you don't want to enter all the airport names (The easy way).
   Remember:  SPACING and SYNTAX is IMPORTANT.

   https://SkyVector.Com   ~~   Good for locating METAR Reporting Stations.

   Note: You can have Dummy LED that is Connected in the chain but Not Used. (See below)
*/
// Define List of Stations              ~~~~~   Some Worldwide Examples
std::vector<String> PROGMEM Stations {  //   << Set Up   - Do NOT modify this line.
  "NULL, STATION NAME",                 // 0 << Reserved - Do NOT modify this line.
  "KCHA, GETNAME",                      // 1 << START modifying from here
  "KRMG, GETNAME",                      // 2    Use FAA Station Name ~ The easy way
  "KVPC, GETNAME",                      // 3    Use FAA Station Name ~ The easy way
  "KATL, GETNAME",                      // 4    Use FAA Station Name ~ The easy way
  "KCTJ, GETNAME",                      // 5    Use FAA Station Name ~ The easy way
  "KLGC, GETNAME",                      // 6    Use FAA Station Name ~ The easy way
  "KCSG, GETNAME",                      // 7    Use FAA Station Name ~ The easy way
  "KMCN, GETNAME",                      // 8    Use FAA Station Name ~ The easy way
  "KCKF, GETNAME",                      // 9    Use FAA Station Name ~ The easy way
  "KABY, GETNAME",                      // 10   Use FAA Station Name ~ The easy way
  "KTLH, TALLAHASSEE, FL",              // 11
  "KVLD, VALDOSTA, GA",                 // 12
  "KAYS, WAYCROSS, GA",                 // 13
  "KJAX, JACKSONVILLE, FL",             // 14
  "KBQK, BRUNSWICK, GA",                // 15
  "KSAV, SAVANNAH, GA",                 // 16
  "KTBR, GETNAME",                      // 17
  "KAGS, AUGUSTA, GA",                  // 18
  "KAHN, ATHENS, GA",                   // 19
  "KCEU, CLEMSON, GA",                  // 20
  "KJES, JESUP (my home town), GA",     // 21
  "KBHC, BAXLEY, GA",                   // 22
  "KAZE, HAZLEHURST, GA",               // 23
  "KCAE, COLUMBIA, SC",                 // 24
  "KVDI, VIDALIA RGNL, GA",             // 25
  "KAFP, ANSON CO, NC",                 // 26
  "KBDU, BOULDER, CO",                  // 27
  "KCVG, CINCINNATI, OH",               // 28
  "KDCA, WASHINGTON National, DC",      // 29
  "KORD, CHICAGO O'HARE, IL",           // 30
  "KMEM, MEMPHIS, TN",                  // 31
  "KMSY, NEW ORLEANS, LA",              // 32
  "KSDF, LOUISVILLE, KY",               // 33
  "KBOS, BOSTON, MA",                   // 34
  "KCLT, CHARLOTTE, NC",                // 35
  "KBNA, NASHVILLE, TN",                // 36
  "EGPN, DUNDEE, SCOTLAND",             // 37
  "EGPH, EDINBURGH, SCOTLAND",          // 38
  "EGPE, INVERNESS, SCOTLAND",          // 39
  "EGLL, LONDON HEATHROW, UK",          // 40
  "EGKK, LONDON GATWICK, UK",           // 41
  "EGCC, MANCHESTER, UK",               // 42
  "EGDM, BOSCOMBE DOWN, UK",            // 43
  "EGYP, MOUNT PLEASANT, Falkland Islands", // 44
  "EHAM, AMSTERDAM SCHIPHOL",           // 45
  "LFSB, GETNAME",                      // 46   Use FAA Station Name
  "KORL, ORLANDO EXEC, FL",             // 47
  "KPBI, WEST PALM BEACH, FL",          // 48
  "KFXE, FT LAUDERDALE EXE, FL",        // 49
  "KFLL, FT LAUDERDALE INT, FL",        // 50
  "KHWO, NORTH PERRY, FL",              // 51
  "KMIA, MIAMI, FL",                    // 52
  "KEYW, KEY WEST, FL",                 // 53
  "KHXD, HILTON HEAD, SC",              // 54
  "KTTS, NASA SPACE LAUNCH FACILITY, FL", // 55
  "KDAB, DAYTONA BEACH, FL",            // 56
  "NULL, NOT USED",                     // 57  You can have Dummy LED that is Connected in the chain but Not Used.
  "KCHS, CHARLESTON, SC",               // 58
  "KMYR, MYRTLE BEACH, SC",             // 59
  "KDAL, DALLAS/LOVE FIELD, TX",        // 60
};                                      // <<  Do NOT delete this line.

/*  Note: In Main_Loop  TASK1: Will optionally turn LEDS OFF at night:  5PM to 8AM ~ Local Time.
    Note: In Display LEDS for Metar/Show Loops: You can optionally suspend a LED Gradient function.
    Note: Temp & Pressure Units: Can be changed See ~ Display SUMMARY Table.
    Note: In Decode_Data: You can optionally Print Updated Stations See ~ To print UPDATED Stations  */
//  ################   END OF SETTINGS  ################


// Set Up Time Server : UTC Time
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;                   // UTC Time : Time Zone 60*60*0 Hrs
const int   daylightOffset_sec = 0;              // UTC Time : Offset 60*60*0 Hr
struct tm timeinfo;                              // Time String "%A, %B %d %Y %H:%M:%S"

PROGMEM String         Metar[No_Stations + 1];   // Metar Codes including "new"
PROGMEM String        Remark[No_Stations + 1];   // Remark Decoded used in Station Display
PROGMEM String   Remark_Code[No_Stations + 1];   // Remark Codes used in Summary Display
PROGMEM String   Sig_Weather[No_Stations + 1];   // Significant Weather
PROGMEM String          Wind[No_Stations + 1];   // Wind speed
PROGMEM String          wDir[No_Stations + 1];   // Wind direction
PROGMEM String           Sky[No_Stations + 1];   // Sky_cover
PROGMEM String     Sky_cover[No_Stations + 1];   // Sky_cover
PROGMEM String      Category[No_Stations + 1];   // VFR  MVFR  IFR  LIFR  NA   NF
PROGMEM String         Visab[No_Stations + 1];   // Visibility
PROGMEM String  Max_Min_Temp[No_Stations + 1];   // Max/Min Temperature deg C
PROGMEM String      Lat_Long[No_Stations + 1];   // Lat_Long in text
PROGMEM float          TempC[No_Stations + 1];   // Temperature deg C
PROGMEM float         DewptC[No_Stations + 1];   // Dew point deg C
PROGMEM float       SeaLpres[No_Stations + 1];   // Sea Level Pressure
PROGMEM float          Altim[No_Stations + 1];   // Altimeter setting
PROGMEM float      old_Altim[No_Stations + 1];   // Previous altimeter setting
PROGMEM float      Elevation[No_Stations + 1];   // Elevation setting
PROGMEM int         old_wDir[No_Stations + 1];   // Previous Wind direction
PROGMEM int   new_cloud_base[No_Stations + 1];   // New Cloud Base
PROGMEM int   old_cloud_base[No_Stations + 1];   // Previous Cloud Base


#define LED_BUILTIN 2         // ON Board LED GPIO 2
PROGMEM String MetarData;     // Raw METAR data for Group of Stations
char Clock[] = "HH:MM";       // Clock  "HH:MM"
byte Hour = 0;                // Latest Hour
byte Minute = 0;              // Latest Minute
String Last_Up_Time;          // Last Update Time  "HH:MM"
byte Last_Up_Min = 0;         // Last Update Minute
byte Group_of_Stations = 18;  // Download a Group of <28 Stations at a time, if connection errors - reduce this
byte Update_Interval = 10;    // Updates Data every 10 Minutes (Don't overload AVIATIONWEATHER.GOV)
byte Count_Down = 0;          // Count to Next Update
byte Station_Num = 1;         // Station # for Server - flash button
int httpCode;                 // Error Code
byte Comms_Flag = 0;          // Communication Flag
String HW_addr;               // WiFi local hardware Address
String SW_addr;               // WiFi local software Address
TaskHandle_t Task1;           // Main_Loop, Core 0
TaskHandle_t Task2;           // Go_Server, Core 1
String ShortFileName;         // Shortened File Name
// To Invoke HTML Display/Web Page after downloading code with a Stable LOGICAL Address, because hardware address may change.
const char* ServerName = "metar";      // Logical Address  Test Link: http://metar.local


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);     // The Onboard LED
  Serial.begin(115200);
  delay(4000);         // Time to press "Clear output" in Serial Monitor

  Serial.printf("\nMETAR Reporting with LEDs and Local WEB SERVER\n");
  ShortFileName = String(__FILE__).substring(String(__FILE__).lastIndexOf("\\") + 1);       // Shortened File Name
  if (ShortFileName == __FILE__) ShortFileName = String(__FILE__).substring(String(__FILE__).lastIndexOf("/") + 1);  // Shortened File Name Using Raspberry Pi
  Serial.printf("Long File Name   ~ %s\n", __FILE__);
  Serial.printf("Short File Name  ~ %s\n", ShortFileName.c_str());
  Serial.printf("Date Compiled    ~ %s\n\n", __DATE__);

  // Initialize the WiFi network settings.
  WiFi.begin(ssid, password);
  WiFi.mode(WIFI_STA);

  if (String(ssid) == "iPhone")  Serial.printf("**  If iPhone DOESN'T Connect:  Check WiFi ~ ON & Select HOT SPOT on iPhone  **\n");

  // CONNECT to WiFi network:
  Serial.printf("WiFi Connecting to %s  ", ssid);
  int count = 0;
  while (WiFi.status() != WL_CONNECTED)   {
    delay(300);    // Wait a little bit
    Serial.printf(".");
    count++;
    if (count > 80) break;    // Loop for 80 tries
  }
  if (count > 80)   Serial.printf("\n ~ Can't Connect to Network\n\n"); else  Serial.printf(" ~ Connected Successfully\n");

  SetupOTA(ServerName, ssid, password);       // In Over_The_Air.h

  if (!MDNS.begin(ServerName) || count > 80) {     // Start mDNS with ServerName
    Serial.printf("\nSOMETHING WENT WRONG\nError setting up MDNS responder !\nProgram Halted  ~  Check Network Settings !!\n");
    while (1) {
      delay(1000);                   // STAY HERE !!
    }
  }

  // Print the Signal Strength:
  long rssi = WiFi.RSSI() + 100;
  Serial.printf("Signal Strength = %ld", rssi);
  if (rssi > 50)  Serial.printf(" (>50 - Good)\n");  else   Serial.printf(" (Could be Better)\n");

  Init_LEDS();         // Initialize LEDs

  Serial.printf("*******************************************\n");

  // Initialize the WiFi settings to Get data from Server.
  wifiMulti.addAP(ssid, password);

  Serial.printf("To View Decoded Station METARs from a Computer or Other Device\n  connected to the %s WiFi Network.\n", ssid);

  HW_addr = "http://" + WiFi.localIP().toString();      // IP Address
  Serial.printf("\n\t\tUse IP Address \t: %s\n", HW_addr.c_str());

  SW_addr = "http://" + String(ServerName) + ".local";  // Logical Address (ServerName)
  Serial.printf("MDNS started ~\tOr Use Address \t: %s\n", SW_addr.c_str());

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);   // Get UTC Time from Server
  Update_Time();          // Set Up Time : Hour & Minute

  char TimeChar[50];
  strftime(TimeChar, sizeof(TimeChar), "%A, %B %d %Y %H:%M - UTC", &timeinfo);
  Serial.printf("Date & Time :\t%s\n\n", TimeChar);

  //Task1 = Main_Loop() function, with priority 1 and executed on core 0 for Getting Data and Display Leds
  xTaskCreatePinnedToCore( Main_Loop, "Task1", 10000, NULL, 1, &Task1, 0);
  delay(200);          // Wait a smidgen

  //Task2 = Go_Server() function, with priority 1 and executed on core 1 for Creating Web Pages and Handling Requests
  xTaskCreatePinnedToCore( Go_Server, "Task2", 10000, NULL, 1, &Task2, 1);
  delay(200);          // Wait a smidgen

  Serial.printf("Dual Core Initialized\n");
  server.begin();      // Start the web server
  Serial.printf("Web Server Started\n");
  Serial.printf("Set Up Complete\n");
  Serial.printf("*******************************************\n");


  // *********** GET FAA Station Name if Station Name = GETNAME
  String List = "";
  for (int n = 1; n <= No_Stations;  n++) {
    if (Stations[n].substring(6, 13) == "GETNAME")  {
      List += Stations[n].substring(0, 5);
    }
  }
  List = List.substring(0, List.length() - 1);    // Remove last "comma"

  Check_Coms();                 // Checking Communication Flag
  Comms_Flag = 1;               // Set Communication Flag 1=Active
  GetData("NAME", List, 1);     // GET Some Metar Routine ~ Station NAME
  Comms_Flag = 0;               // Set Communication Flag 0=Reset

  for (int n = 1; n <= No_Stations;  n++) {
    if (Stations[n].substring(6, 13) == "GETNAME")  {
      Decode_Name(Stations[n].substring(0, 4), n);         // Decode Station Name Routine
      Serial.printf("%s\tNo:%d\tName Decoded\t%s\n", Clock, n, Stations[n].c_str());
    }
  }
  Serial.printf("===========================================\n\n");
}


void loop() {
  // Nothing to do here
}


//  *********** Main_Loop  TASK1 for Getting Data and Display Leds
void Main_Loop( void * pvParameters ) {
  for (;;) {
    Last_Up_Time = Clock;
    Last_Up_Min = Minute;
    Update_Time();                   // Update Current Time : Hour & Minute

    //  Optional Turn LEDS OFF at NIGHT  5PM to 8AM ~ Local Time
    //  US Eastern Time zone:  UTC -4 (UTC -5 during Standard Time)
    byte  On_Hr = 12;     // 8AM =  8:00Hr + 4 = 12 Hr UTC  ~ ON
    byte Off_Hr = 21;     // 5PM = 17:00Hr + 4 = 21 Hr UTC  ~ OFF : Suspend Function
    // Off_Hr = 24;          // ****   Comment Out or Delete this line, To Invoke This Fuction

    if (Hour == Off_Hr)  {                   // Check for Off Hour
      while (Hour != On_Hr)  {               // Loop until On Hour
        Display_Black_LEDS();                // Set All LEDS to Black
        delay(Update_Interval * 60000);      // Wait Update_Interval in milli Seconds
        Update_Time();                       // Update Current Time : Hour & Minute
      }
    }

    if (Last_Up_Min + Update_Interval > 60)   Last_Up_Min = 60 - Update_Interval;
    Count_Down = Last_Up_Min + Update_Interval - Minute;

    Check_Coms();                    // Checking Communication Flag
    Comms_Flag = 1;                  // Set Communication Flag 1=Active
    GetAllMetars();                  // Download All Metars
    Comms_Flag = 0;                  // Set Communication Flag 0=Reset

    Serial.printf("\n%s\tMetar Updated\t\tNext Update in %d Mins\n\n", Clock, Count_Down);

    while (Count_Down > 0)    {
      Display_Metar_LEDS();          // Display Station Metar/Show Loops
      Update_Time();                 // Update CurrentTime : Hour & Minute
      Count_Down = Last_Up_Min + Update_Interval - Minute;
      if (Count_Down > Update_Interval)   Count_Down = 0;
    }
  }
}


// ***********   Display LEDS for Metar/Show Loops
void Display_Metar_LEDS() {
  Display_Cat_LEDS ();              //  Display LEDS for All Categories  ~ Do NOT Delete
  Display_Weather_LEDS (10);        //  Display LEDS for Twinkle Weather ~ Suspendable
  delay(8000);                      //  Delay after Display Weather (Seconds * 1000) ~ Change this but Do NOT Delete

  // ***********   Comment these lines out to suspend a Gradient function

  int Wait_Time = 7000;             //  Delay after Function (Seconds * 1000) ~ Change this but Do NOT Delete
  //Display_Vis_LEDS (Wait_Time);     //  Display LEDS for Visibility [Red-Pink-White] ~ Suspended
  //Display_Wind_Dir_LEDS (Wait_Time);    //  Display LEDS for Wind Direction [Yellow-Red] ~ Suspended
  //Display_Wind_LEDS (Wait_Time);    //  Display LEDS for Wind Speed [Shades of Aqua] ~ Suspended
  Display_Temp_LEDS (Wait_Time);    //  Display LEDS for Temperatures [Blue-Green-Yellow-Red] ~ Suspendable
  Display_Alt_LEDS (Wait_Time);     //  Display LEDS for Distribution of Pressure [Blue-Purple] ~ Suspendable
}


// *********** Set/Get Current Time Clock, Hour, Minute
void Update_Time() {
  if (!getLocalTime(&timeinfo)) {
    Serial.printf("\n\t\t**  FAILED to Get Time  **\n");
    Network_Status ();            // WiFi Network Error
  }
  char TimeChar[6];
  strftime(TimeChar, sizeof(TimeChar), "%H:%M", &timeinfo);
  strcpy(Clock, TimeChar);
  String Time = TimeChar;                     // String  "HH:MM"
  Hour   = Time.substring(0, 2).toInt();      // Byte     HH
  Minute = Time.substring(3, 5).toInt();      // Byte     MM
}


// *********** Initialize LEDs
void Init_LEDS() {
  // Set up the strip configuration
  //FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, No_Stations); // For WS2811
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, No_Stations).setCorrection(TypicalLEDStrip); // For WS2812
  //FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);       // For NEOPIXEL
  FastLED.setBrightness(BRIGHTNESS);           // Set master brightness control (<12=Dim 20=ok >20=Too Bright)
  Display_Black_LEDS();                        // Set All LEDS to "Black"
  Serial.printf("LEDs Initialized for No_Stations = %d\n", No_Stations);
}


// *********** Reset LEDS to Black (Start Up & Turn LEDS OFF at NIGHT)
void Display_Black_LEDS() {
  fill_solid(leds, No_Stations, CRGB::Black);  // Set All leds to Black
  FastLED.show();
  delay(50);    // Wait a smidgen
}


// *********** GET All Metars (in Chunks)
void GetAllMetars() {                    // Get a METAR DATA Group of Stations <20 at a time
  String Type = "METAR";
  for (byte j = 0; j < No_Stations; j = j + 1 + Group_of_Stations) {
    byte Start  = j;
    byte Finish = Start + Group_of_Stations;
    if (Finish > No_Stations)  Finish = No_Stations;
    String List = "";
    for (byte i = Start; i <= Finish; i++) {        // Create a list of Stations
      List += Stations[i].substring(0, 5);
    }
    List = List.substring(0, List.length() - 1);    // Remove last "comma"
    GetData(Type, List, Start);               // GetData Routine ~ Get Some Metar Data (Group of Stations at a time)
    for (int i = Start; i <= Finish; i++) {
      ParseMetar(i);                   // ParseMetar Routine ~ One Station at a time
    }
  }
}


// *********** GET Some Metar Data/Name Group of Stations at a time
void GetData(String Type, String List, int i) {
  MetarData = "";                     // Reset Raw Data for Group of Stations
  String url = "";                    // Reset url string

  // To get Station NAME Information from "Go_Server"
  // Revised Test Link: https://aviationweather.gov/api/data/stationinfo?ids=KORD,KTBR
  String Host_Name = "https://aviationweather.gov/api/data/stationinfo?ids=";
  // Format   = Host_Name + "STATION ID"

  // To get Station DATA used for MetarData from "GetAllMetars" or "Go_Server"
  // Revised Test Link: https://aviationweather.gov/cgi-bin/data/metar.php?datasource=metars&requestType=retrieve&format=xml&hours=0&ids=NULL,KCHA,KRMG,KVPC,KATL
  String Host_Data = "https://aviationweather.gov/cgi-bin/data/metar.php?datasource=metars&requestType=retrieve&format=xml&hours=0&ids=";
  // Format   = Host_Data + "STATION IDs"

  if (Type == "NAME") url = Host_Name + List;  else   url = Host_Data + List;

  if (wifiMulti.run() == WL_CONNECTED)  {
    //digitalWrite(LED_BUILTIN, HIGH);     // ON ~~ Only For Troubleshooting
    HTTPClient https;
    https.begin(url);                      // Start connection and send HTTP header
    httpCode = https.GET();                // httpCode will be negative on error so test or 200 for HTTP_CODE_OK
    if (httpCode == 200 ) {                // HTTP_CODE_OK
      // HTTP header has been send and Server response header has been handled
      // and File FOUND at server. Get back : httpCode = 200 (HTTP_CODE_OK)
      MetarData = https.getString();       // SAVE DATA in MetarData for Group of Stations

      // For Troubleshooting : Print url, MetarData Size, Check Max Allocated Heap & Print MetarData
      //Serial.printf("%s\tNo:%d\tIn GetData : url=%s\n", Clock, i, url.c_str());
      //Serial.printf("%s\tNo:%d\tIn GetData : url=%s\n", Clock, i, url.substring(url.indexOf("stationString="), url.length()).c_str());
      //Serial.printf("%s\tNo:%d\tIn GetData : MetarData=%d  MaxAllocHeap=%d  Free=%d  httpCode=%d ~ %s\n", Clock, i, MetarData.length(), ESP.getMaxAllocHeap(), ESP.getMaxAllocHeap() - MetarData.length(), httpCode, https.errorToString(httpCode).c_str());
      //Serial.printf("%s\tNo:%d\tIn GetData : MetarData : Length=%d\n%s\n", Clock, i, MetarData.length() , MetarData.c_str());

      if (ESP.getMaxAllocHeap() - MetarData.length() < 10500)   { // NO UPDATE : RUNNING LOW OF MEMORY
        Serial.printf("%s\tNo:%d\t%s\tSkipped GROUP ~ No Update, in GetData : Running Low on Memory\n", Clock, i, Stations[i].substring(0, 4).c_str());
        //Serial.printf("%s\tNo:%d\tIn GetData : MetarData Size=%d  MaxAllocHeap=%d  Free Memory=%d  httpCode=%d ~ %s\n", Clock, i, MetarData.length(), ESP.getMaxAllocHeap(), ESP.getMaxAllocHeap() - MetarData.length(), httpCode, https.errorToString(httpCode).c_str());
        httpCode = -100;                   // NO UPDATE for this Group of Stations, in ParseMetar
        Network_Status ();                 // WiFi Network Error
      }
      if (MetarData.length() < 140)   {    // NO UPDATE : NO MetarData DOWNLOADED
        Serial.printf("%s\tNo:%d\t%s\tSkipped ~ No Data Downloaded, in GetData : %i bytes  httpCode = %i\n", Clock, i, Stations[i].substring(0, 4).c_str(), MetarData.length(), httpCode);
        //Serial.printf("%s\tNo:%d\tIn GetData : MetarData Size=%d  MaxAllocHeap=%d  Free=%d  httpCode=%d ~ %s\n", Clock, i, MetarData.length(), ESP.getMaxAllocHeap(), ESP.getMaxAllocHeap() - MetarData.length(), httpCode, https.errorToString(httpCode).c_str());
        httpCode = -200;                   // NO UPDATE for this Group of Stations, in ParseMetar
        Network_Status ();                 // WiFi Network Error
      }

    }  else  {                           // CONNECTION ERROR : NO UPDATE for this Group of Stations, in ParseMetar
      Serial.printf("%s\tNo:%d\t%s\tSkipped GROUP ~ No Update, in GetData : Connection Error[httpCode]= %d ~ %s\n", Clock, i, Stations[i].substring(0, 4).c_str(), httpCode, https.errorToString(httpCode).c_str());
      Network_Status ();                 // WiFi Network Error
    }
    https.end();                         // CLOSE Communications
  }
  digitalWrite(LED_BUILTIN, LOW);        // OFF
}


// ***********   WiFi Network Error
void Network_Status() {
  Serial.printf("%s\t\t\tWiFi Network Status: ", Clock);
  if (WiFi.status() == 0 ) Serial.printf("= 0 ~ Idle\n");
  if (WiFi.status() == 1 ) Serial.printf("= 1 ~ Not Available, SSID can not be reached\n");
  if (WiFi.status() == 2 ) Serial.printf("= 2 ~ Scan Completed\n");
  if (WiFi.status() == 3 ) Serial.printf("= 3 ~ Connected to %s Router\n", String(ssid).c_str());
  if (WiFi.status() == 4 ) Serial.printf("= 4 ~ Failed, Password Incorrect\n");
  if (WiFi.status() == 5 ) Serial.printf("= 5 ~ Router Connection Lost\n");
  if (WiFi.status() == 6 ) Serial.printf("= 6 ~ Disconnected from Router\n");
}


// ***********   Reset All Parameters if Station Not Found in ParseMetar or in Go_Server
void Reset_All_Parameters(int i) {
  Category[i] = "NF";             // Not Found
  Sky[i] = "NA";                  // Not Found
  Sky_cover[i] = "";              // Not Found
  Remark[i] = "";                 // Not Found
  Remark_Code[i] = "";            // Not Found
  Metar[i] = "Station Not Found / Not Reporting";
  Sig_Weather[i] = "Not Found";   // Not Found
  Visab[i] = "NA";                // Not Found
  wDir[i] = "NA";                 // Not Found
  Wind[i] = "NA";                 // Not Found
  TempC[i] = 99;                  // Not Found
  DewptC[i] = 99;                 // Not Found
  Altim[i] = 0;                   // Not Found
  old_Altim[i] = 0;               // Not Found
  new_cloud_base[i] = 0;          // Not Found
  Elevation[i] = 0;               // Not Found
}


// ***********   Parse Metar Data ~ One Station at a time
void ParseMetar(int i) {
  String Single_Metar = "";                                               // Reset Single Metar Data
  String station = Stations[i].substring(0, 4);
  int Station_Start = MetarData.indexOf(station);                         // Search for this Station ID
  int Data_Start = MetarData.indexOf("<METAR>", Station_Start - 24);      // Search for data Start "<METAR>"
  int Data_End = MetarData.indexOf("</METAR>", Data_Start) + 8;           // Search for data End "</METAR>"

  if (station == "NULL")   return;       // NO UPDATE ~ for this Station

  if (httpCode < 0)             {        // NO UPDATE ~ Connection Error
    if (Category[i] == "")      {
      Reset_All_Parameters(i);           // Reset All Parameters
    }  else  if (Category[i] != "NF")    {
    if (Metar[i].indexOf("Error") > 0)    Reset_All_Parameters(i);             // Reset All Parameters
    if (Metar[i].indexOf("Error") < 0)    Metar[i] = "<FONT COLOR='Orange'>Skipped ~ Connection Error </FONT>" + Metar[i];  //  Connection Error
    }
    Serial.printf("%s\tNo:%d\t%s\tConnection Error, in ParseMetar\n", Clock, i, station.c_str());
    return;
  }

  if (Station_Start < 0)         {         // NO UPDATE ~ Station Not Reporting
    if (Metar[i].indexOf("Skipped") > 0)   Reset_All_Parameters(i);             // Reset All Parameters
    if (Metar[i].indexOf("Skipped") < 0)   Metar[i] = "<FONT COLOR='Orange'>Skipped </FONT>" + Metar[i];  //  Skipped
    Serial.printf("%s\tNo:%d\t%s\tStation Not Found, in ParseMetar\n", Clock, i, station.c_str());
    return;
  }

  //If can't Find "</METAR>", Lets Try for MetarData.length
  if (Data_End < 8)   {
    Serial.printf("%s\tNo:%d\t%s\tStation Missing Data ~ No </METAR>, in ParseMetar\n", Clock, i, station.c_str());
    Data_End = MetarData.length();
  }

  if (Data_End > 8)  {       // STATION FOUND

    Single_Metar = MetarData.substring(Data_Start, Data_End);       // Set Single Metar Data

    // Remove found data from MetarData
    MetarData = MetarData.substring(0, Data_Start) + MetarData.substring(Data_End, MetarData.length());

    Decodedata(i, station, Single_Metar);                           // DECODE the Station DATA

  } else {                  //  NO UPDATE ~ Missing Some or All Data ~ SOMETHING IS WRONG : Should Never Get Here
    // For Troubleshooting :
    Serial.printf("%s\tNo:%d\t%s\tIn ParseMetar : Station Missing Data\nSingle_Metar =\n%s\n", Clock, i, station.c_str(), Single_Metar.c_str());
  }
}


// *********** DECODE the Station DATA CODES (First Line  <raw text>)
void Decodedata(int i, String station, String Single_Metar) {
  String old_obs_time = Metar[i].substring(0, 4);                  // Previous Last Observation Time
  if (old_obs_time == "new ")  old_obs_time = Metar[i].substring(4, 8);
  // Otherwise old_obs_time = "" and will UPDATE later

  int search_Strt = Single_Metar.indexOf(station, 0) + 7;                  // Start of search for station
  int search_raw_text = Single_Metar.indexOf("</raw_text>", search_Strt);  // End of station Codes - (First Line "</raw_text")

  if (search_Strt < 7)  {
    // For Troubleshooting :
    Serial.printf("%s\tNo:%d\t%s\tIn Decodedata : Search Failed ~ search_Strt= %d  search_raw_text= %d\n", Clock, i, station.c_str(), search_Strt, search_raw_text);
    Serial.printf("Single_Metar=\n%s\n", Single_Metar.c_str());
    return;
  }

  // Search for Station DATA CODES (First Line  <raw text>)
  int search_From = Single_Metar.indexOf("Z ", search_Strt) + 10;          // Start search from timeZ +10
  int search_End = Single_Metar.indexOf(" A", search_From) + 6;            // Search for "A"  in  US : Station Metar End
  int search_EndQ = Single_Metar.indexOf(" Q", search_From) + 6;           // Search for "Q"  Non US : Station Metar End
  int search_EndR = Single_Metar.indexOf("RMK", search_From);              // Search for "RMK"
  if (search_End > search_EndR)  search_End = search_EndR;                 // No "A" Found in US BUT Found "AO" : EndR = Station Metar End
  if (search_EndQ > search_From)  search_End = search_EndQ;                // Found "Q" Non US : EndQ = Station Metar End

  if (search_End < search_From)  search_End = search_raw_text;             // Something is Wrong : Use Default for End
  if (search_End > search_raw_text)  search_End = search_raw_text;         // Something is Wrong : Use Default for End

  // *** CREATE  STATION METAR (First Line  <raw text>)
  Metar[i] = Single_Metar.substring(search_Strt, search_End);  //  (First Line  <raw text>)

  //Serial.printf("%s\tNo:%d\t%s In Decodedata  Single_Metar= %s\n\n", Clock, i, station.c_str(), Metar[i].c_str());

  // *** UPDATE/SKIP this Station *** : Compare Last Observation Time with [Single_Metar Observation Time] in New STATION METAR
  if (old_obs_time != Metar[i].substring(0, 4))  {       // ***  If NO UPDATE : SKIP this STATION  ***

    // *** NEW DATA: UPDATING STATION METAR  ***
    Metar[i] = "new " + Metar[i];

    // *** Decoding after RMK > Remark[i] ***
    // Search for REMARK in Single_Metar
    //search_From = Single_Metar.indexOf("Z ", search_Strt) + 10;          // Start search from timeZ +10
    search_End = Single_Metar.indexOf(" A", search_From) + 10;             // Search for "A"  in  US : Remark Start
    //search_EndQ = Single_Metar.indexOf(" Q", search_From) + 6;           // Search for "Q"  Non US : Remark Start
    search_EndR = Single_Metar.indexOf("RMK", search_From) + 3;            // Search for "RMK"
    if (search_End > search_EndR)  search_End = search_EndR;               // No "A" Found in US BUT Found "AO" : EndR = Remark Start

    if (search_EndR > search_End)  search_End = search_End - 4;            // Found "RMK" out of place : End"A" = Remark Start
    if (search_EndQ > search_From)  search_End = search_EndQ;              // Found "Q" Non US : EndQ = Remark Start
    if (search_End < search_From)  search_End = search_raw_text;           // No "RMK" Found : Use Default for Remark Start
    if (search_End > search_raw_text)  search_End = search_raw_text;       // Something is Wrong : Use Default for Remark Start

    // *** CREATE  Remark : Codes to Text
    // Metar[i]          Metar Codes including "new" (First Line  <raw text>)
    // Remark_Code[i]    Remarks Codes used in Summary Display
    // Remark[i]         Remarks Decoded used in Station Display

    String Metar_Code = Single_Metar.substring(search_Strt, search_End);      // Station Metar NOT including "new" "station" & "ago"
    Remark_Code[i] = Single_Metar.substring(search_End, search_raw_text);     // Remark Codes in Summary Display
    Remark[i] = "[" + Remark_Code[i] + " ]";     // Will become Decoded Remark Codes used in Station Display

    // ***  OPTIONAL  ***  To print UPDATED Stations or For Troubleshooting : Print Single_Metar & Remark
    // Serial.printf("%s\tNo:%d\t%s %s%s\n", Clock, i, station.c_str(), Metar_Code.c_str(), Remark[i].c_str());        // With square brackets

    // DECODING Remark[i] ~ (MAINLY REMOVE UNWANTED CODES in REMARKS)
    String mesg;
    String text;

    //  **** REMOVE   AO   Automated Station
    int search2 = Remark[i].indexOf(" AO");
    int search3 = Remark[i].indexOf(" ", search2 + 1);

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
    search2 = Remark[i].lastIndexOf(" P0");
    search3 = Remark[i].indexOf(" ", search2 + 1);
    if (search3 - search2 != 6)  search3 = -1;                  // Check for length

    if (search2 >= 0 && search3 >= 0)   {
      text = Remark[i].substring(search2, search3);
      mesg = " <br>Hourly Precip= " + text.substring(3, 4) + "." + text.substring(4, 6) + " In ";
      if (mesg.indexOf("0.00") > 0)   mesg = " <br>Hourly Precip= Trace";
      Remark[i].replace(Remark[i].substring(search2, search3), mesg);
    }

    //  ****  REPLACE   Itnnn  Hourly Ice Accretion in Hundredths of Inch
    //  t = 1 or 3 or 6 hour   nnn= Icing amount
    search2 = Remark[i].lastIndexOf(" I1");
    search3 = Remark[i].indexOf(" ", search2 + 1);
    if (search3 - search2 != 6)  search3 = -1;                  // Check for length

    if (search2 >= 0 && search3 >= 0)   {
      text = Remark[i].substring(search2, search3);
      mesg = " <br>Hourly Ice= " + text.substring(3, 4) + "." + text.substring(4, 6) + " In ";
      if (mesg.indexOf("0.00") > 0)   mesg = " <br>Hourly Ice= Trace";
      Remark[i].replace(Remark[i].substring(search2, search3), mesg);
    }

    //  ****  REPLACE   Itnnn  3 Hr  Ice Accretion in Hundredths of Inch
    //  t = 1 or 3 or 6 hour   nnn= Icing amount
    search2 = Remark[i].lastIndexOf(" I3");
    search3 = Remark[i].indexOf(" ", search2 + 1);
    if (search3 - search2 != 6)  search3 = -1;                  // Check for length

    if (search2 >= 0 && search3 >= 0)   {
      text = Remark[i].substring(search2, search3);
      mesg = " <br>3 Hour Ice= " + text.substring(3, 4) + "." + text.substring(4, 6) + " In ";
      if (mesg.indexOf("0.00") > 0)   mesg = " <br>3 Hour Ice= Trace";
      Remark[i].replace(Remark[i].substring(search2, search3), mesg);
    }

    //  ****  REPLACE   Itnnn  6 Hr  Ice Accretion in Hundredths of Inch
    //  t = 1 or 3 or 6 hour   nnn= Icing amount
    search2 = Remark[i].lastIndexOf(" I6");
    search3 = Remark[i].indexOf(" ", search2 + 1);
    if (search3 - search2 != 6)  search3 = -1;                  // Check for length

    if (search2 >= 0 && search3 >= 0)   {
      text = Remark[i].substring(search2, search3);
      mesg = " <br>6 Hour Ice= " + text.substring(3, 4) + "." + text.substring(4, 6) + " In ";
      if (mesg.indexOf("0.00") > 0)   mesg = " <br>6 Hour Ice= Trace";
      Remark[i].replace(Remark[i].substring(search2, search3), mesg);
    }

    //  ****  REMOVE   TsTTTTTTT   Current Temperature
    // s= 0 Positive s= 1 Negative  T= Temp in Deg C
    search2 = Remark[i].lastIndexOf(" T0");                      //  Positive
    if (search2 < 0)    search2 = Remark[i].lastIndexOf(" T1");  //  Negative
    search3 = Remark[i].indexOf(" ", search2 + 1);

    if (search2 >= 0 && search3 >= 0)  {
      text = Remark[i].substring(search2, search3);
      if (text.charAt(2) == '0')  text.setCharAt(2, '+');  else  text.setCharAt(2, '-');
      if (text.charAt(6) == '0')  text.setCharAt(6, '+');  else  text.setCharAt(6, '-');
      mesg = " <br>Temp=" + text.substring(2, 5) + "." + text.substring(5, 6) + " DewPt=" + text.substring(6, 9) + "." + text.substring(9, 10);
      //Remark[i].replace(Remark[i].substring(search2, search3), mesg);
      Remark[i].replace(Remark[i].substring(search2, search3), "");
    }

    //  ****  REMOVE  1sTTT  6 Hr MAX Temperature at 0000, 0600, 1200, and 1800 UTC
    // s= 0 Positive s= 1 Negative  T= Temp in Deg C
    search2 = Remark[i].lastIndexOf(" 10");                      //  Positive
    if (search2 < 0)    search2 = Remark[i].lastIndexOf(" 11");  //  Negative
    search3 = Remark[i].indexOf(" ", search2 + 1);
    if (search3 - search2 != 6)  search3 = -1;                  // Check for length

    if (search2 >= 0 && search3 >= 0)  {
      text = Remark[i].substring(search2, search3);
      if (text.charAt(2) == '0')  text.setCharAt(2, '+');  else  text.setCharAt(2, '-');
      text = text.substring(2, 5) + "." + text.substring(5, 6);
      //Remark[i].replace(Remark[i].substring(search2, search3), " <br>6 Hour Max Temperature:" + text + " Deg C");
      Remark[i].replace(Remark[i].substring(search2, search3), "");
    }

    //  ****  REMOVE  2sTTT  6 Hr MIN Temperature at 0000, 0600, 1200, and 1800 UTC
    // s= 0 Positive s= 1 Negative  T=Temp in Deg C
    search2 = Remark[i].lastIndexOf(" 20");                      // Positive
    if (search2 < 0)    search2 = Remark[i].lastIndexOf(" 21");  // Negative
    search3 = Remark[i].indexOf(" ", search2 + 1);
    int search1 = Remark[i].indexOf("FT", search2 + 1);                   // Check for NOT 200FT Density Alt
    if (search1 == -1)  search1 = Remark[i].indexOf("V", search2 + 1);    // Check for NOT Visibility
    if (search1 > 0 && search1 < search3)  search2 = -1;                  // If 200FT or VIS
    if (search3 - search2 != 6)  search3 = -1;                   // Check for length

    if (search2 >= 0 && search3 >= 0)    {
      text = Remark[i].substring(search2, search3);
      if (text.charAt(2) == '0')  text.setCharAt(2, '+');  else  text.setCharAt(2, '-');
      text = text.substring(2, 5) + "." + text.substring(5, 6);
      //Remark[i].replace(Remark[i].substring(search2, search3), " <br>6 Hour Min Temperature:" + text + " Deg C");
      Remark[i].replace(Remark[i].substring(search2, search3), "");
    }

    //  ****  REMOVE   3PPPP  Air Pressure at station level in hPa
    search2 = Remark[i].lastIndexOf(" 3");
    search3 = Remark[i].indexOf(" ", search2 + 1);
    search1 = Remark[i].indexOf("FT", search2 + 1);                       // NOT 300FT Density Alt
    if (search1 == -1)  search1 = Remark[i].indexOf("V", search2 + 1);    // NOT Visibility
    if (search1 > 0 && search1 < search3)  search2 = -1;                  // If 300FT or VIS
    if (search3 - search2 != 6)  search3 = -1;                            // Check for length

    if (search2 >= 0 && search3 >= 0)  {
      text = Remark[i].substring(search2 + 2, search3);
      Remark[i].replace(Remark[i].substring(search2, search3), "");
    }

    //  ****  REMOVE    4sTTTsTTT  24 Hr MAX/MIN Temperature
    // s= 0 Positive s= 1 Negative  T=Temp in Deg C
    search2 = Remark[i].lastIndexOf(" 40");                      //  Positive
    if (search2 < 0)    search2 = Remark[i].lastIndexOf(" 41");  //  Negative
    search3 = Remark[i].indexOf(" ", search2 + 1);
    if (search3 - search2 != 10)  search3 = -1;                  // Check for length

    if (search2 >= 0 && search3 >= 0)    {
      text = Remark[i].substring(search2, search3);
      if (text.charAt(2) == '0')  text.setCharAt(2, '+');  else  text.setCharAt(2, '-');
      if (text.charAt(6) == '0')  text.setCharAt(6, '+');  else  text.setCharAt(6, '-');
      text = text.substring(2, 5) + "." + text.substring(5, 6) + " / " + text.substring(6, 9) + "." + text.substring(9, 10);
      //Remark[i].replace(Remark[i].substring(search2, search3), " <br>24 Hour Max/Min Temperature:" + text + " Deg C");
      Remark[i].replace(Remark[i].substring(search2, search3), "");
    }

    //  ****  REPLACE   5appp 3Hr   Pressure Tendency
    // a=4 Steady   a=0123 Incr   a=5678 Decr   ppp=tenths hPa/mb
    search2 = Remark[i].lastIndexOf(" 5");
    search3 = Remark[i].indexOf(" ", search2 + 1);
    search1 = Remark[i].indexOf("FT", search2 + 1);           // NOT 500FT Density Alt
    if (search1 > 0 && search1 < search3)  search3 = -1;      // NOT 500FT Density Alt
    if (search3 - search2 != 6)  search3 = -1;                // Check for length

    if (search2 >= 0 && search3 >= 0)   {
      text = Remark[i].substring(search2 + 1, search3);
      int press_change = 0;
      press_change = text.substring(1, 2).toInt();
      mesg = " <br>";
      if (press_change > 4)  mesg += mesg + "3 Hour Pressure DEC " + Remark[i].substring(search2 + 4, search2 + 5) + "." + Remark[i].substring(search2 + 5, search2 + 6) + " mb";
      if (press_change < 4)  mesg += mesg + "3 Hour Pressure INC " + Remark[i].substring(search2 + 4, search2 + 5) + "." + Remark[i].substring(search2 + 5, search2 + 6) + " mb";
      if (press_change == 4) mesg += mesg + "3 Hour Pressure Steady";
      mesg.replace("0.0 mb", "Negligable");
      Remark[i].replace(Remark[i].substring(search2, search3), mesg);
    }

    //  ****  REPLACE   6RRRR 6Hr   Precipitation Amount in Inches
    // R=precip amount in Hundredths of Inch ??
    search2 = Remark[i].lastIndexOf(" 60");
    search3 = Remark[i].indexOf(" ", search2 + 1);
    search1 = Remark[i].indexOf("FT", search2 + 1);           // NOT 600FT Density Alt
    if (search1 > 0 && search1 < search3)  search2 = -1;      // NOT 600FT Density Alt
    if (search3 - search2 != 6)  search3 = -1;                // Check for length

    if (search2 >= 0 && search3 >= 0)  {
      text = Remark[i].substring(search2 + 1, search3);
      mesg = " <br>6 Hour Precip= " + text.substring(2, 3) + "." + text.substring(3, 6) + " In ";
      if (mesg.indexOf("0.00") > 0)   mesg = " <br>6 Hour Precip= Trace";
      Remark[i].replace(Remark[i].substring(search2, search3), mesg);
    }

    //  ****  REPLACE   7RRRR  24Hr Precipitation Amount in Inches
    // R=precip amount in Hundredths of Inch ??
    search2 = Remark[i].lastIndexOf(" 70");
    search3 = Remark[i].indexOf(" ", search2 + 1);
    search1 = Remark[i].indexOf("FT", search2 + 1);           // NOT 700FT Density Alt
    if (search1 > 0 && search1 < search3)  search3 = -1;      // NOT 700FT Density Alt
    if (search3 - search2 != 6)  search3 = -1;                // Check for length

    if (search2 >= 0 && search3 >= 0)     {
      text = Remark[i].substring(search2 + 1, search3);
      mesg = " <br>24 Hour Precip= " + text.substring(2, 3) + "." + text.substring(3, 5) + " In ";
      Remark[i].replace(Remark[i].substring(search2, search3), mesg);
    }

    //  ****  REMOVE   8/LMH  Cloud Types
    // /=above overcast 3= dense cirrus 6=stratus 9-CB
    search2 = Remark[i].lastIndexOf(" 8/");
    search3 = Remark[i].indexOf(" ", search2 + 1);
    if (search3 - search2 != 6)  search3 = -1;                // Check for length

    if (search2 >= 0 && search3 >= 0)   {
      text = Remark[i].substring(search2 + 1, search3);
      mesg = text.substring(2, 5);
      Remark[i].replace(Remark[i].substring(search2, search3), "");
    }

    //  ****  REMOVE   8/CLCMCH  Cloud Cover
    // 8/CLCMCH, shall be reported and coded in 3- and 6-hourly reports
    search2 = Remark[i].lastIndexOf(" 8/");
    search3 = Remark[i].indexOf(" ", search2 + 1);

    if (search2 >= 0 && search3 >= 0)      {
      text = Remark[i].substring(search2 + 1, search3);
      mesg = text.substring(2, 6);
      Remark[i].replace(Remark[i].substring(search2, search3), "");
    }

    // ****  REMOVE  933RRR   Water Equivalent of Snow on Ground in tenths in the 0800 UTC report
    search2 = Remark[i].lastIndexOf(" 933");
    search3 = Remark[i].indexOf(" ", search2 + 1);

    if (search2 >= 0 && search3 >= 0)    {
      text = Remark[i].substring(search2 + 1, search3);
      mesg = text.substring(3, 6);
      Remark[i].replace(Remark[i].substring(search2, search3), "");
    }

    //  ****  REPLACE  4/SSS  Snow Depth  at 0000, 0600, 1200, and 1800 UTC
    // SSS Inches Snow on the Ground
    search2 = Remark[i].lastIndexOf(" 4/");
    search3 = Remark[i].indexOf(" ", search2 + 1);
    if (search3 - search2 != 6)  search3 = -1;                  // Check for length

    if (search2 >= 0 && search3 >= 0)   {
      text = Remark[i].substring(search2 + 4, search3);
      mesg = " <br>Snow on the Ground= " + text + " In ";
      Remark[i].replace(Remark[i].substring(search2, search3), mesg);
    }

    //  ****  REPLACE  98MMM   Duration of Sunshine in Minutes
    search2 = Remark[i].lastIndexOf(" 98");
    search3 = Remark[i].indexOf(" ", search2 + 1);
    if (search3 - search2 != 6)  search3 = -1;              // Check for length

    if (search2 >= 0 && search3 >= 0)    {
      text = Remark[i].substring(search2 + 1, search3);
      float sun = (float)text.substring(2, 5).toInt() / 60.0;
      mesg = " <br>Sunshine=" + String(sun) + " Hrs";
      Remark[i].replace(Remark[i].substring(search2, search3), mesg);
    }


    // ****  DECODE WEATHER in METAR from (Single_Metar or Metar[i]) and PRINT ERRORS  ****


    // Searching flight_category in Single Metar
    search1 = Single_Metar.indexOf("<flight_category>") + 17;
    search2 = Single_Metar.indexOf("</flight_category>");
    if (search2 > search1)  {
      Category[i] = Single_Metar.substring(search1, search2);
    }  else  {
      Category[i] = "NA";
      Serial.printf("%s\tNo:%d\t%s\tFlight Category Not Found, in Decodedata\n", Clock, i, station.c_str());
    }

    // Significant Weather in Metar (First Line  <raw text>)
    Sig_Weather[i] = "";
    // Wind Gusts
    search1 = Metar[i].indexOf("G");
    search2 = Metar[i].indexOf("KT");
    if (search1 > 0 && search2 - search1 > 2)
      Sig_Weather[i] =  Sig_Weather[i] + "Wind Gusts to " + Metar[i].substring(search2 - 2, search2) + " Kts<br>";

    // Variable Wind Dir in Metar (First Line  <raw text>)
    search1 = Metar[i].indexOf("0V");
    search2 = Metar[i].indexOf("FT");  // Not Runway Vis
    if (search1 > 0  && search2 < 0)
      Sig_Weather[i] += "<br>Wind Direction: " + Metar[i].substring(search1 - 2, search1 + 1) + " Variable " + Metar[i].substring(search1 + 2, search1 + 5) + " Deg<br>";

    // Variable Vis in Metar (First Line  <raw text>)
    search1 = Metar[i].indexOf("SM R");
    search2 = Metar[i].indexOf("V", search1);
    search3 = Metar[i].indexOf("FT", search1);
    if (search1 > 0 && search3 > 0)  {
      if (search1 < 0  ||  search2 > search3)
        Sig_Weather[i] += "<br>Runway Visibility: " + Metar[i].substring(search1 + 3, search3) + " Ft<br>";
      else
        Sig_Weather[i] += "<br>Runway Visibility: " + Metar[i].substring(search1 + 3, search2) + " Ft Variable "
                          + Metar[i].substring(search2 + 1, search3) + " Ft<br>";
    }

    //  Searching Lat_Long in Single Metar
    search1 = Single_Metar.indexOf("<latitude>") + 10;
    search2 = Single_Metar.indexOf("</latitude>");
    search3 = Single_Metar.indexOf("<longitude>") + 11;
    int search4 = Single_Metar.indexOf("</longitude>");
    if (search2 < 0  ||  search4 < 0 )  {
      Serial.printf("%s\tNo:%d\t%s\tLat_Long Not Found, in Decodedata\n", Clock, i, station.c_str());
    } else {
      Lat_Long[i] = Single_Metar.substring(search1, search2) + "," + Single_Metar.substring(search3, search4);
    }

    // Searching temp_c in Single Metar TempC[i] in deg C
    search1 = Single_Metar.indexOf("<temp_c") + 8;
    search2 = Single_Metar.indexOf("</temp");
    if (search1 < 8)   Serial.printf("%s\tNo:%d\t%s\tTemperature Not Found, in Decodedata\n", Clock, i, station.c_str());
    if (search1 < 8)   TempC[i] = 99;   else  TempC[i] = Single_Metar.substring(search1, search2).toFloat();

    // Searching Max/Min Temp in Single Metar Max_Min_Temp in deg C
    search1 = Single_Metar.indexOf("<maxT_c") + 8;
    search2 = Single_Metar.indexOf("</maxT_c");
    search3 = Single_Metar.indexOf("<minT_c") + 8;
    search4 = Single_Metar.indexOf("</minT_c");
    if (search2 > 0    ||  search4 > 0 )  {
      Max_Min_Temp[i] = "<br>24 Hour Max/Min Temp : " + Single_Metar.substring(search1, search2) + " / " + Single_Metar.substring(search3, search4) + " Deg C";
    }   else    {
      Max_Min_Temp[i] = "";
    }

    // Searching dewpoint_c in Single Metar
    search1 = Single_Metar.indexOf("<dewpoint_c") + 12;
    search2 = Single_Metar.indexOf("</dewpoint");
    if (search1 < 12)   Serial.printf("%s\tNo:%d\t%s\tDewpoint Not Found, in Decodedata\n", Clock, i, station.c_str());
    if (search1 < 12) DewptC[i] = 99;  else  DewptC[i] = Single_Metar.substring(search1, search2).toFloat();

    // Searching Wind_dir_degrees in Single Metar
    old_wDir[i] = wDir[i].toInt();
    search1 = Single_Metar.indexOf("wind_dir_degrees") + 17;
    search2 = Single_Metar.indexOf("</wind_dir_degree");
    if (search1 < 17) wDir[i] = "NA";  else  wDir[i] = Single_Metar.substring(search1, search2);
    if (Metar[i].indexOf("VRB") > 0) wDir[i] = "VRB";  //  (First Line  <raw text>)
    if (wDir[i] == "NA")   Serial.printf("%s\tNo:%d\t%s\tWind Not Found, in Decodedata\n", Clock, i, station.c_str());

    // Searching Wind_speed_kt in Single Metar
    search1 = Single_Metar.indexOf("wind_speed_kt") + 14;
    search2 = Single_Metar.indexOf("</wind_speed");
    if (search1 < 14) Wind[i] = "NA";  else  Wind[i] = Single_Metar.substring(search1, search2) + " KT";
    if (Wind[i] == "0 KT")  Wind[i] = "CALM";

    // Searching  visibility_statute_mi in Single Metar
    search1 = Single_Metar.indexOf("visibility_statute_mi") + 22;
    search2 = Single_Metar.indexOf("</visibility");
    if (search1 < 22)  Visab[i] = "NA";  else  Visab[i] = Single_Metar.substring(search1, search2);
    if (search1 < 22)  Serial.printf("%s\tNo:%d\t%s\tVisibility Not Found, in Decodedata\n", Clock, i, station.c_str());

    // Lets look for Measured Vis in Metar (First Line  <raw text>)
    search2 = Metar[i].indexOf("KT ");           // Start Search from Here
    search3 = Metar[i].indexOf("M", search2);    // Look for Measured Vis in Metar
    if (search1 > 22 && search3 - search2 == 3)  Visab[i] += "m";

    // Searching  <wx_string>RA BR</wx_string>    =   Sig_Weather
    search1 = Single_Metar.indexOf("<wx_string>") + 11;
    search2 = Single_Metar.indexOf("</wx_string");
    if (search1 < 11)  Sig_Weather[i] += "";  else  Sig_Weather[i] += Single_Metar.substring(search1, search2);

    // Other Significant Weather in wx_string
    Sig_Weather[i].replace("VCTS", " In the Vicinity : Thunderstorm ;");
    Sig_Weather[i].replace("VC", " In the Vicinity : ");
    Sig_Weather[i].replace("BR", " Mist ");

    Sig_Weather[i].replace("FZ", " Freezing");
    Sig_Weather[i].replace("MI", " Shallow");
    Sig_Weather[i].replace("PR", " Partial");
    Sig_Weather[i].replace("BC", " Patches of");
    Sig_Weather[i].replace("FG", " Fog ");
    Sig_Weather[i].replace("DU", " Dust ");
    Sig_Weather[i].replace("FU", " Smoke ");
    Sig_Weather[i].replace("BL", " Blowing");
    Sig_Weather[i].replace("DR", " Drifting");
    Sig_Weather[i].replace("SN", " Snow ");
    Sig_Weather[i].replace("SG", " Snow Grains; ");
    Sig_Weather[i].replace("HZ", " Haze ");
    Sig_Weather[i].replace("FY", " Spray ");
    Sig_Weather[i].replace("SA", " Sand ");
    Sig_Weather[i].replace("PO", " Dust/Sand ");
    Sig_Weather[i].replace("SQ", " Squalls ");
    Sig_Weather[i].replace("RA", " Rain ");
    Sig_Weather[i].replace("SH", " Showers ");
    Sig_Weather[i].replace("DZ", " Drizzle ");
    Sig_Weather[i].replace("GS", " Small Hail/Snow Pellets ");
    Sig_Weather[i].replace("GR", " Large Hail ");
    Sig_Weather[i].replace("IC", " Ice Crystals ");
    Sig_Weather[i].replace("PL", " Ice Pellets ");
    Sig_Weather[i].replace("UP", " Unknown Precipitation ");
    Sig_Weather[i].replace("VA", " Volcanic Ash ");
    Sig_Weather[i].replace("+",  " Heavy");
    Sig_Weather[i].replace("-",  " Light");

    if (Sig_Weather[i].indexOf("TS") >= 0)  Sig_Weather[i] += " Thunderstorm";
    Sig_Weather[i].replace("TS", "");

    Sig_Weather[i].replace("Kts", "KTS");          // Fix for Wind Gusts

    Sig_Weather[i] += "<br>";

    // Searching Cumulonimbus Clouds in Metar (First Line  <raw text>)
    if (Metar[i].indexOf("CBMAM") > 0) Sig_Weather[i] += "Mammatus Cumulonimbus Clouds<br>";
    if (Metar[i].indexOf("CB") > 0) Sig_Weather[i] += "Cumulonimbus Clouds<br>";
    if (Metar[i].indexOf("TCU") > 0) Sig_Weather[i] += "Towering Cumulonimbus Clouds<br>";

    if (Sig_Weather[i] == "<br>")  Sig_Weather[i] = "None";

    // CLEAN UP   "<br>"
    Sig_Weather[i].replace("<br><br>",  "<br>");         // Fix for Double "<br>"

    if (Sig_Weather[i].substring(0, 4) == "<br>")        // Remove First "<br>"
      Sig_Weather[i] = Sig_Weather[i].substring(4, Sig_Weather[i].length());

    if (Sig_Weather[i].substring(Sig_Weather[i].length() - 4, Sig_Weather[i].length()) == "<br>")    // Remove last "<br>"
      Sig_Weather[i] = Sig_Weather[i].substring(0, Sig_Weather[i].length() - 4);

    // Searching  <sky_condition in Single Metar
    old_cloud_base[i] = new_cloud_base[i];
    int search0 = Single_Metar.indexOf("<sky_condition");            // Start Search from Here

    // Searching   new_cloud_base in Single Metar
    search2 = Single_Metar.indexOf("_ft_agl=", search0) + 9;
    search3 = Single_Metar.indexOf(" />", search0) - 1;
    new_cloud_base[i] = Single_Metar.substring(search2, search3).toInt();

    search1 = Single_Metar.indexOf("sky_cover", search0) + 11;
    if (search1 < 11)  Sky[i] = "NA";  else  Sky[i] = Single_Metar.substring(search1, search1 + 3);   {
      if (Sky[i] == "OVX")  Sky[i] = "OBSECURED";
      if (Sky[i] == "CAV")  Sky[i] = "CLEAR";
      if (Sky[i] == "CLR")  Sky[i] = "CLEAR";
      if (Sky[i] == "SKC")  Sky[i] = "CLEAR";
    }
    if (Sky[i] == "CLEAR")   new_cloud_base[i] = 99999;
    if (Sky[i] == "OBSECURED")   new_cloud_base[i] = 0;
    if (Sky[i] == "NA")  {
      new_cloud_base[i] = 0;
      old_cloud_base[i] = 0;
      Serial.printf("%s\tNo:%d\t%s\tSky Cover Not Found, in Decodedata\n", Clock, i, station.c_str());
    }

    // Searching  Sky_cover "OVC" "BKN" "SCT" "FEW" in Single Metar
    String Cover = "";
    while (search0 > 11)   {
      search1 = Single_Metar.indexOf("sky_cover", search0) + 11;

      if (Single_Metar.substring(search1, search1 + 3) == "OVC")  {
        search2 = Single_Metar.indexOf("_ft_agl=", search1) + 9;
        search3 = Single_Metar.indexOf(" />", search1) - 1;
        Cover += "Overcast Cloud Layer at " + Single_Metar.substring(search2, search3) + " Ft<br>";
      } else

        if (Single_Metar.substring(search1, search1 + 3) == "BKN")  {
          search2 = Single_Metar.indexOf("_ft_agl=", search1) + 9;
          search3 = Single_Metar.indexOf(" />", search1) - 1;
          Cover += "Broken Clouds at " + Single_Metar.substring(search2, search3) + " Ft<br>";
        } else

          if (Single_Metar.substring(search1, search1 + 3) == "SCT")  {
            search2 = Single_Metar.indexOf("_ft_agl=", search1) + 9;
            search3 = Single_Metar.indexOf(" />", search1) - 1;
            Cover += "Scattered Clouds at " + Single_Metar.substring(search2, search3) + " Ft<br>";
          } else

            if (Single_Metar.substring(search1, search1 + 3) == "FEW")  {
              search2 = Single_Metar.indexOf("_ft_agl=", search1) + 9;
              search3 = Single_Metar.indexOf(" />", search1) - 1;
              Cover += "Few Cloulds at " + Single_Metar.substring(search2, search3) + " Ft<br>";
            }
      search0 = Single_Metar.indexOf("sky_cover", search0) + 11;
    }

    Cover = Cover.substring(0, Cover.length() - 4);    // Remove last "<br>"
    if (Cover.length() > 3)  Sky_cover[i] = Cover;   else Sky_cover[i] = "";

    // Vertical Visibility Searching <vert_vis_ft> in Single Metar
    search0 = Single_Metar.indexOf("<vert_vis_ft>") + 13;
    if (search0 > 13)  {
      Sig_Weather[i] += "<br>Vertical Visibility = " + Single_Metar.substring(search0, search0 + 3) + " Ft";
      new_cloud_base[i] = Single_Metar.substring(search0, search0 + 3).toInt();
    }

    // Searching <altim_in_hg in Single Metar
    old_Altim[i] = Altim[i];
    search0 = Single_Metar.indexOf("<altim_in_hg>") + 13;
    search1 = Single_Metar.indexOf("</altim_in_hg>");
    if (search0 < 13)  Serial.printf("%s\tNo:%d\t%s\tAltimeter Not Found, in Decodedata\n", Clock, i, station.c_str());
    if (search0 < 13) Altim[i] = 0;  else  Altim[i] = Single_Metar.substring(search0, search1).toFloat();

    // Searching <sea_level_pressure_mb in Single Metar
    search0 = Single_Metar.indexOf("<sea_level_pressure_mb>") + 23;
    if (search0 < 23) SeaLpres[i] = 0;  else  SeaLpres[i] = Single_Metar.substring(search0, search0 + 6).toFloat();

    // Searching elevation_m in Single Metar
    search0 = Single_Metar.indexOf("<elevation") + 13;
    search1 = Single_Metar.indexOf("</elevation");
    if (search0 < 13) Elevation[i] = 0; else Elevation[i] = Single_Metar.substring(search0, search1).toFloat();

    // ****  DECODE WEATHER in REMARKS and make readable in Dictionary Function
    Station_Num = i;                                               // For Troubleshooting in Print Remarks
    String weather = Decode_Weather(Remark[i]);                    // Decode_Weather Routine
    Remark[i] = weather;                                           // Return with readable weather

  }                               // End of NO UPDATE Station
  Display_LED(i, 20);             // Display This Station LED
}


//  *********** For REMARKS (Remark[i]) ~ Make codes readable in this Dictionary Function ~ Decode_Weather Routine
String Decode_Weather(String weather) {
  // Glossary of CODES
  weather.replace("BECMG", "<br>becoming :");            // Clean Up Later
  weather.replace("TEMPO", "<br>Temporary :");           // Clean Up Later
  weather.replace("CAVOK", "cavok");                     // Rename Later
  weather.replace("RERA", "Recent : Rain");
  weather.replace("REDZ", "Recent : Drizzle");
  weather.replace("RTS", "Routes");
  weather.replace("AWOS", "awos");                       // Rename Later
  weather.replace("QFE", "qfe");                         // Rename Later
  weather.replace("SKC ", "sky");                        // Rename Later
  weather.replace("MIN", "minimum");                     // Rename Later
  weather.replace("OTS", "NA");
  weather.replace("FALSE ", "FALSeast");                 // Rename Later

  weather.replace("NOSIG", "<br>No Signifiant Weather");
  weather.replace("NSW", "<br>No Signifiant Weather");
  weather.replace("NSC", "<br>No Signifiant Clouds");
  weather.replace("DATA", "Data");
  weather.replace("DA", "Density Alt");
  weather.replace("DENSITY ALT ", " Density Alt =");
  weather.replace("DEWPT", "Dew Point");
  weather.replace("CONS", "Continuous");
  weather.replace("PDMTLY", "Predominantly");
  weather.replace("TRANSPARENT", "Transparent");
  weather.replace("TORNADO", "Tornado");
  weather.replace("WATERSPOUT", "Waterspout");

  weather.replace("ICE", "Ice");
  weather.replace("STNRY", "Stationary");
  weather.replace("DRFTNG", "Drifting");
  weather.replace("DRFTG", "Drifting");
  weather.replace("SLWLY", "Slowly");
  weather.replace("BANK", "bank");               // Rename Later

  weather.replace("OBSCURED", "Obscured");
  weather.replace("OBSC", "Obscured");
  weather.replace("FROPA", "Frontal Passage ");
  weather.replace("PRESFR", " Pressure Falling Rapidly ");
  weather.replace("PRESRR", " Pressure Rising Rapidly ");
  weather.replace("FROIN", " Frost On The Indicator ");
  weather.replace("ALSTG", "Alitmeter Setting");
  weather.replace("ALTSTG", "Alitmeter Setting");
  weather.replace("AT AP", "At Airport");

  weather.replace("SLPNO", " Sea Level Pressure NA ");
  weather.replace("PNO", " Rain Gauge NA ");
  weather.replace("FZRANO", " Freezing Rain Sensor NA ");
  weather.replace("TSNO", " Thunderstorm Sensor NA ");
  weather.replace("RVRNO", " Runway visibility Sensor NA ");     // Rename Later
  weather.replace("CHINO", " Secondary Ceiling Height Sensor NA ");
  weather.replace("VISNO", " visibility Sensor NA ");            // Rename Later
  weather.replace("PWINO ", " Peak Wind Sensor NA ");
  weather.replace("CEILOMETER", " Ceilometer ");            // CEILOMETER OTS

  weather.replace("PK WND", "Peak wind");                   // See below
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

  weather.replace("SHELF", " Shelf");
  weather.replace("DATA", "Data");
  weather.replace("AND", "and");
  weather.replace("UTC", "utc");                     // Rename Later
  weather.replace("ESTD", "eaststimated");           // Rename Later
  weather.replace("ESTMD", "eaststimated");          // Rename Later

  weather.replace("PAST", "Past");
  weather.replace("PRSNT", "Present");
  weather.replace("LESS THAN", "Less Than");

  weather.replace("THN", "Thin");
  weather.replace("THRU", "Thru");
  weather.replace("HR ", "Hour ");
  weather.replace("WSHFT", "Wind Shift");
  weather.replace("ICG", " Icing");
  weather.replace("PCPN", " Precip");
  weather.replace("MTNS", " Mountains");
  weather.replace("MTS", " Mountains");
  weather.replace("ACC", "AC");

  weather.replace("ACFT MSHP", " Aircraft Mishap");
  weather.replace("STFD", "Staffed");
  weather.replace("FM", "From ");
  weather.replace("FST", " First");
  weather.replace("LAST", " Last");
  weather.replace("OBS", "Observation");
  weather.replace("OB", "Observation");
  weather.replace("NXT", "Next");
  weather.replace("CNVTV", "Conv");
  weather.replace("CVCTV", "Conv");                  // Rename Later
  weather.replace("FUNNEL CLOUD ", "Funnel Cloud");
  weather.replace("PLUME", "Plume");
  weather.replace("BIRD", "bird");                   // Rename Later
  weather.replace("UNKN", "Unknown");

  weather.replace("BINOVC", " KN in OC");            // Rename Later
  weather.replace("BKN V OVC", " KN V OC");          // Rename Later
  weather.replace("OVC V BKN", " OC V KN");          // Rename Later

  weather.replace("BKN0", "<br>KN Clouds at 0");      // Rename Later
  weather.replace("SCT0", "<br>Scattered Clouds at 0");
  weather.replace("FEW0", "<br>Few Clouds at 0");
  weather.replace("OVC0", "<br>OC at 0");            // Rename Later
  weather.replace("SCT0", "<br>Scattered Clouds at 0");
  weather.replace("BKN", "KN Clouds");               // Rename Later
  weather.replace("SCT", "Scattered Clouds ");
  weather.replace("FEW", "Few Clouds");
  weather.replace("OVC", "OC Clouds");               // Rename Later

  weather.replace("MDT CU", "<br>ModerateCU");
  weather.replace("MDT", "<br>Moderate");
  weather.replace("EMBD", "<br>mbedded");            // Rename Later
  weather.replace("EMBED", "<br>mbedded");           // Rename Later

  weather.replace("OCNL LTG", "OCNLLTG");
  weather.replace("OCNL LT", "OCNLLTG");
  weather.replace("OCNL", "<br>Occasional");
  weather.replace("FREQ", "FRQ");
  weather.replace("FRQ LTG", "FRQLTG");
  weather.replace("FRQ", "Frequent");
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
  weather.replace("LGTICCG", "LTG LTinC and LTtoG");   // Lightning in Cloud and Ground
  weather.replace("LTICGCG", "LTG LTinC and LTtoG");   // Lightning in Cloud and Ground
  weather.replace("LTGICCA", "LTG LTinC CA");          // Lightning in Cloud and Air
  weather.replace("LTGCG", "LTG LTtoG");               // Lightning in Ground
  weather.replace("LTCIC", "LTG LTinC");               // Lightning in Cloud
  weather.replace("LTGCC", "LTG LTinC");               // Lightning in Cloud
  weather.replace("LTGIC", "LTG LTinC");               // Lightning in Cloud
  weather.replace("CG", " and LTtoG");                 // Lightning to Ground (fix)
  weather.replace("LTNG", "LTG");
  weather.replace("LTG", " Lightning");
  weather.replace("LTinC", "in Clouds");
  weather.replace("LTtoG", "to Ground");
  weather.replace("CA", "and Cloud to Air");

  weather.replace("ALQDS", "All Quadrents ");
  weather.replace("ALQS", "All Quadrents ");
  weather.replace("DIST", "Distant");
  weather.replace("DSTN", "Distant");
  weather.replace("DSNT", "Distant");
  weather.replace("DST", "Distant");
  weather.replace("MOVD", "Moved");
  weather.replace("MOVG", "Moving");
  weather.replace("MOV", "Movement");
  weather.replace("STNRY", "Stationary");

  weather.replace("DZB", "<br>Drizzle began");         // Rename Later
  weather.replace("DZE", "<br>Drizzle ended");         // Rename Later
  weather.replace("FZRAB", "<br>Freezing Rain began"); // Rename Later
  weather.replace("RAB", "<br>Rain began");            // Rename Later
  weather.replace("RAE", "<br>Rain ended");            // Rename Later
  weather.replace("SNB", "<br>Snow began");            // Rename Later
  weather.replace("SNE", "<br>Snow ended");            // Rename Later

  weather.replace(" E", " east");                       // Rename Later
  weather.replace("E ", "east ");                       // Rename Later
  weather.replace("NE", "northeast");                   // Rename Later
  weather.replace(" SE", " Seast");                     // Rename Later
  weather.replace("E-", "east-");                       // Rename Later
  weather.replace("-E", "-east");                       // Rename Later
  weather.replace("-SE", "-Seast");                     // Rename Later
  weather.replace(" S ", " South ");
  weather.replace(" N ", " North ");
  weather.replace(" W ", " West ");

  weather.replace("SNINCR", " Snow Increasing Rapidily");
  weather.replace("TWRINC", " Tower visibility Increasing");// Rename Later
  weather.replace("SFC VIS", " Surface VIS");
  weather.replace("SFC", "Surface");
  weather.replace("TWR VIS", "Tower VIS");
  weather.replace("TWR", "Tower VIS");
  weather.replace("VV", "visibility variable");         // Rename Later
  weather.replace("VISBL", "visabile");                 // Rename Later
  weather.replace("VIS M", "visibility Measured");      // Rename Later
  weather.replace("VIS", "visibility");                 // Rename Later
  weather.replace("INC", "Increasing");
  weather.replace("DEC", "Decreasing");

  weather.replace(" - ", " to ");
  weather.replace(" -", " Light ");
  weather.replace(" +", " Heavy ");

  weather.replace("SHRASN", "Rain and Snow Showers");
  weather.replace("SHRA", "Rain Showers");
  weather.replace("SHSN", "Snow Showers");
  weather.replace("RASN", "Rain and Snow");
  weather.replace("TSRA", "Rain and Thunderstorm ");
  weather.replace("VCSH", "Rain Showers VC");
  weather.replace("VCTS", "TS VC");
  weather.replace("VC", "In the vicinity");            // Rename Later
  weather.replace("TS", "<br>Thunderstorm");

  weather.replace("HIER", "Higher");
  weather.replace("LWR", "Lower");

  weather.replace("DISSIPATED", "Dissipated ");
  weather.replace("DSIPTD", "Dissipated ");
  weather.replace("DSIPT", "Dissipated ");
  weather.replace("DSPTD", "Dissipated ");
  weather.replace("DSPT", "Dissipated ");
  weather.replace("HVY", "Heavy");
  weather.replace("LGT", "Light");

  weather.replace("OHD", "overhead");                // Rename Later
  weather.replace("OVD", "overhead");                // Rename Later
  weather.replace("VRB", "variable");                // Rename Later
  weather.replace("VRY", "very");                    // Rename Later
  weather.replace("VERY", "very");                   // Rename Later
  weather.replace("VSBY", "visibility");             // Rename Later
  weather.replace("VSB", "visibile");                // Rename Later
  weather.replace("VIRGA", " virga");                // Rename Later
  weather.replace("V", "variable");                  // Rename Later

  weather.replace("CIG", "<br>Ceiling at");

  weather.replace("ACSL", "Standing Lenticular Altocumulus Clouds");
  weather.replace("SC", "Stratocumulus Clouds");
  weather.replace("CBMAM", "mammatusCB");

  weather.replace("BLU", "blu");                     // See Military Later
  weather.replace("WHT", "wht");                     // See Military Later
  weather.replace("GRN", "grn");                     // See Military Later
  weather.replace("YLO", "ylo");                     // See Military Later
  weather.replace("AMB", "amb");                     // See Military Later
  weather.replace("RED", "redd");                    // See Military Later
  weather.replace("BLACK", "black");                 // See Military Later

  weather.replace("BC", " Patches of ");
  weather.replace("BL", " blowing ");                // Rename Later
  weather.replace("DR", " Drifting ");
  weather.replace("FZ", " Freezing ");

  weather.replace("MI", "Shallow");
  weather.replace("PTCHY", "Patchy");
  weather.replace("MOD", "Moderate");

  weather.replace("DZ", " Drizzle");
  weather.replace("RA", "Rain");
  weather.replace("SN", "Snow");
  weather.replace("SG", "Snow Grains");

  weather.replace("BR", "Mist");
  weather.replace("IC", " Ice Cristals");
  weather.replace("PL", " Ice Pellets");
  weather.replace("GS", " Small Hail/Snow Pellets");
  weather.replace("GR", " Large Hail");
  weather.replace("FG", "Fog");
  weather.replace("FU", "Smoke");
  weather.replace("HZY", "Hazy");
  weather.replace("HZ", "Haze");
  weather.replace("DS", "Dust Storm");
  weather.replace("DU", "Dust");
  weather.replace("FC", "Funnel Cloud");
  weather.replace("UP", "Unknown Precip");
  weather.replace("SH", "Showers");
  weather.replace("TCU", " TC");                     // Rename Later

  weather.replace("RainG", "Ragged ");
  weather.replace("TR", "Trace ");
  weather.replace("RE", "<br>Recent : ");

  weather.replace("RF", "Rainfall ");
  weather.replace("RMK ", "<br>Remark : ");         // Rename Later

  weather.replace("CB", " Cumulonimbus Clouds");
  weather.replace("CC", "Cirrocumulus Clouds");
  weather.replace("CI", "Cirrus Clouds");
  weather.replace("CS", "Cirostratus Clouds");
  weather.replace("CU", " Cumulus Clouds");
  weather.replace("CF", "Cumulusfractus Clouds");
  weather.replace("SF", "Stratus Fractus Clouds");
  weather.replace("ST", "Stratus Clouds");
  weather.replace("NS", "Nimbostratus Clouds");
  weather.replace("AC", "Altocumulus Clouds");
  weather.replace("AS", "Altostratus Clouds");

  weather.replace("B", " began");                      // Rename Later
  weather.replace("E", " ended");                      // Rename Later

  weather.replace("Clouds1", "Clouds=1 Oktas<br>");
  weather.replace("Clouds2", "Clouds=2 Oktas<br>");
  weather.replace("Clouds3", "Clouds=3 Oktas<br>");
  weather.replace("Clouds4", "Clouds=4 Oktas<br>");
  weather.replace("Clouds5", "Clouds=5 Oktas<br>");
  weather.replace("Clouds6", "Clouds=6 Oktas<br>");
  weather.replace("Clouds7", "Clouds=7 Oktas<br>");
  weather.replace("Clouds8", "Clouds=8 Oktas<br>");

  weather.replace("blu blu", "blu");                   // Military
  weather.replace("wht wht", "wht");                   // Military
  weather.replace("grn grn", "grn");                   // Military
  weather.replace("ylo ylo", "ylo");                   // Military
  weather.replace("amb amb", "amb");                   // Military
  weather.replace("redd redd", "redd");                // Military
  weather.replace("blu", "BLUE: Cloud Base >2500ft Visibility >8km<br>");
  weather.replace("wht", "WHITE: Cloud Base >1500ft Visibility >5km<br>");
  weather.replace("grn", "GREEN: Cloud Base >700ft Visibility >3.7km<br>");
  weather.replace("ylo1", "YELLOW: Cloud Base >500ft Visibility >2500m<br>");
  weather.replace("ylo2", "YELLOW: Cloud Base >300ft Visibility >1600m<br>");
  weather.replace("ylo", "YELLOW: Cloud Base >300ft Visibility >1600m<br>");
  weather.replace("amb", "AMBER: Cloud Base >200ft Visibility >800m<br>");
  weather.replace("redd", "RED: Cloud Base <200ft Visibility <800m<br>");
  weather.replace("black", "BLACK: Runway is Unusable<br>");

  weather.replace("KN", "Broken");                     // Rename
  weather.replace("OC", "Overcast");                   // Rename
  weather.replace("Conv", "Convective");               // Rename
  weather.replace("TC", "Towering Cumulus Clouds");    // Rename
  weather.replace("overhead", "Overhead");             // Rename
  weather.replace("northeast", "NE");                  // Rename
  weather.replace("east", "E");                        // Rename
  weather.replace("var", "Var");                       // Rename
  weather.replace("very", "Very");                     // Rename
  weather.replace("vi", "Vi");                         // Rename
  weather.replace("bank", "Bank");                     // Rename
  weather.replace("blowing", "Blowing");               // Rename
  weather.replace("began", "Began");                   // Rename
  weather.replace("ended", "Ended");                   // Rename
  weather.replace("mammatus", "Mammatus");             // Rename
  weather.replace("mbedded", "Embedded");              // Rename
  weather.replace("awos", "AWOS");                     // Rename
  weather.replace("cavok", "CAVOK");                   // Rename
  weather.replace("utc", "UTC ");                      // Rename
  weather.replace("qfe", "QFE ");                      // Rename
  weather.replace("sky", "Sky");                       // Rename
  weather.replace("minimum", "MIN");                   // Rename
  weather.replace("bird", "BIRD");                     // Rename

  weather.replace("SOG", "<br>Snow on the Ground=");


  //  Search for Ceiling [CIG in US] in weather / Decoded Remarks
  int search0 = weather.indexOf("Ceiling");
  if (search0 > 0)  {
    //  Found Ceiling
    Decode_Alt(weather, search0);  // Decode Alt Routine
    int search1 = weather.indexOf("Ceiling", search0 + 10);
    if (search1 > 0)   Decode_Alt(weather, search1);  // Decode Alt Routine
  }  else  {
    // Search for Ceiling [ NON US ] in weather / Decoded Remarks
    int search1 = weather.indexOf("9999");
    if (search1 > 0)  {
      weather = weather.substring(0, search1) + "Ceiling ~ Unlimited  " + weather.substring(search1 + 5, weather.length());
    }
    search0 = weather.indexOf(": ");   // Start
    search1 = weather.indexOf("00 ");  // End
    if (search0 > 0 && search1 > 0)  {
      weather = weather.substring(0, search1 - 2) + "Ceiling at " + weather.substring(search1 - 2, search1 + 2) + " Ft " + weather.substring(search1 + 2, weather.length());
    }
  }


  //  Search for Wind / Gusts in weather / Decoded Remarks
  search0 = weather.lastIndexOf("KT");
  if (search0 >= 0) {
    int search1 = weather.lastIndexOf(" ", search0);  // Start
    int search2 = weather.lastIndexOf("G", search0);  // Gust
    String Wind;
    if (search0 - search1 == 9) {
      Wind = " Wind ~" + weather.substring(search1, search1 + 4) + " Deg " + weather.substring(search1 + 4, search2) + " KT Gusting to " + weather.substring(search2 + 1, search0) + " KT ";
    }
    if (search2 < 0) {  // No Gust
      Wind = " Wind ~" + weather.substring(search1, search1 + 4) + " Deg " + weather.substring(search1 + 4, search0) + " KT ";
    }
    if (search0 - search1 > 10 ) {  // Variable
      Wind = " Wind ~" + weather.substring(search1, search2 - 2) + " "  + weather.substring(search2 - 2, search2) + " KT Gusting to " + weather.substring(search2 + 1, search0) + " KT ";
    }
    weather.replace(weather.substring(search1, search0 + 2), Wind);
  }


  //   Search for Clouds in weather / Decoded Remarks
  search0 = weather.indexOf("Broken");
  if (search0 > 0)   Decode_Alt(weather, search0);  // Decode Alt Routine
  int search1 = weather.indexOf("Broken", search0 + 10);
  if (search0 > 0  &&  search1 > 0)   Decode_Alt(weather, search1);  // Decode Alt Routine

  search0 = weather.indexOf("Scattered");
  if (search0 > 0)   Decode_Alt(weather, search0);  // Decode Alt Routine
  search1 = weather.indexOf("Scattered", search0 + 10);
  if (search0 > 0 && search1 > 0)   Decode_Alt(weather, search1);  // Decode Alt Routine

  search0 = weather.indexOf("Few");
  if (search0 > 0)   Decode_Alt(weather, search0);  // Decode Alt Routine
  search1 = weather.indexOf("Few", search0 + 10);
  if (search0 > 0 && search1 > 0)   Decode_Alt(weather, search1);  // Decode Alt Routine

  search0 = weather.indexOf("Overcast");
  if (search0 > 0)   Decode_Alt(weather, search0);  // Decode Alt Routine


  //  Search for Peak Wind in weather / Decoded Remarks
  search0 = weather.indexOf("Peak wind");
  if (search0 >= 0) {
    int search1 = weather.indexOf("/", search0 + 9);
    String Dir = "Peak Wind " + weather.substring(search1 - 5, search1 - 2) + " Deg ";
    String Wind = weather.substring(search1 - 2, search1) + " KT at ";
    String At = weather.substring(search1 + 1, search1 + 5) + " ";
    String text = Dir + Wind + At;
    weather.replace(weather.substring(search0, search1 + 5), String(text));
  }


  // For Troubleshooting :
  /* To Print UPDATED Stations or For Troubleshooting : Print Single_Metar & Remark
     Invoke line before "DECODING Remark[i] ~ (MAINLY REMOVE UNWANTED CODES in REMARKS)" and the following Print Statements:  */
   // Serial.printf("\tSig_Weather\t.%s.\n", Sig_Weather[Station_Num].c_str());
   // Serial.printf("\tRemarks\t\t.%s.\n", weather.c_str());

  weather.replace(" $", "");                             // Clean Up ~ Station needs Maintenance
  weather.replace("<br> <br>", "<br>");                  // Clean Up
  weather.replace("[ <br>", "");                         // Clean Up
  weather.replace("[ ", "");                             // Clean Up
  weather.replace("[", "");                              // Clean Up
  weather.replace(" ]", "");                             // Clean Up
  weather.replace("]", "");                              // Clean Up
  weather.replace("  ", "<br>");                         // Clean Up
  weather.replace("<br><br>", "<br>");                   // Clean Up
  weather.replace(":<br>", ": ");                        // Clean Up
  weather.replace("Temporary", "TEMPORARY");             // Clean Up
  weather.replace("becoming", "BECOMING");               // Clean Up
  weather.replace("Recent", "RECENT");                   // Clean Up
  weather.replace("Remark", "REMARK");                   // Clean Up

  // For Troubleshooting :
   // Serial.printf("\tFinal Remark\t.%s.\n\n", weather.c_str());

  return weather;                  // Return with Readable Weather
}


// ***********  Decode Alt in CIG or Clouds. Search "at" in weather string / Decoded Remarks
void Decode_Alt(String & weather, int search) {
  int search0 = weather.indexOf(" at ", search) + 4;
  if (search0 - search < 21 && search0 > 4)  {
    char Ceiling[6];
    strcpy(Ceiling, weather.substring(search0, search0 + 3).c_str());
    int ceiling = 0;
    for (int i = 0; i < 3; i++) {
      ceiling = ceiling * 10 + (Ceiling[i] - '0');
    }
    if (weather.substring(search0 + 3, search0 + 4) != " ")   // Add a Space if Neccessary
      weather = weather.substring(0, search0 + 3) + " " + weather.substring(search0 + 3, weather.length());
    if (ceiling > 100) ceiling = 0;   // Check for Errors
    if (ceiling > 0) weather = weather.substring(0, search0) + String(ceiling * 100) + " Ft" + weather.substring(search0 + 3, weather.length());

    // Search for "Variable"
    search0 = weather.indexOf("Variable", search);
    if (search0 > 0 )  {
      char Ceiling[6];
      strcpy(Ceiling, weather.substring(search0 + 8, search0 + 11).c_str());
      int ceiling = 0;
      for (int i = 0; i < 3; i++) {
        ceiling = ceiling * 10 + (Ceiling[i] - '0');
      }
      if (ceiling > 100) ceiling = 0;   // Check for Errors
      if (ceiling > 0)   weather = weather.substring(0, search0) + "Variable " + String(ceiling * 100) + " Ft " + weather.substring(search0 + 12, weather.length());
    }
  }
}


// *********** Display One Station LED
void Display_LED(int index, int wait) {
  if (index == 0)  return;
  leds[index - 1] = 0xaf5800;    // Orange Decoding Data
  FastLED.show();
  delay(wait);
  Set_Cat_LED(index);           // Set Category for This Station LED
  FastLED.show();
}


// ***********  Display Weather on LEDS
void Display_Weather_LEDS (int wait) {
  for (int index = 1; index < (No_Stations + 1); index++)  {

    if (Sig_Weather[index] != "None")   {        // IF NOT "None" in Sig Wx,  Display Weather
      Station_Num = index;                       // Station # for Server - flash button

      //  Twinkle for Flashing Weather   (index,  red,green,blue, pulses, on, off time)
      if (Sig_Weather[index].indexOf("KT") > 0)     Twinkle(index, 0x00, 0xff, 0xff, 1, 400, 300); //Gusts   Aqua
      if (Metar[index].indexOf("TS") > 0)    Twinkle(index, 0xff, 0xff, 0xff, 4,   5, 900); //Thunder White
      if (Metar[index].indexOf("FZ") > 0)    Twinkle(index, 0x00, 0x00, 0x70, 3, 300, 400); //Freezing Blue
      if (Metar[index].indexOf("SN") > 0)    Twinkle(index, 0xaf, 0xaf, 0xaf, 4, 300, 600); //Snow    White
      if (Metar[index].indexOf("SG") > 0)    Twinkle(index, 0xaf, 0xaf, 0xaf, 4, 300, 600); //Snow    White
      if (Metar[index].indexOf("RA") > 0)    Twinkle(index, 0x22, 0xfc, 0x00, 5, 300, 300); //Rain    Green
      if (Metar[index].indexOf("SH") > 0)    Twinkle(index, 0x22, 0xfc, 0x00, 2, 300, 300); //Showers Green
      if (Metar[index].indexOf("FG") > 0)    Twinkle(index, 0x30, 0x40, 0x26, 3, 400, 500); //Fog   L Yellow
      if (Metar[index].indexOf("GS") > 0)    Twinkle(index, 0x88, 0x88, 0x00, 4, 100, 800); //S Hail  Yellow
      if (Metar[index].indexOf("GR") > 0)    Twinkle(index, 0x88, 0x88, 0x00, 5, 100, 800); //L Hail  Yellow
      if (Metar[index].indexOf("IC") > 0)    Twinkle(index, 0x00, 0x00, 0x80, 3, 300, 400); //Ice C   Blue
      if (Metar[index].indexOf("PL") > 0)    Twinkle(index, 0x00, 0x00, 0xa0, 3, 300, 400); //Ice P   Blue
      if (Metar[index].indexOf("BR") > 0)    Twinkle(index, 0x20, 0x50, 0x00, 3, 500, 500); //Mist  L Green
      if (Metar[index].indexOf("DZ") > 0)    Twinkle(index, 0x22, 0x50, 0x22, 3, 300, 300); //Drizzle L Green
      if (Metar[index].indexOf("HZ") > 0)    Twinkle(index, 0x20, 0x20, 0x20, 3, 400, 400); //Haze    White/purple
      // if (Metar[index].indexOf("TCU") > 0)   Twinkle(index, 0x30, 0x30, 0x30, 2, 300, 300); //CU      Grey
      // if (Metar[index].indexOf("CB") > 0)    Twinkle(index, 0x30, 0x30, 0x40, 2, 300, 300); //CB      Grey
      //  Rest of Weather = YELLOW
      if (Metar[index].indexOf("DU") > 0)    Twinkle(index, 0x88, 0x88, 0x00, 3, 500, 500); //Dust    Yellow
      if (Metar[index].indexOf("FU") > 0)    Twinkle(index, 0x88, 0x88, 0x00, 3, 500, 500); //Smoke   Yellow
      if (Metar[index].indexOf("FY") > 0)    Twinkle(index, 0x88, 0x88, 0x00, 3, 500, 500); //Spray   Yellow
      if (Metar[index].indexOf("SA") > 0)    Twinkle(index, 0x88, 0x88, 0x00, 3, 500, 500); //Sand    Yellow
      if (Metar[index].indexOf("PO") > 0)    Twinkle(index, 0x88, 0x88, 0x00, 3, 500, 500); //Dust&Sand Yellow
      if (Metar[index].indexOf("SQ") > 0)    Twinkle(index, 0x88, 0x88, 0x00, 3, 500, 500); //Squalls Yellow
      if (Metar[index].indexOf("VA") > 0)    Twinkle(index, 0x88, 0x88, 0x00, 3, 500, 500); //Volcanic Ash Yellow
      if (Metar[index].indexOf("UP") > 0)    Twinkle(index, 0x88, 0x88, 0x00, 3, 500, 500); //Unknown Yellow
    }
    Set_Cat_LED (index);                    // Set Category for This Station LED
    FastLED.show();
    delay(100);    // Wait a smidgen
  }
  delay(wait);
}


// ***********  Twinkle for Flashing Weather
void Twinkle (int index, byte red, byte green, byte blue, byte pulses, int on_time, int off_time) {
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


// *********** Display ALL Categories on LEDS
void Display_Cat_LEDS () {
  for (int i = 0; i < (No_Stations + 1); i++) {
    Set_Cat_LED (i);  // Set Category for This Station LED
  }
  FastLED.show();
  delay(200);        // Wait a smidgen
}


// *********** Set Category for One Station LED
void Set_Cat_LED (int i)  {
  if (Category[i] == "")     Category[i] = "NA";
  if (Category[i] == "VFR")  leds[i - 1] = CRGB::DarkGreen;
  if (Category[i] == "MVFR") leds[i - 1] = CRGB::DarkBlue;
  if (Category[i] == "IFR")  leds[i - 1] = CRGB::DarkRed;
  if (Category[i] == "LIFR") leds[i - 1] = CRGB::DarkMagenta;
  if (Category[i] == "NA")   leds[i - 1] = CRGB(30, 30, 0); // Very Dim Yellow
  if (Category[i] == "NF")   leds[i - 1] = CRGB(30, 30, 0); // Very Dim Yellow
}


// *********** Display Visibility [Red White] on LEDS Gradient
void Display_Vis_LEDS (int wait) {
  for (int i = 1; i < (No_Stations + 1); i++) {
    byte hue     = 14  + Visab[i].toInt() * 4;       // (red white) [ 14 ~ 46 ]
    byte sat     = 128 - Visab[i].toInt() * 16;      // (red white) [128 ~ 0 ]
    byte bright  = 120;                              // [ <100=Dim  120=ok  >160=Too Bright/Much Power ]
    leds[i - 1] = CHSV(hue, sat, bright);            // ( hue, sat, bright )
    if (Visab[i].toInt() > 8)  leds[i - 1] = CHSV( 48, 0, bright);         // White (Vis > 8)
    if (Visab[i].indexOf("+") >= 0)  leds[i - 1] = CHSV( 48, 0, bright);   // White
    if (Stations[i].substring(0, 4) == "NULL" || Visab[i] == "NA")  leds[i - 1] = CHSV( 0, 0, 0);
  }
  FastLED.show();
  delay(wait);
}


// *********** Display Wind Direction [Yellow Red] on LEDS Gradient
void Display_Wind_Dir_LEDS (int wait) {
  for (byte i = 1; i < (No_Stations + 1); i++) {
    int newDir = wDir[i].toInt();
    if (wDir[i] == "VRB")  newDir = 0;
    int oldDir = old_wDir[i];
    int diff = oldDir - newDir;
    if (newDir > 0 && oldDir > 0) {
      if (diff < 0)      diff = newDir - oldDir;
      if (diff > 300)    diff = 360 - diff;
      leds[i - 1] = CHSV(66 - diff / 5, 120, 120); // ( hue, sat, bright )
    }
    if (Stations[i].substring(0, 4) == "NULL" || Wind[i] == "NA")  leds[i - 1] = CHSV( 0, 0, 0);
  }
  FastLED.show();
  delay(wait);
}


// *********** Display Winds [Aqua] on LEDS Gradient
void Display_Wind_LEDS (int wait) {
  for (byte i = 1; i < (No_Stations + 1); i++) {
    byte wind = Wind[i].toInt();
    leds[i - 1] = CRGB(0, wind * 7, wind * 7);
    if (wind == 0 || Category[i].substring(0, 1) == "NF")  leds[i - 1] = CHSV( 0, 0, 0);
  }
  FastLED.show();
  delay(wait);
}


// *********** Display Temperatures [Blue Green Yellow Orange Red Purple] on LEDS Gradient
void Display_Temp_LEDS (int wait) {
  for (byte i = 1; i < (No_Stations + 1); i++) {
    int hue = 160 - TempC[i] * 4;           //  purple blue green yellow orange red [160 ~ 0 ]
    if (hue < 32)  hue = 220 + hue;         //  and pink to purple < 32C or 90F
    leds[i - 1] = CHSV( hue, 180, 150);     // ( hue, sat, bright )
    if (Stations[i].substring(0, 4) == "NULL" || TempC[i] == 99)  leds[i - 1] = CHSV( 0, 0, 0);
  }
  FastLED.show();
  delay(wait);
}


// *********** Display Altimeter Pressure [Blue Purple] on LEDS Gradient
void Display_Alt_LEDS (int wait) {
  for (byte i = 1; i < (No_Stations + 1); i++) {
    byte hue = (Altim[i] - 29.92) * 100;        //  (normally) blue purple [ 70 ~ 270 ]
    leds[i - 1] = CHSV( hue + 170, 180, 150);   // ( hue, sat, bright )
    if (Stations[i].substring(0, 4) == "NULL" || Altim[i] == 0)   leds[i - 1] = CHSV( 0, 0, 0);
  }
  FastLED.show();
  delay(wait);
}


// ***********   CHECK COMMUNICATIONS FLAG
void Check_Coms() {             // Checking Communication Flag
  if (Comms_Flag > 0)  {
    Serial.printf("%s\tWAITING to Get Data  ~  Comms In Use\n", Clock);
    int count = 0;
    while (Comms_Flag > 0) {    // Checking Communication Flag 1=Active
      delay(1000);
      count++;                  // Loop for 100 seconds
      if (count > 100)  break;
    }
  }
}


//  *********** Go_Server HTML  TASK 2 for Creating Web Pages and Handling Requests
void Go_Server ( void * pvParameters ) {
  int diff_in_clouds = 2750;      // Significant CHANGE IN CLOUD BASE
  float diff_in_press  = 0.045;   // Significant CHANGE IN PRESSURE
  String header;                  // Header for Server
  int sta_n = 1;
  byte Station_Flag = 1;
  byte Summary_Flag = 1;
  String html_code;

  for (;;) {

    ArduinoOTA.handle();   //  In Over_The_Air.h

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
              client.println();                  // Send  a blank line

              // *******   CHECKING BUTTONS & Handling Requests    ******
              // Checking from Station METAR (Any Airport ID Button) or METAR Summary
              int search = header.indexOf("GET /get?Airport_Code=");
              int search1 = header.indexOf(" HTTP", search);

              if (search >= 0) {
                String Airport_Code = header.substring(search + 22, search1);
                Airport_Code.toUpperCase();     // Changes all letters to UPPER CASE
                sta_n = -1;
                for (int i = 0; i < (No_Stations + 1); i++) {    // See if Airport_Code is in the Data Base
                  if (Airport_Code == Stations[i].substring(0, 4))    sta_n = i;
                }

                if (sta_n == -1)  {     // Airport_Code NOT in Data Base
                  sta_n = 0;            // Normally Set to sta_n = 0; : Can be set to Any Station (For Troubleshooting)

                  Reset_All_Parameters(sta_n);             // Reset All Parameters
                  Stations[sta_n] = Airport_Code;
                  if (Airport_Code.length() != 4) {        // Checking for Length Error in Station Name
                    Stations[sta_n] = "NULL,";             // Reset Station Name
                    Metar[sta_n] = Airport_Code + " : Error in Station Name";
                  }  else {
                    void Check_Coms();          // Checking Communication Flag
                    Comms_Flag = 1;             // Set Communication Flag 1=Active
                    GetData("NAME", Stations[sta_n].substring(0, 4), sta_n);            // GET Some Metar ~ Station NAME
                    Decode_Name(Stations[sta_n].substring(0, 4), sta_n);                // Decode Station NAME
                    GetData("METAR", Stations[sta_n].substring(0, 4), sta_n);           // GET Some Metar ~ Station METAR
                    Comms_Flag = 0;             // Set Communication Flag 0=Reset
                    ParseMetar(sta_n);          // Parse Metar DATA
                  }
                }
                Summary_Flag = 0;
                Station_Flag = 1;
              }

              // Checking which BUTTON was Pressed
              search = header.indexOf("GET /back HTTP/1.1");
              if (search >= 0) {             // From Display Station PREVIOUS Button
                sta_n = sta_n - 1;
                if (sta_n < 0)  sta_n = No_Stations;
                while (Stations[sta_n].substring(0, 4) == "NULL")  {
                  sta_n = sta_n - 1;
                  if (sta_n < 0)  sta_n = No_Stations;
                }
                Summary_Flag = 0;
                Station_Flag = 1;
              }

              search = header.indexOf("GET /next HTTP/1.1");
              if (search >= 0) {             // From Display Station NEXT Button
                sta_n = sta_n + 1;
                if (sta_n > No_Stations)  sta_n = 0;
                while (Stations[sta_n].substring(0, 4) == "NULL")  {
                  sta_n = sta_n + 1;
                  if (sta_n > No_Stations) sta_n = 0;
                }
                Summary_Flag = 0;
                Station_Flag = 1;
              }

              search = header.indexOf("GET /flash HTTP/1.1");
              if (search >= 0) {
                sta_n = Station_Num;        // From Display Station FLASH Button
                if (sta_n < 1 || sta_n > No_Stations)  sta_n = 1;
                Summary_Flag = 0;
                Station_Flag = 1;
              }

              search = header.indexOf("GET /summary HTTP/1.1");
              if (search >= 0) {            // From Display Station SUMMARY Button
                Summary_Flag = 1;
                Station_Flag = 1;
              }

              if (Summary_Flag == 1)  {
                // *********** DISPLAY SUMMARY ***********
                // Display the HTML web page responsive in any web browser, Page Header, Title, Style & Page Body
                html_code = "<!DOCTYPE html><html><HEAD><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
                html_code += "<TITLE>METAR</TITLE>";
                html_code += "<STYLE> html { FONT-family: Helvetica; display: inline-block; margin: 0px auto; text-align: left;}</STYLE></HEAD>";
                html_code += "<BODY><h2>METAR Summary</h2>";

                Update_Time();                     // GET CurrentTime : Hour & Minute
                if (Last_Up_Min + Update_Interval > 60)   Last_Up_Min = 60 - Update_Interval;
                Count_Down = Last_Up_Min + Update_Interval - Minute;
                if (Count_Down > Update_Interval)   Count_Down = 0;

                html_code += "<B>Summary of Weather Conditions</B><BR>";
                html_code += "Current Time : " + String(Clock) + " UTC &nbsp&nbsp&nbsp&nbsp&nbsp&nbsp <FONT COLOR='Purple'>Last Update : " + Last_Up_Time + " UTC &nbsp&nbsp&nbsp&nbsp&nbsp&nbsp Next Update in " + String(Count_Down) + " Minutes</FONT><BR>";
                html_code += "Click on <B>STATION NAME</B> to View Station Details<BR>";
                client.print(html_code);

                // Display SUMMARY Table ***********
                String Deg = "Deg F";  // Set Temperature Units   ******  For Celsius : Change this to "Deg C"  ******
                String Alt = "in Hg";  // Set Altimeter Units     ******  For mBar : Change this to "mBar"  ******

                html_code = "<TABLE BORDER='2' CELLPADDING='5'>";
                html_code += "<TR BGCOLOR='Cyan'><TD>No:</TD><TD>Station Name:</TD><TD>CAT</TD><TD>SKY<BR>COVER</TD><TD>VIS<BR>Miles</TD><TD>WIND<BR>From</TD><TD>WIND<BR>Speed</TD><TD>";
                html_code += "TEMP&nbsp<BR>" + Deg + "</TD><TD>ALT<BR>" + Alt + "</TD><TD>METAR Codes & Remarks</TD></TR></FONT>";
                client.print(html_code);

                for (int i = 0; i < (No_Stations + 1); i++) {
                  String color = "<TD><FONT COLOR=";
                  if (Category[i] == "VFR" ) color += "'Green'>";
                  if (Category[i] == "MVFR") color += "'Blue'>";
                  if (Category[i] == "IFR" ) color += "'Red'>";
                  if (Category[i] == "LIFR") color += "'Magenta'>";
                  if (Category[i] == "NA")   color += "'Black'>";
                  if (Category[i] == "NF")   color += "'Grey'>";

                  // Display Station No & ID in SUMMARY
                  if (Stations[i].substring(0, 4) != "NULL")    {  // Skip Line
                    if (i == sta_n )   {
                      html_code = "<TR><TD BGCOLOR='Yellow'>" + String(i) + "</TD><TD BGCOLOR='Yellow'>" + color.substring(4, color.length()) + Stations[i] + "</FONT></TD>";
                    } else {
                      // Display LINK:  Makes a request in the header ("GET /get?Airport_Code=XXXX")
                      html_code = "<TR><TD>" + String(i) + "</TD><TD><a href='/get?Airport_Code=" + String(Stations[i].substring(0, 4)) + "'>" + color.substring(4, color.length()) + Stations[i] + "</a></FONT></TD>";
                    }
                    client.print(html_code);

                    // Display Category in SUMMARY
                    client.print(color + Category[i] + "</FONT></TD>");

                    // Display Sky Cover in SUMMARY
                    if (Sky[i].length() != 3)   {
                      client.print(color + Sky[i]);
                    } else {
                      if (old_cloud_base[i] > 0 && new_cloud_base[i] <= old_cloud_base[i] - diff_in_clouds)     // Significant DECREASE in Cloud Base Triggers MistyRose
                        client.print(F("<TD BGCOLOR='MistyRose'><FONT COLOR='Purple'>"));   else  client.print(color);
                      client.print(Sky[i] + "&nbspat<BR>" + new_cloud_base[i] + "&nbspFt&nbsp");
                      if (old_cloud_base[i] > 0)    {
                        if (new_cloud_base[i]  > old_cloud_base[i])  client.print(F("&uArr;"));  //up arrow
                        if (new_cloud_base[i]  < old_cloud_base[i])  client.print(F("&dArr;"));  //down arrow
                        if (new_cloud_base[i] == old_cloud_base[i])  client.print(F("&rArr;"));  //right arrow
                      }
                    }
                    client.print(F("</FONT></TD>"));

                    // Display Visibility in SUMMARY
                    client.print(color + Visab[i] + "</FONT></TD>");
                    // Display Wind Direction in SUMMARY
                    int newDir = wDir[i].toInt();
                    int oldDir = old_wDir[i];
                    int diff = oldDir - newDir;
                    if (newDir != 0 && oldDir != 0) {
                      if (diff < 0)      diff = newDir - oldDir;
                      if (diff > 300)    diff = 360 - diff;
                      if (diff > 30)     client.print("<TD BGCOLOR='MistyRose'><FONT COLOR='Purple'>" + wDir[i] + "</FONT></TD>");
                      else   client.print(color + wDir[i] + "</FONT></TD>");
                    } else   client.print(color + wDir[i] + "</FONT></TD>");

                    // Display Wind Speed in SUMMARY
                    if (Category[i] == "NF")  client.print(color + "NA</FONT></TD>");   else   client.print(color + Wind[i] + "</FONT></TD>");

                    // Display Temperature in SUMMARY  ***  Set Temperature Units See Above ~ Display SUMMARY Table  ***
                    float TempF = TempC[i] * 1.8 + 32;  // deg F
                    if (TempC[i] == 99)   client.print(color + "NA");
                    else  {
                      if (Deg == "Deg C")   client.print(color + String(TempC[i], 1));
                      else client.print(color + String(TempF, 1));
                    }
                    client.print(F("</FONT></TD>"));

                    // Display Altimeter in SUMMARY  ***  Set Pressure Units See Above ~ Display SUMMARY Table  ***
                    if (Altim[i] == 0)  client.print(color + "NA");
                    else  {
                      byte wx_flag = 0;
                      float Altm = Altim[i];
                      if (Alt == "mBar")  Altm = Altim[i] * 1013.2 / 29.92;  else  Altm = Altim[i];
                      if (old_Altim[i] > 0.1)   {
                        if (Altim[i] >= old_Altim[i] + diff_in_press)  wx_flag = 1;    // Significant INCREASE in Pressure
                        if (Altim[i] <= old_Altim[i] - diff_in_press)  wx_flag = 1;    // Significant DECREASE in Pressure
                        if (wx_flag == 1) client.print("<TD BGCOLOR = 'MistyRose'><FONT COLOR='Purple'>" + String(Altm, 2));
                        else  client.print(color + String(Altm, 2));
                        if (Altim[i] > old_Altim[i])   client.print(F("<BR>&nbsp&nbsp&uArr; ")); // up arrow
                        if (Altim[i] < old_Altim[i])   client.print(F("<BR>&nbsp&nbsp&dArr; ")); // down arrow
                        if (Altim[i] == old_Altim[i])  client.print(F("<BR>&nbsp&nbsp&rArr; ")); // right arrow
                      }  else  {
                        client.print(color + String(Altm, 2));
                      }
                    }
                    client.print(F("</FONT></TD>"));

                    // Display Metar in SUMMARY
                    Update_Ago(i);             //   Update Minutes Ago
                    if (Metar[i].substring(0, 3) == "new")  client.print(F("<TD><FONT COLOR='Purple'>"));  else  client.print(color);
                    client.print(Metar[i]);

                    // Display Remark in SUMMARY
                    if (Remark_Code[i].length() > 3)   {
                      client.print("<BR><FONT COLOR='Navy'>Remarks : " + String(Remark_Code[i]));
                    }
                    client.print(F("</FONT></TD></TR>"));
                  }
                }
                client.print(F("</TABLE>"));
              }


              if (Station_Flag == 1)  {
                // *********** DISPLAY STATION  ***********
                html_code = "<!DOCTYPE html><html><HEAD><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
                html_code += "<TITLE>METAR</TITLE>";
                html_code += "<STYLE> html { FONT-family: Helvetica; display: inline-block; margin: 0px auto; text-align: left;}</STYLE></HEAD>";
                html_code += "<BODY><h2>Station METAR</h2>";

                if (Station_Flag == 1 || Summary_Flag == 1)  {
                  html_code += "<P><FONT COLOR='Purple'><B>For : " +  Stations[sta_n] + "&nbsp&nbsp#&nbsp&nbsp" + String(sta_n) + "</B></FONT><BR>";

                  // Display BUTTONS: Makes a request in the header ("GET /back HTTP/1.1")
                  html_code += "<a href='/back'><INPUT TYPE='button' VALUE='Previous Station' onClick= sta_n></a>";
                  html_code += "<a href='/flash'><INPUT TYPE='button' VALUE='Flashing LED' onClick= sta_n></a>";
                  html_code += "<a href='/next'><INPUT TYPE='button' VALUE='Next Station' onClick= sta_n></a>";
                  html_code += "&nbsp&nbsp&nbsp&nbsp<a href=/summary><INPUT TYPE='button' VALUE='Summary of Stations' onClick= sta_n></a>";
                  html_code += "<BR><FONT COLOR='Navy'>&nbsp&nbsp&nbsp&nbspPress <B>'BUTTON'</B> when <B>LED is Flashing</B></FONT><BR>";

                  // Display TABLE/FORM: Makes a request in the header ("GET /get?Airport_Code=")
                  html_code += "<FORM METHOD='get' ACTION='get'>";
                  html_code += "<TABLE BORDER='0' CELLPADDING='1'>";
                  html_code += "<TR><TD>Enter<FONT COLOR='Navy'><B> Any Airport ID </B></FONT>Code:</TD><TD>";
                  html_code += "<INPUT TYPE='text' NAME='Airport_Code' SIZE='5'>";
                  html_code += "</TD></TR></TABLE></FORM>";

                  // Update LED and Current Time in STATION
                  Display_LED(sta_n, 300);   //  Display One Station LED
                  Update_Time();             //  GET CurrentTime : Hour & Minute

                  if (Last_Up_Min + Update_Interval > 60)   Last_Up_Min = 60 - Update_Interval;
                  Count_Down = Last_Up_Min + Update_Interval - Minute;
                  if (Count_Down > Update_Interval)   Count_Down = 0;

                  String CountDown_Time = "<FONT COLOR='Purple'>Current Time : " + String(Clock) + " UTC &nbsp&nbsp&nbsp&nbsp&nbsp&nbsp Next Update in less than 0 Minutes</FONT><BR>";
                  CountDown_Time.replace("than 0", "than " + String(Count_Down));
                  if (Count_Down == 1)  CountDown_Time.replace("Minutes", "Minute");
                  html_code += CountDown_Time;

                  // Display STATION Table  ***********
                  String Bcol = "BORDERCOLOR=";
                  if (Category[sta_n] == "VFR" ) Bcol += "'Green'";
                  if (Category[sta_n] == "MVFR") Bcol += "'Blue'";
                  if (Category[sta_n] == "IFR" ) Bcol += "'Red'";
                  if (Category[sta_n] == "LIFR") Bcol += "'Magenta'";
                  if (Category[sta_n] == "NA")   Bcol += "'Black'";
                  if (Category[sta_n] == "NF")   Bcol += "'Grey'";
                  html_code += "<TABLE " + Bcol + " BORDER='3' CELLPADDING='5'>";
                  String color = "<FONT " + Bcol.substring(6, Bcol.length()) + ">";

                  // Display Flight Category in STATION
                  html_code += "<TR><TD>Flight Category</TD><TD>" + color + "<B>" + String(Category[sta_n]) + "</B></FONT>  for " + String(Stations[sta_n]) + "</TD></TR>";

                  // Display Metar in STATION
                  Update_Ago(sta_n);             //   Update Minutes Ago
                  html_code += "<TR><TD>Station Metar</TD>";
                  if (Metar[sta_n].substring(0, 3) == "new" )  html_code += "<TD><FONT COLOR='Purple'>" + String(Metar[sta_n]) + "</FONT>"; else  html_code += "<TD>" + String(Metar[sta_n]);
                  html_code += "</TD></TR>";

                  // Display Significant Weather in STATION
                  html_code += "<TR><TD>" + color + "Significant Weather</FONT><BR><FONT COLOR='Purple'>Decoded Remarks</FONT></TD><TD>" + color + String(Sig_Weather[sta_n]) + "</FONT>";

                  //  Comments for Weather and Cloud Base in STATION
                  if (new_cloud_base[sta_n] > 1 && new_cloud_base[sta_n] <= 1200)
                    html_code += "<BR><FONT COLOR='Gold'>Low Cloud Base</FONT>";
                  if (old_cloud_base[sta_n] > 0 && old_Altim[sta_n] > 0)    {
                    if (new_cloud_base[sta_n] > old_cloud_base[sta_n] && Altim[sta_n] > old_Altim[sta_n])
                      html_code += "<BR><FONT COLOR='Gold'>Weather is Getting Better</FONT>";
                    if (new_cloud_base[sta_n] < old_cloud_base[sta_n] && Altim[sta_n] < old_Altim[sta_n])
                      html_code += "<BR><FONT COLOR='Gold'>Weather is Getting Worse</FONT>";
                  }

                  //  Remarks in STATION
                  if (Remark[sta_n].length() > 1)
                    html_code += "<BR><FONT COLOR='Purple'>" + String(Remark[sta_n]) + "</FONT>";
                  else html_code += "<BR><FONT COLOR='Purple'>None</FONT>";

                  //  Sky Cover in STATION
                  html_code += "</TD></TR><TR><TD>" + color + "Sky Cover & Ceiling</TD><TD>";
                  String SkyC;
                  if (Sky_cover[sta_n] == "")  SkyC = Sky[sta_n];  else SkyC = Sky_cover[sta_n];
                  int search0 = SkyC.indexOf(" Ft");
                  if (search0 < 0)  search0 = SkyC.length();
                  if (old_cloud_base[sta_n] > 0 && Sky[sta_n] != "NA")  {

                    if (new_cloud_base[sta_n] > old_cloud_base[sta_n])  { // INCREASE
                      SkyC = SkyC.substring(0, search0) +
                             SkyC.substring(search0, search0 + 3) + "<FONT COLOR='Purple'>&nbsp&nbsp&nbsp&uArr; from " + String(old_cloud_base[sta_n]) + " Ft</FONT>" +
                             SkyC.substring(search0 + 3, SkyC.length());
                    }
                    if (new_cloud_base[sta_n] < old_cloud_base[sta_n])  { // DECREASE
                      SkyC = SkyC.substring(0, search0) +
                             SkyC.substring(search0, search0 + 3) + "<FONT COLOR='Purple'>&nbsp&nbsp&nbsp&dArr; from " + String(old_cloud_base[sta_n]) + " Ft</FONT>" +
                             SkyC.substring(search0 + 3, SkyC.length());
                    }
                    if (new_cloud_base[sta_n] == old_cloud_base[sta_n]) { // NO CHANGE
                      SkyC = SkyC.substring(0, search0) +
                             SkyC.substring(search0, search0 + 3) + "<FONT COLOR='Purple'>&nbsp&nbsp&nbsp&rArr; No Change</FONT>" +
                             SkyC.substring(search0 + 3, SkyC.length());
                    }
                    SkyC.replace("99999 Ft", "CLEAR");

                    //  Significant Change in Cloud Base
                    if (old_cloud_base[sta_n] > 0 && old_Altim[sta_n] > 0)    {
                      if (new_cloud_base[sta_n] >= old_cloud_base[sta_n] + diff_in_clouds) // INCREASE
                        html_code += "<FONT COLOR='Navy'>Significant Increase in Cloud Base</FONT><BR>";
                      if (new_cloud_base[sta_n] <= old_cloud_base[sta_n] - diff_in_clouds) // DECREASE
                        html_code += "<FONT COLOR='Orange'>Significant Decrease in Cloud Base</FONT><BR>";
                    }
                  }
                  html_code += color + SkyC;
                  html_code += "</FONT></TD></TR>";

                  //  Visibility in STATION
                  html_code += "<TR><TD>" + color + "Visibility</TD><TD>";
                  String Vis = Visab[sta_n] + " Statute Miles";
                  if (Vis.indexOf("m") >= 0)  Vis = "Measured " + Vis;
                  if (Vis.indexOf("m") >= 0)  Vis.replace("m", "");
                  if (Visab[sta_n].indexOf("+") >= 0)  Vis = "Unlimited";
                  if (Visab[sta_n] != "NA")  html_code += color + Vis + "</FONT></TD></TR>";  else  html_code += "NA</FONT></TD></TR>";

                  //  Wind in STATION
                  html_code += "<TR><TD>Wind From</TD><TD>";
                  int newDir = wDir[sta_n].toInt();
                  int oldDir = old_wDir[sta_n];
                  if (wDir[sta_n] == "VRB")  newDir = 0;
                  int diff = oldDir - newDir;
                  if (Wind[sta_n] == "CALM")   html_code += String(Wind[sta_n]);
                  else  html_code += String(wDir[sta_n]) + " Deg at " + String(Wind[sta_n]);
                  if (newDir > 0 && oldDir > 0) {
                    if (diff < 0)      diff = newDir - oldDir;
                    if (diff > 300)    diff = 360 - diff;
                    if (diff > 30) {
                      html_code += "<FONT COLOR='Orange'> : Significant Change from " + String(old_wDir[sta_n]) + " Deg</FONT>";
                    }  else  {
                      if (newDir != oldDir)   html_code += "<FONT COLOR='Purple'> : Previously " + String(old_wDir[sta_n]) + " Deg</FONT>";
                      if (newDir == oldDir)   html_code += "<FONT COLOR='Purple'> : No Change in Direction</FONT>";
                    }
                  }
                  html_code += "</FONT></TD></TR>";

                  //  Temperature and Heat Index or Wind Chill in STATION
                  html_code += "<TR><TD>Temperature</TD><TD>";
                  if (TempC[sta_n] <= 0)  html_code += "<FONT COLOR='Blue'>";  else   html_code += "<FONT COLOR='Black'>";
                  if (TempC[sta_n] == 99)   html_code += "NA</FONT>";
                  else  {
                    // Display_Text (Temperature) in STATION
                    String Display_Text = Format_Temp_Text(TempC[sta_n], TempC[sta_n] * 1.8 + 32);
                    html_code += Display_Text + "</FONT>";
                    if (TempC[sta_n] >= 35.0)   html_code += "<FONT SIZE='-1' FONT COLOR='Red'><I>&nbsp&nbsp&nbspAnd HOT</I></FONT>";

                    // Heat Index in STATION
                    // Don't display Heat Index unless Temperature > 18 Deg C  and  Heat Index > Temperature
                    float Heat_Index = Calc_Heat_Index(sta_n);             // *** Calculate Heat_Index Routine
                    if (TempC[sta_n] >= 18  &&  TempC[sta_n] > DewptC[sta_n]  &&  Heat_Index >= TempC[sta_n]) {

                      // Display_Text (Heat Index) in STATION
                      String Display_Text = Format_Temp_Text(Heat_Index, Heat_Index * 1.8 + 32);
                      html_code += "<BR><FONT COLOR='Purple'>" + Display_Text + "&nbsp&nbsp&nbspHeat Index</FONT>";
                      if (Heat_Index >= 37.7)   html_code += "<FONT SIZE='-1' FONT COLOR='Orange'><I>&nbsp&nbsp&nbspAnd HOT</I></FONT>";
                      if (Heat_Index >= 40.5)   html_code += "<FONT SIZE='-1' FONT COLOR='Red'><I> and Dangerous</I></FONT>";
                    }

                    // Wind Chill in STATION
                    // Don't display Wind Chill unless Wind Speed > 3 KTS and Temperature < 14 Deg C
                    if (Wind[sta_n].toInt() > 3 && TempC[sta_n] < 10) {      // Note: Nugged this down
                      float Wind_Chill = Calc_Wind_Chill(sta_n);             // *** Calculate Wind Chill Routine
                      if (Wind_Chill <= TempC[sta_n]) {
                        if (Wind_Chill <= 0)  html_code += "<FONT COLOR='Blue'>";  else   html_code += "<FONT COLOR='Purple'>";

                        // Display_Text (Wind Chill)  in STATION
                        String Display_Text = Format_Temp_Text(Wind_Chill, Wind_Chill * 1.8 + 32);
                        html_code += "<BR>" + Display_Text + "&nbsp&nbsp&nbspWind Chill</FONT>";
                      }
                    }
                  }

                  // Display_Text (Max_Min_Temp)  in STATION
                  html_code += Max_Min_Temp[sta_n] + "</FONT>";

                  //  Dew Point & Relative Humidity in STATION
                  if (DewptC[sta_n] == 99)  html_code += "</TD></TR><TR><TD>Dew Point</TD><TD>NA</FONT>";
                  else  {
                    html_code += "</TD></TR><TR><TD>Dew Point<BR><FONT COLOR='Purple'>Relative Humidity</FONT></TD><TD>";
                    if (DewptC[sta_n] <= 0)  html_code += "<FONT COLOR='Blue'>";  else   html_code += "<FONT COLOR='Black'>";

                    // Display_Text (Dew Point) in STATION
                    String Display_Text = Format_Temp_Text(DewptC[sta_n], DewptC[sta_n] * 1.8 + 32);
                    html_code += Display_Text + "</FONT>";

                    // Display Relative Humidity in STATION
                    float Rel_Humid = Calc_Rel_Humid(sta_n);             // *** Calculate Relative Humidity Routine
                    if (DewptC[sta_n] >= 23.0 && Rel_Humid >= 60.0)   html_code += "<FONT SIZE='-1' FONT COLOR='Red'><I>&nbsp&nbsp&nbspAnd Muggy</I></FONT>";
                    html_code += "<BR><FONT COLOR='Purple'>&nbsp&nbsp&nbsp" + String(Rel_Humid, 0) + " %</FONT>";
                  }

                  //  Sea_Level_Pressure_mb in STATION
                  //if (SeaLpres[sta_n] != 0)  html_code += "</TD></TR><TR><TD>Sea Level Pressure [QFE]</TD><TD>" + String(SeaLpres[sta_n], 1) + " mBar&nbsp&nbsp&nbsp:&nbsp&nbsp&nbsp" + String(SeaLpres[sta_n] / 29.92, 2) + " in&nbspHg&nbsp&nbsp";

                  //  Altimeter in STATION
                  if (Altim[sta_n] == 0)  html_code += "</TD></TR><TR><TD>Altimeter [QNH]</TD><TD>NA";
                  else  {
                    html_code += "</TD></TR><TR><TD>Altimeter [QNH]</TD><TD>" +  String(Altim[sta_n] * 1013.2 / 29.92, 1) + " mBar&nbsp&nbsp&nbsp:&nbsp&nbsp&nbsp" + String(Altim[sta_n], 2) + " in&nbspHg&nbsp&nbsp";
                    if (old_Altim[sta_n] > 0)  {
                      if (Altim[sta_n] > old_Altim[sta_n]) {
                        if (Altim[sta_n] > old_Altim[sta_n] + diff_in_press)  html_code += "<FONT COLOR='Orange'>Significant Change&nbsp</FONT>";
                        html_code += "&uArr; from ";   //up arrow
                        html_code += String(old_Altim[sta_n], 2);
                      }
                      if (Altim[sta_n] < old_Altim[sta_n]) {
                        if (Altim[sta_n] < old_Altim[sta_n] - diff_in_press)   html_code += "<FONT COLOR='Orange'>Significant Change&nbsp</FONT>";
                        html_code += "&dArr; from ";   //down arrow
                        html_code += String(old_Altim[sta_n], 2);
                      }
                      if (Altim[sta_n] == old_Altim[sta_n])   html_code += "&rArr; Steady";   //right arrow
                    }
                  }

                  //  Elevation & Density Altitude in STATION
                  html_code += "</TD></TR><TR><TD>Elevation</TD><TD> " + String(Elevation[sta_n], 1) + " m&nbsp&nbsp&nbsp:&nbsp&nbsp&nbsp" + String(Elevation[sta_n] * 3.28, 1) + " Ft</TD></TR>";

                  html_code += "<TR><TD><FONT COLOR='Purple'>Estimated Density Altitude</TD><TD>";
                  float Density_Alt = Calc_Density_Alt(sta_n);             // *** Calculate Density Altitude Routine
                  if ((TempC[sta_n] == 99) || Altim[sta_n] == 0)   html_code += "<FONT COLOR='Purple'>NA</TD></TR>"; else  html_code += "<FONT COLOR='Purple'>" + String(Density_Alt / 3.28, 1) + " m&nbsp&nbsp&nbsp:&nbsp&nbsp&nbsp" + String(Density_Alt, 1) + " Ft</TD></TR>";

                  html_code += "</FONT></TABLE>";             //  End of Table in Station

                  // AIRPORT INFORMATION from AirNav.com in STATION
                  // Test Link: https://www.airnav.com/airport/Ksav
                  String airnav = "<BR><a href='https://www.airnav.com/airport/" + String(Stations[sta_n].substring(0, 4)) + "' target='_blank'><B>AIRPORT INFORMATION</B></a>";
                  html_code += airnav + "<FONT COLOR='Orange'><FONT SIZE='-1'>&nbsp&nbsp&nbsp for US Airports Only</FONT><BR>";

                  // AIRPORT RADAR from AviationWeather.gov in STATION
                  // Test Link: https://aviationweather.gov/gfa/?center=31.5521,-81.8896&zoom=9
                  String cord = Lat_Long[sta_n];
                  String radar = "<BR><a href='https://aviationweather.gov/gfa/?center=" + cord + "&zoom=10'target='_blank'><B>AIRPORT RADAR</B></a>";
                  html_code += radar + "<FONT COLOR='Orange'><FONT SIZE='-1'>&nbsp&nbsp&nbsp for US Airports Only</FONT><BR>";

                  // Display Foooter and Close
                  html_code += "<BR><FONT COLOR='Navy' FONT SIZE='-1'>File Name &nbsp&nbsp&nbsp&nbsp&nbsp&nbsp: " + String(ShortFileName);
                  html_code += "<BR>URL Address &nbsp: " + SW_addr;
                  html_code += "<BR>H/W Address &nbsp: " + HW_addr;
                  html_code += "<BR>Connected to &nbsp: " + String(ssid);
                  html_code += "<BR><B>Dedicated to : F. Hugh Magee</B>";
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



// *********** DECODE the Station NAME
void Decode_Name(String Station_name, int i) {
  int search0 = MetarData.indexOf(Station_name) + 1;      // Start Search from Here
  int search1 = MetarData.indexOf("Site:", search0);
  int search2 = MetarData.indexOf("State:", search0);
  int search3 = MetarData.indexOf("Country:", search0);
  int search4 = MetarData.indexOf("Site type", search0);  // End of text
  Stations[i] = Station_name + ", ";                      // Site plus Comma
  if (search1 > 0 && search2 > 0)  {                      // Adds State or Country
    if (MetarData.substring(search3 + 9, search4 - 3) == "US")  {
      Stations[i] += MetarData.substring(search1 + 6, search2 - 3) + ", " + MetarData.substring(search2 + 7, search3 - 3);
    }  else
      Stations[i] += MetarData.substring(search1 + 6, search2 - 3) + ", " + MetarData.substring(search3 + 9, search4 - 3);
  }
  if (Stations[i].length() < 7)
    Serial.printf("%s\tNo:%d\t%s\tNo Name Decoded, in Decode_Name\n", Clock, i, Station_name.c_str());
}


// ***********   Update Minutes Ago
void Update_Ago(int i)  {
  if (Category[i] == "NF")  return;
  int search0 = Metar[i].indexOf("Z", 4);
  if (search0 < 0)  return;
  byte obsh = Metar[i].substring(search0 - 4, search0 - 2).toInt();  // METAR Obs Time - Hour
  byte obsm = Metar[i].substring(search0 - 2, search0).toInt();      // METAR Obs Time - Minute
  int ago = ((Hour - obsh) * 60) + Minute - obsm;                    // Minutes Ago
  if (ago < 0)    ago = ((24 - obsh) * 60) + Minute - obsm;
  int search1 = Metar[i].indexOf("(");
  if (search1 < 0)  Metar[i] = Metar[i] + " (" + String(ago) + "m&nbspAgo)";   // Add Minutes Ago
  else
    Metar[i] = Metar[i].substring(0, search1 - 1) + " (" + String(ago) + "m&nbspAgo)";   // Update Minutes Ago
}


// ***********   Calculate Relative Humidity
float Calc_Rel_Humid(int sta_n)  {
  /* RH = 100 × {exp [17.625 × Dp  / (243.04 + Dp )]/exp [17.625 × T/ (243.04 + T)]}
     T & Dp in Deg C
  */
  float Rel_Humid = 100 * (exp((17.625 * DewptC[sta_n]) / (243.04 + DewptC[sta_n]))) / (exp((17.625 * TempC[sta_n]) / (243.04 + TempC[sta_n])));
  return Rel_Humid;
}


// ***********   Calculate Heat Index
float Calc_Heat_Index(int sta_n)  {
  /* HI = -42.379 + 2.04901523*T + 10.14333127*RH - 0.22475541*T*RH - 0.00683783*T*T - 0.05481717*RH*RH + 0.00122874*T*T*RH + 0.00085282*T*RH*RH - 0.00000199*T*T*RH*RH
     T  = air temperature (Deg F) [if T  > 18 Deg C > 57 Deg F]
     RH = relative humidity (%)
  */
  float Rel_Humid = Calc_Rel_Humid(sta_n);          // Calculate Relative Humidity
  float Temp = TempC[sta_n] * 1.8 + 32;  // Deg F
  float Heat_Index = (-42.379 + (2.04901523 * Temp) + (10.14333127 * Rel_Humid) - (0.22475541 * Temp * Rel_Humid) - (0.00683783 * Temp * Temp) - (0.05481717 * Rel_Humid * Rel_Humid)  + (0.00122874 * Temp * Temp * Rel_Humid) + (0.00085282 * Temp * Rel_Humid * Rel_Humid) - (0.00000199 * Temp * Temp * Rel_Humid * Rel_Humid) - 32 ) * 5 / 9;
  return Heat_Index;
}


// ***********   Calculate Wind Chill
float Calc_Wind_Chill(int sta_n)  {
  /*If wind_speed KTS > 3 && temperature <= 14 Deg C
    Wind_Chill = 13.12 + 0.6215 * Tair - 11.37 * POWER(wind_speed,0.16)+0.3965 * Tair * POWER(wind_speed,0.16)
  */
  int Wind_Speed = Wind[sta_n].toInt();
  float wind_speed = Wind_Speed * 1.852; // Convert to Kph
  float Wind_Chill = 13.12 + 0.6215 * TempC[sta_n] - 11.37 * pow(wind_speed, 0.16) + 0.3965 * TempC[sta_n] * pow(wind_speed, 0.16);
  if (Wind_Chill < 0 ) Wind_Chill = Wind_Chill - 0.5;  else  Wind_Chill = Wind_Chill + 0.5;
  return Wind_Chill;
}

// ***********   Calculate Density Altitude
float Calc_Density_Alt(int sta_n)  {
  /*Example: Elevation is 5000 feet and Altimeter is 30.09 [QNH] and Temperature(OAT) is 28*C
    Pressure Altitude =  5000 + (( 29.92 - 30.09 ) * 1000 )
    Standard Temperature is 15 degrees C at sea level and Std temperature rate is 2*C per 1000
    Std Temperature difference for 5000 ft is 15*C – 10*C [2*C per 1000 ft] = 5*C
    Aproximation for Temp Altitude = 120 * (OAT – Temp diff)
    Temp Altitude = 120 * (28*C – 5*C)
    Density Altitude = Pressure Altitude + Temp Altitude in feet
  */
  float elevationF = Elevation[sta_n] * 3.28;    //  to Feet
  float Press_Alt = elevationF + (1000 * (29.92 - Altim[sta_n]));    // in Feet
  float Temp_Alt = 120 * (TempC[sta_n] - (15 - (2 * elevationF / 1000)));    // in Feet
  float Density_Alt = Press_Alt + Temp_Alt;   // in Feet
  return Density_Alt;
}

String Format_Temp_Text (float DegC, float DegF) {
  char buffer[32];
  sprintf(buffer, "%6.1f Deg C   : %6.1f Deg F", DegC, DegF);
  String Display_Text = String(buffer);
  Display_Text.replace(" ", "&nbsp");
  return Display_Text;
}
