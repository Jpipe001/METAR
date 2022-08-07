# METAR
+ In Memory of F. Hugh Magee, brother of John Magee author of poem HIGH FLIGHT.
+ Created by John Pipe with lots of help from David Bird and some code from Rui Santos.

This programme uses the two ESP32 cores, one to Get Data and Display Leds, the other to run a webserver for Creating Web Pages and Handling Requests :

+ A chain of up to 100 leds (WS2811 or WS2812 or NEOPIXELS), showing the Flight Catorgry with Colors, GREEN, BLUE, RED and MAGENTA plus more.

+ HTML Pages, showing a Summary of the METAR codes and the Invividual Station Display of Decoded Data on a Local Network.

Either or Both of the above can be implemented.

The number and names of the Stations are Configurable for your application.

+ GETTING DATA : Updates METARS approximately every six minutes, so nearly REAL TIME data, from AVIATIONWEATHER.GOV.

+ WIRING OF LEDS : Three wires to  a set of WS2812 LEDS (Add a 220K to 330K resistor in the Data Line).
![Hardware](https://user-images.githubusercontent.com/24758833/157292122-1b9d380f-331b-4fc7-8e6e-f1c82105f2ea.jpg)

Note: With less than 50 LEDS an external Power Supply is not neccessary and can be driven by the ESP32 but at minimal brightness.


+ MOUNTING OF LEDS : I found it useful to mark the led strip with an arrow on the positive connection showing data in/out direction to make it easier to wire up later.
+ Cut the led strip up and tin the connections.
+ Push a pin through the front of the chart where you want place the led, then turn the chart over and place the led over the hole.  I used Scotch Tape to keep it in place.  Then wire and solder.  Note, keep the wires as short as possible.
+ MOUNTING OF LEDS PLAN B : Use a foam backing. Lay the chart on the foam backing and push the pin through the chart and into the backing.  Now, fix the leds to the backing foam where ever there is a pin hole.  Wire and solder.

+ The idea is mount the LEDS behind a Chart so the light shines through the chart.

![IMG_2947](https://user-images.githubusercontent.com/24758833/157293897-e3d9f13f-debc-4f80-a27e-ef0c16ccbe5c.JPG)

+ So the Front of the chart looks like this:

![IMG_2916](https://user-images.githubusercontent.com/24758833/157294109-c94372d3-fa67-4a66-8e46-7be0fdde13f8.JPG)


+

+ HTML DISPLAYS : Can be accesed by any device connected to a WiFi local network.
+ METAR Summary Display Example:


![image](https://user-images.githubusercontent.com/24758833/180669712-ec0df327-760a-49f8-941e-93d3c7773029.png)


+ METAR Station Display Example:


![image](https://user-images.githubusercontent.com/24758833/183297159-c19801f8-f4aa-4bc4-815a-bca285adbfa8.png)


+ See Additional Remarks/Comments in the program file.
+ Not for Commercial Use.
