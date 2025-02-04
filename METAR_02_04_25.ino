/* 02/04/25  Latest Software on Github : https://github.com/Jpipe001/METAR    <<   Check for Updates
   See also on Github : https://github.com/Jpipe001/LEDpicker  LED Color Picker

  METAR Reporting with LEDs and Local WEB SERVER
  In Memory of F. Hugh Magee, brother of John Magee author of poem HIGH FLIGHT.
  https://en.wikipedia.org/wiki/John_Gillespie_Magee_Jr.                         ~  GOOD READ
  https://joeclarksblog.com/wp-content/uploads/2012/11/high_flight-790x1024.jpg  ~  Poem HIGH FLIGHT

  https://youtu.be/Yg61_kyG2zE      HomebuiltHELP; The video that started me on this project.
  https://youtu.be/xPlN_Tk3VLQ      Getting Started with ESP32 video from DroneBot Workshop.

  https://aeronav.faa.gov/visual/   FAA Wall Chart downloads and then edit to suit.

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

  SUMMARY:
  Created by John Pipe with help from David Bird.
  This is a comprehensive and configurable package for ESP32 and has evolved over the years.
  It uses a large amount of memory forcing me to use older versions of the Libary and Board Configurations because of the memory limitiations.
  ******************************************************************************
  *** Use FastLED  by Daniel Garcia        Must Use Version 3.7.4 or Earlier ***
  *** and Espressif Systems Board Manager  Must Use Version 3.0.7 or Earlier ***
  ******************************************************************************
  ESP32 is a single 2.4 GHz Wi-Fi-and-Bluetooth SoC (System On a Chip) designed by Espressif Systems.
  Once the code is downloaded, it will run by it's self and will automatically restart, even after a power off.

  A computer, with FREE software https://www.arduino.cc/en/Main/Software , is needed to initially configure,
  download software and (optionally) display any program messages using the (built in) Serial Monitor.
  (Watch Getting Started with ESP32 video from DroneBot Workshop, link above).

  Updates METARS approximately every ten minutes, so nearly REAL TIME data, from AVIATIONWEATHER.GOV.
  A set of WS2811 or WS2812 LEDS show all station CATEGORIES (similar to the HomebuiltHELP video, link above).
  Then cycles through all the stations and flashes individually for:
  Wind Gusts(Cyan)[suspendable], Precipitation(Green/White), Ice(Blue), Other(Yellow).
  Then displays GRADIENTS [suspendable] for all stations, for Visibility [White to Red], Wind Gusts [Shades of Cyan] ~ Suspended,
  Temperature Gradient [Blue Green Yellow Orange Red] and Altimeter Pressure Gradient [Blue to Purple].

  To use "Over_The_Air".  First: "Upload" code as normal, using a cable, to initialise OTA.
  Then use the menu "Tools", "Port" and select metar xx.x.x.xx and "Upload" again.
  (Direct connection with the computer is now NOT required.)

  DISPLAYS: Viewable with a computer or cell phone connected on the SAME network:
  SUMMARY gives a colorful SUMMARY overview and STATION shows DECODED METAR information and much MORE.

  NOTE: To view these, you need the http address which is shown at start up,
  if the Serial Monitor is switched on or Click on this Logical Name Test Link  http://metar.local/summary

  Makes a GREAT Christmas Tree Chain of Lights, (and a Good Conversation Piece).

  OPTIONAL:
   In Main Loop: Turn LEDS OFF at NIGHT  5PM to 8AM ~ Local Time ~ Disabled
   In Main Loop: Reduce LED Power usage while getting Data ~ Disabled
   In Display_Metar_LEDS: Gradient Effect ~ Partially Enabled
   In Single_Metar: Print Stations that were Updated  ~ Disabled
   In Checking AIRPORT CODE: Airport Code can be assigned to any Station No ~ Disabled Set to 0
   In Display SUMMARY Table  ~  Change Units of Temperature "Deg F" & Altimeter "in Hg"

  MADE THINGS A LITTLE BETTER, BUG FIXES, IMPROVEMENTS, REPAIRS TO TIME-SPACE CONTINUUM, ETC, ETC.
  Includes: Decoded Metar, Current UTC Time, Elevation Ft, Estimated Density Altitude Ft, Temp & Pressure Units can be changed.
  ANY Airport code may be used in the Worldwide FAA Data Base, but optimized for US airports.

  Questions, Comments or Suggestions "Mail to" - Jpipe001@Gmail.com. Your Feedback is Appreciated and I can send you updates.

  RECENT CHANGES:
  Added Get FAA Station Name if Station Name = GETNAME 06/10/24
  Added Max / Min Temperature in Station Display  06/25/24
  Added Radar link in Station Display  07/25/24
  Swatted a few Bugs, Minor Improvements to make things Better 09/26/24
  Improved Version   12/03/24
  Revised Decode_Name & GetData  12/20/24
  Improved Station Display   01/19/25
*/

// Include the following Libaries:
#include <Arduino.h>
#include "Over_The_Air.h" // Over The Air Software Updates  *** Put This File in the SAME Folder as METAR ***
#include <FastLED.h>      // FastLED  by Daniel Garcia  *** Must Use Version 3.7.4 or Earlier ***
#include <WiFiMulti.h>    // WifiMulti_Generic  by Khoi Hoang
#include <HTTPClient.h>   // HttpClient  by Adrian McEwen
#include <ESPmDNS.h>      // MDNS_Generic  by Khoi Hoang
WiFiMulti wifiMulti;
WiFiServer server(80);    // Set server port to 80

//  ################   CONFIGURE YOUR SETTINGS HERE  ################

// Please Modify the "Your Network Name" and "Your Network Password" To suit your Project

const char*        ssid = "NETGEAR46";       // Your Network SSID
const char*    password = "icysea351";       // Your Network Password

/* Please Modify the "No_Stations and LED_TYPE and COLOR_ORDER" To suit your Project
  Program Note: LED_TYPE and COLOR_ORDER are Important. If LED is RED when it should be GREEN,
  Then Change the LED_TYPE to WS2811 and COLOR_ORDER to RGB. BE SURE you have FastLED Version 3.7.4 or Earlier.*/

// Set Up LEDS   ***   Connect LEDS to Ground & 5 Volts on ESP32 pins (There are Varients, so Check)
#define No_Stations          65      // Number of LEDs / Number of Stations (100 Stations is possible but not recommended)
#define LED_TYPE         WS2812      // WS2811 or WS2812 or NEOPIXEL
#define COLOR_ORDER         GRB      // (WS2811 = RGB) or (WS2812 = GRB) or (NEOPIXEL = CRGB)
#define DATA_PIN              5      // Connect LED Data Line to pin D5/P5/GPIO5  *** With CURRENT LIMITING 330 Ohm Resistor in Line ***
#define NUM_LEDS    No_Stations      // Number of LEDs
#define BRIGHTNESS           18      // Master LED Brightness (<12=Dim 16~20=ok >20=Too Bright) Set as required
CRGB leds[NUM_LEDS];                 // Color Order for LEDs ~ CRGB Colors
#define FRAMES_PER_SECOND   120

//Please Modify the "List of Stations" To suit your Project.
/*   This is the Order of LEDs; "NULL" is for NO airport or LED not used.
   Station ID in CAPITALS are REQUIRED !!
   First SEVEN Characters are REQUIRED !! (including the first Comma and the Space, See examples below)

   Edit or Over type your Station ID and METAR Reporting Station (Airport) Name.
   and include the "quotes" and the "first and last comma" (SYNTAX is IMPORTANT).

   You can use the FAA Airport Name by using  "GETNAME"  OR  Customise the Station Name.
   "GETNAME" is useful if you don't want to enter all the airport names (The easy way).
   Remember:  SPACING and SYNTAX is IMPORTANT.

   https://SkyVector.Com   ~~   Good for locating METAR Reporting Stations.

   Note: You can have Dummy LED that is Connected in the chain but Not Used. (See below)
*/
// Define List of Stations   ~~~~~   Some Worldwide Examples
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
  "KLAX, GETNAME",                      // 33   Use FAA Station Name
  "KBOS, BOSTON, MA",                   // 34
  "KCLT, CHARLOTTE, NC",                // 35
  "KBNA, NASHVILLE, TN",                // 36
  "EGPN, DUNDEE, SCOTLAND",             // 37
  "EGPH, EDINBURGH, SCOTLAND",          // 38
  "EGPE, INVERNESS, SCOTLAND",          // 39
  "EGLL, LONDON HEATHROW, UK",          // 40
  "EGKK, LONDON GATWICK, UK",           // 41
  "EGCC, MANCHESTER, UK",               // 42
  "EGDM, GETNAME",                      // 43   Use FAA Station Name
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
  "KGOV, GETNAME",// 61
  "KDTW, GETNAME",// 62
  "KMGN, GETNAME",// 63
  "KSDF, GETNAME",// 64
  "CWDV, GETNAME",// 65
};                        // <<  Do NOT delete this line ~ End of Station List

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

// In Flash Memory
PROGMEM String                        HW_addr;   // WiFi local Hardware Address
PROGMEM String                        SW_addr;   // WiFi local URL Address
PROGMEM String                  ShortFileName;   // Shortened File Name
PROGMEM String                       Raw_Data;   // Metar Data from aviationweather.gov
PROGMEM String     Stn_Metar[No_Stations + 1];   // Metar Codes including "new" and "Ago"
PROGMEM String    Sum_Remark[No_Stations + 1];   // Remark Codes used in Summary Display
PROGMEM String    Stn_Remark[No_Stations + 1];   // Remark Decoded used in Station Display
PROGMEM String   Sig_Weather[No_Stations + 1];   // Significant Weather used in Station Display
PROGMEM String          Wind[No_Stations + 1];   // Wind speed
PROGMEM String      new_wDir[No_Stations + 1];   // New Wind direction
PROGMEM int         old_wDir[No_Stations + 1];   // Previous Wind direction
PROGMEM String           Sky[No_Stations + 1];   // Sky_cover used in Summary Display BKN
PROGMEM String     Sky_cover[No_Stations + 1];   // Sky_cover used in Station Display Broken
PROGMEM String      Category[No_Stations + 1];   // VFR  MVFR  IFR  LIFR  NA   NF
PROGMEM String         Visab[No_Stations + 1];   // Visibility
PROGMEM String      Lat_Long[No_Stations + 1];   // Lat_Long in text
PROGMEM float          TempC[No_Stations + 1];   // Temperature deg C
PROGMEM float         DewptC[No_Stations + 1];   // Dew point deg C
PROGMEM float       SeaLpres[No_Stations + 1];   // Sea Level Pressure
PROGMEM float          Altim[No_Stations + 1];   // Altimeter setting
PROGMEM float      old_Altim[No_Stations + 1];   // Previous altimeter setting
PROGMEM float      Elevation[No_Stations + 1];   // Elevation setting
PROGMEM int   new_cloud_base[No_Stations + 1];   // New Cloud Base
PROGMEM int   old_cloud_base[No_Stations + 1];   // Previous Cloud Base

#define On_Board_Led 2        // ON Board LED GPIO 2
char Clock[] = "HH:MM";       // Clock  "HH:MM"
byte Hour = 0;                // Latest Hour
byte Minute = 0;              // Latest Minute
String Last_Up_Time;          // Last Update Time  "HH:MM"
byte Last_Up_Min = 0;         // Last Update Minute
byte Group_of_Stations = 17;  // Download a Group of <20 Stations at a time, if "Station Missing Data" ~ reduce this
byte Update_Interval = 10;    // Updates Data every 10 Minutes (Don't overload aviationweather.gov)
byte Count_Down = 0;          // Count down to Next Update
int Stn_Flash = 1;            // Station # for Server - flash button
int httpCode;                 // HTTP Error Code
byte Comms_Flag = 0;          // Communication Flag
TaskHandle_t Task1;           // Main_Loop, Core 0 for Getting Data and Display Leds
TaskHandle_t Task2;           // Go_Server, Core 1 for Creating Web Pages and Handling Requests
const char* ServerName = "metar";
/* To create a Stable HTML Display/Web Page with a Stable (hardware) LOGICAL Address,
   because the hardware address may change after a Router Power Failure or Reset.
   after downloading code ~ Test Link: http://metar.local/summary
*/


void setup() {
  pinMode(On_Board_Led, OUTPUT);     // The Onboard LED
  Serial.begin(115200);
  delay(4000);         // Time to press "Clear output" in Serial Monitor
  Serial.printf("\nMETAR Reporting with LEDs and Local WEB SERVER\n");
  ShortFileName = String(__FILE__).substring(String(__FILE__).lastIndexOf("\\") + 1);       // Shortened File Name
  if (ShortFileName == __FILE__) ShortFileName = String(__FILE__).substring(String(__FILE__).lastIndexOf("/") + 1);  // Shortened File Name Using Raspberry Pi
  Serial.printf("Long File Name   ~ %s\n", __FILE__);
  Serial.printf("Short File Name  ~ %s\n", ShortFileName.c_str());
  Serial.printf("Date Compiled    ~ %s\n", __DATE__);

  // Initialize the WiFi network settings.
  WiFi.begin(ssid, password);
  WiFi.mode(WIFI_STA);

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

  SetupOTA(ServerName, ssid, password);      // Set Up ~ Over_The_Air.h

  if (!MDNS.begin(ServerName) || count > 80) {     // Start mDNS with ServerName
    Serial.printf("\nSOMETHING WENT WRONG\nError setting up MDNS responder !\nProgram Halted  ~  Check Network Settings !!\n");
    while (1) {
      delay(1000);      // STAY HERE
    }
  }

  long rssi = WiFi.RSSI() + 100;   // Print the Signal Strength:
  Serial.printf("Signal Strength = %ld", rssi);
  if (rssi > 50)  Serial.printf(" (>50 - Good)\n");  else   Serial.printf(" (Could be Better)\n");

  Init_LEDS();         // Initialize LEDs

  Serial.printf("*******************************************\n");

  // Initialize the WiFi settings to Get data from Server.
  wifiMulti.addAP(ssid, password);

  Serial.printf("To View Decoded Station METARs from a Computer or Other Device connected to the %s WiFi Network.\n", ssid);

  HW_addr = "http://" + WiFi.localIP().toString();      // IP Address
  Serial.printf("\t\tIP Address \t: %s\n", HW_addr.c_str());

  SW_addr = "http://" + String(ServerName) + ".local";  // Logical Address (ServerName)
  Serial.printf("MDNS started ~\tUse Address\t: %s\n", SW_addr.c_str());

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);   // Get UTC Time from Server
  Update_Time();        // Set Up Time : Hour & Minute

  char TimeChar[50];
  strftime(TimeChar, sizeof(TimeChar), "%A, %B %d %Y %H:%M - UTC", &timeinfo);
  Serial.printf("Date & Time :\t%s\n*******************************************\n", TimeChar);

  Get_FAA_Data();      // GET FAA Lat/Long and Elevation and Station Name if Station Name = GETNAME

  //Task1 = Main_Loop() function, with priority 1 and executed on core 0 for Getting Data and Display Leds
  xTaskCreatePinnedToCore( Main_Loop, "Task1", 10000, NULL, 1, &Task1, 0);
  delay(200);          // Wait a smidgen

  //Task2 = Go_Server() function, with priority 1 and executed on core 1 for Creating Web Pages and Handling Requests
  xTaskCreatePinnedToCore( Go_Server, "Task2", 10000, NULL, 1, &Task2, 1);
  delay(200);          // Wait a smidgen

  Serial.printf("Dual Core Initialized\n");
  server.begin();      // Start the web server
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
    byte Off_Hr = 21;     // 5PM = 17:00Hr + 4 = 21 Hr UTC  ~ OFF : Function Suspended
    Off_Hr = 24;          // Comment Out or Delete this line, To Invoke This Fuction

    if (Hour == Off_Hr)  {                   // Check for Off Hour
      while (Hour != On_Hr)  {               // Loop until On Hour
        Display_Black_LEDS();                // Set All LEDS to Black
        delay(Update_Interval * 60000);      // Wait Update_Interval in milli Seconds
        Update_Time();                       // Update Current Time : Hour & Minute
      }
    }
    if (Last_Up_Min + Update_Interval > 60)   Last_Up_Min = 60 - Update_Interval;
    Count_Down = Last_Up_Min + Update_Interval - Minute;

    Get_All_Metars();                // Get All METAR DATA Routine
    Serial.printf("%s\tMetar Updated\tNext Update in %d Mins\n\n", Clock, Count_Down);

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
  Display_Cat_LEDS ();               //  Display LEDS for All Categories  ~ Do NOT Delete
  //digitalWrite(On_Board_Led, HIGH);  //  ON ~~ Used if No Leds
  Display_Weather_LEDS (10);         //  Display LEDS for Twinkle Weather ~ Suspendable
  //digitalWrite(On_Board_Led, LOW);   //  OFF ~~ Used if No Leds
  delay(2000);                       //  Delay after Display Weather (Seconds * 1000) ~ Change this but Do NOT Delete

  // ***********   Comment these lines out to Suspend a Gradient function with "//"
  int Wait_Time = 5000;             //  Delay after Function (Seconds * 1000) ~ Change this but Do NOT Delete
  //Display_Vis_LEDS (Wait_Time);     //  Display LEDS for Visibility [Red-Pink-White]
  //Display_Wind_LEDS (Wait_Time);    //  Display LEDS for Wind Speed [Shades of Aqua]
  Display_Temp_LEDS (Wait_Time);    //  Display LEDS for Temperatures [Blue-Green-Yellow-Red]
  Display_Alt_LEDS (Wait_Time);     //  Display LEDS for Distribution of Pressure [Blue-Purple]
}


// *********** Set/Get Current Time Clock, Hour, Minute
void Update_Time() {
  if (!getLocalTime(&timeinfo)) {
    Serial.printf("**  FAILED to Get Time  **\n");
    Network_Status ();            // WiFi Network Error
  }
  char TimeChar[6];
  strftime(TimeChar, sizeof(TimeChar), "%H:%M", &timeinfo);
  strcpy(Clock, TimeChar);
  String Time = TimeChar;                     // String  "HH:MM"
  Hour   = Time.substring(0, 2).toInt();      // Byte     HH
  Minute = Time.substring(3, 5).toInt();      // Byte     MM
}


void Get_FAA_Data() {      // *********** GET Lat/Long and Elevation and FAA Station Name if Station Name = GETNAME
  int Sta_n[No_Stations + 1];
  String List = "";
  for (int n = 0; n <= No_Stations; n = n + Group_of_Stations) {
    int Start  = n;
    int Finish = Start + Group_of_Stations - 1;
    if (Finish > No_Stations)  Finish = No_Stations;
    for (int i = Start; i <= Finish; i++) {        // Create a list of Stations
      Sta_n[i] = 0;
      if (Stations[i].substring(0, 4) == "NULL")  Reset_All_Parameters(i);
      List += Stations[i].substring(0, 5);
      Sta_n[i] = i;
    }
    List = List.substring(0, List.length() - 1);   // Remove last "comma"
    if (List.length() > 3)  Serial.printf("No:%d\tIn Get_FAA_Data: List=%s\n", n, List.c_str());
    if (List.length() > 3)  GetData("NAME", List, Start);    // GetData Routine ~ Group of Stations
    List = "";
    for (int i = Start; i <= Finish; i++) {
      if (Sta_n[i] > 0)
        Decode_Name(Stations[i].substring(0, 4), Sta_n[i]);  // Decode Station Name Routine ~ One Station at a time
    }
  }
}


void Decode_Name(String Station_name, int n) {        // ***   DECODE Lat/Long and Elevation and the Station NAME
  int search0 = Raw_Data.indexOf(Station_name);       // Start Search from Here
  int search1 = Raw_Data.indexOf("Latitude:", search0);
  int search2 = Raw_Data.indexOf("Longitude:", search1);
  int search3 = Raw_Data.indexOf("Elevation:", search1);
  int search4 = Raw_Data.indexOf("Site:", search1);
  int search5 = Raw_Data.indexOf("State:", search1);
  int search6 = Raw_Data.indexOf("Country:", search1);
  if (search0 > 0 && search1 > 0)  {
    Lat_Long[n] = Raw_Data.substring(search1 + 10, search2 - 3) + ", " + Raw_Data.substring(search2 + 11, search3 - 3);
    Elevation[n] = Raw_Data.substring(search3 + 11, search4 - 3).toFloat();  // In meters
    if (Stations[n].substring(6, 13) == "GETNAME" || n == 0)  {
      Stations[n] = Station_name + ", ";  // Station ID
      Stations[n] += Raw_Data.substring(search4 + 6, search5 - 3) + ", ";  // Site
      Stations[n] += Raw_Data.substring(search5 + 7, search5 + 9) + ", ";  // State
      Stations[n] += Raw_Data.substring(search6 + 9, search6 + 11);        // Country
      Stations[n].replace(", US", "");  // Remove "US" as Country
    }
    if (Stations[n].length() < 7)
      Serial.printf("%s\tNo:%d\t%s\tIn Decode_Name: No Name Decoded\n", Clock, n, Stations[n].c_str());
  }
}


void Init_LEDS() {                    // *********** Initialize LEDs  Set up the strip configuration
  FastLED.setBrightness(BRIGHTNESS);          // Master LED Brightness
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, No_Stations).setCorrection(TypicalLEDStrip); // For WS2811 & WS2812
  //FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);       // For NEOPIXEL
  Display_Black_LEDS();                        // Set All LEDS to "Black" Routine
  Serial.printf("LEDs Initialized for No_Stations = %d\nUPDATED every %d minutes %d Stations at a time.\n", No_Stations, Update_Interval, Group_of_Stations);
}


void Display_Black_LEDS() {        // *********** Reset LEDS to Black (Start Up & Turn LEDS OFF at NIGHT)
  fill_solid(leds, No_Stations, CRGB::Black);  // Set All leds to Black
  FastLED.show();
  delay(50);    // Wait a smidgen
}


void Get_All_Metars() {         // *********** GET All Metars (in Groups of Stations <20 at a time)
  for (byte j = 0; j < No_Stations; j = j + 1 + Group_of_Stations) {
    byte Start  = j;
    byte Finish = Start + Group_of_Stations;
    if (Finish > No_Stations)  Finish = No_Stations;
    String List = "";
    for (byte i = Start; i <= Finish; i++) {        // Create a list of Stations
      List += Stations[i].substring(0, 5);
    }
    List = List.substring(0, List.length() - 1);    // Remove last "comma"
    Check_Coms();
    Comms_Flag = 1;                 // Set Communication Flag Active = 1
    GetData("METAR", List, Start);  // GetData Routine ~ Group of Stations
    Comms_Flag = 0;                 // Reset Communication Flag = 0
    for (int i = Start; i <= Finish; i++) {
      ParseMetar(i);                // ParseMetar Routine ~ One Station at a time
    }
  }
}


void GetData(String Type, String List, int i) {    // *********** GET Some Metar Data/Name Group of Stations
  Raw_Data = "";                // Reset Raw Data for Group of Stations
  String url = "";              // Reset url string
  // To get Station NAME & Information from "Get_FAA_Data" or "Go_Server" revised 12/14/24
  // New Revised Test Link:  https://aviationweather.gov/api/data/stationinfo?ids=KCHA,KORD,KTBR,EGDM,CYQX,EHKV,EHHW
  String Host_Name = "https://aviationweather.gov/api/data/stationinfo?ids=";  // Format = text

  // To get Station DATA used for Raw_Data from "Get_All_Metars" or "Go_Server" revised 08/23/24
  // New Revised Test Link: https://aviationweather.gov/api/data/metar?ids&format=xml&hours=0&ids=NULL,KCHA,KRMG,KVPC,KATL,KLAL,KGOV,KPLN,KSDF,KDTW,KMYR,CYDC
  String Host_Data = "https://aviationweather.gov/api/data/metar?ids&format=xml&ids=";  // Format = xml

  // Format  = Host_Name or Host_Data + "STATION IDs"
  if (Type == "NAME") url = Host_Name + List;  else   url = Host_Data + List;

  if (wifiMulti.run() == WL_CONNECTED)  {
    HTTPClient https;
    https.begin(url);                      // Start connection and send HTTP header
    httpCode = https.GET();                // httpCode will be negative on error so test or 200 for HTTP_CODE_OK
    if (httpCode == 200 ) {                // HTTP_CODE_OK
      // HTTP header has been send and Server response header has been handled
      // and File FOUND at server. Get back : httpCode = 200 (HTTP_CODE_OK)
      Raw_Data = https.getString();        // SAVE DATA in Raw_Data for Group of Stations

      if (ESP.getMaxAllocHeap() - Raw_Data.length() < 10500)   { // NO UPDATE : RUNNING LOW OF MEMORY
        Serial.printf("\n%s\tNo:%d\t%s\tSkipped ~ NO UPDATE Running Low on Memory, in GetData\n", Clock, i, Stations[i].substring(0, 4).c_str());
        //Serial.printf("\n%s\tNo:%d\tIn GetData : Raw_Data Size=%d  MaxAllocHeap=%d  Free Memory=%d  httpCode=%d ~ %s\n", Clock, i, Raw_Data.length(), ESP.getMaxAllocHeap(), ESP.getMaxAllocHeap() - Raw_Data.length(), httpCode, https.errorToString(httpCode).c_str());
        httpCode = -100;                  // NO UPDATE for this Group of Stations, in ParseMetar
        Network_Status ();                // WiFi Network Error
      }
      if (Raw_Data.length() < 140)   {    // NO UPDATE : NO Raw_Data DOWNLOADED
        Serial.printf("\n%s\tNo:%d\t%s\tSkipped ~ NO DATA Downloaded, in GetData : %i bytes  httpCode = %i\n", Clock, i, Stations[i].substring(0, 4).c_str(), Raw_Data.length(), httpCode);
        //Serial.printf("\n%s\tNo:%d\tIn GetData : Raw_Data Size=%d  MaxAllocHeap=%d  Free=%d  httpCode=%d ~ %s\n", Clock, i, Raw_Data.length(), ESP.getMaxAllocHeap(), ESP.getMaxAllocHeap() - Raw_Data.length(), httpCode, https.errorToString(httpCode).c_str());
        httpCode = -200;                   // NO UPDATE for this Group of Stations, in ParseMetar
        Network_Status ();                 // WiFi Network Error
      }
    }  else  {                             // CONNECTION ERROR : NO UPDATE for this Group of Stations, in ParseMetar
      Serial.printf("\n%s\tNo:%d\t%s\tSkipped ~ NO UPDATE Connection Error, in GetData\n\t\t\tConnection Error[httpCode]= %d ~ %s\n", Clock, i, Stations[i].substring(0, 4).c_str(), httpCode, https.errorToString(httpCode).c_str());
      Network_Status ();                   // WiFi Network Error
    }
    https.end();                           // CLOSE Communications
  }
}


void Network_Status() {     // ***********   WiFi Network Error
  Serial.printf("%s\t\t\tWiFi Network Status: ", Clock);
  if (WiFi.status() == 0 ) Serial.printf("= 0 ~ Idle\n");
  if (WiFi.status() == 1 ) Serial.printf("= 1 ~ Not Available, SSID can not be reached\n");
  if (WiFi.status() == 2 ) Serial.printf("= 2 ~ Scan Completed\n");
  if (WiFi.status() == 3 ) Serial.printf("= 3 ~ Connected to %s Router\n", String(ssid).c_str());
  if (WiFi.status() == 4 ) Serial.printf("= 4 ~ Failed, Password Incorrect\n");
  if (WiFi.status() == 5 ) Serial.printf("= 5 ~ Router Connection Lost\n");
  if (WiFi.status() == 6 ) Serial.printf("= 6 ~ Disconnected from Router\n");
}


void Reset_All_Parameters(int i) {    // ***********   Reset All Parameters if Station Not Found in ParseMetar or in Go_Server
  Stn_Metar[i] = "Station Not Found / Not Reporting";
  Sig_Weather[i] = "None"; // Not Found
  Category[i] = "NF";      // Not Found
  Sky[i] = "NA";           // Not Found
  Sky_cover[i] = "";       // Not Found
  Stn_Remark[i] = "";      // Not Found
  Sum_Remark[i] = "";      // Not Found
  Visab[i] = "NA";         // Not Found
  new_wDir[i] = "NA";      // Not Found
  Wind[i] = "NA";          // Not Found
  TempC[i] = 99;           // Not Found
  DewptC[i] = 99;          // Not Found
  Altim[i] = 0;            // Not Found
  old_Altim[i] = 0;        // Not Found
  new_cloud_base[i] = 0;   // Not Found
  Elevation[i] = 0;        // Not Found
}


void ParseMetar(int i) {     // ***********   Parse Metar Data ~ One Station at a time
  String Single_Metar = "";                                 // Reset Single Metar Data ~ For One Station
  String station = Stations[i].substring(0, 4);             // Station ID
  int Start_Search = Raw_Data.indexOf(station) - 24;        // Search Station ID
  int Data_Start = Raw_Data.indexOf("<METAR>", Start_Search);  // Search for Data Start "<METAR>"
  int Data_End = Raw_Data.indexOf("</METAR>", Data_Start) + 8; // Search for Data End "</METAR>"

  int index = Stn_Metar[i].lastIndexOf("</font>");
  if (index > 0)  Stn_Metar[i] = Stn_Metar[i].substring(index + 7, Stn_Metar[i].length());
  if (station == "NULL")   return;// NO UPDATE ~ for this Station
  if (httpCode < 0)   {           // NO UPDATE ~ httpCode < 0 Error
    if (index < 0)    {
      Stn_Metar[i] = "<font color='Orange'>Skipped ~ Connection Error ~ Using Old Data<br></font>" + Stn_Metar[i];
    }  else   {
      Reset_All_Parameters(i);    // Reset All Parameters
      Stn_Metar[i] = "<font color='Orange'>Connection Error or<br></font>" + Stn_Metar[i];
    }
    Serial.printf("%s\tNo:%d\t%s\tConnection Error, in ParseMetar\n", Clock, i, station.c_str());
    return;
  }
  if (Start_Search < 0)  {       // NO UPDATE ~ Station Not Found / Not Reporting ~ Skipped
    if (index < 0)       {
      Stn_Metar[i] = "<font color='Orange'>Skipped ~ Station Not Found ~ Using Old Data<br></font>" + Stn_Metar[i];
    }  else  {
      Reset_All_Parameters(i);   // Reset All Parameters
      Stn_Metar[i] = "<font color='Orange'>Skipped<br></font>" + Stn_Metar[i];
    }
    Serial.printf("%s\tNo:%d\t%s\tStation Not Found, in ParseMetar\n", Clock, i, station.c_str());
    return;
  }
  if (Data_End == 7)   {         // NO UPDATE ~ If can't Find "</METAR>" in Raw_Data ~ Missing Data
    if (index < 0)     {
      Stn_Metar[i] = "<font color='Orange'>Missing Metar Data ~ Using Old Data<br></font>" + Stn_Metar[i];
    }  else  {
      Reset_All_Parameters(i);   // Reset All Parameters
      Stn_Metar[i] = "<font color='Orange'>Connection Error ~ Missing Data<br></font>" + Stn_Metar[i];
    }
    Serial.printf("%s\tNo:%d\t%s\tStation Missing Data ~ No </METAR>, in ParseMetar ~ Reduce Groups of Stations\n", Clock, i, station.c_str());
    return;
  }

  if (Data_End > 12)  {         // STATION FOUND
    Single_Metar = Raw_Data.substring(Data_Start, Data_End);// Create Single Metar Data
    // For Troubleshooting :
    //Serial.printf("\nIn ParseMetar: Single_Metar ~ No:%d  %s Data_Start:%d  Data_End:%d\nFirst 150 Chars\n.%s.\n", i, station.c_str(), Data_Start, Data_End, Single_Metar.substring(0, 150).c_str());

    // Remove found data from Raw_Data
    Raw_Data = Raw_Data.substring(0, Data_Start - 5) + Raw_Data.substring(Data_End, Raw_Data.length());

    Decodedata(i, station, Single_Metar);                    // DECODE the Station DATA
  }
}


void Decodedata(int i, String station, String Single_Metar) {  // *********** DECODE the Station DATA CODES (First Line  <raw text>)
  String old_obs_time = Stn_Metar[i].substring(0, 4);       // Previous Last Observation Time
  if (old_obs_time == "new ")  old_obs_time = Stn_Metar[i].substring(4, 8);
  // Otherwise old_obs_time = "" and will UPDATE later

  int Start = Single_Metar.indexOf(station) + 7;       // Start of search for station
  int Last = Single_Metar.indexOf("</raw_text>");      // End of station Codes - (First Line "</raw_text")

  if (Start < 7 || Last < 7)  {
    // For Troubleshooting :
    Serial.printf("%s\tNo:%d\t%s\tERROR In Decodedata : Search Failed ~ Start= %d  search_raw_text(Last)= %d\n", Clock, i, station.c_str(), Start, Last);
    //Serial.printf("\nSingle_Metar=\n%s\n", Single_Metar.c_str());
    return;
  }

  // SEARCH for Station DATA CODES (First Line  <raw text>)
  int From = Single_Metar.indexOf("Z ", Start) + 10; // Start search from timeZ + 10
  int  End = Single_Metar.lastIndexOf(" Q") + 6;     // Search for "Q"  Non US : Station Metar End
  if (End < 6) End = Single_Metar.indexOf(" A", From) + 6; // Search for "A"  in  US : Station Metar End
  int EndR = Single_Metar.indexOf(" RMK");           // Search for "RMK"
  if (EndR < 0)    EndR = End;                       // No RMK

  // *** CREATE  STATION METAR Used in Decode WEATHER NOT with "new" & "station" & "ago"
  Stn_Metar[i] = Single_Metar.substring(Start, EndR);      // (First Line  <raw text>)

  // *** UPDATE/SKIP this Station *** : Compare Last Observation Time with [Single_Metar Observation Time] in New STATION METAR
  if (old_obs_time != Stn_Metar[i].substring(0, 4))  { // ***  If NO UPDATE : SKIP this STATION  ***

    // *** STATION METAR Used in Decode WEATHER   ***  Add "new "
    Stn_Metar[i] = "new " + Stn_Metar[i];

    // *** CREATE  Remark Codes
    Sum_Remark[i] = Single_Metar.substring(EndR, Last); // Remark used in Summary Display
    Sum_Remark[i].replace("RMK", "");                   // Remove "RMK" leaving only the Codes
    Stn_Remark[i] = Sum_Remark[i];   // Will become Decoded Remark used in Station Display

    // *** DECODING REMARKS *** Remark_text
    String Remark_text = Sum_Remark[i];

    // REMOVE AO2 and everything before
    int search1 = Remark_text.indexOf(" AO");
    int search2 = Remark_text.indexOf(" ", search1 + 1);  // end of AO
    if (search1 >= 0)
      Remark_text = Remark_text.substring(search2, Remark_text.length());

    // REMOVE SLP
    search1 = Remark_text.indexOf(" SLP");
    if (search1 >= 0)
      Remark_text = Remark_text.substring(0, search1 + 1) + Remark_text.substring(search1 + 7, Remark_text.length());

    // REMOVE T0 or T1 ~ Temp & Dew Pt
    search1 = Remark_text.indexOf(" T0");
    if (search1 < 0)  search1 = Remark_text.indexOf(" T1");
    if (search1 >= 0)
      Remark_text = Remark_text.substring(0, search1 + 1) + Remark_text.substring(search1 + 10, Remark_text.length());

    // REPLACE  6 Hour Max & Min Temp  <maxT_c>1.7</maxT_c> & <minT_c>-9.1</minT_c>  at 0000, 0600, 1200, and 1800 UTC
    search1 = Remark_text.indexOf(" 10");
    if (search1 < 0)  search1 = Remark_text.indexOf(" 11");
    search2 = Remark_text.indexOf(" ", search1 + 7);  // end of Temp
    int search3 = Single_Metar.indexOf("<maxT_c>") + 8;
    int search4 = Single_Metar.indexOf("</maxT_c>");
    int search5 = Single_Metar.indexOf("<minT_c>") + 8;
    int search6 = Single_Metar.indexOf("</minT_c>");
    if (search1 >= 0) {
      if (search4 > search3 && search6 > search5)
        Remark_text = Remark_text.substring(0, search1 + 1)
                      + "<br>6 Hour Max Temperature =%" + Single_Metar.substring(search3, search4) + " Deg C<br>"
                      + "6 Hour Min Temperature =%" + Single_Metar.substring(search5, search6) + " Deg C<br>"
                      + Remark_text.substring(search2, Remark_text.length());
    }

    // REPLACE  <three_hr_pressure_tendency_mb>-0.2</three_hr_pressure_tendency_mb>
    search1 = Remark_text.indexOf(" 5");
    search2 = Remark_text.indexOf(" ", search1 + 1);  // end of Press
    search3 = Single_Metar.indexOf("<three_hr_pressure_tendency_mb>") + 31;
    search4 = Single_Metar.indexOf("</three_hr_pressure_tendency_mb>");
    if (search1 >= 0) {
      if (search4 > search3)
        Remark_text = Remark_text.substring(0, search1 + 1)
                      + "<br>3 Hour Pressure Tendency =%" + Single_Metar.substring(search3, search4) + " mb<br>"
                      + Remark_text.substring(search2, Remark_text.length());
      search3 = Remark_text.indexOf("%-");
      if (search3 < 0)   Remark_text.replace("%", "%+");
    }

    // REPLACE  Precipition in Hundredths of Inch   pppp = Amount in Hundredths of Inch
    String Type;
    search1 = Remark_text.indexOf(" P0");  //Hourly = Ppppp
    if (search1 >= 0) {
      Type = "Hourly Precipition = ";
      Remark_text = Decode_Precip (search1, Type, Remark_text);
    }
    search1 = Remark_text.indexOf(" 6");  //6 Hour = 6pppp  at 0000, 0600, 1200, and 1800 UTC
    if (search1 >= 0) {
      Type = "6 Hour Precipition = ";
      Remark_text = Decode_Precip (search1, Type, Remark_text);
    }
    search1 = Remark_text.indexOf(" 7");  //24 Hour = 6pppp
    if (search1 >= 0) {
      Type = "24 Hour Precipition = ";
      Remark_text = Decode_Precip (search1, Type, Remark_text);
    }

    //  ****  REPLACE   Itnnn  Hourly Ice Accretion in Hundredths of Inch    t = 1 or 3 or 6 hour   nnn= Icing amount
    search1 = Remark_text.lastIndexOf(" I1");
    if (search1 >= 0) {
      Type = "Hourly Ice = ";
      Remark_text.replace("I1", "I0");
      Remark_text = Decode_Precip (search1, Type, Remark_text);
    }
    search1 = Remark_text.lastIndexOf(" I3");
    if (search1 >= 0) {
      Type = "3 Hour Ice = ";
      Remark_text.replace("I3", "I0");
      Remark_text = Decode_Precip (search1, Type, Remark_text);
    }
    search1 = Remark_text.lastIndexOf(" I6");  //  at 0000, 0600, 1200, and 1800 UTC
    if (search1 >= 0) {
      Type = "6 Hour Ice = ";
      Remark_text.replace("I6", "I0");
      Remark_text = Decode_Precip (search1, Type, Remark_text);
    }

    // REMOVE 933RRR   Water Equivalent of Snow on Ground in tenths in the 0800 UTC report
    search1 = Remark_text.indexOf(" 933");
    if (search1 >= 0)
      Remark_text = Remark_text.substring(0, search1 + 1) + Remark_text.substring(search1 + 7, Remark_text.length());
    /*
        // ****  REPLACE  933RRR   Water Equivalent of Snow on Ground in tenths in the 0800 UTC report
        search1 = Remark_text.indexOf(" 933");
        search2 = Remark_text.indexOf(" ", search1 + 5);  // end of Code
        if (search1 >= 0 )    {
          Type = "Water Equivalent = ";
          Remark_text = Remark_text.substring(0, search1 + 1)
                        + Remark_text.substring(search1 + 4, search2) + "00"
                        + Remark_text.substring(search2, Remark_text.length());
          Remark_text = Decode_Precip (search1, Type, Remark_text);
        }
    */


    //  ****  REPLACE  4/sss  Snow Depth  at 0000, 0600, 1200, and 1800 UTC   sss = Inches Snow on the Ground
    search1 = Remark_text.indexOf(" 4/");
    search2 = Remark_text.indexOf(" ", search1 + 4);  // end of Code
    if (search1 >= 0)   {
      Type = "Snow Depth = ";
      Remark_text = Remark_text.substring(0, search1 + 1)
                    + Remark_text.substring(search1 + 3, search2) + "00"
                    + Remark_text.substring(search2, Remark_text.length());
      Remark_text = Decode_Precip (search1, Type, Remark_text);
    }

    //  ****  REPLACE  SOG sss Snow on the Ground  sss = Inches Snow on the Ground
    search1 = Remark_text.indexOf("SOG");
    search2 = Remark_text.indexOf(" ", search1 + 5);  // end of Code
    if (search1 >= 0)   {
      Type = "Snow on the Ground = ";
      Remark_text = Remark_text.substring(0, search1 + 1) + "%"
                    + Remark_text.substring(search1 + 4, search2) + "00"
                    + Remark_text.substring(search2, Remark_text.length());
      Remark_text = Decode_Precip (search1, Type, Remark_text);
    }


    // *****  Create DATA FOR DISPLAYS  ****
    // SEARCH flight_category in Single Metar for Display
    search1 = Single_Metar.indexOf("<flight_category>") + 17;
    search2 = Single_Metar.indexOf("</flight_category>");
    if (search2 > search1)  {
      Category[i] = Single_Metar.substring(search1, search2);
    }  else  {
      Category[i] = "NA";
      Serial.printf("%s\tNo:%d\t%s\tFlight Category Not Available, in Decodedata\n", Clock, i, station.c_str());
    }
    // SEARCH temp_c in Single Metar TempC[i] in deg C for Display
    search1 = Single_Metar.indexOf("<temp_c") + 8;
    search2 = Single_Metar.indexOf("</temp");
    if (search1 < 8)   TempC[i] = 99;   else  TempC[i] = Single_Metar.substring(search1, search2).toFloat();
    if (search1 < 8)   Serial.printf("%s\tNo:%d\t%s\tTemperature Not Found, in Decodedata\n", Clock, i, station.c_str());

    // SEARCH dewpoint_c in Single Metar DewptC[i] in deg C for Display
    search1 = Single_Metar.indexOf("<dewpoint_c") + 12;
    search2 = Single_Metar.indexOf("</dewpoint");
    if (search1 < 12) DewptC[i] = 99;  else  DewptC[i] = Single_Metar.substring(search1, search2).toFloat();
    if (search1 < 12)   Serial.printf("%s\tNo:%d\t%s\tDewpoint Not Found, in Decodedata\n", Clock, i, station.c_str());

    // SEARCH Wind_dir_degrees in Single Metar for Display
    old_wDir[i] = new_wDir[i].toInt();
    search1 = Single_Metar.indexOf("wind_dir_degrees") + 17;
    search2 = Single_Metar.indexOf("</wind_dir_degrees");
    if (search1 < 17) new_wDir[i] = "NA";  else  new_wDir[i] = Single_Metar.substring(search1, search2);
    if (Stn_Metar[i].indexOf("VRB") > 0) new_wDir[i] = "VRB";  //  (First Line  <raw text>)
    if (new_wDir[i] == "NA")   Serial.printf("%s\tNo:%d\t%s\tWind Not Available, in Decodedata\n", Clock, i, station.c_str());

    // SEARCH Wind_speed_kt in Single Metar for Display
    search1 = Single_Metar.indexOf("wind_speed_kt") + 14;
    search2 = Single_Metar.indexOf("</wind_speed");
    if (search1 < 14) Wind[i] = "NA";  else  Wind[i] = Single_Metar.substring(search1, search2) + " KT";
    if (Wind[i] == "0 KT")  Wind[i] = "CALM";

    // SEARCH visibility_statute_mi in Single Metar for Display
    search1 = Single_Metar.indexOf("visibility_statute_mi") + 22;
    search2 = Single_Metar.indexOf("</visibility");
    if (search1 < 22)  Visab[i] = "NA";  else  Visab[i] = Single_Metar.substring(search1, search2);
    if (search1 < 22)  Serial.printf("%s\tNo:%d\t%s\tVisibility Not Available, in Decodedata\n", Clock, i, station.c_str());
    // Lets look for Measured Vis in Metar (First Line  <raw text>)
    search2 = Stn_Metar[i].indexOf("KT ");           // Start Search from Here
    search3 = Stn_Metar[i].indexOf("M", search2);    // Look for Measured Vis in Metar
    if (search1 > 22 && search3 - search2 == 3)  Visab[i] += "m";

    // SEARCH sky_cover & cloud_base in Single Metar for Display  Lowest cloud level ~ new_cloud_base[i]
    old_cloud_base[i] = new_cloud_base[i];
    search1 = Single_Metar.indexOf("sky_cover") + 11;        // sky cover ~ Sky[i]
    search2 = Single_Metar.indexOf("_ft_agl", search1) + 9;  // new_cloud_base[i]
    search3 = Single_Metar.indexOf("/>", search2) - 2;       // new_cloud_base[i]
    if (search2 > 9)  new_cloud_base[i] = Single_Metar.substring(search2, search3).toInt();
    if (search1 < 11)  Sky[i] = "NA";  else  Sky[i] = Single_Metar.substring(search1, search1 + 3); {
      if (Sky[i] == "OVX")  new_cloud_base[i] = 0;
      if (Sky[i] == "OVX")  Sky[i] = "OBSECURED";
      if (Sky[i] == "CAV")  Sky[i] = "CLEAR";
      if (Sky[i] == "SKC")  Sky[i] = "CLEAR";
      if (Sky[i] == "CLR")  Sky[i] = "CLEAR";
      if (Sky[i] == "CLEAR")   new_cloud_base[i] = 99999;
    }
    if (Sky[i] == "NA")  {
      new_cloud_base[i] = 0;
      old_cloud_base[i] = 0;
      Serial.printf("%s\tNo:%d\t%s\tSky Cover Not Found, in Decodedata\n", Clock, i, station.c_str());
    }

    // SEARCH <altim_in_hg in Single Metar
    old_Altim[i] = Altim[i];
    search1 = Single_Metar.indexOf("<altim_in_hg>") + 13;
    search2 = Single_Metar.indexOf("</altim_in_hg>");
    if (search1 < 13)  Serial.printf("%s\tNo:%d\t%s\tAltimeter Not Found, in Decodedata\n", Clock, i, station.c_str());
    if (search1 < 13) Altim[i] = 0;  else  Altim[i] = Single_Metar.substring(search1, search2).toFloat();

    // SEARCH <sea_level_pressure_mb in Single Metar
    search1 = Single_Metar.indexOf("<sea_level_pressure_mb>") + 23;
    if (search1 < 23) SeaLpres[i] = 0;
    else  SeaLpres[i] = Single_Metar.substring(search1, search1 + 6).toFloat();


    // ****  Decode WEATHER with Stn_Metar[i] and make readable in Dictionary Function  ****
    int searchZ = Stn_Metar[i].indexOf("Z") + 2;
    search1 = Stn_Metar[i].lastIndexOf(" Q");
    if (search1 < 0 )  search1 = Stn_Metar[i].lastIndexOf(" A");
    search2 = Stn_Metar[i].lastIndexOf("/", search1);  // Search for temp/dewpt
    search3 = Stn_Metar[i].lastIndexOf(" ", search2);  // Search for begining of temp/dewpt
    if (search2 < 0)  search3 = search1;               // No Temp
    if (search3 < 0)  search3 = Stn_Metar[i].length(); // Something went Wrong

    String weather = Stn_Metar[i].substring(searchZ, search3) + " <br>";
    weather.replace("NDV", "");        // Remove
    weather.replace(" SKC", "");       // Remove
    weather.replace(" CLR", "");       // Remove
    weather.replace(" CAVOK", "");     // Remove
    weather = Decode_Weather("Weather", i, station, weather);    // Decode_Weather Routine
    Sig_Weather[i] = weather;          // Return with readable Weather to Sig_Weather


    // ****  Decode REMARK with Remark_text and make readable in Dictionary Function  ****
    weather = Remark_text + " <br>";   // requires the " " and <br>
    weather = Decode_Weather("Remark", i, station, weather);    // Decode_Weather Routine
    Stn_Remark[i] = weather;           // Return with readable Remark to Stn_Remark
  }
  Display_LED(i, 20);                              // Display This Station LED
}


// ****  Decode Precip for Precipition and Icing and Snow  ****
String Decode_Precip(int Search, String Type, String Remark_text) {
  //Serial.printf("Search =%d   Type=.%s.   Remark_text=.%s.  ", Search, Type.c_str(), Remark_text.substring(Search, Search + 6).c_str());
  //Serial.printf("Value =%s.%s\n", Remark_text.substring(Search + 2, Search + 4).c_str(), Remark_text.substring(Search + 4, Search + 6).c_str());

  if (isDigit(Remark_text.charAt(Search + 4)) && isDigit(Remark_text.charAt(Search + 5)))  {  // Test if last two char are digit (minus 1)
    Remark_text = Remark_text.substring(0, Search) + " <br>" + Type
                  + Remark_text.substring(Search + 2, Search + 4) + "." + Remark_text.substring(Search + 4, Search + 6) + " In<br>"
                  + Remark_text.substring(Search + 6, Remark_text.length());

    //Serial.printf("Final Remark_text=.%s.\n\n", Remark_text.c_str());
    Remark_text.replace("00.00 In", "Trace");
  }
  return Remark_text;
}


//  *****   For Stn_Remark[i] & Sig_Weather[i] Codes ~ Make READABLE Codes in this Dictionary Function
String Decode_Weather(String code, int Stn, String station, String weather) {
  weather.replace("BECMG", "<br>becoming : ");         // Rename Later
  weather.replace("TEMPO", "<br>temporary : ");        // Rename Later
  weather.replace("CAVOK", "cavok");                   // Rename Later
  weather.replace("LVP", "Low%visibility%Procedures");
  weather.replace("RTS", "Routes");
  weather.replace("AWOS", "awos");                     // Rename Later
  weather.replace("QFE", "qfe");                       // Rename Later
  weather.replace("SKC ", "%sky");                     // Rename Later
  weather.replace("MIN", "minimum");                   // Rename Later
  weather.replace("OTS", "Out%of%Service");
  weather.replace("FALSE", "False");
  weather.replace("FCST", "Forecast");
  weather.replace("NOSIG", "<br>No%Signifiant%Weather");
  weather.replace("NSW", "<br>No%Signifiant%Weather");
  weather.replace("NSC", "<br>No%Signifiant Clouds");
  weather.replace("NCD", "<br>No Clouds Detected");
  weather.replace("DATA", "Data");
  weather.replace("DA", "<br>Density Alt");
  weather.replace("DENSITY ALT -", "<br>Density Alt = _");
  weather.replace("DENSITY ALT ", "<br>Density Alt = ");
  weather.replace("DEWPT", "Dew Point");
  weather.replace("DP", "Dew Point");
  weather.replace("CONS", "Continuous");
  weather.replace("PDMTLY", "Predominantly");
  weather.replace("PRESENT", "<br>Present");
  weather.replace("TRANSPARENT", "Transparent");
  weather.replace("TORNADO", "Tornado");
  weather.replace("WATERSPOUT", "Waterspout");
  weather.replace("ICE", "Ice");
  weather.replace("STNRY", "%Stationary");
  weather.replace("DRFTNG", "Drifting");
  weather.replace("DRFTG", "Drifting");
  weather.replace("SLWLY", "%Slowly");
  weather.replace("BANK", "bank");              // Rename Later
  weather.replace("OBSCURED", "Obscured");
  weather.replace("OBSC", "Obscured");
  weather.replace("FROPA", "<br>Frontal Passage<br>");
  weather.replace("PRESFR", "<br>Pressure Falling Rapidly<br>");
  weather.replace("PRESRR", "<br>Pressure Rising Rapidly<br>");
  weather.replace("FROIN", "Frost On The Indicator ");
  weather.replace("ALSTG", "Alitmeter%Setting");
  weather.replace("ALTSTG", "Alitmeter%Setting");
  weather.replace("AT AP", "At Airport");
  weather.replace("SLPNO", "<br>Sea Level Pressure%NA");
  weather.replace("PNO", "<br>Rain Gauge%NA ");
  weather.replace("FZRANO", "<br>Freezing Rain%Sensor%NA ");
  weather.replace("TSNO", "<br>Thunderstorm%Sensor%NA ");
  weather.replace("RVRNO", "<br>Runway visibility%Sensor%NA ");    // Rename Later
  weather.replace("CHINO", "<br>Secondary Ceiling Height%Sensor%NA ");
  weather.replace("VISNO", "<br>visibility%Sensor%NA");           // Rename Later
  weather.replace("PWINO", "<br>Peak%Wind%Sensor%NA");
  weather.replace("CEILOMETER", " Ceilometer ");                // CEILOMETER OTS
  weather.replace("PK WND", "Peak wind");                       // See below
  weather.replace("WNDSHFT", "<br>Wind%Shift");
  weather.replace("WSHFT ", "<br>Wind%Shift at ");
  weather.replace("WIND", "%Wind");
  weather.replace("WND", "%Wind");
  weather.replace(" W0", "%W0");
  weather.replace("WS", "%Wind%Shear");
  weather.replace("WX", "%Weather");
  weather.replace("ALF", "Aloft");
  weather.replace("GRADL", "Gradually");
  weather.replace("GRAD", "Gradual");
  weather.replace("LGR", "Longer");
  weather.replace("MISG", "Missing");
  weather.replace("ASOCTD", "Associated");
  weather.replace("CONTRAILS", "Contrails");
  weather.replace("SHELF", "%Shelf");
  weather.replace("DATA", "Data");
  weather.replace("AND", " and ");
  weather.replace("UTC", "utc");                  // Rename Later
  weather.replace("ESTD", "%estimated");          // Rename Later
  weather.replace("ESTMD", "%estimated");         // Rename Later
  weather.replace("PAST", "Past");
  weather.replace("PRSNT", "Present");
  weather.replace("LESS THAN", "Less Than");
  weather.replace("THN", "Thin");
  weather.replace("THRU", "Thru");
  weather.replace(" HR", " Hour");
  weather.replace("ICG", " Icing");
  weather.replace("PCPN", " Precip");
  weather.replace("MTNS", " Mountains");
  weather.replace("MTS", " Mountains");
  weather.replace("ACC", "AC");
  weather.replace("ACFT MSHP", "<br>Aircraft Mishap<br>");
  weather.replace("STFD", "%Staffed");
  weather.replace("FM", "From ");
  weather.replace("FST", " First");
  weather.replace("LAST", " Last");
  weather.replace("OBS", "Observation");
  weather.replace("OB", "Observation");
  weather.replace("NXT", "%Next");
  weather.replace("CLD ", "Clouds");
  weather.replace("CNVTV", "Conv");                 // Rename Later
  weather.replace("CVCTV", "Conv");                 // Rename Later
  weather.replace("FUNNEL CLOUD ", "<br>Funnel Cloud");
  weather.replace("PLUME", "Plume");
  weather.replace("BIRD", "bird");                  // Rename Later
  weather.replace("UNKN", "Unknown");
  weather.replace("HIER", "Higher");
  weather.replace("LWR", "Lower");
  weather.replace("DISSIPATED", "Dissipated ");
  weather.replace("DSIPTD", "Dissipated ");
  weather.replace("DSIPT", "Dissipated ");
  weather.replace("DSPTD", "Dissipated ");
  weather.replace("DSPT", "Dissipated ");
  weather.replace("HVY", "Heavy ");
  weather.replace("LGT", "Light ");
  weather.replace("REDZ", "Recent : Drizzle ");
  weather.replace("RERA", "Recent : Rain ");
  weather.replace("BLU", "blu");                    // See Military Later
  weather.replace("WHT", "wht");                    // See Military Later
  weather.replace("GRN", "grn");                    // See Military Later
  weather.replace("YLO", "ylo");                    // See Military Later
  weather.replace("AMB", "amb");                    // See Military Later
  weather.replace("RED", "redd");                   // See Military Later
  weather.replace("BLACK", "black");                // See Military Later
  weather.replace("BINOVC", " broken in OC");       // Rename Later
  weather.replace("V BKN", " variable broken Clouds");     // Rename Later
  weather.replace("V OVC", " variable OC");                // Rename Later
  weather.replace("V SCT", " variable%Scattered Clouds"); // Rename Later
  weather.replace("BKN", "broken Clouds ");     // Rename Later
  weather.replace("SCT", "%Scattered Clouds ");
  weather.replace("FEW", "Few Clouds ");
  weather.replace("OVC", "OC Clouds ");         // Rename Later
  weather.replace("MDT", "<br>Moderate");
  weather.replace("EMBD", "mbedded");           // Rename Later
  weather.replace("EMBED", " mbedded");         // Rename Later
  weather.replace("OCNL LTG", "OCNLLTG");
  weather.replace("OCNL LT", "OCNLLTG");
  weather.replace("OCNL", "<br>Occasional ");
  weather.replace("FREQ", "FRQ");
  weather.replace("FRQ LTG", "FRQLTG");
  weather.replace("FRQ", "<br>Frequent ");
  weather.replace("IN VICINTY", "VC");
  weather.replace("LTGICCCCA", "LTG LTinC CA");       // Lightning in Cloud and Air
  weather.replace("LTGICCACG", "LTG LTtoG CA");       // Lightning in Cloud Air and Ground
  weather.replace("LTGCGICCC", "LTG LTinC and LTtoG"); // Lightning in Cloud and Ground
  weather.replace("LTGICCCCG", "LTG LTinC and LTtoG"); // Lightning in Cloud and Ground
  weather.replace("LTGICIC", "LTG LTinC");            // Lightning in Cloud
  weather.replace("LTGICCC", "LTG LTinC");            // Lightning in Cloud
  weather.replace("LTGCGIC", "LTG LTinC and LTtoG");  // Lightning in Cloud and Ground
  weather.replace("LTGICCG", "LTG LTinC and LTtoG");  // Lightning in Cloud and Ground
  weather.replace("LTGCCCG", "LTG LTinC and LTtoG");  // Lightning in Cloud and Ground
  weather.replace("LGTICCG", "LTG LTinC and LTtoG");  // Lightning in Cloud and Ground
  weather.replace("LTICGCG", "LTG LTinC and LTtoG");  // Lightning in Cloud and Ground
  weather.replace("LTGICCA", "LTG LTinC CA");         // Lightning in Cloud and Air
  weather.replace("LTGCG", "LTG LTtoG");              // Lightning in Ground
  weather.replace("LTCIC", "LTG LTinC");              // Lightning in Cloud
  weather.replace("LTGCC", "LTG LTinC");              // Lightning in Cloud
  weather.replace("LTGIC", "LTG LTinC");              // Lightning in Cloud
  weather.replace("LTNG", "LTG");
  weather.replace("LTG", "Lightning");
  weather.replace("CG", " and LTtoG");                // Lightning to Ground (fix)
  weather.replace("LTinC", "in Clouds");
  weather.replace("LTtoG", "to Ground");
  weather.replace("CA", " and Cloud to Air<br>");
  weather.replace("ALQDS", "All Quadrents ");
  weather.replace("ALQS", "All Quadrents ");
  weather.replace("DIST", "Distant ");
  weather.replace("DSTN", "Distant ");
  weather.replace("DSNT", "Distant");
  weather.replace("DST", "Distant ");
  weather.replace("MOVD", "Moved ");
  weather.replace("MOVG", "Moving ");
  weather.replace("STNRY", "%Stationary ");
  weather.replace("MOV", "<br>Movement ");
  weather.replace("FZDZB", "<br>Freezing Drizzle%began"); // Rename Later
  weather.replace("DZB", "<br>Drizzle%began");         // Rename Later
  weather.replace("FZRAB", "<br>Freezing Rain%began"); // Rename Later
  weather.replace("RAB", "<br>Rain%began");            // Rename Later
  weather.replace("SNB", "<br>Snow%began");            // Rename Later
  weather.replace("PLB", "<br>Ice Pellets%began");     // Rename Later
  weather.replace("UPB", "<br>Unknown Precip%began");  // Rename Later
  weather.replace("TSB", "<br>Thunderstorm%began");    // Rename Later
  weather.replace("-VC", "VC-");
  weather.replace("+VC", "VC+");
  weather.replace("VCFG", "Fog in the vicinity<br>");
  weather.replace("VCSH", "Rain%Showers in the vicinity<br>");
  weather.replace("VCTS", "Thunderstorm in the vicinity<br>");
  weather.replace(" VC", "In the vicinity ");       // Rename Later
  weather.replace("BCFG", "<br>Patches of Fog");
  weather.replace("PRFG", "<br>Partial Fog");
  weather.replace("SHRASN", "Rain and%Snow%Showers");
  weather.replace("SHRA", "Rain%Showers");
  weather.replace("SHSN", "%Snow%Showers");
  weather.replace("SHGS", "%Small Hail /%Snow Pellets%Showers");
  weather.replace("RASN", "Rain and%Snow");
  weather.replace("TSBR", "Thunderstorm and Mist");
  weather.replace("TSRA", "Rain and Thunderstorm ");
  weather.replace("TS", "Thunderstorm");
  weather.replace(" BC", " Patches of ");
  weather.replace(" BL", " blowing ");                // Rename Later
  weather.replace(" DR", " Drifting ");
  weather.replace("FZ", "%Freezing ");
  weather.replace(" MI", "%Shallow ");
  weather.replace(" PTCHY", " Patchy ");
  weather.replace(" MOD", " Moderate ");
  weather.replace(" RAG", "Ragged ");
  weather.replace("CIG", "<br>Ceiling");
  weather.replace("SNINCR", "%Snow Increasing Rapidily");
  weather.replace("TWRINC", "<br>Tower visibility Increasing"); // Rename Later
  weather.replace("SFC VIS", "<br>Surface VIS");
  weather.replace("SFC", "%Surface");
  weather.replace("TWR VIS", "<br>Tower VIS");
  weather.replace("TWR", "<br>Tower VIS");
  weather.replace("VV", "<br>vertical visibility");        // Rename Later
  weather.replace("VISBL", "visabile");                // Rename Later
  weather.replace("VIS M", "visibility Measured ");     // Rename Later
  weather.replace("VIS", "visibility");                // Rename Later
  weather.replace("INC", "Increasing");
  weather.replace("DEC", "Decreasing");
  weather.replace("DZ", "%Drizzle%");
  weather.replace("RA", "%Rain%");
  weather.replace("SN", "%Snow%");
  weather.replace("SG", "%Snow Grains%");
  weather.replace("BR", "Mist ");
  weather.replace("IC", "%Ice Cristals%");
  weather.replace("PL", "%Ice Pellets%");
  weather.replace("GS", "%Small Hail /%Snow Pellets%");
  weather.replace("GR", "%Large Hail%");
  weather.replace("FG", "Fog ");
  weather.replace("FU", "%Smoke ");
  weather.replace("HZY", "Hazy ");
  weather.replace("HZ", "Haze ");
  weather.replace("DS", "Dust%Storm ");
  weather.replace("DU", "Dust ");
  weather.replace("FC", "Funnel Cloud ");
  weather.replace("UP", "Unknown Precip%");
  weather.replace("SA", "%Sand ");
  weather.replace("SH", "%Showers ");
  weather.replace("TR", "%Trace");
  weather.replace("RE", "<br>recent : ");         // Rename Later
  weather.replace("RF", "Rainfall ");
  weather.replace("RMK ", "<br>remark : ");       // Rename Later
  weather.replace(" CBMAM", " Mammatus CB");
  weather.replace(" TCU", " Towering Cumulus Clouds");
  weather.replace(" CU", " Cumulus Clouds");
  weather.replace(" CB", " Cumulonimbus Clouds");
  weather.replace(" CU", " Cumulus Clouds");
  weather.replace("AC", "<br>Altocumulus Clouds");
  weather.replace("ACSL", "<br>Standing Lenticular Altocumulus Clouds");
  weather.replace("AS", "<br>Altostratus Clouds");
  weather.replace("CC", "<br>Cirrocumulus Clouds");
  weather.replace("CF", "<br>Cumulusfractus Clouds");
  weather.replace("CI", "<br>Cirrus Clouds");
  weather.replace("CS", "<br>Cirostratus Clouds");
  weather.replace("NS", "<br>Nimbostratus Clouds");
  weather.replace("SC", "<br>Stratocumulus Clouds");
  weather.replace("SF", "<br>Stratus Fractus Clouds");
  weather.replace("ST", "<br>Stratus Clouds");
  weather.replace("CB", "<br>? Cumulonimbus Clouds");
  weather.replace("TCU", "<br>? Towering Cumulus Clouds");
  weather.replace("CU", "<br>? Cumulus Clouds");
  weather.replace(" NW", "%North_West");          // Rename Later
  weather.replace(" NE", "%North_east");          // Rename Later
  weather.replace(" SE", "%South_east");          // Rename Later
  weather.replace(" SW", "%South_West");          // Rename Later
  weather.replace(" N", " North");
  weather.replace(" S", " South");
  weather.replace(" W", " West");
  weather.replace(" E", " east");                 // Rename Later
  weather.replace("-NW", " to North_West");       // Rename Later
  weather.replace("-NE", " to North_east");       // Rename Later
  weather.replace("-SE", " to South_east");       // Rename Later
  weather.replace("-SW", " to South_West");       // Rename Later
  weather.replace("-N", " to North");             // Rename Later
  weather.replace("-S", " to South");             // Rename Later
  weather.replace("-W", " to West");              // Rename Later
  weather.replace("-E", " to east");              // Rename Later
  weather.replace(" - ", " to ");
  weather.replace(" -", " Light ");
  weather.replace(" +", " Heavy ");
  weather.replace("OHD", "overhead");               // Rename Later
  weather.replace("OVD", "overhead");               // Rename Later
  weather.replace("VRB", "variable");               // Rename Later
  weather.replace("VRY", "very");                   // Rename Later
  weather.replace("VERY", "very");                  // Rename Later
  weather.replace("VSBY", "visibility");            // Rename Later
  weather.replace("VSB", "visibile");               // Rename Later
  weather.replace("VIRGA", " virga ");              // Rename Later
  weather.replace("V", "variable");                 // Rename Later
  if (code == "Remark")  {
    weather.replace("B", "%began");                     // Rename Later
    weather.replace("E", "%ended");                     // Rename Later
    //weather.replace("Thunderstorm", "<br>Thunderstorm");
    weather.replace("Clouds1", "Clouds = 1 Oktas<br>");
    weather.replace("Clouds2", "Clouds = 2 Oktas<br>");
    weather.replace("Clouds3", "Clouds = 3 Oktas<br>");
    weather.replace("Clouds4", "Clouds = 4 Oktas<br>");
    weather.replace("Clouds5", "Clouds = 5 Oktas<br>");
    weather.replace("Clouds6", "Clouds = 6 Oktas<br>");
    weather.replace("Clouds7", "Clouds = 7 Oktas<br>");
    weather.replace("Clouds8", "Clouds = 8 Oktas<br>");
    weather.replace("%Trace", "Trace<br>");
    weather.replace("blu blu", "blu");                  // Military
    weather.replace("wht wht", "wht");                  // Military
    weather.replace("grn grn", "grn");                  // Military
    weather.replace("ylo ylo", "ylo");                  // Military
    weather.replace("amb amb", "amb");                  // Military
    weather.replace("redd redd", "redd");               // Military
    weather.replace("blu", "<br>BLUE: Cloud Base > 2500ft Visibility > 8km<br>");
    weather.replace("wht", "<br>WHITE: Cloud Base > 1500ft Visibility > 5km<br>");
    weather.replace("grn", "<br>GREEN: Cloud Base > 700ft Visibility > 3.7km<br>");
    weather.replace("ylo1", "<br>YELLOW: Cloud Base > 500ft Visibility > 2500m<br>");
    weather.replace("ylo2", "<br>YELLOW: Cloud Base > 300ft Visibility > 1600m<br>");
    weather.replace("ylo", "<br>YELLOW: Cloud Base > 300ft Visibility > 1600m<br>");
    weather.replace("amb", "<br>AMBER: Cloud Base > 200ft Visibility > 800m<br>");
    weather.replace("redd", "<br>RED: Cloud Base < 200ft Visibility < 800m<br>");
    weather.replace("black", "<br>BLACK: Runway is Unusable<br>");
  }

  weather.replace("broken", "Broken");                // Rename
  weather.replace("OC", "Overcast");                  // Rename
  weather.replace("Conv", "Convective");              // Rename
  weather.replace("overhead", "Overhead");            // Rename
  weather.replace("east", "East");                    // Rename
  weather.replace("_", " - ");                        // Rename
  weather.replace("%", " ");                          // Rename
  weather.replace("very", "Very");                    // Rename
  weather.replace("vi", "Vi");                        // Rename
  weather.replace("bank", "Bank");                    // Rename
  weather.replace("blowing", "Blowing");              // Rename
  weather.replace("began", "Began ");                 // Rename
  weather.replace("ended", "Ended ");                 // Rename
  weather.replace("estimated", "Estimated");          // Rename
  weather.replace("mbedded", " Embedded");            // Rename
  weather.replace("awos", "AWOS");                    // Rename
  weather.replace("cavok", "CAVOK");                  // Rename
  weather.replace("utc", "UTC ");                     // Rename
  weather.replace("qfe", "QFE ");                     // Rename
  weather.replace("sky", "Sky");                      // Rename
  weather.replace("minimum", "MIN");                  // Rename
  weather.replace("bird", "BIRD");                    // Rename


  // ******  SEARCH in Weather or Remark String  ******
  // SEARCH for Wind & Variable Runway Vis & Clouds in weather
  int Q = Stn_Metar[Stn].indexOf(" Q");
  int KT = weather.indexOf("KT");
  int SM = weather.indexOf("SM");
  int Gusts = weather.indexOf("G");
  int Var = weather.indexOf("0v");    // Variable
  int last = KT + 2;                  // Delete after KT or SM
  if  (Var > KT)   last = Var + 12;   // Delete after Wind Variable
  if  (SM >= KT)   last = SM + 2;     // No Wind Available
  if  (last < 0)   last = weather.indexOf(" ");   // Something went Wrong

  String gusts = "<br>Wind Gusting to " + weather.substring(Gusts + 1, Gusts + 3) + " Kts<br>";
  String varbl = "<br>Wind Variable ~ " + weather.substring(Var - 2, Var + 1) + " Deg to "  + weather.substring(Var + 9, Var + 12) + " Deg<br>";
  String text;

  if (code == "Weather")  {    //  ADD Gusts & Variable Wind in weather
    //Serial.printf("Stn:%d  Initial Code =\t.%s.\nPointers:\t\t\tKT=%d  SM=%d   Gusts=%d  Var=%d  last=%d\n", Stn, weather.c_str(), KT, SM, Gusts, Var, last);

    if (Gusts > 0 || Var > 0 )   {
      if (KT - Gusts == 3)        text = gusts;         // Gusts
      if (Var > 0 && Var < last)  text = text + varbl;  // Variable Wind
    }
    if  (Q > 0)  {             //  Non US Station
      if (KT > 0)  {
        int E = weather.indexOf(" ", KT + 4);   // end of code
        if (weather.charAt(KT + 6) == 'v')  E = weather.indexOf(" ", KT + 20);   // test if "v"ariable
        if (weather.charAt(E - 1) == '0' )        // Test if last char "0"
          text += "<br>Visibility ~" + weather.substring(E - 5, E) + " Meters<br>";
        weather = text + weather.substring(E, weather.length());
      }
    } else {
      weather = text + weather.substring(last, weather.length());
    }
  }
  // For Troubleshooting or Print Updated Stations:
  if (code == "Weather")  Serial.printf("WEATHER No:%d\t%s\t.%s.\nWorking Code\t\t.%s.\n", Stn, station.c_str(), Stn_Metar[Stn].c_str(), weather.c_str());
  if (code == "Remark")   Serial.printf("REMARK  No:%d\t%s\t.%s.\nWorking Code\t\t.%s.\n", Stn, station.c_str(), Stn_Remark[Stn].c_str(), weather.c_str());

  // SEARCH for Runway Vis in Weather and Remarks
  int search1 = weather.indexOf(" R");    // Find Runway
  while (search1 >= 0)  {
    int search2 = weather.indexOf("/", search1);      // End of Runway No
    int search3 = weather.indexOf("0v", search1);     // Variable Vis
    int search4 = weather.indexOf("FT", search1);     // US Airport Vis
    int search5 = weather.indexOf(" ", search2) - 1;  // end of code
    if (search2 < 0 && search3 < 0) break;  // Not Runway
    if (search2 - search1 < 6)  {

      //Serial.printf("\nStn:%d Runway search :\t.%s.\n", Stn, weather.substring(search1, search5 + 1).c_str());
      //Serial.printf("Stn:%d Runway search : \tsearch1 =%d  search2 =%d  search3 =%d  search4 =%d  search5 =%d\n", Stn, search1, search2, search3, search4, search5);

      text = "<br>Runway " + weather.substring(search1 + 2, search2) +  " Visibility ~ ";

      if (search3 < 0 && search4 > 0)  text += weather.substring(search2 + 1, search4) + " Ft"; // US Runway Visibility
      if (search3 > 0 && search4 > 0)  text += weather.substring(search2 + 1, search3 + 1)  // US Runway Variable Visibility
            + " Ft Variable to " + weather.substring(search3 + 9, search4) + " Ft";
      //Serial.printf("Stn:%d Runway text =\t.%s.\n", Stn, text.c_str());

      text.replace("M", "Lower than ");
      text.replace("P", "Greater than ");
      /*
        text.replace("/D", " (Decreasing)");
        text.replace("/N", " (No Change)");
        text.replace("/U", " (Increasing)");
      */
      if (Q > 0)  {     // For Non US Runway
        text += weather.substring(search2 + 1, search5) + " Meters";
        if (weather.charAt(search5) == 'D')  text += " (Decreasing)";
        if (weather.charAt(search5) == 'N')  text += " (No Change)";
        if (weather.charAt(search5) == 'U')  text += " (Increasing)";
      }
      //Serial.printf("Stn:%d Runway text =\t.%s.\n", Stn, text.c_str());
      weather.replace(weather.substring(search1, search5 + 1), text + "<br>");
    }
    search1 = weather.indexOf(" R", search5);  // Look for Next Runway
  }

  //  SEARCH for Variable Ceiling in Weather and Remarks  "CIG"
  int search0 = weather.indexOf("Ceiling");
  if (search0 > 0) {
    search1 = weather.indexOf("variable", search0);
    int search2 = weather.indexOf(" ", search1 + 8); // end of code
    int search3;
    if (search1 > 0) {  //  SEARCH for Variable Ceiling  "CIG VRB 6 - 10"
      search2 = weather.indexOf(" to ", search1);  // searching for "CIG VRB 6 to 10"
      if (search2 > 0) {
        search3 = weather.indexOf(" ", search2 + 5);  // end of code
        text = weather.substring(search1, search1 + 16) + weather.substring(search2 - 2, search2);
        //Serial.printf("Stn:%d Ceiling text 0 = .%s.\n", Stn, text.c_str());
      }
      // Searching for "CIG 004variable009"
      search2 = search1 + 11;
      if (search2 - search1 == 11) { //  SEARCH for "CIG 004variable009"
        text = weather.substring(search0, search2);
        text = Decode_Alt(text);   // Decode Alt Routine "CIG 004variable009"
        //Serial.printf("Stn:%d Ceiling text 1 = .%s.\n", Stn, text.c_str());
        weather.replace(weather.substring(search0, search2), text);
      }
    }
    if (search1 < 0) { //  SEARCH for "CIG 004"
      text = Decode_Alt("Clouds " + weather.substring(search0 + 8, search0 + 11));   // Decode Alt Routine  "Ceiling 002"
      text.replace("Clouds", "Ceiling");
      text.replace("///", " NA<br>");
      //Serial.printf("Stn:%d Ceiling text 2=.%s.\n", Stn, text.c_str());
      weather.replace(weather.substring(search0, search0 + 11), text);
    }
  }

  //  SEARCH for Visibility in Weather and Remarks  "VIS M" or "VIS 2 1/4V4 1/2" or "VIS VRB"
  search0 = weather.indexOf("Visibility");      //  SEARCH for Visibility
  if (search0 > 0) {
    search1 = weather.indexOf(" ", search0);    //  end of Visibility code
    int search2 = weather.indexOf("variable");  //  SEARCH for Variable in  "VIS 2 1/4V4 1/2" or "VIS VRB 1-2"
    if (search2 > 0) {  //  SEARCH for Variable
      int search3;      //  end of Variable code
      if (isDigit(weather.charAt(search2 + 10)))  search3 = weather.indexOf(" ", search2 + 10);
      else search3 = weather.indexOf(" ", search2);
      text = "<br>Visibility ~ " + weather.substring(search1, search2)
             +  " Variable to " + weather.substring(search2 + 8, search3) + " Miles<br>";
      if (search2 - search1 > 1 ) {
        weather.replace(weather.substring(search0, search3), text);
      } else {
        text = "<br>Visibility ~  Variable ";
        weather.replace(weather.substring(search0, search3), text);
      }
      //Serial.printf("Vis Text =.%s.\n", text.c_str());
    }
  }

  //  SEARCH for vertical Visibility in Weather and Remarks  "VV"
  search0 = weather.indexOf("vertical Visibility");    //  SEARCH for vertical Visibility "VV0001"
  if (search0 > 0) {
    search1 = search0 + 19;
    int search2 = weather.indexOf(" ", search0 + 10); //  end of code
    text = "<br>Vertical Clouds " + weather.substring(search1, search2);
    text = Decode_Alt(text);       // Decode Alt Routine "Broken Clouds 026"
    text.replace("Clouds", "Visibility");
    text.replace("///", " NA<br>");
    weather.replace(weather.substring(search0, search2), text);
  }

  //  SEARCH for CLOUDS in weather and Remarks
  weather.replace("///", "");          // Delete extra "///" in clouds
  if (code == "Weather")  Sky_cover[Stn] = "";
  int CLDS = weather.indexOf("Clouds");
  if (CLDS > 0)  {          // Found Clouds
    String clouds;
    while (CLDS > 0)  {
      int End = CLDS + 10;  // End of code index
      int Cld_Type = 0;     // Cloud type index
      if (weather.substring(CLDS - 4, CLDS - 1)  == "Few")        Cld_Type = weather.indexOf("Few", CLDS - 4);
      if (weather.substring(CLDS - 7, CLDS - 1)  == "Broken")     Cld_Type = weather.indexOf("Broken", CLDS - 7);
      if (weather.substring(CLDS - 10, CLDS - 1) == "Scattered")  Cld_Type = weather.indexOf("Scattered", CLDS - 10);
      if (weather.substring(CLDS - 9, CLDS - 1)  == "Overcast")   Cld_Type = weather.indexOf("Overcast", CLDS - 9);

      if (Cld_Type > 0)  {
        int Cu = weather.indexOf("?");
        String Clouds_for_Alt = weather.substring(Cld_Type, End);
        if (Cu > 0 && Cu < End + 12)  {
          int clds = weather.indexOf("Clouds", Cu) + 6;
          text = weather.substring(Cu + 1, clds);
          weather.replace(weather.substring(Cu - 4, clds), "");
          Clouds_for_Alt.replace("Clouds", text);
        }
        clouds = Decode_Alt(Clouds_for_Alt) + "<br>";    // Decode Alt Routine
      }
      if (code == "Weather")  {
        Sky_cover[Stn] += clouds;
        if (Sky_cover[Stn].length() > 150)  Serial.printf("%s\tNo:%d\t%s\tError in Clouds ~ Weather: Sky_cover > 150 = %d\n", Clock, Stn, station.c_str(), Sky_cover[Stn].length());
        if (Sky_cover[Stn].length() > 150)  break;
        if (clouds.indexOf("Ft") < 0)  Serial.printf("%s\tNo:%d\t%s\tError in Clouds ~ Weather: No Alt\n", Clock, Stn, station.c_str());
        if (clouds.indexOf("Ft") < 0)  break;
        weather.replace(weather.substring(Cld_Type - 1, End), "");
        CLDS = weather.indexOf("Clouds");
      }
      if (code == "Remark")   {
        clouds = "<br>" + clouds;
        if (Cld_Type < 1)  Serial.printf("%s\tNo:%d\t%s\tError in Clouds ~ Remark: No Clouds\n", Clock, Stn, station.c_str());
        if (Cld_Type < 1)  break;
        if (clouds.indexOf("Ft") < 1)  Serial.printf("%s\tNo:%d\t%s\tError in Clouds ~ Remark: No Alt\n", Clock, Stn, station.c_str());
        if (clouds.indexOf("Ft") > 1)  {
          weather.replace(weather.substring(Cld_Type, End), clouds);
        }
        int br = weather.indexOf("<br>", CLDS) + 5;
        CLDS = weather.indexOf("Clouds", br);  // CLDS = index of "Clouds"
      }
    }
  }

  if (code == "Remark")  {          //  FOR REMARKS ONLY
    //  SEARCH for Gusts
    KT = weather.indexOf("KT");
    Gusts = weather.indexOf("G");
    if (KT - Gusts == 3)  weather.replace(weather.substring(Gusts - 5, Gusts + 5), gusts);  // Replace Gusts

    //  SEARCH for Peak Wind in Remarks
    search1 = weather.indexOf("Peak wind");
    if (search1 >= 0)  {
      int search2 = weather.indexOf("/", search1);
      String Dir = "<br>Peak Wind from " + weather.substring(search2 - 5, search2 - 2) + " Deg ";
      String Wind = weather.substring(search2 - 2, search2) + " KT ";
      int search3 = weather.indexOf(" ", search2);  // end of code
      String At = "at time " +  weather.substring(search2 + 1, search3) + "<br>";
      text = Dir + Wind + At;
      weather.replace(weather.substring(search1, search3), text);
    }
    // SEARCH for Visibility in Meters (Non US Station)
    int Tempo = weather.indexOf("temporary") + 16;
    int Becmg = weather.indexOf("becoming") + 15;
    if (Tempo > 16 && weather.charAt(Tempo) == '0')          // Test if last char "0"
      weather.replace(weather.substring(Tempo - 3, Tempo + 1), "<br>Visibility ~ " + weather.substring(Tempo - 3, Tempo + 1) + " Meters<br>");
    if (Becmg > 15 && weather.charAt(Becmg) == '0')          // Test if last char "0"
      weather.replace(weather.substring(Becmg - 3, Becmg + 1), "<br>Visibility ~ " + weather.substring(Becmg - 3, Becmg + 1) + " Meters<br>");

    //weather.replace("99999", "CLEAR");

  }

  // ***  OPTIONAL  ***  To print UPDATED Stations or For Troubleshooting
  //Serial.printf("Weather Code\t\t.%s.\n", weather.c_str());

  weather.replace("becoming", "BECOMING");       // Rename
  weather.replace("recent", "RECENT");           // Rename
  weather.replace("remark", "REMARK");           // Rename
  weather.replace("temporary", "TEMPORARY");     // Rename
  weather.replace("var", "Var");                 // Rename
  weather.replace("//", "");                     // Clean Up
  weather.replace("$", "");                      // Clean Up ~ Station needs Maintenance
  weather.replace("?", "");                      // Clean Up ~ in Clouds or Unknown
  weather.replace("  ", " ");                    // Clean Up
  weather.replace("<br> <br>", "<br>");          // Clean Up
  weather.replace("<br> ", "<br>");              // Clean Up
  weather.replace(" <br>", "<br>");              // Clean Up
  weather.replace("<br><br>", "<br>");           // Clean Up

  //Serial.printf("CLEAN UP weather Code\t.%s.\n", weather.c_str());

  // MORE CLEAN UP
  if (weather.substring(0, 1) == " ")
    weather = weather.substring(1, weather.length());  // Remove First "space"

  if (weather.substring(0, 4) == "<br>")
    weather = weather.substring(4, weather.length());  // Remove First "<br>"
  search1 = weather.lastIndexOf("<br>");

  if (search1 > 0)   weather = weather.substring(0, search1);     // Remove last "<br>"
  if (weather == "" || weather == " ")  weather = "None";         // Clean Up

  // For Troubleshooting :
  Serial.printf("FINAL weather Code\t.%s.\n\n", weather.c_str());
  return weather;                  // Return with Readable Weather
}


// ***********  Decode Alt for Clouds or Variable Ceiling
String Decode_Alt(String clouds) {
  int Lo_val = 0;
  int Hi_val = 0;
  char Ceiling[6];
  int search0 = clouds.indexOf("Clouds") + 7;    // Search for "Clouds" or "VV" = "Clouds 026" or "CIG" = "Clouds 026"
  if (search0 > 6)  {
    strcpy(Ceiling, clouds.substring(search0, search0 + 3).c_str());
    for (int i = 0; i < 3; i++) {
      Lo_val = Lo_val * 10 + (Ceiling[i] - '0');
    }
    if (Lo_val < 0 || Lo_val > 999 )   return clouds;   // Check for Errors
    clouds.replace(clouds.substring(search0, search0 + 3), ("at " + String(Lo_val * 100) + " Ft"));
  }
  search0 = clouds.indexOf("variable");    // Search for "VIS Var" = Ceiling 007variable013
  if (search0 > 0)  {
    strcpy(Ceiling, clouds.substring(search0 - 3, search0).c_str());
    for (int i = 0; i < 3; i++) {
      Lo_val = Lo_val * 10 + (Ceiling[i] - '0');
    }
    if (Lo_val < 0 || Lo_val > 999 )   return clouds;   // Check for Errors
    strcpy(Ceiling, clouds.substring(search0 + 8, search0 + 11).c_str());
    for (int i = 0; i < 3; i++) {
      Hi_val = Hi_val * 10 + (Ceiling[i] - '0');
    }
    if (Hi_val < 0 || Hi_val > 999 )   return clouds;   // Check for Errors
    clouds = "<br>Ceiling Variable from " + String(Lo_val * 100) + " to " + String(Hi_val * 100) + " Ft<br>";
  }
  //Serial.printf("In Decode_Alt: Return Code = .%s.\n", clouds.c_str());
  return clouds;
}


void Display_LED(int index, int wait) {    // *********** Display One Station LED
  if (index == 0)  return;
  leds[index - 1] = 0xaf5800;   // Orange Decoding Data
  FastLED.show();
  delay(wait);
  Set_Cat_LED(index);           // Set Category for This Station LED
  FastLED.show();
}


void Display_Weather_LEDS (int wait) {    // ***********  Display Weather on LEDS
  for (int index = 1; index < (No_Stations + 1); index++)  {
    if (Sig_Weather[index] != "None")   { // IF NOT "None" in Sig Wx,  Display Weather
      Stn_Flash = index;                  // Station # for Server - flash button
      //  For Twinkle Routine    ~    Flashing Weather    (index,  red,green,blue, pulses, on, off time)
      if (Sig_Weather[index].indexOf("Gust") > 0)  Twinkle(index, 0x00, 0x70, 0x70, 2, 400, 300); //Gusts   Aqua
      if (Stn_Metar[index].indexOf("TS") > 0)      Twinkle(index, 0xff, 0xff, 0xff, 4,   5, 900); //Thunder White
      if (Stn_Metar[index].indexOf("FZ") > 0)      Twinkle(index, 0x00, 0x00, 0x70, 3, 300, 400); //Freezing Blue
      if (Sig_Weather[index].indexOf("Rain") > 0)  Twinkle(index, 0x00, 0x70, 0x00, 4, 300, 500); //Rain    Green
      if (Stn_Metar[index].indexOf("SN") > 0)      Twinkle(index, 0x70, 0x70, 0x70, 4, 300, 600); //Snow    White
      if (Stn_Metar[index].indexOf("SG") > 0)      Twinkle(index, 0x50, 0x50, 0x50, 4, 300, 600); //Snow  Less White
      if (Stn_Metar[index].indexOf("DZ") > 0)      Twinkle(index, 0x22, 0xfc, 0x00, 2, 200, 300); //Drizzle Green
      if (Stn_Metar[index].indexOf("BR") > 0)      Twinkle(index, 0x20, 0x6d, 0x00, 3, 500, 500); //Mist  L Green
      if (Stn_Metar[index].indexOf("FG") > 0)      Twinkle(index, 0x2e, 0x40, 0x40, 3, 400, 500); //Fog   Dark White  2e4040 Using ledpicker
      if (Stn_Metar[index].indexOf("GS") > 0)      Twinkle(index, 0x48, 0x48, 0x00, 4, 100, 800); //S Hail  Yellow
      if (Stn_Metar[index].indexOf("GR") > 0)      Twinkle(index, 0x68, 0x68, 0x00, 5, 100, 800); //L Hail  Yellow
      if (Stn_Metar[index].indexOf("IC") > 0)      Twinkle(index, 0x00, 0x00, 0x80, 3, 300, 400); //Ice C   Blue
      if (Stn_Metar[index].indexOf("PL") > 0)      Twinkle(index, 0x00, 0x00, 0xa0, 3, 300, 400); //Ice P   Blue
      if (Stn_Metar[index].indexOf("HZ") > 0)      Twinkle(index, 0x20, 0x20, 0x20, 3, 400, 400); //Haze    Purple
      if (Stn_Metar[index].indexOf("FU") > 0)      Twinkle(index, 0x2a, 0x32, 0x00, 3, 500, 500); //Smoke   Light Yellow 2a3200 Using ledpicker
      //  Rest of Weather = YELLOW
      if (Stn_Metar[index].indexOf("DU") > 0)    Twinkle(index, 0x88, 0x88, 0x00, 3, 500, 500); //Dust    Yellow
      if (Stn_Metar[index].indexOf("FY") > 0)    Twinkle(index, 0x88, 0x88, 0x00, 3, 500, 500); //Spray   Yellow
      if (Stn_Metar[index].indexOf("SA") > 0)    Twinkle(index, 0x88, 0x88, 0x00, 3, 500, 500); //Sand    Yellow
      if (Stn_Metar[index].indexOf("PO") > 0)    Twinkle(index, 0x88, 0x88, 0x00, 3, 500, 500); //Dust&Sand Yellow
      if (Stn_Metar[index].indexOf("SQ") > 0)    Twinkle(index, 0x88, 0x88, 0x00, 3, 500, 500); //Squalls Yellow
      if (Stn_Metar[index].indexOf("VA") > 0)    Twinkle(index, 0x88, 0x88, 0x00, 3, 500, 500); //Volcanic Ash Yellow
      if (Stn_Metar[index].indexOf("UP") > 0)    Twinkle(index, 0x88, 0x88, 0x00, 3, 500, 500); //Unknown Yellow
    }
    Set_Cat_LED (index);                    // Set Category for This Station LED
    FastLED.show();
    delay(100);    // Wait a smidgen
  }
  delay(wait);
}


// ***********  Twinkle Routine for Flashing Weather
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


void Display_Cat_LEDS () {    // *********** Display ALL Categories on LEDS Routine
  for (int i = 0; i < (No_Stations + 1); i++) {
    Set_Cat_LED (i);  // Set Category for This Station LED
  }
  FastLED.show();
  delay(200);        // Wait a smidgen
}


void Set_Cat_LED (int i)  {    // *********** Set Category for One Station LED Routine
  if (Category[i] == "")     Category[i] = "NF";
  if (Category[i] == "VFR")  leds[i - 1] = CRGB::DarkGreen;
  if (Category[i] == "MVFR") leds[i - 1] = CRGB::DarkBlue;
  if (Category[i] == "IFR")  leds[i - 1] = CRGB::DarkRed;
  if (Category[i] == "LIFR") leds[i - 1] = CRGB::DarkMagenta;
  if (Category[i] == "NA")   leds[i - 1] = CRGB(20, 30, 0); // Very Dim Yellow
  if (Category[i] == "NF")   leds[i - 1] = CRGB(20, 30, 0); // Very Dim Yellow
  if (Stations[i].substring(0, 4) == "NULL")   leds[i - 1] = CRGB::Black;
}


void Display_Vis_LEDS (int wait) {    // *********** Display Visibility [Red White] on LEDS Gradient Routine
  for (int i = 1; i < (No_Stations + 1); i++) {
    byte hue     = 14  + Visab[i].toInt() * 4;       // (red white) [ 14 ~ 46 ]
    byte sat     = 128 - Visab[i].toInt() * 16;      // (red white) [128 ~ 0 ]
    byte bright  = 120;                              // [ <100=Dim  120=ok  >160=Too Bright/Much Power ]
    leds[i - 1] = CHSV(hue, sat, bright);            // ( hue, sat, bright )
    if (Visab[i].toInt() > 8)  leds[i - 1] = CHSV( 48, 0, bright);         // White (Vis > 8)
    if (Visab[i].indexOf("+") >= 0)  leds[i - 1] = CHSV( 48, 0, bright);   // White
    if (Category[i] == "NULL" || Category[i] == "NF" || Visab[i] == "NA")  leds[i - 1] = CHSV( 0, 0, 0);
  }
  FastLED.show();
  delay(wait);
}


void Display_Wind_LEDS (int wait) {    // *********** Display Winds [Aqua] on LEDS Gradient Routine
  for (byte i = 1; i < (No_Stations + 1); i++) {
    byte wind = Wind[i].toInt();
    leds[i - 1] = CRGB(0, wind * 7, wind * 7);
    if (Category[i] == "NULL" || Category[i] == "NF" || wind == 0)  leds[i - 1] = CHSV( 0, 0, 0);
  }
  FastLED.show();
  delay(wait);
}


// *********** Display Temperatures [Purple Blue Green Yellow Orange Red] on LEDS Gradient Routine
void Display_Temp_LEDS (int wait) {
  for (byte i = 1; i < (No_Stations + 1); i++) {
    int hue = 160 - TempC[i] * 4;           //  purple blue green yellow orange red [160 ~ 0 ]
    if (hue > 200)  hue = 200;              //  purple < -10C
    //if (TempC[i] < 0) Serial.printf(" Station =%d\tTemp =%.1f  \thue =%d\n", i, TempC[i], hue);
    leds[i - 1] = CHSV( hue, 180, 150);     // ( hue, sat, bright )
    if (Category[i] == "NULL" || Category[i] == "NF" || TempC[i] == 99) leds[i - 1] = CHSV( 0, 0, 0);
  }
  FastLED.show();
  delay(wait);
}


// *********** Display Altimeter Pressure [Blue Purple] on LEDS Gradient Routine
void Display_Alt_LEDS (int wait) {
  for (byte i = 1; i < (No_Stations + 1); i++) {
    byte hue = (Altim[i] - 29.92) * 100;        //  (normally) blue purple [ 70 ~ 270 ]
    leds[i - 1] = CHSV( hue + 170, 180, 150);   // ( hue, sat, bright )
    if (Category[i] == "NULL" || Category[i] == "NF" || Altim[i] == 0)   leds[i - 1] = CHSV( 0, 0, 0);
  }
  FastLED.show();
  delay(wait);
}


void Check_Coms()  {        // Checking Communication Flag
  if (Comms_Flag > 0)  {
    Serial.printf("%s\tWAITING to Get Data  ~  Comms In Use\n", Clock);
    int count = 0;
    while (Comms_Flag > 0) {    // Checking Communication Flag 1=Active
      delay(1000);
      count++;              // Loop for 100 seconds
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

                  if (Airport_Code.length() != 4) {   // Checking for Length Error in Station Name
                    Stn_Metar[sta_n] = "<FONT COLOR='Red'><B>" + Airport_Code + " : Error in Station Name</B></FONT>";
                  }  else  {
                    Reset_All_Parameters(sta_n);
                    Check_Coms();
                    Comms_Flag = 1;             // Set Communication Flag 1=Active
                    GetData("NAME", Airport_Code, sta_n);   // GET Station NAME Data
                    Decode_Name(Airport_Code, sta_n);       // Decode Station NAME
                    GetData("METAR", Airport_Code, sta_n);  // GET Station METAR Data
                    ParseMetar(sta_n);                      // Parse Station METAR Data
                    Comms_Flag = 0;             // Set Communication Flag 0=Reset
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
                sta_n = Stn_Flash;        // From Display Station FLASH Button
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
                html_code = "<!DOCTYPE html><html><HEAD><meta name=\'viewport\' content=\'width=device-width, initial-scale=1\'>";
                html_code += "<TITLE>METAR</TITLE>";
                html_code += "<STYLE> html { FONT-family: Helvetica; display: inline-block; margin: 0px auto; text-align: left;}</STYLE></HEAD>";
                html_code += "<BODY><h2>METAR Summary</h2>";

                Update_Time();                     // GET CurrentTime : Hour & Minute
                if (Last_Up_Min + Update_Interval > 60)   Last_Up_Min = 60 - Update_Interval;
                Count_Down = Last_Up_Min + Update_Interval - Minute;
                if (Count_Down > Update_Interval)   Count_Down = 0;

                html_code += "<B>Summary of Weather Conditions</B><br>";
                html_code += "Current Time : " + String(Clock) + " UTC &nbsp&nbsp&nbsp&nbsp&nbsp&nbsp <FONT COLOR='Purple'>Last Update : " + Last_Up_Time + " UTC &nbsp&nbsp&nbsp&nbsp&nbsp&nbsp Next Update in " + String(Count_Down) + " Minutes</FONT><br>";
                html_code += "Click on <B>STATION NAME</B> to View Station Details<br>";

                // Display SUMMARY Table ***********
                //String Deg = "Deg&nbspC";    // Set Temperature Units   ******  For Deg C : Change this to "Deg C"
                String Deg = "Deg&nbspF";      // Set Temperature Units   ******  For Deg F : Change this to "Deg F"
                //String Alt = "in&nbspmBar";  // Set Altimeter Units     ******  For mBar : Change this to "mBar"
                String Alt = "in&nbspHg";      // Set Altimeter Units     ******  For Hg : Change this to "Hg"

                html_code += "<TABLE BORDER='2' CELLPADDING='5'>";
                html_code += "<TR BGCOLOR='Cyan'><TD>No:</TD><TD>Station Name:</TD><TD>CAT</TD><TD>SKY<br>COVER</TD><TD>VIS<br>Miles</TD><TD>WIND<br>From</TD><TD>WIND<br>Speed</TD><TD>";
                html_code += "TEMP&nbsp<br>" + Deg + "</TD><TD>ALT<br>" + Alt + "</TD><TD>METAR Codes & Remarks</TD></TR></FONT>";
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
                    } else      // Display LINK:  Makes a request in the header ("GET /get?Airport_Code=XXXX")
                      html_code = "<TR><TD>" + String(i) + "</TD><TD><a href='/get?Airport_Code=" + String(Stations[i].substring(0, 4)) + "'>" + color.substring(4, color.length()) + Stations[i] + "</a></FONT></TD>";

                    // Display Category in SUMMARY
                    html_code += color + Category[i] + "</FONT></TD>";

                    // Display Sky Cover in SUMMARY
                    if (Sky[i].length() != 3)  {    //   NA or OBSCURED or CLEAR
                      html_code += color + Sky[i];
                    } else {                        //   BKN FEW OVC SCT
                      if (old_cloud_base[i] > 0 && new_cloud_base[i] <= old_cloud_base[i] - diff_in_clouds)   // Significant DECREASE in Cloud Base
                        html_code += "<TD BGCOLOR='MistyRose' FONT COLOR='Purple'>" +  Sky[i] + "&nbspat<br>" + String(new_cloud_base[i]) + "&nbspFt";
                      else  html_code += color + Sky[i] + "&nbspat<br>" + String(new_cloud_base[i]) + "&nbspFt";
                      if (old_cloud_base[i] > 0)  {
                        if (new_cloud_base[i]  > old_cloud_base[i])  html_code += "&nbsp&uArr;";  //up arrow
                        if (new_cloud_base[i]  < old_cloud_base[i])  html_code += "&nbsp&dArr;";  //down arrow
                        if (new_cloud_base[i] == old_cloud_base[i])  html_code += "&nbsp&rArr;";  //right arrow
                      }
                    }
                    html_code += "</FONT></TD>";

                    // Display Visibility in SUMMARY
                    html_code += color + Visab[i] + "</FONT></TD>";

                    // Display Wind Direction in SUMMARY
                    int newDir = new_wDir[i].toInt();
                    int diff = old_wDir[i] - newDir;
                    if (newDir != 0 && old_wDir[i] != 0) {
                      if (diff < 0)      diff = newDir - old_wDir[i];
                      if (diff > 300)    diff = 360 - diff;
                      if (diff > 40)     html_code += "<TD BGCOLOR='MistyRose' FONT COLOR='Purple'>" + new_wDir[i] + "</FONT></TD>";
                      else  html_code += color + new_wDir[i] + "</FONT></TD>";
                    } else  html_code += color + new_wDir[i] + "</FONT></TD>";

                    // Display Wind Speed in SUMMARY
                    if (Category[i] == "NF")  html_code += color + "NA</FONT></TD>";
                    else   html_code += color + Wind[i] + "</FONT></TD>";

                    // Display Temperature in SUMMARY  ***  Set Temperature Units See Above ~ Display SUMMARY Table  ***
                    float TempF = TempC[i] * 1.8 + 32;  // deg F

                    if (TempC[i] == 99)   html_code += color + "NA" + "</FONT></TD>";
                    else  {
                      if (Deg == "Deg&nbspC")   html_code += color + String(TempC[i], 1) + "</FONT></TD>";
                      else html_code += color + String(TempF, 1) + "</FONT></TD>";
                    }

                    // Display Altimeter in SUMMARY  ***  Set Pressure Units See Above ~ Display SUMMARY Table  ***
                    if (Altim[i] == 0)  html_code += color + "NA" + "</FONT></TD>";
                    else  {
                      byte wx_flag = 0;
                      float Altm = Altim[i];
                      if (Alt == "in&nbspmBar")  Altm = Altim[i] * 1013.2 / 29.92;  else  Altm = Altim[i];
                      if (old_Altim[i] > 0.1)   {
                        if (Altim[i] >= old_Altim[i] + diff_in_press)  wx_flag = 1;    // Significant INCREASE in Pressure
                        if (Altim[i] <= old_Altim[i] - diff_in_press)  wx_flag = 1;    // Significant DECREASE in Pressure
                        if (wx_flag == 1) html_code += "<TD BGCOLOR='MistyRose' FONT COLOR='Purple'>" + String(Altm, 2);
                        else  html_code += color + String(Altm, 2);
                        if (Altim[i] > old_Altim[i])   html_code += "<br>&nbsp&nbsp&uArr;"; // up arrow
                        if (Altim[i] < old_Altim[i])   html_code += "<br>&nbsp&nbsp&dArr;"; // down arrow
                        if (Altim[i] == old_Altim[i])  html_code += "<br>&nbsp&nbsp&rArr;"; // right arrow
                        html_code += "</FONT></TD>";
                      }  else  html_code += color + String(Altm, 2) + "</FONT></TD>";
                    }

                    // Display Metar in SUMMARY
                    Update_Ago(i);             //   Update Minutes Ago
                    if (Stn_Metar[i].substring(0, 3) == "new")  html_code += "<TD><FONT COLOR='Purple'>";
                    else  html_code += color;
                    html_code += Stn_Metar[i];

                    // Display Remark in SUMMARY
                    if (Sum_Remark[i].length() > 3)   {
                      html_code += "<br><FONT COLOR='Navy'>Remarks : " + String(Sum_Remark[i]);
                    }
                    html_code += "</FONT></TD></TR>";
                    client.print(html_code);
                  }  // If Station != NULL
                }
                client.print("</TABLE>");
              }

              if (Station_Flag == 1)  {
                // *********** DISPLAY STATION  ***********
                html_code = "<!DOCTYPE html><html><HEAD><meta name=\'viewport\' content=\'width=device-width, initial-scale=1\'>";
                html_code += "<TITLE>METAR</TITLE>";
                html_code += "<STYLE> html { FONT-family: Helvetica; display: inline-block; margin: 0px auto; text-align: left;}</STYLE></HEAD>";
                html_code += "<BODY><h2>Station METAR</h2>";

                if (Station_Flag == 1 || Summary_Flag == 1)  {
                  html_code += "<P><FONT COLOR='Purple'><B>For : " +  Stations[sta_n] + "&nbsp&nbsp#&nbsp&nbsp" + String(sta_n) + "</B></FONT><br>";

                  // Display BUTTONS: Makes a request in the header ("GET /back HTTP/1.1")
                  html_code += "<a href='/back'><INPUT TYPE='button' VALUE='Previous Station' onClick= sta_n></a>";
                  html_code += "<a href='/flash'><INPUT TYPE='button' VALUE='Flashing LED' onClick= sta_n></a>";
                  html_code += "<a href='/next'><INPUT TYPE='button' VALUE='Next Station' onClick= sta_n></a>";
                  html_code += "&nbsp&nbsp&nbsp&nbsp<a href=/summary><INPUT TYPE='button' VALUE='Summary of Stations' onClick= sta_n></a>";
                  html_code += "<br><FONT COLOR='Navy'>&nbsp&nbsp&nbsp&nbspPress <B>'BUTTON'</B> when <B>LED is Flashing</B></FONT><br>";

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

                  String CountDown_Time = "<FONT COLOR='Purple'>Current Time : " + String(Clock) + " UTC &nbsp&nbsp&nbsp&nbsp&nbsp&nbsp Next Update in less than 0 Minutes</FONT><br>";
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
                  if (Stn_Metar[sta_n].substring(0, 3) == "new" )  html_code += "<TD><FONT COLOR='Purple'>" + String(Stn_Metar[sta_n]) + "</FONT>"; else  html_code += "<TD>" + String(Stn_Metar[sta_n]);
                  html_code += "</TD></TR>";

                  // Display Significant Weather in STATION
                  html_code += "<TR><TD>" + color + "Significant Weather</FONT><br><FONT COLOR='Purple'>Decoded Remarks</FONT></TD><TD>" + color + String(Sig_Weather[sta_n]) + "</FONT>";

                  //  Comments for Weather and Cloud Base in STATION
                  if (new_cloud_base[sta_n] > 1 && new_cloud_base[sta_n] <= 1200)
                    html_code += "<br><FONT COLOR='Gold'>Low Cloud Base</FONT>";
                  if (old_cloud_base[sta_n] > 0 && old_Altim[sta_n] > 0)    {
                    if (new_cloud_base[sta_n] > old_cloud_base[sta_n] && Altim[sta_n] > old_Altim[sta_n])
                      html_code += "<br><FONT COLOR='Gold'>Weather is Getting Better</FONT>";
                    if (new_cloud_base[sta_n] < old_cloud_base[sta_n] && Altim[sta_n] < old_Altim[sta_n])
                      html_code += "<br><FONT COLOR='Gold'>Weather is Getting Worse</FONT>";
                  }

                  //  Remarks in STATION
                  if (Stn_Remark[sta_n].length() > 1)
                    html_code += "<br><FONT COLOR='Purple'>" + String(Stn_Remark[sta_n]) + "</FONT>";

                  //  Sky Cover in STATION
                  html_code += "</TD></TR><TR><TD>" + color + "Sky Cover</TD><TD>";
                  //  Significant Change in Cloud Base
                  if (old_cloud_base[sta_n] > 0)   {

                    //Serial.printf("sta_n=%d  old_cloud_base=%d   new_cloud_base=%d\n", sta_n, old_cloud_base[sta_n], new_cloud_base[sta_n]);

                    if (new_cloud_base[sta_n] > old_cloud_base[sta_n] + diff_in_clouds) // INCREASE
                      html_code += "<FONT COLOR='Navy'>Significant Increase in Cloud Base</FONT><br>";
                    if (new_cloud_base[sta_n] < old_cloud_base[sta_n] - diff_in_clouds) // DECREASE
                      html_code += "<FONT COLOR='Orange'>Significant Decrease in Cloud Base</FONT><br>";
                  }
                  //  Add Change First Cloud Base
                  String text = Sky_cover[sta_n];
                  if (text == "")  text = Sky[sta_n];
                  int search0 = Sky_cover[sta_n].indexOf(" Ft");
                  if (search0 > 0 && old_cloud_base[sta_n] > 0)  {
                    if (new_cloud_base[sta_n] > old_cloud_base[sta_n])   // INCREASE
                      text = Sky_cover[sta_n].substring(0, search0 + 3) + "<FONT COLOR='Purple'>&nbsp&nbsp&nbsp&uArr; from " + String(old_cloud_base[sta_n]) + " Ft";
                    if (new_cloud_base[sta_n] < old_cloud_base[sta_n])   // DECREASE
                      if (old_cloud_base[sta_n] == 99999)  text = Sky_cover[sta_n].substring(0, search0 + 3) + "<FONT COLOR='Purple'>&nbsp&nbsp&nbsp&dArr; from CLEAR";
                      else text = Sky_cover[sta_n].substring(0, search0 + 3) + "<FONT COLOR='Purple'>&nbsp&nbsp&nbsp&dArr; from " + String(old_cloud_base[sta_n]) + " Ft";
                    if (new_cloud_base[sta_n] == old_cloud_base[sta_n])  // NO CHANGE
                      text = Sky_cover[sta_n].substring(0, search0 + 3) + "<FONT COLOR='Purple'>&nbsp&nbsp&nbsp&rArr; No Change";
                    text += "</FONT>" + Sky_cover[sta_n].substring(search0 + 3, Sky_cover[sta_n].length());
                  }
                  html_code += color + text + "</FONT></TD></TR>";

                  //  Visibility in STATION
                  html_code += "<TR><TD>" + color + "Visibility</TD><TD>";
                  text = Visab[sta_n] + " Statute Miles";
                  if (text.indexOf("m") >= 0)  text.replace("m", " ~ Measured");
                  if (text.indexOf("+") >= 0)  text = "Unlimited";
                  if (Visab[sta_n] != "NA")  html_code += color + text + "</FONT></TD></TR>";  else  html_code += "NA</FONT></TD></TR>";

                  //  Wind in STATION
                  html_code += "<TR><TD>Wind From</TD><TD>";
                  int newDir = new_wDir[sta_n].toInt();
                  if (new_wDir[sta_n] == "VRB")  newDir = 0;
                  int diff = old_wDir[sta_n] - newDir;
                  if (Wind[sta_n] == "CALM")   html_code += String(Wind[sta_n]);
                  else  html_code += String(new_wDir[sta_n]) + " Deg at " + String(Wind[sta_n]);
                  if (newDir > 0 && old_wDir[sta_n] > 0) {
                    if (diff < 0)      diff = newDir - old_wDir[sta_n];
                    if (diff > 300)    diff = 360 - diff;
                    if (diff > 30) {
                      html_code += "<FONT COLOR='Orange'> : Significant Change from " + String(old_wDir[sta_n]) + " Deg</FONT>";
                    }  else  {
                      if (newDir != old_wDir[sta_n])   html_code += "<FONT COLOR='Purple'> : Previously " + String(old_wDir[sta_n]) + " Deg</FONT>";
                      if (newDir == old_wDir[sta_n])   html_code += "<FONT COLOR='Purple'> : No Change in Direction</FONT>";
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
                      html_code += "<br><FONT COLOR='Purple'>" + Display_Text + "&nbsp&nbsp&nbspHeat Index</FONT>";
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
                        html_code += "<br>" + Display_Text + "&nbsp&nbsp&nbspWind Chill</FONT>";
                      }
                    }
                  }

                  //  Dew Point & Relative Humidity in STATION
                  if (DewptC[sta_n] == 99)  html_code += "</TD></TR><TR><TD>Dew Point</TD><TD>NA</FONT>";
                  else  {
                    html_code += "</TD></TR><TR><TD>Dew Point<br><FONT COLOR='Purple'>Relative Humidity</FONT></TD><TD>";
                    if (DewptC[sta_n] <= 0)  html_code += "<FONT COLOR='Blue'>";  else   html_code += "<FONT COLOR='Black'>";

                    // Display_Text (Dew Point) in STATION
                    String Display_Text = Format_Temp_Text(DewptC[sta_n], DewptC[sta_n] * 1.8 + 32);
                    html_code += Display_Text + "</FONT>";

                    // Display Relative Humidity in STATION
                    float Rel_Humid = Calc_Rel_Humid(sta_n);             // *** Calculate Relative Humidity Routine
                    if (DewptC[sta_n] >= 23.0 && Rel_Humid >= 60.0)   html_code += "<FONT SIZE='-1' FONT COLOR='Red'><I>&nbsp&nbsp&nbspAnd Muggy</I></FONT>";
                    html_code += "<br><FONT COLOR='Purple'>&nbsp&nbsp&nbsp" + String(Rel_Humid, 0) + " %</FONT>";
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
                  String airnav = "<br><a href='https://www.airnav.com/airport/" + String(Stations[sta_n].substring(0, 4)) + "' target='_blank'><B>AIRPORT INFORMATION</B></a>";
                  html_code += airnav + "<FONT COLOR='Orange'><FONT SIZE='-1'>&nbsp&nbsp&nbsp for US Airports Only</FONT><br>";

                  // AIRPORT RADAR from AviationWeather.gov in STATION
                  // Test Link: https://aviationweather.gov/gfa/?center=31.5521,-81.8896&zoom=9   = KJES
                  // Test Link: https://aviationweather.gov/gfa/?layers=rad,metar,airep,prog&center=31.5521,-81.8896&zoom=10  = KJES
                  if (Category[sta_n] != "NF")    {
                    String Center_Zoom = "&center=" + Lat_Long[sta_n] + "&zoom=10";
                    String radar = "<br><a href='https://aviationweather.gov/gfa/?&layers=rad,metar,airep,prog" + Center_Zoom + "'target='_blank'><B>AIRPORT RADAR</B></a>";
                    html_code += radar + "<FONT COLOR='Orange'><FONT SIZE='-1'>&nbsp&nbsp&nbsp for US Airports Only</FONT><br>";
                  }

                  // Display Foooter and Close
                  html_code += "<br><FONT COLOR='Navy' FONT SIZE='-1'>File Name &nbsp&nbsp&nbsp&nbsp&nbsp&nbsp: " + String(ShortFileName);
                  html_code += "<br>URL Address &nbsp: " + SW_addr;
                  html_code += "<br>H/W Address &nbsp: " + HW_addr;
                  html_code += "<br>Connected to &nbsp: " + String(ssid);
                  html_code += "<br><B>Dedicated to : F. Hugh Magee</B>";
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


void Update_Ago(int i)  {    // ***********   Update Minutes Ago
  if (Category[i] == "NF")  return;
  int search0 = Stn_Metar[i].indexOf("Z", 4);
  if (search0 < 0)  return;
  byte obsh = Stn_Metar[i].substring(search0 - 4, search0 - 2).toInt();  // METAR Obs Time - Hour
  byte obsm = Stn_Metar[i].substring(search0 - 2, search0).toInt();      // METAR Obs Time - Minute
  int ago = ((Hour - obsh) * 60) + Minute - obsm;                    // Minutes Ago
  if (ago < 0)    ago = ((24 - obsh) * 60) + Minute - obsm;
  int search1 = Stn_Metar[i].indexOf("(");
  if (search1 < 0)  Stn_Metar[i] = Stn_Metar[i] + " (" + String(ago) + "m&nbspAgo)";   // Add Minutes Ago
  else  Stn_Metar[i] = Stn_Metar[i].substring(0, search1 - 1) + " (" + String(ago) + "m&nbspAgo)";   // Update Minutes Ago
}


float Calc_Rel_Humid(int sta_n)  {    // ***********   Calculate Relative Humidity
  /* RH = 100 × {exp [17.625 × Dp  / (243.04 + Dp )]/exp [17.625 × T/ (243.04 + T)]}
     T & Dp in Deg C
  */
  float Rel_Humid = 100 * (exp((17.625 * DewptC[sta_n]) / (243.04 + DewptC[sta_n]))) / (exp((17.625 * TempC[sta_n]) / (243.04 + TempC[sta_n])));
  return Rel_Humid;
}


float Calc_Heat_Index(int sta_n)  {    // ***********   Calculate Heat Index
  /* HI = -42.379 + 2.04901523*T + 10.14333127*RH - 0.22475541*T*RH - 0.00683783*T*T - 0.05481717*RH*RH + 0.00122874*T*T*RH + 0.00085282*T*RH*RH - 0.00000199*T*T*RH*RH
     T  = air temperature (Deg F) [if T  > 18 Deg C > 57 Deg F]
     RH = relative humidity (%)
  */
  float Rel_Humid = Calc_Rel_Humid(sta_n);          // Calculate Relative Humidity
  float Temp = TempC[sta_n] * 1.8 + 32;  // Deg F
  float Heat_Index = (-42.379 + (2.04901523 * Temp) + (10.14333127 * Rel_Humid) - (0.22475541 * Temp * Rel_Humid) - (0.00683783 * Temp * Temp) - (0.05481717 * Rel_Humid * Rel_Humid)  + (0.00122874 * Temp * Temp * Rel_Humid) + (0.00085282 * Temp * Rel_Humid * Rel_Humid) - (0.00000199 * Temp * Temp * Rel_Humid * Rel_Humid) - 32 ) * 5 / 9;
  return Heat_Index;
}


float Calc_Wind_Chill(int sta_n)  {    // ***********   Calculate Wind Chill
  /*If wind_speed KTS > 3 && temperature <= 14 Deg C
    Wind_Chill = 13.12 + 0.6215 * Tair - 11.37 * POWER(wind_speed,0.16)+0.3965 * Tair * POWER(wind_speed,0.16)
  */
  int Wind_Speed = Wind[sta_n].toInt();
  float wind_speed = Wind_Speed * 1.852; // Convert to Kph
  float Wind_Chill = 13.12 + 0.6215 * TempC[sta_n] - 11.37 * pow(wind_speed, 0.16) + 0.3965 * TempC[sta_n] * pow(wind_speed, 0.16);
  if (Wind_Chill < 0 ) Wind_Chill = Wind_Chill - 0.5;  else  Wind_Chill = Wind_Chill + 0.5;
  return Wind_Chill;
}


float Calc_Density_Alt(int sta_n)  {    // ***********   Calculate Density Altitude
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
