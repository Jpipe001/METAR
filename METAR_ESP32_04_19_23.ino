
/* 04/19/23  Latest Software on Github : https://github.com/Jpipe001/METAR    <<   Check for Latest Update

  METAR Reporting with LEDs and Local WEB SERVER
  In Memory of F. Hugh Magee, brother of John Magee author of poem HIGH FLIGHT.
  https://en.wikipedia.org/wiki/John_Gillespie_Magee_Jr.                         ~  GOOD READ
  https://joeclarksblog.com/wp-content/uploads/2012/11/high_flight-790x1024.jpg  ~  Poem HIGH FLIGHT

  https://youtu.be/Yg61_kyG2zE    HomebuiltHELP; The video that started me on this project.
  https://youtu.be/xPlN_Tk3VLQ    Getting Started with ESP32 video from DroneBot Workshop.

  https://www.aviationweather.gov/docs/metar/Stations.txt    List of ALL Stations Codes(Worldwide FAA Data Base).
  https://aeronav.faa.gov/visual/01-27-2022/PDFs/            FAA Wall Chart downloads and then edit to suit.

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
  No of Airports = 100 tested 6/24/20 (60+ possible with external power).
  When Error, successfully recovers. If not, press "RESET" button on ESP32.
  The code is stored on the ESP32 Development Board 2.4GHz WiFi+Bluetooth Dual Mode,
  once the code is downloaded, it will run by it's self and will automatically restart, even after a power off.
  ESP32 requires internet access (Network or Hot Spot) to download METARS.

  A computer, with FREE software https://www.arduino.cc/en/Main/Software , is needed to initially configure,
  download software and (optionally) display any program messages using the (built in) Serial Monitor.
  (Watch Getting Started with ESP32 video from DroneBot Workshop, link above).

  Updates METARS approximately every six minutes, so nearly REAL TIME data, from AVIATIONWEATHER.GOV.
  A set of WS2812 LEDS show all station CATEGORIES (similar to the HomebuiltHELP video, link above).
  Then cycles through all the stations and flashes individually for:
  Wind Gusts(Cyan)[suspendable], Precipitation(Green/White), Ice(Blue), Other(Yellow) and Significant Change(Orange).
  Then displays "RAINBOW" for all stations, for Visibility [Yellow to Red], Wind Speed Gradient [Cyan],
  Temperature Gradient [Blue Green Yellow Orange Red] and Altimeter Pressure Gradient [Blue to Purple].

  DISPLAYS: Viewable with a computer or cell phone connected to the SAME network:
  SUMMARY html gives a colorful overview and
  STATION html shows DECODED METAR information and much MORE.  (See Below for Improvements)

  NOTE: To view these, you need the http address which is shown at start up, if the Serial Monitor is switched on.
  or Click on this Logical Name Test Link  http://metar.local

  Makes a GREAT Christmas Tree Chain of Lights, TOO (and a Good Conversation Piece).

  OPTIONAL:
   In Main Loop: Turn LEDS OFF at NIGHT  5PM to 8AM ~ Local Time ~ Enabled
   In Main Loop: Reduce LED Power usage while getting Data ~ Disabled
   In Display_Metar_LEDS: "Rainbow" Effect ~ Partially Enabled
   In Parsed_metar: Print Stations that were Updated  ~ Disabled
   In Checking AIRPORT CODE: Airport Code can be assigned to any Station No ~ Disabled Set to 0

  MADE THINGS A LITTLE BETTER, BUG FIXES, IMPROVEMENTS, REPAIRS TO TIME-SPACE CONTINUUM, ETC, ETC.
  Includes: Decoded Metar, Current UTC Time, Temperature in Deg F, Elevation Ft, Estimated Density Altitude Ft.
  ANY Airport code may be used in the Worldwide FAA Data Base(see above link), but optimized for US airports.
  Questions, Comments or Suggestions to Jpipe001@Gmail.com.

  //  RECENT CHANGES:
  Started this project 10/31/19
  Modified Significant Weather to include Cloud Cover, RVR & Weather 12/31/19
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
  Logical Address to the server with http://metar.local 10/29/20
  Added Remarks 02/11/21
  Modified Dictionary 04/14/21
  Modified HTML 04/29/21
  Fixed rem pointer error 05/18/21
  Removed Flashing Sig Weather 07/12/21
  Added debug print of remarks 07/30/21
  More Readable, Modified Dictionary 08/27/21
  More Reliable, More Modified Dictionary 01/19/22
  Modified to Summary Page to Jump to a Station 03/2/22
  Few minor Tweaks 03/11/22
  Reading Codes Backwards 03/14/22
  Modified Dictionary 03/22/22
  Modified Search 04/04/22
  Added Wind to remarks 04/04/22
  Changed to Printf  06/12/22
  Changed Vis/Temp/Press Display Colors a Little 06/26/22
  Added Remarks to Display Summary 07/29/22
  ENTERED Station Can be set to Any Station 07/04/22
  Added Heat Index, Windchill, Relative Humidity to Station Display 07/23/22
  Cleaned up Remarks 08/01/22
  Added to Alitmeter 08/04/22
  Optional to Reduce LED Power usage while getting Data 08/09/22
  Improved Error Handling in GetData, if Comms Error; Stations/LEDS Skip Update 08/18/22
  Added Ago Update in Displays 08/24/22
  Added "LIGHTS OUT" at Night 08/26/22
  Improved Ceiling 11/02/22
  Allow Over The Air Software Updates 04/06/23
  Added AirNav link in Station Display 04/08/23
  Minor Improvements to make things Better 04/19/23.
*/

// Include the folling Libaries:
#include <Arduino.h>
#include "ESP32_OTA.h"    // Over The Air Software Updates ~ Put This File in the SAME Folder as METAR

#include <FastLED.h>      // FastLED  by Daniel Garcia
#include <WiFiMulti.h>    // WifiMulti_Generic  by Khoi Hoang
WiFiMulti wifiMulti;
#include <HTTPClient.h>   // HttpClient  by Adrian McEwen
#include <ESPmDNS.h>      // MDNS_Generic  by Khoi Hoang
WiFiServer server(80);    // Set web server port number to 80

// To get Station DATA used for MetarData in "GetData" Routine
// Test link: https://www.aviationweather.gov/adds/dataserver_current/httpparam?datasource=metars&requestType=retrieve&format=xml&mostRecentForEachStation=constraint&hoursBeforeNow=1.25&stationString=KFLL,KFXE
String    host = "https://aviationweather.gov";
String    urlb = "/adds/dataserver_current/httpparam?datasource=metars&requestType=retrieve&format=xml&mostRecentForEachStation=constraint&hoursBeforeNow=1.25&stationString=";

// To get Station NAME and Information in "Go_Server" and "Decode_Name" Routines
// Test link: https://aviationweather.gov/adds/dataserver_current/httpparam?dataSource=Stations&requestType=retrieve&format=xml&stationString=KFLL
String    urls = "/adds/dataserver_current/httpparam?dataSource=Stations&requestType=retrieve&format=xml&stationString=";

// To Invoke HTML Display/Web Page after downloading code with a Stable LOGICAL Address, because hardware address may change.
// Test link Address: http://metar.local


//  ################   CONFIGURE YOUR NETWORK SETTINGS HERE  ################
const char*      ssid = "your network name";          // your network SSID (name)
const char*  password = "your network password";      // your network password

//const char*      ssid = "iPhone";          // your network SSID (name) ~ iPhone Example
//const char*  password = "johnjohn";        // your network password


// Set Up Time Server
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;                // UTC Time : Time Zone 60*60*0 Hrs
const int   daylightOffset_sec = 0;           // UTC Time : Offset 60*60*0 Hr
struct tm timeinfo;                           // Time String "%A, %B %d %Y %H:%M:%S"


// Set Up LEDS
#define No_Stations          52      // Number of Stations / Number of LEDs
#define NUM_LEDS    No_Stations      // Number of LEDs
#define DATA_PIN              5      // Connect LED Data Line to pin D5/P5/GPIO5  *** With 220 to 330 Ohm Resistor in Line ***
// Connect LEDS to Ground & 5 Volts on ESP32 pins
#define LED_TYPE         WS2812      // WS2811 or WS2812 or NEOPIXEL
#define COLOR_ORDER         GRB      // WS2811 are RGB or WS2812 are GRB or NEOPIXEL are CRGB
#define BRIGHTNESS           16      // Master LED Brightness (<12=Dim 16~20=ok >20=Too Bright/Much Power) Set as required
#define FRAMES_PER_SECOND   120
CRGB leds[NUM_LEDS];                 // Color Order for LEDs ~ CRGB for Displaying Gradients


// Define STATIONS Variables      ~~~~~      Some Examples
std::vector<String> PROGMEM Stations {  //   << Set Up   - Do NOT delete this line.
  "NULL, STATION NAME         ",        // 0 << Reserved - Do NOT delete this line.
  "KCHA, CHATTANOOGA, TN      ",        // 1 << START modifying from here
  "KRMG, ROME, GA             ",        // 2  Order of LEDs; NULL for no airport or LED not used
  "KVPC, CARTERSVILLE, GA     ",        // 3
  "KATL, ATLANTA, GA          ",        // 4  First FIVE Characters are REQUIRED !!
  "KCTJ, CARROLTON, GA        ",        // 5
  "KLGC, LA GRANGE, GA        ",        // 6  Over type your Station Code and Station Name.
  "KCSG, COLUMBUS, GA         ",        // 7  and include the "quotes" and the "first and last comma" (SYNTAX is IMPORTANT).
  "KMCN, MACON, GA            ",        // 8  Padding after Station Name is not necessary.
  "KCKF, CORDELLE, GA         ",        // 9
  "KABY, ALBANY, GA           ",        // 10 Add or Delete Stations as necessary.
  "KTLH, TALLAHASSEE, FL      ",        // 11
  "KVLD, VALDOSTA, GA         ",        // 12 Note: SKYVECTOR.COM is good for locating METAR Reporting Stations
  "KAYS, WAYCROSS, GA         ",        // 13
  "KJAX, JACKSONVILLE, FL     ",        // 14
  "KBQK, BRUNSWICK, GA        ",        // 15
  "KSAV, SAVANNAH, GA         ",        // 16
  "KTBR, STATESBORO, GA       ",        // 17
  "KAGS, AUGUSTA, GA          ",        // 18
  "KAHN, ATHENS, GA           ",        // 19
  "KCEU, CLEMSON, GA          ",        // 20
  "KJES, JESUP (my home town), GA",     // 21 You can customise the Station Name.
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
  "EGYP, MOUNT PLEASANT, Falkland Islands", // 44
  "EHAM, AMSTERDAM SCHIPHOL   ",        // 45
  "LFSB, BASEL, SWITZERLAND   ",        // 46
  "KORL, ORLANDO, FL          ",        // 47
  "KPBI, WEST PALM BEACH, FL  ",        // 48
  "KFXE, FT LAUDERDALE EXE, FL",        // 49
  "KFLL, FT LAUDERDALE INT, FL",        // 50
  "KHWO, NTH PERRY, FL        ",        // 51
  "KMIA, MIAMI, FL            ",        // 52
  "KEYW, KEY WEST, FL         ",        // 53
  "KHXD, HILTON HEAD, SC      ",        // 54
  "KTTS, NASA SPACE LAUNCH FACILITY, FL",  // 55
  "KDAB, DAYTONA BEACH, FL    ",        // 56
  "NULL, STATION NAME         ",        // 57 You can have dummy LED that is not used but is connected in the chain.
  "KCHS, CHARLESTON, SC       ",        // 58
  "KMYR, MYRTLE BEACH, SC     ",        // 59
  "KDAL, DALLAS/LOVE FIELD, TX ",       // 60
};                                      // << Do NOT delete this line.

//  ################   END OF SETTINGS  ################

PROGMEM String  StationMetar[No_Stations + 1];   // Station Metar code including "new"
PROGMEM String StationRemark[No_Stations + 1];   // Station Remark code including "brackets"
PROGMEM String        Remark[No_Stations + 1];   // Final Remark for Station (text)
PROGMEM String   Sig_Weather[No_Stations + 1];   // Significant Weather
PROGMEM String          Wind[No_Stations + 1];   // Wind speed
PROGMEM String          wDir[No_Stations + 1];   // Wind direction
PROGMEM String           Sky[No_Stations + 1];   // Sky_cover
PROGMEM String      Category[No_Stations + 1];   // NULL   VFR    MVFR   IFR    LIFR
PROGMEM float          Visab[No_Stations + 1];   // Visibility
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
PROGMEM String MetarData;     // Raw METAR data
char Clock[] = "HH:MM";       // Clock  "HH:MM"
byte Hour = 0;                // Latest Hour
byte Minute = 0;              // Latest Minute
String Last_Up_Time;          // Last Update Time  "HH:MM"
byte Last_Up_Min = 0;         // Last Update Minute
byte Group_of_Stations = 21;  // Get a Group of <28 Stations at a time
byte Update_Interval = 6;     // Updates Data every 6 Minutes (Don't overload AVIATIONWEATHER.GOV)
byte Count_Down = 0;          // Count to Next Update
byte Station_Num = 1;         // Station # for Server - flash button
int httpCode;                 // Error Code
byte Comms_Flag = 0;          // Communication Flag
String HW_addr;               // WiFi local hardware Address
String SW_addr;               // WiFi local software Address
TaskHandle_t Task1;           // Main_Loop, Core 0
TaskHandle_t Task2;           // Go_Server, Core 1
String ShortFileName;         // Shortened File Name
const char* ServerName = "metar";      // Logical Address http://metar.local


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);     // The Onboard LED
  Serial.begin(115200);
  delay(4000);         // Time to press "Clear output" in Serial Monitor

  Serial.printf("\nMETAR Reporting with LEDs and Local WEB SERVER\n");
  ShortFileName = String(__FILE__).substring(String(__FILE__).lastIndexOf("\\") + 1);       // Shortened File Name
  if (ShortFileName == __FILE__) ShortFileName = String(__FILE__).substring(String(__FILE__).lastIndexOf("/") + 1);  // Shortened File Name in case Raspberry pi
  Serial.printf("Long File Name   ~ %s\n", __FILE__);
  Serial.printf("Short File Name  ~ %s\n", ShortFileName.c_str());
  Serial.printf("Date Compiled    ~ %s\n\n", __DATE__);

  // Initialize the WiFi network settings.
  WiFi.begin(ssid, password);

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

  SetupOTA(ServerName, ssid, password);   // ESP32_OTA.h Set Up

  if (!MDNS.begin(ServerName) || count > 80) {     // Start mDNS with ServerName
    Serial.printf("\nSOMETHING WENT WRONG\nProgram Halted  ~  Check Network Settings !!\nError setting up MDNS responder !\n");
    while (1) {
      delay(1000);                   // Stay here
    }
  }

  // Print the Signal Strength:
  long rssi = WiFi.RSSI() + 100;
  Serial.printf("Signal Strength = %ld", rssi);
  if (rssi > 50)  Serial.printf(" (>50 - Good)\n");  else   Serial.printf(" (Could be Better)\n");

  Init_LEDS();         // Initialize LEDs

  Serial.printf("*******************************************\n");

  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(ssid, password);

  Serial.printf("To View Decoded Station METARs from a Computer or \nCell Phone connected to the %s WiFi Network.\n", ssid);

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
    Off_Hr = 24;          // ****   Delete this line, To Invoke This Fuction

    if (Hour == Off_Hr)  {               // Check for Off Hour
      while (Hour != On_Hr)  {           // Loop until On Hour
        Display_Black_LEDS();            // Set All LEDS to Black
        delay(60 * 1000);                // Wait a Minute 60 Seconds
        Update_Time();                   // Update Current Time : Hour & Minute
      }
    }

    if (Last_Up_Min + Update_Interval > 60)   Last_Up_Min = 60 - Update_Interval;
    Count_Down = Last_Up_Min + Update_Interval - Minute;

    if (Comms_Flag > 0)  {
      Serial.printf("%s   WAITING to Get Metar Update  -  Comms In Use\n", Clock);
      int count = 0;
      while (Comms_Flag > 0) {    // Checking Communication Flag 1=Active
        delay(1000);
        count++;                  // loop for 100 seconds
        if (count > 100)  break;
      }
    }
    delay(200);                      // Wait a smidgen
    Comms_Flag = 1;                  // Communication Flag 1=Active
    //Display_Black_LEDS();          // Optional ~ To Reduce Power before getting Data Set All LEDS to Black
    GetAllMetars();                  // Get All Metars and Display Categories
    Comms_Flag = 0;                  // Communication Flag 0=Reset

    Serial.printf("%s\tMetar Data Updated\tNext Update in %d Mins\n", Clock, Count_Down);

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
  Display_Cat_LEDS ();              //  Display LEDS for All Categories  ~~ Do NOT Delete
  Display_Weather_LEDS (10);        //  Display LEDS for Twinkle Weather ~~ Suspendable
  delay(8000);                      //  Delay after Display Weather (Seconds * 1000) ~~ Change this but Do NOT Delete

  // ***********   Comment these lines out to suspend a function

  int Wait_Time = 6000;             //  Delay after Function (Seconds * 1000) ~~ Change this but Do NOT Delete
  Display_Vis_LEDS (Wait_Time);     //  Display LEDS for Visibility [Red-Pink-White]
  //Display_Wind_LEDS (Wait_Time);    //  Display LEDS for Wind Speed [Shades of Aqua] ~ Suspended
  Display_Temp_LEDS (Wait_Time);    //  Display LEDS for Temperatures [Blue-Green-Yellow-Red]
  Display_Alt_LEDS (Wait_Time);     //  Display LEDS for Distribution of Pressure [Blue-Purple]
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
  Display_Black_LEDS();                        // Set All LEDS to Black
  Serial.printf("LEDs Initialized for No_Stations = %d\n", No_Stations);
}


// *********** Reset LEDS to Black (Start Up & before Get Data)
void Display_Black_LEDS() {
  fill_solid(leds, No_Stations, CRGB::Black);  // Set All leds to Black
  FastLED.show();
  delay(50);    // Wait a smidgen
}


// *********** GET All Metars in Chunks
void GetAllMetars() {                    // Get a Group of Stations <20 at a time
  for (byte j = 0; j < No_Stations; j = j + 1 + Group_of_Stations) {
    byte Start  = j;
    byte Finish = Start + Group_of_Stations;
    if (Finish > No_Stations)  Finish = No_Stations;
    String url = "";
    for (byte i = Start; i <= Finish; i++) {
      String station = Stations[i].substring(0, 5);
      url = url + String(station);
    }
    GetData(url, Start);               // GetData Routine ~ Get Some Metar Data (Group of Stations at a time)
    for (int i = Start; i <= Finish; i++) {
      ParseMetar(i);                  // ParseMetar Routine ~ One Station at a time
    }
  }
}


// *********** GET Some Metar Data/Name Group of Stations at a time
void GetData(String url, int i) {
  MetarData = "";                     // Reset Raw Data for Group of Stations
  if (url == "NAME") url = host + urls + Stations[i];  else   url = host + urlb + url;
  url = url.substring(0, url.length() - 1);    // Remove last "comma"

  if (wifiMulti.run() == WL_CONNECTED)  {
    //digitalWrite(LED_BUILTIN, HIGH);   // ON ~~ Only For Troubleshooting
    HTTPClient https;
    https.begin(url);                    // Start connection and send HTTP header
    httpCode = https.GET();              // httpCode will be negative on error so test or 200 for HTTP_CODE_OK
    if (httpCode == 200 ) {              // HTTP_CODE_OK
      // HTTP header has been send and Server response header has been handled
      // and File FOUND at server. Get back : httpCode = 200 (HTTP_CODE_OK)
      MetarData = https.getString();     // SAVE DATA in MetarData
      https.end();                       // CLOSE Communications
      digitalWrite(LED_BUILTIN, LOW);    // OFF

      // For Troubleshooting : Print url, MetarData Size, Check Max Allocated Heap & Print MetarData
      //Serial.printf("%s\tNo:%d\tIn GetData : url=%s\n", Clock, i, url.substring(url.indexOf("stationString="), url.length()).c_str());
      //Serial.printf("%s\tNo:%d\tIn GetData : MetarData=%d  MaxAllocHeap=%d  Free=%d  httpCode=%d ~ %s\n", Clock, i, MetarData.length(), ESP.getMaxAllocHeap(), ESP.getMaxAllocHeap() - MetarData.length(), httpCode, https.errorToString(httpCode).c_str());
      //Serial.printf("%s\tNo:%d\tIn GetData : MetarData:\n%s\n", Clock, i, MetarData.c_str());

      if (ESP.getMaxAllocHeap() - MetarData.length() < 10500)   { // NO UPDATE : RUNNING LOW OF MEMORY
        httpCode = -100;                                          // NO UPDATE for this Group of Stations, in ParseMetar
        Serial.printf("%s\tNo:%d\tNo Update ~ Skipped Group, in GetData : Running Low on Memory\n", Clock, i);
      }
      if (MetarData.length() < 500)   {                           // NO UPDATE : NO MetarData DOWNLOADED
        httpCode = -200;                                          // NO UPDATE for this Group of Stations, in ParseMetar
        Serial.printf("%s\tNo:%d\tNo Update ~ Skipped Group, in GetData : No MetarData Downloaded\n", Clock, i);
        Network_Status ();               // WiFi Network Error
      }

    }  else  {                           // CONNECTION ERROR : NO UPDATE for this Group of Stations, in ParseMetar
      https.end();                       // CLOSE Communications
      digitalWrite(LED_BUILTIN, LOW);    // OFF
      Serial.printf("%s\tNo:%d\tNo Update ~ Skipped Group, in GetData : Connection Error=%d ~ %s\n", Clock, i, httpCode, https.errorToString(httpCode).c_str());
      Network_Status ();                 // WiFi Network Error
    }
  }
}


// ***********   WiFi Network Error
void Network_Status() {
  Serial.printf("%s\t\tWiFi Network Status: ", Clock);
  if (WiFi.status() == 0 ) Serial.printf("= 0 ~ Idle");
  if (WiFi.status() == 1 ) Serial.printf("= 1 ~ Not Available, SSID can not be reached");
  if (WiFi.status() == 2 ) Serial.printf("= 2 ~ Scan Completed");
  if (WiFi.status() == 3 ) Serial.printf("= 3 ~ Connected");
  if (WiFi.status() == 4 ) Serial.printf("= 4 ~ Failed, password incorrect");
  if (WiFi.status() == 5 ) Serial.printf("= 5 ~ Connection Lost");
  if (WiFi.status() == 6 ) Serial.printf("= 6 ~ Disconnected");
  Serial.printf("\n");
}


// ***********   Reset All Parameters if Station Not Found in ParseMetar/Go_Serverl
void Reset_All_Parameters(int i) {
  Category[i] = "NF";             // Not Found
  Sky[i] = "NA";                  // Not Found
  StationRemark[i] = "";
  StationMetar[i] = "Station Not Reporting";
  if (httpCode != HTTP_CODE_OK)  StationMetar[i] = "Connection Error";
  Sig_Weather[i] = "Not Found";   // Not Found
  Visab[i] = 0;                   // Not Found
  wDir[i] = "NA";                 // Not Found
  Wind[i] = "NA";                 // Not Found
  TempC[i] = 0;                   // Not Found
  DewptC[i] = 0;                  // Not Found
  Altim[i] = 0;                   // Not Found
  old_Altim[i] = 0;               // Not Found
  Remark[i] = "";                 // Not Found
  Elevation[i] = 0;               // Not Found
}


// ***********   Parse Metar Data
void ParseMetar(int i) {
  String Parsed_metar = "";
  String station = Stations[i].substring(0, 4);

  if (station == "NULL")   return;       // NO UPDATE for this Station
  if (httpCode <= 0)  {                  // NO UPDATE ~ Connection Error or No Data Downloaded
    if (StationMetar[i].indexOf("Skipped") < 0)   StationMetar[i] = "<FONT COLOR='Orange'>Skipped </FONT>" + StationMetar[i];
    return;
  }

  int Data_Start = MetarData.indexOf(station, 0);                       // Search for Station ID
  int Data_End  = MetarData.indexOf("</METAR>", Data_Start + 1) + 8;    // Search for data end "</METAR>"

  // For Troubleshooting :
  //Serial.printf("%s\tNo:%d\t.%s.\tIn ParseMetar : StationMetar[i] = %s\n", Clock, i, station.c_str(), StationMetar[i].c_str());
  //Serial.printf("%s\tNo:%d\t.%s.\tIn ParseMetar : Data_Start = %6d\tData_End = %6d\tMetarData.length = %6d\n", Clock, i, station.c_str(), Data_Start, Data_End, MetarData.length());

  if (Data_Start > 0)  {     // STATION FOUND
    Parsed_metar = MetarData.substring(Data_Start, Data_End);       // Parse Metar Data

    // Remove found data from MetarData
    MetarData = MetarData.substring(0, Data_Start) + MetarData.substring(Data_End, MetarData.length());

    Decodedata(i, station, Parsed_metar);                           // DECODE the Station DATA

  } else {                  // STATION NOT FOUND
    Serial.printf("%s\tNo:%d\t%s\tStation Not Reporting, in ParseMetar\n", Clock, i, station.c_str());
    Reset_All_Parameters(i);       // Reset All Parameters if STATION NOT FOUND
    Display_LED(i, 20);            // Display Station LED
  }
}


// *********** DECODE the Station DATA
void Decodedata(int i, String station, String Parsed_metar) {
  String old_obs_time = StationMetar[i].substring(0, 4);                  // Previous Last Observation Time
  if (old_obs_time == "new ")  old_obs_time = StationMetar[i].substring(4, 8);
  // Otherwise old_obs_time = "" and will UPDATE later

  int search_Strt = Parsed_metar.indexOf(station, 0) + 7;                 // Start of search
  int search_Raw_Text = Parsed_metar.indexOf("</raw_text", search_Strt);  // End of Remark - "</raw_text"
  if (search_Strt < 7)  {
    Serial.printf("%s\tNo:%d\t%s\tFailed in Decodedata search_Strt= %d\n", Clock, i, station.c_str(), search_Strt);
    return;
  }

  // Search for Station Metar End in Parsed_metar
  int search_From = Parsed_metar.indexOf("Z ", search_Strt) + 10;          // Start search from timeZ +10
  int search_End = Parsed_metar.indexOf(" A", search_From) + 6;            // Search for "A"  in  US : Station Metar End
  int search_EndQ = Parsed_metar.indexOf(" Q", search_From) + 6;           // Search for "Q"  Non US : Station Metar End
  int search_EndR = Parsed_metar.indexOf("RMK", search_From) + 3;          // Search for "RMK"

  if (search_End > search_EndR)  search_End = search_EndR;                 // No "A" Found in US BUT Found "AO" : EndR = Station Metar End
  if (search_EndQ > search_From)  search_End = search_EndQ;                // Found "Q" Non US : EndQ = Station Metar End

  if (search_End < search_From)  search_End = search_Raw_Text;             // Something is Wrong : Use Default for End
  if (search_End > search_Raw_Text)  search_End = search_Raw_Text;         // Something is Wrong : Use Default for End

  // Append Minutes ago updates on every cycle
  Update_Time();                                               // Get Time : Hour & Minute
  int obsh = Parsed_metar.substring(7, 9).toInt();             // METAR Obs Time - Hour
  int obsm = Parsed_metar.substring(9, 11).toInt();            // METAR Obs Time - Minute
  int ago = ((Hour - obsh) * 60) + Minute - obsm;              // Minutes ago
  if (ago < 0)    ago = ((24 - obsh) * 60) + Minute - obsm;

  // *** CREATE  Station Metar : Codes updates on every cycle
  StationMetar[i] = Parsed_metar.substring(search_Strt, search_End) + " (" + String(ago) + "m&nbspago)";  // Append ago

  // *** UPDATE/SKIP this Station *** : Check Last Observation Time with [Parsed_metar Observation Time] in StationMetar
  if (old_obs_time != StationMetar[i].substring(0, 4))  {       // ***  If NO UPDATE : SKIP this STATION  ***

    // *** NEW DATA: UPDATING STATION
    StationMetar[i] = "new " + StationMetar[i];

    // *** Decoding after RMK > Remark[i]
    String mesg;
    String text;

    // Search for Remark Start in Parsed_metar
    search_From = Parsed_metar.indexOf("Z ", search_Strt) + 10;            // Start search from timeZ +10
    search_End = Parsed_metar.indexOf(" A", search_From) + 10;             // Search for "A"  in  US : Remark Start
    search_EndQ = Parsed_metar.indexOf(" Q", search_From) + 6;             // Search for "Q"  Non US : Remark Start
    search_EndR = Parsed_metar.indexOf("RMK", search_From) + 3;            // Search for "RMK"

    if (search_End > search_EndR)  search_End = search_EndR;               // No "A" Found in US BUT Found "AO" : EndR = Remark Start
    if (search_EndR > search_End)  search_End = search_End - 4;            // Found "RMK" out of place : End"A" = Remark Start
    if (search_EndQ > search_From)  search_End = search_EndQ;              // Found "Q" Non US : EndQ = Remark Start

    if (search_End < search_From)  search_End = search_Raw_Text;           // No "RMK" Found : Use Default for Remark Start
    if (search_End > search_Raw_Text)  search_End = search_Raw_Text;       // Something is Wrong : Use Default for Remark Start

    // *** CREATE  Remark : Codes to Text
    // StationMetar[i]    Station Metar Codes including "new" & "ago"
    //StationRemark[i]    Station Remark Codes used in Sumary Display
    //       Remark[i]    Station Remark Codes, Overwrites to Decoded Remark for Station (text)

    String Metar_Code = Parsed_metar.substring(search_Strt, search_End);             // Station Metar code NOT including "new" "station" & "ago"
    StationRemark[i] = Parsed_metar.substring(search_End, search_Raw_Text);          // Station Remark code NOT including Brackets in Sumary Display
    Remark[i] = "[" + Parsed_metar.substring(search_End, search_Raw_Text) + " ]";    // Adds Brackets and a SPACE for easy viewing

    // ***  OPTIONAL  ***  To print UPDATED Stations or For Troubleshooting : Print Parsed_metar
    //Serial.printf("No:%d\t%s %s %s\n", i, station.c_str(), Metar_Code.c_str(), Remark[i].c_str());

    // DECODING Remark  (MAINLY REMOVE UNWANTED CODES)

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
      if (search2 > 4)  mesg = " <br>";  else mesg = "";
      mesg = mesg + "Hourly Precip=" + text.substring(3, 4) + "." + text.substring(4, 6) + '"';
      mesg.replace("0.00", "Trace");
      Remark[i].replace(Remark[i].substring(search2, search3), mesg);
    }


    //  ****  REPLACE   Itnnn  Hourly Ice Accretion in Hundredths of Inch [Hourly]
    //  t = 1 or 3 or 6 hour   nnn= Icing amount
    search2 = Remark[i].lastIndexOf(" I1");
    search3 = Remark[i].indexOf(" ", search2 + 1);
    if (search3 - search2 != 6)  search3 = -1;                  // Check for length

    if (search2 >= 0 && search3 >= 0)   {
      text = Remark[i].substring(search2, search3);
      if (search2 > 4)  mesg = " <br>";  else mesg = "";
      mesg = mesg + "Hourly Ice=" + text.substring(3, 4) + "." + text.substring(4, 6) + '"';
      mesg.replace("0.00", "Trace");
      Remark[i].replace(Remark[i].substring(search2, search3), mesg);
    }


    //  ****  REPLACE   Itnnn  Ice Accretion in Hundredths of Inch [3 Hour]
    //  t = 1 or 3 or 6 hour   nnn= Icing amount
    search2 = Remark[i].lastIndexOf(" I3");
    search3 = Remark[i].indexOf(" ", search2 + 1);
    if (search3 - search2 != 6)  search3 = -1;                  // Check for length

    if (search2 >= 0 && search3 >= 0)   {
      text = Remark[i].substring(search2, search3);
      if (search2 > 4)  mesg = " <br>";  else mesg = "";
      mesg = mesg + "3 Hour Ice=" + text.substring(3, 4) + "." + text.substring(4, 6) + '"';
      mesg.replace("0.00", "Trace");
      Remark[i].replace(Remark[i].substring(search2, search3), mesg);
    }


    //  ****  REPLACE   Itnnn  Ice Accretion in Hundredths of Inch [6 Hour]
    //  t = 1 or 3 or 6 hour   nnn= Icing amount
    search2 = Remark[i].lastIndexOf(" I6");
    search3 = Remark[i].indexOf(" ", search2 + 1);
    if (search3 - search2 != 6)  search3 = -1;                  // Check for length

    if (search2 >= 0 && search3 >= 0)   {
      text = Remark[i].substring(search2, search3);
      if (search2 > 4)  mesg = " <br>";  else mesg = "";
      mesg = mesg + "6 Hour Ice=" + text.substring(3, 4) + "." + text.substring(4, 6) + '"';
      mesg.replace("0.00", "Trace");
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
      text = text.substring(2, 5) + "." + text.substring(5, 6) + " DewPt " + text.substring(6, 9) + "." + text.substring(9, 10);
      mesg = "";
      Remark[i].replace(Remark[i].substring(search2, search3), mesg);
    }


    //  ****  REMOVE  1sTTT  6 Hr MAX Temperature
    //3-hourly report: A METAR taken at 0300, 0900, 1500, or 2100 UTC.
    //6-hourly report: A METAR taken at 0000, 0600, 1200, or 1800 UTC
    // at 0000, 0600, 1200, and 1800 UTC observations
    // s= 0 Positive s= 1 Negative  T= Temp in Deg C
    search2 = Remark[i].lastIndexOf(" 10");                      //  Positive
    if (search2 < 0)    search2 = Remark[i].lastIndexOf(" 11");  //  Negative
    search3 = Remark[i].indexOf(" ", search2 + 1);
    if (search3 - search2 != 6)  search3 = -1;                  // Check for length

    if (search2 >= 0 && search3 >= 0)  {
      text = Remark[i].substring(search2, search3);
      if (text.charAt(2) == '0')  text.setCharAt(2, '+');  else  text.setCharAt(2, '-');
      text = text.substring(2, 5) + "." + text.substring(5, 6);
      Remark[i].replace(Remark[i].substring(search2, search3), "");
    }


    //  ****  REMOVE  2sTTT  6 Hr MIN Temperature
    // s= 0 Positive s= 1 Negative  T=Temp in Deg C
    search2 = Remark[i].lastIndexOf(" 20");                      // Positive
    if (search2 < 0)    search2 = Remark[i].lastIndexOf(" 21");  // Negative
    search3 = Remark[i].indexOf(" ", search2 + 1);
    int search1 = Remark[i].indexOf("FT", search2 + 1);                   // Check for NOT 200FT Density Alt
    if (search1 == -1)  search1 = Remark[i].indexOf("V", search2 + 1);    // Check for NOT Visability
    if (search1 > 0 && search1 < search3)  search2 = -1;                  // If 200FT or VIS
    if (search3 - search2 != 6)  search3 = -1;                   // Check for length

    if (search2 >= 0 && search3 >= 0)    {
      text = Remark[i].substring(search2, search3);
      if (text.charAt(2) == '0')  text.setCharAt(2, '+');  else  text.setCharAt(2, '-');
      text = text.substring(2, 5) + "." + text.substring(5, 6);
      Remark[i].replace(Remark[i].substring(search2, search3), "");
    }


    //  ****  REMOVE   3PPPP  Air Pressure at station level in hPa
    search2 = Remark[i].lastIndexOf(" 3");
    search3 = Remark[i].indexOf(" ", search2 + 1);
    search1 = Remark[i].indexOf("FT", search2 + 1);                       // NOT 300FT Density Alt
    if (search1 == -1)  search1 = Remark[i].indexOf("V", search2 + 1);    // NOT Visability
    if (search1 > 0 && search1 < search3)  search2 = -1;                  // If 300FT or VIS
    if (search3 - search2 != 6)  search3 = -1;                            // Check for length

    if (search2 >= 0 && search3 >= 0)  {
      text = Remark[i].substring(search2 + 2, search3);
      Remark[i].replace(Remark[i].substring(search2, search3), "");
    }


    //  ****  REMOVE    4sTTTsTTT  24 Hr MAX/MIN Temperature
    // 24-hour report at 0000 UTC
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
      Remark[i].replace(Remark[i].substring(search2, search3), "");
    }


    //  ****  REPLACE   6RRRR 3Hr   Precipitation Amount in Inches
    // R=precip amount in Hundredths of Inch ??
    search2 = Remark[i].lastIndexOf(" 60");
    search3 = Remark[i].indexOf(" ", search2 + 1);
    search1 = Remark[i].indexOf("FT", search2 + 1);           // NOT 600FT Density Alt
    if (search1 > 0 && search1 < search3)  search2 = -1;      // NOT 600FT Density Alt
    if (search3 - search2 != 6)  search3 = -1;                // Check for length

    if (search2 >= 0 && search3 >= 0)  {
      text = Remark[i].substring(search2 + 1, search3);
      if (search2 > 4)  mesg = " <br>";  else mesg = "";
      mesg = mesg + "3 Hour Precip=" + text.substring(2, 3) + "." + text.substring(3, 6) + '"';
      mesg.replace("0.00", "Trace");
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
      if (search2 > 4)  mesg = " <br>";  else mesg = "";
      mesg = mesg + "24 Hour Precip=" + text.substring(2, 3) + "." + text.substring(3, 5) + '"';
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


    //  ****  REPLACE   4/SSS  Snow Depth   0000, 0600, 1200, and 1800 UTC
    // SSS Inches Snow on the Ground
    search2 = Remark[i].lastIndexOf(" 4/");
    search3 = Remark[i].indexOf(" ", search2 + 1);
    if (search3 - search2 != 6)  search3 = -1;                  // Check for length

    if (search2 >= 0 && search3 >= 0)   {
      text = Remark[i].substring(search2 + 4, search3);
      if (search2 > 4)   mesg = " <br>";  else    mesg = "";
      mesg = mesg + "Snow on the Ground=" + text + '"';
      Remark[i].replace(Remark[i].substring(search2, search3), mesg);
    }


    //  ****  REPLACE   5appp   Pressure Tendency   3- and 6-hourly report
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
      if (search2 > 4)   mesg = "<br>";  else    mesg = "";
      if (press_change > 4)  mesg = mesg + "3 Hour Pressure DEC " + Remark[i].substring(search2 + 4, search2 + 5) + "." + Remark[i].substring(search2 + 5, search2 + 6) + " mb";
      if (press_change < 4)  mesg = mesg + "3 Hour Pressure INC " + Remark[i].substring(search2 + 4, search2 + 5) + "." + Remark[i].substring(search2 + 5, search2 + 6) + " mb";
      if (press_change == 4) mesg = mesg + "3 Hour Pressure Steady";
      mesg.replace("0.0 mb", "Negligable");
      Remark[i].replace(Remark[i].substring(search2, search3), mesg);
    }


    //  ****  REPLACE  98MMM   Duration of Sunshine in Minutes
    search2 = Remark[i].lastIndexOf(" 98");
    search3 = Remark[i].indexOf(" ", search2 + 1);
    if (search3 - search2 != 6)  search3 = -1;              // Check for length

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


    // ****  DECODE METAR / WEATHER and Print Errors
    // Searching flight_category
    int search0 = Parsed_metar.indexOf("<flight_category") + 17;    // Search Between search0
    search1 = Parsed_metar.indexOf("</flight_category");            // Search Between search1
    if (search0 < 17) Category[i] = "NA";  else  Category[i] = Parsed_metar.substring(search0, search1);
    if (Category[i] == "NA")   Serial.printf("%s\tNo:%d\t%s\tFlight Category Not Found, in Decodedata\n", Clock, i, station.c_str());

    // Significant Weather
    Sig_Weather[i] = "";
    // Wind Gusts
    search0 = StationMetar[i].indexOf("G");
    search1 = StationMetar[i].indexOf("KT");
    if (search0 > 0 && search1 - search0 > 2)
      Sig_Weather[i] =  Sig_Weather[i] + "Gusts to " + StationMetar[i].substring(search1 - 2, search1) + " KTS&nbsp&nbsp&nbsp";

    // Variable Wind Dir
    search0 = StationMetar[i].indexOf("0V");
    search1 = StationMetar[i].indexOf("FT");
    if (search0 > 0  && search1 < 0)
      Sig_Weather[i] = Sig_Weather[i] + "Wind Direction Variable: " + StationMetar[i].substring(search0 - 2, search0 + 5) + "&nbsp&nbsp&nbsp";

    // Significant Weather
    search0 = StationMetar[i].indexOf("SM R");
    search1 = StationMetar[i].indexOf("FT");
    if (search0 > 0 && search1 > 0)
      Sig_Weather[i] = Sig_Weather[i] + "Runway Vis: " + StationMetar[i].substring(search0 + 3, search1 + 2) + "&nbsp&nbsp&nbsp";

    // Significant Weather in metar and making readable Weather
    if (StationMetar[i].indexOf("BL") > 0)    Sig_Weather[i] = Sig_Weather[i] + " Blowing";
    if (StationMetar[i].indexOf("DR") > 0)    Sig_Weather[i] = Sig_Weather[i] + " Drifting";
    if (StationMetar[i].indexOf("FZ") > 0)    Sig_Weather[i] = Sig_Weather[i] + " Freezing";
    if (StationMetar[i].indexOf("MI") > 0)    Sig_Weather[i] = Sig_Weather[i] + " Shallow";
    if (StationMetar[i].indexOf("PR") > 0)    Sig_Weather[i] = Sig_Weather[i] + " Partial";
    if (StationMetar[i].indexOf("BC") > 0)    Sig_Weather[i] = Sig_Weather[i] + " Patches of";
    if (StationMetar[i].indexOf("FG") > 0)    Sig_Weather[i] = Sig_Weather[i] + " Fog; ";
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
    if (StationMetar[i].indexOf("SN") > 0)    Sig_Weather[i] = Sig_Weather[i] + " Snow ";
    if (StationMetar[i].indexOf("SH") > 0)    Sig_Weather[i] = Sig_Weather[i] + " Showers ";
    if (StationMetar[i].indexOf("SG") > 0)    Sig_Weather[i] = Sig_Weather[i] + " Snow Grains; ";
    if (StationMetar[i].indexOf("BR") > 0)    Sig_Weather[i] = Sig_Weather[i] + " Mist; ";
    if (StationMetar[i].indexOf("DZ") > 0)    Sig_Weather[i] = Sig_Weather[i] + " Drizzle; ";
    if (StationMetar[i].indexOf("GS") > 0)    Sig_Weather[i] = Sig_Weather[i] + " Small Hail/Snow Pellets; ";
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

    // Searching temp_c        TempC[i] = deg C
    search0 = Parsed_metar.indexOf("<temp_c") + 8;
    search1 = Parsed_metar.indexOf("</temp");
    if (search0 < 8)   Serial.printf("%s\tNo:%d\t%s\tTemperature Not Found, in Decodedata\n", Clock, i, station.c_str());
    if (search0 < 8) TempC[i] = 0;  else  TempC[i] = Parsed_metar.substring(search0, search1).toFloat() + 0.001;

    // Searching dewpoint_c
    search0 = Parsed_metar.indexOf("<dewpoint_c") + 12;
    search1 = Parsed_metar.indexOf("</dewpoint");
    if (search0 < 12) DewptC[i] = 0;  else  DewptC[i] = Parsed_metar.substring(search0, search1).toFloat() + 0.001;

    // Searching Wind_dir_degrees
    old_wDir[i] = wDir[i].toInt();
    search0 = Parsed_metar.indexOf("wind_dir_degrees") + 17;
    search1 = Parsed_metar.indexOf("</wind_dir_degree");
    if (search0 < 17) wDir[i] = "NA";  else  wDir[i] = Parsed_metar.substring(search0, search1);
    if (StationMetar[i].indexOf("VRB") > 0) wDir[i] = "VRB";
    if (wDir[i] == "NA")   Serial.printf("%s\tNo:%d\t%s\tWind Dir Not Found, in Decodedata\n", Clock, i, station.c_str());

    // Searching Wind_speed_kt
    search0 = Parsed_metar.indexOf("wind_speed_kt") + 14;
    search1 = Parsed_metar.indexOf("</wind_speed");
    if (search0 < 14) Wind[i] = "NA";  else  Wind[i] = Parsed_metar.substring(search0, search1) + " KT";
    if (Wind[i] == "0 KT")  Wind[i] = "CALM";

    // Searching  visibility_statute_mi
    search0 = Parsed_metar.indexOf("visibility_statute_mi") + 22;
    search1 = Parsed_metar.indexOf("</visibility");
    if (search0 < 22)  Visab[i] = 0;  else  Visab[i] = Parsed_metar.substring(search0, search1).toFloat();
    if (search0 < 22)  Serial.printf("%s\tNo:%d\t%s\tVisibility Not Found, in Decodedata\n", Clock, i, station.c_str());

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
    if (Sky[i] == "NA")   old_cloud_base[i] = 0;
    if (Sky[i] == "NA")  Serial.printf("%s\tNo:%d\t%s\tSky Cover Not Found, in Decodedata\n", Clock, i, station.c_str());

    // Vertical Visibility Searching <vert_vis_ft>
    search0 = Parsed_metar.indexOf("<vert_vis_ft>") + 13;
    if (search0 > 13)  {
      Sig_Weather[i] = Sig_Weather[i] + "<br>Vertical Visibility = " + Parsed_metar.substring(search0, search0 + 3) + " Ft";
      new_cloud_base[i] = Parsed_metar.substring(search0, search0 + 3).toInt();
    }

    // Searching <altim_in_hg
    old_Altim[i] = Altim[i];
    search0 = Parsed_metar.indexOf("<altim_in_hg>") + 13;
    search1 = Parsed_metar.indexOf("</altim_in_hg>");
    if (search0 < 13)  Serial.printf("%s\tNo:%d\t%s\tAltimeter Not Found, in Decodedata\n", Clock, i, station.c_str());
    if (search0 < 13) Altim[i] = 0;  else  Altim[i] = Parsed_metar.substring(search0, search1).toFloat();

    // Searching <sea_level_pressure_mb
    search0 = Parsed_metar.indexOf("<sea_level_pressure_mb>") + 23;
    if (search0 < 23) SeaLpres[i] = 0;  else  SeaLpres[i] = Parsed_metar.substring(search0, search0 + 6).toFloat();

    // Searching elevation_m
    search0 = Parsed_metar.indexOf("<elevation") + 13;
    search1 = Parsed_metar.indexOf("</elevation");
    if (search0 < 13) Elevation[i] = 0; else Elevation[i] = Parsed_metar.substring(search0, search1).toFloat();

  }                               // UPDATE Station
  Display_LED(i, 20);            // Display This Station LED
}


//  *********** Decode Weather in REMARKS and make readable in Dictionary Function
String Decode_Weather(String weather) {
  // Glossary of CODES
  weather.replace("BECMG", "CMG");                       // Rename Later
  weather.replace("CAVOK", "cavok");                     // Rename Later
  weather.replace("RERA", "Recent: Rain");
  weather.replace("REDZ", "Recent: Drizzle");
  weather.replace("RTS", "Routes");
  weather.replace("TEMPO", " Temporary:");
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
  weather.replace("DA", " Density Alt");
  weather.replace("DENSITY ALT", " Density Alt");
  weather.replace("DEWPT", " Dew Point");
  weather.replace("CONS ", " Continuous");
  weather.replace("PDMTLY", " Predominantly");
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
  weather.replace("FROPA", " Frontal Passage ");
  weather.replace("PRESFR", " Pressure Falling Rapidly ");
  weather.replace("PRESRR", " Pressure Rising Rapidly ");
  weather.replace("FROIN", " Frost On The Indicator ");
  weather.replace("ALSTG", " Alitmeter Setting");
  weather.replace("ALTSTG", " Alitmeter Setting");
  weather.replace("AT AP", "At Airport");

  weather.replace("SLPNO", " Sea Level Pressure NA ");
  weather.replace("PNO", " Rain Gauge NA ");
  weather.replace("FZRANO", " Freezing Rain Sensor NA ");
  weather.replace("TSNO", " Thunderstorm Sensor NA ");
  weather.replace("RVRNO", " Runway visibility Sensor NA ");     // Rename Later
  weather.replace("CHINO", " Secondary Ceiling Height Sensor NA");
  weather.replace("VISNO", " visibility Sensor NA");    // Rename Later
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
  weather.replace("SNINCR", " Snow Increasing Rapidily");
  weather.replace("ACFT MSHP", " Aircraft Mishap");
  weather.replace("STFD", "Staffed");
  weather.replace("FM", " From");
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

  weather.replace("BKN", "KN CLD");                  // Rename Later
  weather.replace("SCT", "Scattered CLD");
  weather.replace("FEW", "Few CLD");
  weather.replace("MDT CU", " ModerateCU");
  weather.replace("MDT", "Moderate");
  weather.replace("CLD", "Clouds at ");
  weather.replace("EMBD", "mbedded");                 // Rename Later
  weather.replace("EMBED", "mbedded");                // Rename Later
  weather.replace("BINOVC", " KN in OC");             // Rename Later

  weather.replace("OCNL LTG", "OCNLLTG");
  weather.replace("OCNL LT", "OCNLLTG");
  weather.replace("OCNL", " Occasional");
  weather.replace("FREQ", " Frequent");
  weather.replace("FRQ LTG", "FRQLTG");
  weather.replace("FRQ", " Frequent");
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
  weather.replace("LTNG", " Lightning");
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

  weather.replace("SFC VIS", " SurfaceVIS");
  weather.replace("SFC", "Surface");
  weather.replace("TWR VIS", " TowerVIS");
  weather.replace("TWR", " TowerVIS");
  weather.replace("VV", "visability variable");         // Rename Later
  weather.replace("VISBL", "visabile");                 // Rename Later
  weather.replace("VIS M", "visability Measured");      // Rename Later
  weather.replace("VIS", " visability");                // Rename Later
  weather.replace("INC", "Increasing");
  weather.replace("DEC", "Decreasing");

  weather.replace(" +", " Heavy ");
  weather.replace(" -", " Light ");
  weather.replace(" - ", " to ");

  weather.replace("SHRASN", "Rain and Snow Showers");
  weather.replace("SHRA", "Rain Showers");
  weather.replace("SHSN", "Snow Showers");
  weather.replace("RASN", "Rain and Snow");
  weather.replace("TSRA", "Rain and Thunderstorm");
  weather.replace("VCSH", " Showers VC");
  weather.replace("VCSH", "Rain Shower VC");
  weather.replace("VCTS", "TS VC");
  weather.replace("OVC", "OC");                        // Rename Later
  weather.replace("VC", "In the vicinity");            // Rename Later

  weather.replace("HIER", "Higher");
  weather.replace("LWR", "Lower");

  weather.replace("DISSIPATED", "Dissipated");
  weather.replace("DSIPTD", "Dissipated");
  weather.replace("DSIPT", "Dissipated");
  weather.replace("DSPTD", "Dissipated");
  weather.replace("DSPT", "Dissipated");
  weather.replace("HVY", "Heavy");
  weather.replace("LGT", "Light");

  weather.replace("OHD", "oerhead");                 // Rename Later
  weather.replace("OVD", "oerhead");                 // Rename Later
  weather.replace("VRB", "variable");                // Rename Later
  weather.replace("VRY", "very");                    // Rename Later
  weather.replace("VERY", "very");                   // Rename Later
  weather.replace("VSBY", "visibility");             // Rename Later
  weather.replace("VSB", "visibile");                // Rename Later
  weather.replace("VIRGA", " virga");                // Rename Later
  weather.replace("V", "variable");                  // Rename Later

  weather.replace("CIG", " Ceiling");
  weather.replace("ACSL", " Standing Lenticular Altocumulus Clouds");
  weather.replace("SC", " Stratocumulus Clouds");
  weather.replace("CBMAM", " mammatusCB");

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

  weather.replace(" DZ", " Drizzle");
  weather.replace(" RA", " Rain");

  weather.replace("DZ", " Drizzle");
  weather.replace("RA", "Rain ");
  weather.replace("SN", "Snow ");
  weather.replace("SG", " Snow Grains");

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
  weather.replace("TCU", "TC");                     // Rename Later
  weather.replace("TS", " Thunderstorm");

  weather.replace("RainG", "Ragged ");
  weather.replace("TR", "Trace ");
  weather.replace("RE", "<br>Recent:");

  weather.replace("RF", "Rainfall ");
  weather.replace("RMK ", " Remark:");

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
  weather.replace("TC", " Towering Cumulus Clouds");   // Rename
  weather.replace("CMG", " Becoming:");                // Rename
  weather.replace("oerhead", "Overhead");              // Rename

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

  weather.replace("SOG", "<br>Snow on the Ground");

  // Wind Gusts / Wind
  int search0 = weather.lastIndexOf("KT");
  if (search0 >= 0) {
    int search1 = weather.lastIndexOf(" ", search0);
    int search2 = weather.lastIndexOf("G", search0);
    if (search2 == search1 + 6) {
      String Wind = " Wind ~" + weather.substring(search1, search1 + 4) + "Deg " + weather.substring(search1 + 4, search2) + "KT Gusting to " + weather.substring(search2 + 1, search0) + "KT ";
      weather.replace(weather.substring(search1, search0 + 2), Wind);
    }
    if (search2 <= 0) {
      String Wind = " Wind ~" + weather.substring(search1, search1 + 4) + "Deg " + weather.substring(search1 + 4, search0) + "KT ";
      weather.replace(weather.substring(search1, search0 + 2), Wind);
    }
  }

  // Peak Wind
  search0 = weather.indexOf("Peak wind");
  if (search0 >= 0) {
    int search1 = weather.indexOf("/", search0 + 9);
    String Dir = "Peak Wind " + weather.substring(search1 - 5, search1 - 2) + "Deg ";
    String Wind = weather.substring(search1 - 2, search1) + "KT at ";
    String At = weather.substring(search1 + 1, search1 + 5) + " ";
    weather.replace(weather.substring(search0, search1 + 5), Dir + Wind + At);
  }

  // Ceiling / Variable
  search0 = weather.indexOf("Ceiling");
  if (search0 >= 0) {
    char Ceiling[6];
    strcpy(Ceiling, weather.substring(search0 + 8, search0 + 11).c_str());
    int ceiling = 0;
    for (int i = 0; i < 3; i++) {
      ceiling = ceiling * 10 + (Ceiling[i] - '0');
    }
    weather.replace(weather.substring(search0 + 8, search0 + 11), String(ceiling * 100) + " Ft");

    search0 = weather.indexOf("Variable", search0);
    if (search0 >= 0) {
      char Variable[6];
      strcpy(Variable, weather.substring(search0 + 8, search0 + 11).c_str());
      int variable = 0;
      for (int i = 0; i < 3; i++) {
        variable = variable * 10 + (Variable[i] - '0');
      }
      weather.replace(weather.substring(search0, search0 + 11), + " Variable " + String(variable * 100) + " Ft ");
    }
  }


  // Temporary: [ Ceiling ]
  search0 = weather.indexOf("Temporary:");
  if (search0 >= 0) {
    char Ceiling[5];
    int search1 = weather.indexOf("9999", search0);
    if (search1 >= 0)  {
      strcpy(Ceiling, weather.substring(search1, search1 + 4).c_str());
      weather.replace(weather.substring(search1, search1 + 4), " Ceiling ~ Unlimited ");   // Replace code
    } else {
      int search2 = weather.indexOf("000", search0);
      if (search2 >= 0)  {
        strcpy(Ceiling, weather.substring(search2 - 1, search2 + 3).c_str());
        String text = " Ceiling ~ " + String(Ceiling) + " m ";
        weather.replace(weather.substring(search2 - 2, search2 + 3), String(text));    // Replace code
      }
    }
  }


  // For Troubleshooting:
  //Serial.printf("\tRemarks\t\t.%s.\n", weather.c_str());

  weather.replace(" $", "");                             // Clean Up ~ Station needs Maintenance
  weather.replace("[ <br>", "");                         // Clean Up
  weather.replace("[ ", "");                             // Clean Up
  weather.replace("[", "");                              // Clean Up
  weather.replace(" ]", "");                             // Clean Up
  weather.replace("]", "");                              // Clean Up
  weather.replace("  ", "<br>");                         // Clean Up
  weather.replace("<br><br>", "<br>");                   // Clean Up
  weather.replace("<br> ", "");                          // Clean Up
  weather.replace("Temporary", "TEMPORARY");             // Clean Up
  weather.replace("Becoming", "BECOMING");               // Clean Up
  weather.replace("Recent", "RECENT");                   // Clean Up

  // For Troubleshooting:
  //Serial.printf("\tFinal Remark\t.%s.\n\n", weather.c_str());

  return weather;                  // Return with Readable Weather
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
      if (StationMetar[index].indexOf("FZ") > 0)    Twinkle(index, 0x00, 0x00, 0x70, 3, 300, 400); //Freezing Blue
      if (StationMetar[index].indexOf("FG") > 0)    Twinkle(index, 0x30, 0x40, 0x26, 3, 400, 500); //Fog   L Yellow
      if (StationMetar[index].indexOf("BR") > 0)    Twinkle(index, 0x20, 0x50, 0x00, 3, 500, 500); //Mist  L Green
      if (StationMetar[index].indexOf("DZ") > 0)    Twinkle(index, 0x22, 0x50, 0x22, 3, 300, 300); //Drizzle L Green
      if (StationMetar[index].indexOf("RA") > 0)    Twinkle(index, 0x22, 0xFC, 0x00, 5, 300, 300); //Rain    Green
      if (StationMetar[index].indexOf("HZ") > 0)    Twinkle(index, 0x20, 0x20, 0x20, 3, 400, 400); //Haze    White/purple
      if (StationMetar[index].indexOf("SN") > 0)    Twinkle(index, 0xaf, 0xaf, 0xaf, 4, 300, 600); //Snow    White
      if (StationMetar[index].indexOf("SG") > 0)    Twinkle(index, 0xaf, 0xaf, 0xaf, 4, 300, 600); //Snow    White
      if (StationMetar[index].indexOf("TS") > 0)    Twinkle(index, 0xff, 0xff, 0xff, 4,   5, 900); //Thunder White
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
  for (int i = 1; i < (No_Stations + 1); i++) {
    Set_Cat_LED (i);  // Set Category for This Station LED
  }
  FastLED.show();
  delay(200);        // Wait a smidgen
}


// *********** Set Category for One Station LED
void Set_Cat_LED (int i)  {
  if (Category[i] == "VFR")  leds[i - 1] = CRGB::DarkGreen;
  if (Category[i] == "MVFR") leds[i - 1] = CRGB::DarkBlue;
  if (Category[i] == "IFR")  leds[i - 1] = CRGB::DarkRed;
  if (Category[i] == "LIFR") leds[i - 1] = CRGB::DarkMagenta;
  //if (Category[i] == "NF")   leds[i - 1] = CRGB::Black;
  if (Category[i] == "NF")   leds[i - 1] = CRGB(30, 30, 0); // Very Dim Yellow
  if (Category[i] == "NA")   leds[i - 1] = CRGB(30, 30, 0); // Very Dim Yellow
}



// *********** Display Visibility [Red White] on LEDS
void Display_Vis_LEDS (int wait) {
  for (byte i = 1; i < (No_Stations + 1); i++) {
    byte hue     = 14  + Visab[i] * 4;       // (red white) [ 14 ~ 54 ]
    byte sat     = 160 - Visab[i] * 16;      // (red white) [ 0 ~ 160 ]
    byte bright  = 120;                      // [ <100=Dim  120=ok  >160=Too Bright/Much Power ]
    leds[i - 1] = CHSV(hue, sat, bright);    // ( hue, sat, bright )
    if (Visab[i] == 0 || Category[i] == "NF")  leds[i - 1] = CHSV( 0, 0, 0);
  }
  FastLED.show();
  delay(wait);
}



// *********** Display Winds [Aqua] on LEDS
void Display_Wind_LEDS (int wait) {
  for (byte i = 1; i < (No_Stations + 1); i++) {
    byte wind = Wind[i].toInt();
    leds[i - 1] = CRGB(0, wind * 7, wind * 7);    // ( red, green, blue )
    if (wind == 0 || Category[i] == "NF")  leds[i - 1] = CHSV( 0, 0, 0);
  }
  FastLED.show();
  delay(wait);
}


// *********** Display Temperatures [Blue Green Yellow Orange Red] on LEDS
void Display_Temp_LEDS (int wait) {
  for (byte i = 1; i < (No_Stations + 1); i++) {
    byte hue = 160 - TempC[i] * 4;         //  purple blue green yellow orange red [160 ~ 0 ]
    leds[i - 1] = CHSV( hue, 180, 150);   // ( hue, sat, bright )
    if (TempC[i] == 0 || Category[i] == "NF")  leds[i - 1] = CHSV( 0, 0, 0);
  }
  FastLED.show();
  delay(wait);
}


// *********** Display Altimeter Pressure [Blue Purple] on LEDS
void Display_Alt_LEDS (int wait) {
  for (byte i = 1; i < (No_Stations + 1); i++) {
    byte hue = (Altim[i] - 29.92) * 100;        //  (normally) blue purple [ 70 ~ 270 ]
    leds[i - 1] = CHSV( hue + 170, 180, 150);   // ( hue, sat, bright )
    if (Altim[i] == 0 || Category[i] == "NF")   leds[i - 1] = CHSV( 0, 0, 0);
  }
  FastLED.show();
  delay(wait);
}


// *********** DECODE the Station NAME
void Decode_Name(int i) {
  String Station_name = Stations[i].substring(0, 4);
  int search0 = MetarData.indexOf(Station_name) + 1;      // Start Search from Here
  int search1 = MetarData.indexOf("<site", search0);
  int search2 = MetarData.indexOf("</site", search0);
  if (search1 > 0 && search2 > 0)  Station_name = Station_name + ", " + MetarData.substring(search1 + 6, search2) + ", ";
  search1 = MetarData.indexOf("<country", search0);
  search2 = MetarData.indexOf("</country", search0);
  if (MetarData.substring(search1 + 9, search2) == "US") {
    search1 = MetarData.indexOf("<state", search0);
    search2 = MetarData.indexOf("</state", search0);
    if (search1 > 0)  Station_name = Station_name + MetarData.substring(search1 + 7, search2);
  }
  if (search1 > 0 && search2 > 0)  Station_name = Station_name + MetarData.substring(search1 + 9, search2);
  Stations[i] = Station_name;
  if (Stations[i].length() < 6 )  {    // ERROR in STATION NAME
    Stations[i] = "NULL,";             // Reset Station Name
    StationMetar[i] = Station_name + " : ERROR in STATION NAME or Not Reporting";
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

    ArduinoOTA.handle();   // ESP32_OTA.h

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

                if (sta_n == -1)  {           // Airport_Code NOT in Data Base
                  sta_n = 0;                  // Normally Set to sta_n = 0; : ADD as Stations[0]
                  //sta_n = No_Stations;      // Can be set to Any Station For Troubleshooting (Set to overwrite last station)

                  Reset_All_Parameters(sta_n);             // Reset All Parameters
                  Stations[sta_n] = Airport_Code + ",";    // Add a Comma
                  if (Airport_Code.length() < 4) {         // Checking for Length Error in Station Name
                    Stations[sta_n] = "NULL,";             // Reset Station Name
                    StationMetar[sta_n] = Airport_Code + " : ERROR in STATION NAME";
                  }  else {

                    if (Comms_Flag > 0)  {        // Get Station Metar  ~ Station NAME and DATA
                      Serial.printf("%s   WAITING to Get Station Metar for %s -  Comms In Use\n", Clock, Airport_Code.c_str());
                      int count = 0;
                      while (Comms_Flag > 0) {    // Checking Communication Flag 1=Active
                        delay(1000);
                        count++;
                        if (count > 100)  break;  // loop for 100 seconds
                      }
                    }
                    // Communication Flag - GetData is NOT running Communication Flag = 0
                    delay(200);                 // Wait a smidgen
                    Comms_Flag = 1;             // Set Communication Flag 1=Active
                    GetData("NAME", sta_n);     // GET Some Metar ~ Station NAME
                    Decode_Name(sta_n);         // Decode Station NAME
                    GetData(Stations[sta_n].substring(0, 5), sta_n); // GET Some Metar ~ Station DATA
                    ParseMetar(sta_n);          // Parse Metar DATA
                    Comms_Flag = 0;             // Set Communication Flag 0=Reset
                  }
                }
                Summary_Flag = 0;
                Station_Flag = 1;
              }

              // Checking which BUTTON was Pressed
              search = header.indexOf("GET /back HTTP/1.1");
              if (search >= 0) {             // From Display Station BACK Button
                sta_n = sta_n - 1;
                if (sta_n < 0)  sta_n = No_Stations;
                if (Stations[sta_n].substring(0, 4) == "NULL")   sta_n = sta_n - 1;
                if (sta_n < 0)  sta_n = No_Stations;
                Summary_Flag = 0;
                Station_Flag = 1;
              }
              search = header.indexOf("GET /next HTTP/1.1");
              if (search >= 0) {             // From Display Station NEXT Button
                sta_n = sta_n + 1;
                if (sta_n > No_Stations) sta_n = 0;
                if (Stations[sta_n].substring(0, 4) == "NULL")   sta_n = sta_n + 1;
                if (sta_n > No_Stations) sta_n = 0;
                Summary_Flag = 0;
                Station_Flag = 1;
              }
              search = header.indexOf("GET /flash HTTP/1.1");
              if (search >= 0) {
                sta_n = Station_Num;             // From Display Station FLASH Button
                if (sta_n < 1 || sta_n > No_Stations)  sta_n = 1;
                Summary_Flag = 0;
                Station_Flag = 1;
              }
              search = header.indexOf("GET /summary HTTP/1.1");
              if (search >= 0) {             // From Display Station SUMMARY Button
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
                String Deg = "TEMP<BR>Deg F";      // Set Temperature Units   ******  For Celsius : Change this to "TEMP<BR>Deg C"  ******
                String Alt = "ALT<BR>in Hg";       // Set Altimeter Units     ******  For mBar : Change this to "ALT<BR>mBar"  ******

                html_code = "<TABLE BORDER='2' CELLPADDING='5'>";
                html_code += "<TR BGCOLOR='Cyan'><TD>No:</TD><TD>Station Name</TD><TD>CAT</TD><TD>SKY<BR>COVER</TD><TD>VIS<BR>Miles</TD><TD>WIND<BR>From</TD><TD>WIND<BR>Speed</TD><TD>";
                html_code += Deg + "</TD><TD>" + Alt + "</TD><TD>METAR Codes & Remarks</TD></TR></FONT>";
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
                      html_code = "<TR><TD BGCOLOR='Yellow'>" + String(i) + "</TD><TD BGCOLOR='Yellow'>" + color.substring(4, color.length()) + Stations[i] + "</FONT></TD>";
                    } else {
                      // Display LINK:  Makes a request in the header ("GET /get?Airport_Code=XXXX")
                      html_code = "<TR><TD>" + String(i) + "</TD><TD><a href='/get?Airport_Code=" + String(Stations[i].substring(0, 4)) + "'>" + color.substring(4, color.length()) + Stations[i] + "</a></FONT></TD>";
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
                    if (Visab[i] > 0)  client.print(color + String(Visab[i]) + "</FONT></TD>");  else  client.print(color + "NA</FONT></TD>");

                    // Display Wind Direction in SUMMARY
                    int newDir = wDir[i].toInt();
                    int oldDir = old_wDir[i];
                    if (newDir != 0 && oldDir != 0) {
                      int diff = oldDir - newDir;
                      if (diff < 0)      diff = newDir - oldDir;
                      if (diff > 300)    diff = 360 - diff;
                      if (diff > 30)     client.print("<TD BGCOLOR='MistyRose'><FONT COLOR='Purple'>" + wDir[i] + "</FONT></TD>");
                      else   client.print(color + wDir[i] + "</FONT></TD>");
                    }  else   client.print(color + wDir[i] + "</FONT></TD>");

                    // Display Wind Speed in SUMMARY
                    client.print(color + Wind[i] + "</FONT></TD>");

                    // Display Temperature in SUMMARY  ***  Set Temperature Units See Above (Display SUMMARY Table)  ***
                    float TempF = TempC[i] * 1.8 + 32;  // deg F
                    if (TempC[i] == 0 && DewptC[i] == 0)  client.print(color + "NA </FONT></TD>");
                    else if (Deg == "Deg C")   client.print(color + String(TempC[i], 1) + "</FONT></TD>");
                    else client.print(color + String(TempF, 1) + "</FONT></TD>");

                    // Display Altimeter in SUMMARY  ***  Set Pressure Units See Above (Display SUMMARY Table)  ***
                    if (Altim[i] == 0)   client.print(color + "NA");   else  {
                      byte wx_flag = 0;
                      float Altm = Altim[i];
                      if (Alt == "ALT<BR>mBar")  Altm = Altim[i] * 1013.2 / 29.92;
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
                    if (StationMetar[i].substring(0, 3) == "new")  client.print(F("<TD><FONT COLOR='Purple'>"));  else  client.print(color);
                    client.print(StationMetar[i]);

                    // Display Remark in SUMMARY
                    if (StationRemark[i].length() > 3)   client.print("<BR><FONT COLOR='Navy'>Remarks:" + String(StationRemark[i]) + "</FONT></TD></TR>");  else  client.print("</FONT></TD></TR>");
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
                  client.print(html_code);

                  // Display TABLE/FORM: Makes a request in the header ("GET /get?Airport_Code=")
                  html_code = "<FORM METHOD='get' ACTION='get'>";
                  html_code += "<TABLE BORDER='0' CELLPADDING='1'>";
                  html_code += "<TR><TD>Enter<FONT COLOR='Navy'><B> Any Airport ID </B></FONT>Code:</TD><TD>";
                  html_code += "<INPUT TYPE='text' NAME='Airport_Code' SIZE='5'>";
                  html_code += "</TD></TR></TABLE></FORM>";
                  client.print(html_code);

                  // Update LED and Current Time in STATION
                  Display_LED(sta_n, 300);   //  Display One Station LED
                  Update_Time();             //  GET CurrentTime : Hour & Minute

                  if (Last_Up_Min + Update_Interval > 60)   Last_Up_Min = 60 - Update_Interval;
                  Count_Down = Last_Up_Min + Update_Interval - Minute;
                  if (Count_Down > Update_Interval)   Count_Down = 0;

                  String CountDown_Time = "<FONT COLOR='Purple'>Current Time : " + String(Clock) + " UTC &nbsp&nbsp&nbsp&nbsp&nbsp&nbsp Next Update in less than 0 Minutes</FONT><BR>";
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
                  Update_Ago(sta_n);             //   Update Minutes Ago
                  html_code += "<TR><TD>Station Metar</TD>";
                  if (StationMetar[sta_n].substring(0, 3) == "new" )  html_code += "<TD><FONT COLOR='Purple'>" + String(StationMetar[sta_n]) + "</FONT>"; else  html_code += "<TD>" + String(StationMetar[sta_n]);
                  html_code += "</TD></TR>";

                  // Display Significant Weather in STATION
                  html_code += "<TR>" + color + "Significant Weather<BR><FONT COLOR='Purple'>Decoded Remarks</FONT></TD>" + color + String(Sig_Weather[sta_n]) + "</FONT>";

                  //  Comments for Weather and Cloud Base in STATION
                  if (Sky[sta_n].substring(0, 3) == "OVC")
                    html_code += "<BR><FONT " + Bcol.substring(6, Bcol.length()) + ">Overcast Cloud Layer</FONT>";
                  if (new_cloud_base[sta_n] > 1 && new_cloud_base[sta_n] <= 1200)
                    html_code += "<BR><FONT " + Bcol.substring(6, Bcol.length()) + ">Low Cloud Base</FONT>";
                  if (old_cloud_base[sta_n] > 0 && old_Altim[sta_n] > 0)    {
                    if (new_cloud_base[sta_n] > old_cloud_base[sta_n] && Altim[sta_n] > old_Altim[sta_n])
                      html_code += "<BR><FONT COLOR='Navy'>Weather is Getting Better</FONT>";
                    if (new_cloud_base[sta_n] < old_cloud_base[sta_n] && Altim[sta_n] < old_Altim[sta_n])
                      html_code += "<BR><FONT COLOR='Navy'>Weather is Getting Worse</FONT>";
                  }

                  //  Remarks in STATION
                  if (Remark[sta_n].length() > 1)
                    html_code += "<BR><FONT COLOR='Purple'>" + String(Remark[sta_n]) + "</FONT>";
                  else html_code += "<BR><FONT COLOR='Purple'>None</FONT>";

                  //  Sky Cover in STATION
                  html_code += "</TD></TR><TR><TD>Sky Cover</TD>" + color;
                  html_code += String(Sky[sta_n]) + "&nbsp&nbsp&nbsp";
                  if (old_cloud_base[sta_n] > 0 && Sky[sta_n] != "NA")  {
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
                  if (old_cloud_base[sta_n] > 0 && old_Altim[sta_n] > 0)    {
                    if (new_cloud_base[sta_n] >= old_cloud_base[sta_n] + diff_in_clouds) // INCREASE
                      html_code += "<BR><FONT COLOR='Navy'>Significant Increase in Cloud Base";
                    if (new_cloud_base[sta_n] <= old_cloud_base[sta_n] - diff_in_clouds) // DECREASE
                      html_code += "<BR><FONT COLOR='Orange'>Significant Decrease in Cloud Base";
                  }
                  html_code += "</FONT></TD></TR>";

                  //  Visibility in STATION
                  html_code += "<TR><TD>Visibility</TD>" + color;
                  if (Visab[sta_n] > 0)  html_code += String(Visab[sta_n]) + " Statute miles</FONT></TD></TR>";  else  html_code += "NA</FONT></TD></TR>";

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

                  //  Temperature and Heat Index or Wind Chill in STATION
                  html_code += "<TR><TD>Temperature</TD><TD>";
                  if (TempC[sta_n] <= 0)  html_code += "<FONT COLOR='Blue'>";  else   html_code += "<FONT COLOR='Black'>";
                  if (TempC[sta_n] == 0 && DewptC[sta_n] == 0)   html_code += "NA</FONT>";
                  else  {
                    // Display_Text (Temperature) in STATION
                    String Display_Text = Format_Temp_Text(TempC[sta_n], TempC[sta_n] * 1.8 + 32);
                    html_code += Display_Text + "</FONT>";
                    if (TempC[sta_n] >= 35.0)   html_code += "<FONT SIZE='-1' FONT COLOR='Red'><I>&nbsp&nbsp&nbspAnd HOT</I></FONT>";

                    // Heat Index in STATION
                    // Don't display Heat Index unless Temperature > 18 Deg C  and  Heat Index > Temperature
                    float Heat_Index = Calc_Heat_Index(sta_n);             // *** Calculate Heat_Index
                    if (TempC[sta_n] >= 18  &&  Heat_Index >= TempC[sta_n]) {

                      // Display_Text (Heat Index) in STATION
                      String Display_Text = Format_Temp_Text(Heat_Index, Heat_Index * 1.8 + 32);
                      html_code += "<BR><FONT COLOR='Purple'>" + Display_Text + "&nbsp&nbsp&nbspHeat Index</FONT>";
                    }

                    // Wind Chill in STATION
                    // Don't display Wind Chill unless Wind Speed > 3 KTS and Temperature < 14 Deg C
                    if (Wind[sta_n].toInt() > 3 && TempC[sta_n] < 10) {      // Note: Nugged this down
                      float Wind_Chill = Calc_Wind_Chill(sta_n);             // *** Calculate Wind Chill
                      if (Wind_Chill <= TempC[sta_n]) {
                        if (Wind_Chill <= 0)  html_code += "<FONT COLOR='Blue'>";  else   html_code += "<FONT COLOR='Purple'>";

                        // Display_Text (Wind Chill)  in STATION
                        String Display_Text = Format_Temp_Text(Wind_Chill, Wind_Chill * 1.8 + 32);
                        html_code += "<BR>" + Display_Text + "&nbsp&nbsp&nbspWind Chill</FONT>";
                      }
                    }
                  }

                  //  Dew Point & Relative Humidity in STATION
                  html_code += "</TD></TR><TR><TD>Dew Point<BR><FONT COLOR='Purple'>Relative Humidity</FONT></TD><TD>";
                  if (DewptC[sta_n] <= 0)  html_code += "<FONT COLOR='Blue'>";  else   html_code += "<FONT COLOR='Black'>";
                  if (DewptC[sta_n] == 0)   html_code += "NA";
                  else  {
                    // Display_Text (Dew Point) in STATION
                    String Display_Text = Format_Temp_Text(DewptC[sta_n], DewptC[sta_n] * 1.8 + 32);
                    html_code += Display_Text + "</FONT>";

                    // Display Relative Humidity in STATION
                    if (DewptC[sta_n] >= 24.0)   html_code += "<FONT SIZE='-1' FONT COLOR='Red'><I>&nbsp&nbsp&nbspAnd Muggy</I></FONT>";
                    float Rel_Humid = Calc_Rel_Humid(sta_n);             // *** Calculate Relative Humidity
                    html_code += "<BR><FONT COLOR='Purple'>&nbsp&nbsp&nbsp" + String(Rel_Humid, 0) + " %</FONT>";
                  }

                  //  Altimeter in STATION
                  if (Altim[sta_n] == 0)  html_code += "</TD></TR><TR><TD>Altimeter [QNH]</TD><TD>NA";   else
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

                  //  Elevation & Density Altitude in STATION
                  html_code += "</TD></TR><TR><TD>Elevation</TD><TD> " + String(Elevation[sta_n], 1) + " m&nbsp&nbsp&nbsp:&nbsp&nbsp&nbsp" + String(Elevation[sta_n] * 3.28, 1) + " Ft</TD></TR>";

                  html_code += "<TR><TD><FONT COLOR='Purple'>Estimated Density Altitude</TD><TD>";
                  float Density_Alt = Calc_Density_Alt(sta_n);             // *** Calculate Density Altitude
                  if (TempC[sta_n] == 0 || Altim[sta_n] == 0)   html_code += "<FONT COLOR='Purple'>NA</TD></TR>"; else  html_code += "<FONT COLOR='Purple'>" + String(Density_Alt / 3.28, 1) + " m&nbsp&nbsp&nbsp:&nbsp&nbsp&nbsp" + String(Density_Alt, 1) + " Ft</TD></TR>";

                  html_code += "</FONT></TABLE>";             //  End of Table in Station
                  client.print(html_code);

                  // Display LINK:  AIRPORT INFORMATION from AirNav.com
                  // https://www.airnav.com/airport/Ksav
                  String airnav = "<BR><FONT COLOR='Orange'><a href='https://www.airnav.com/airport/" + String(Stations[sta_n].substring(0, 4)) + "'><B>AIRPORT INFORMATION</B></a>";
                  html_code = airnav + " <FONT SIZE='-1'>for US Airports<BR>Note: Press the BACK ARROW to return to THIS PAGE<BR></FONT>";
                  client.print(html_code);

                  // Display Foooter and Close
                  html_code = "<BR><FONT COLOR='Navy' FONT SIZE='-1'>File Name &nbsp&nbsp&nbsp&nbsp&nbsp&nbsp: " + String(ShortFileName);
                  html_code += "<BR>Connected to &nbsp: " + String(ssid);
                  html_code += "<BR>H/W Address &nbsp: " + HW_addr;
                  html_code += "<BR>URL Address &nbsp: " + SW_addr;
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


// ***********   Update Minutes Ago
void Update_Ago(int i)  {
  int search0 = StationMetar[i].indexOf("Z");
  byte obsh = StationMetar[i].substring(search0 - 4, search0 - 2).toInt();  // METAR Obs Time - Hour
  byte obsm = StationMetar[i].substring(search0 - 2, search0).toInt();      // METAR Obs Time - Minute
  int ago = ((Hour - obsh) * 60) + Minute - obsm;                           // Minutes Ago
  if (ago < 0)    ago = ((24 - obsh) * 60) + Minute - obsm;
  int search1 = StationMetar[i].indexOf("(");
  if (search0 > 0 && search1 > 0)
    StationMetar[i] =  StationMetar[i].substring(0, search1) + "(" + String(ago) + "m&nbspAgo)";
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
