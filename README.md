# METAR
+ In Memory of F. Hugh Magee, brother of John Magee author of poem HIGH FLIGHT.
+ Created by John Pipe with lots of help from David Bird and some code from Rui Santos.

This programme uses the two ESP32 cores, one to Get Data and Handle the LEDS, the other to run a webserver to get and display the data :

+ A chain of up to 100 leds (WS2811 or WS2812 or NEOPIXELS), showing the Flight Catorgry with Colors, GREEN, BLUE, RED and MAGENTA plus more.

+ HTML Pages, showing a Summary of the METAR codes and the Invividual Station Display of Decoded Data on a Local Network.

Either or Both of the above can be implemented.

The number and names of the Stations are Configurable for your application.

+ GETTING DATA : Updates METARS approximately every six minutes, so nearly REAL TIME data, from AVIATIONWEATHER.GOV.

+ WIRING OF LEDS : Three wires to  a set of WS2812 LEDS.
![Hardware](https://user-images.githubusercontent.com/24758833/157292122-1b9d380f-331b-4fc7-8e6e-f1c82105f2ea.jpg)

Note: With less than 50 LEDS an external Power Supply is not neccessary and can be driven by the ESP32 but at minimal brightness.

+ The idea is mount the LEDS behind a Chart so the light shines through the chart.

![IMG_2947](https://user-images.githubusercontent.com/24758833/157293897-e3d9f13f-debc-4f80-a27e-ef0c16ccbe5c.JPG)

+ So the Front of the chart looks like this:

![IMG_2916](https://user-images.githubusercontent.com/24758833/157294109-c94372d3-fa67-4a66-8e46-7be0fdde13f8.JPG)


+

+ HTML DISPLAYS : Can be accesed by any device connected to a WiFi local network.
+ METAR Summary Display Example:


![Summary_3_8](https://user-images.githubusercontent.com/24758833/157302360-1a6c893c-3b95-476c-b7d6-a997288b4bfb.jpg)


+ METAR Station Display Example:


![ATLANTA_3_8](https://user-images.githubusercontent.com/24758833/157302434-2aca9c6a-6f6b-4dd7-b3f8-ecf3f084f094.jpg)



End
