# METAR
This is a project to WOW your friends and colleges.

+ In Memory of F. Hugh Magee, brother of John Magee author of poem HIGH FLIGHT.
+ Created by John Pipe with lots of help from David Bird and some code from Rui Santos.

+ Most Metar Charts use a Raspberry Pi, this uses a less expensive option of ESP32 and gives much more.  
It uses HTML displays that your favorite web browser can access to display a Summary Display and a Station Display without needing extra hardware.
  
Which shows decoded METAR information and much more. Please see examples below.

This programme uses the two ESP32 cores, one to Get Data and Display Leds, the other to run a webserver for Creating Web Pages and Handling Requests :

+ A chain of up to 100 leds (WS2811 or WS2812 or NEOPIXELS), showing the Flight Category with Colors, GREEN, BLUE, RED and MAGENTA plus more.

+ HTML Pages, showing a Summary of the METAR codes and the Invividual Station Display of Decoded Data on a Local Network.

Either or Both of the above can be implemented.

The number and names of the Stations are Configurable for your application.

+ GETTING DATA : Updates METARS approximately every six minutes, so nearly REAL TIME data, from AVIATIONWEATHER.GOV.

+ WIRING OF LEDS : Three wires to  a set of WS2812 LEDS (Add a 220 to 330 resistor in the Data Line).
+

![ESP32_ _leds_wiring](https://user-images.githubusercontent.com/24758833/197342655-f6e17e1f-f41f-4e63-b6c1-63a42799240c.png)

Note: With less than 50 LEDS an external Power Supply is not neccessary and can be driven by the ESP32 but at minimal brightness.


+ MOUNTING OF LEDS : I found it useful to mark the led strip with an arrow on the positive connection showing data in/out direction to make it easier to wire up later.
+ Cut the led strip up and tin the connections.
+ Push a pin through the front of the chart where you want place the led, then turn the chart over and place the led over the hole.  I used Scotch Tape to keep it in place.  Then wire and solder.  Note, keep the wires as short as possible.
+ MOUNTING OF LEDS PLAN B : Use a foam backing. Lay the chart on the foam backing and push the pin through the chart and into the backing.  Now, fix the leds to the backing foam where ever there is a pin hole.  Wire and solder.

+ The idea is mount the LEDS behind a Chart so the light shines through the chart.

![IMG_2947](https://user-images.githubusercontent.com/24758833/157293897-e3d9f13f-debc-4f80-a27e-ef0c16ccbe5c.JPG)

+ So the Front of the chart looks like this:

![IMG_2916](https://user-images.githubusercontent.com/24758833/157294109-c94372d3-fa67-4a66-8e46-7be0fdde13f8.JPG)


+ HTML DISPLAYS : Can be accesed by any device connected to a WiFi local network.

+ Just Enter the Logical Address http://metar.local or the IP Address in your Browser.
+ METAR Summary Display Example:


![METAR_Summary](https://user-images.githubusercontent.com/24758833/197342754-2bac1db7-003e-47b2-8072-c6f3a0c6f7f8.png)


+ METAR Station Display Example:


![image](https://user-images.githubusercontent.com/24758833/183297159-c19801f8-f4aa-4bc4-815a-bca285adbfa8.png)

+ Added OVER THE AIR updates:

To use this you need to add the library "Over_The_Air.h" in this respository to the SAME folder as the METAR program.

Edit the METAR program with your credentials.

First: "Upload" as normal, using a cable, to initialise OTA.

Then use the menu  "Tools", "Port" and select  metar xx.x.x.xx and "Upload" again, then it should work "Over the Air".  (Direct connection with the computer is not required.)

There are some videos on how to use "OTA" on YouTube.

+ See Additional Remarks/Comments in the program file.
+ Not for Commercial Use.
