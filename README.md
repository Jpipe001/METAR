## METAR Map and Decoded Displays
### This is a METAR MAP project to WOW your friends and colleges.

+ In Memory of **F. Hugh Magee**, brother of John Magee (WW2 Fighter Ace) and author of a **cool poem** called **HIGH FLIGHT**.
+ Created by John Pipe with lots of help from **David Bird** and some code from **Rui Santos.**

+ This Metar Map works **WORLDWIDE** to show Airport METARs from Avationweather.Gov.

+ Most Metar Maps use a Raspberry Pi, this uses a **less expensive option** of ESP32 and gives much more.
  
+ Works with WS2811 Leds that other Metar Maps use and can be easily upgraded.

+ It creates HTML displays that your favorite web browser can access to display an Airport Summary Display and Station Displays.  This shows decoded METAR information and much more without needing extra hardware.  Please see examples below.

  ---
 #### YOU WILL NEED:
 + The Arduino IDE Software to compile and load the code, which is free.
 + An ESP32 Development Board.
 +  A chain of up to 100 Color changing LEDS (WS2811 or WS2812 or NEOPIXELS), showing the Airport Flight Category with Colors, GREEN, BLUE, RED and MAGENTA plus Weather indication and more.
   #### Note: The LEDS are not required to just get these Color Coded Displays of the Airport Summary and Decoded METARs, see examples below.  Feel free to down load the above files and take a "test drive".

---

#### In the software, the number and names of the Airports are Configurable for your application.

+ GETTING DATA : Updates METARS approximately every ten minutes, so nearly REAL TIME data, from AVIATIONWEATHER.GOV.  (Includes the Latest Revision)
---

+ WIRING OF LEDS : Three wires to  a set of LEDS (Add a 330 resistor in the Data Line).

![ESP32_ _leds_wiring](https://user-images.githubusercontent.com/24758833/197342655-f6e17e1f-f41f-4e63-b6c1-63a42799240c.png)

Note: With less than 60 LEDS an external Power Supply is not neccessary and can be driven by the ESP32 but at **minimal brightness.**

+ MOUNTING OF LEDS : I found it useful to mark the led strip with an arrow on the positive connection showing data in/out direction to make it easier to wire up later.
+ Cut the led strip up and tin the connections.
+ Push a pin through the front of the chart where you want place the led, then turn the chart over and place the led over the hole.  I used Scotch Tape to keep it in place.  Then wire and solder.  Note, keep the wires as short as possible.
+ MOUNTING OF LEDS PLAN B : Use a foam backing. Lay the chart on the foam backing and push the pin through the chart and into the backing.  Now, fix the leds to the backing foam where ever there is a pin hole.  Wire and solder.

+ The idea is mount the LEDS behind a Chart so the light shines through the chart.

![IMG_2947](https://user-images.githubusercontent.com/24758833/157293897-e3d9f13f-debc-4f80-a27e-ef0c16ccbe5c.JPG)


+ So the Front of the chart looks like this: Showing the Flight Category with Colors, GREEN, BLUE, RED and MAGENTA plus more.

![IMG_2916](https://user-images.githubusercontent.com/24758833/157294109-c94372d3-fa67-4a66-8e46-7be0fdde13f8.JPG)

#### Of course, All of this can be mounted in a frame and be made much more presentable.

---

+ HTML DISPLAYS : Can be accesed by any device connected to the **WiFi local network.**

+ Just Enter the Logical Address http://metar.local or the IP Address in your Browser.

---

+ **METAR Summary Display** Example:

![METAR_Summary](https://user-images.githubusercontent.com/24758833/197342754-2bac1db7-003e-47b2-8072-c6f3a0c6f7f8.png)

#### Just "Click" on the STATION name.
---

+ **Updated: METAR Station Display** Example:

![2024-08-04-KFLL](https://github.com/user-attachments/assets/be266e73-e3f8-47f0-a2a8-6efdb23cfa61)

#### "Any Airport ID" can be enterd, even if it is not in the database.
---
+ **Recently Added:  AIRPORT INFORMATION**  ~  AirNav Link in Station Display for US airports

![2024-08-04-KFFL_AirNav](https://github.com/user-attachments/assets/09c26d76-98c2-424a-830e-2b9cf100d2bd)

 + **Recently Added:  AIRPORT RADAR**  ~  Radar Link in Station Display for US airports

![2024-08-04-KFLL_Radar](https://github.com/user-attachments/assets/8880e728-0b18-411c-9fa9-31f1432b6336)

---

+ **Previously Added** OVER THE AIR updates:

To use this you need to add the library "Over_The_Air.h" in this respository to the **SAME** folder as the METAR program.

First: "Upload" as normal, using a cable, to initialise OTA.

Then use the menu  "Tools", "Port" and select  metar xx.x.x.xx and "Upload" again, then it should work "Over the Air".  (Direct connection with the computer is now not required.)

There are some videos on how to use "OTA" on YouTube.

---

+ See Additional Remarks/Comments in the program file.
+ Not for Commercial Use.
