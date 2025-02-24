/*********
  Modified 02/08/25 ~ Borrowed code from Andreas Spiess at https://github.com/SensorsIot/ESP32-OTA
  Put This "Over_The_Air.h" File in the SAME Folder as METAR
*********/
// Port defaults to 3232
// ArduinoOTA.setPort(3232);

// Hostname defaults to esp3232-[MAC]
// ArduinoOTA.setHostname("myesp32");

// No authentication by default
// ArduinoOTA.setPassword("admin");

// Password can be set with it's md5 value as well
// MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
// ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

//#include <WiFi.h>
//#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#if defined(ESP32_RTOS) && defined(ESP32)
void ota_handle( void * parameter ) {
  for (;;) {
    ArduinoOTA.handle();
    delay(3500);
  }
}
#endif

void SetupOTA(const char* nameprefix, const char* ssid, const char* password) {
  
  /*
    // Configure the hostname
    uint16_t maxlen = strlen(nameprefix) + 7;
    char *fullhostname = new char[maxlen];
    uint8_t mac[6];
    WiFi.macAddress(mac);
    snprintf(fullhostname, maxlen, "%s-%02x%02x%02x", nameprefix, mac[3], mac[4], mac[5]);
    ArduinoOTA.setHostname(fullhostname);
    delete[] fullhostname;

    // Configure and start the WiFi station
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    // Wait for connection
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("Connection Failed!   ~  Check Network Settings !\n");
    break;
    }
  */

  ArduinoOTA.begin();

  /*
    Serial.printf(" ~~~~~~~~~~~~\n");
    Serial.printf("OTA Initialized\n");
    Serial.printf("IP Address  : %s\n", WiFi.localIP().toString().c_str());
    Serial.printf("MAC address : %s\n", String(WiFi.macAddress()).c_str());
    Serial.printf("OTA Done !\n");
    Serial.printf(" ~~~~~~~~~~~~ \n");

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u\r", (progress / (total / 100)));
    });
  */

  ArduinoOTA.onStart([]() {
    //NOTE: make .detach() here for all functions called by Ticker.h library - not to interrupt transfer process in any way.
    String ftype;
    if (ArduinoOTA.getCommand() == U_FLASH)
      ftype = "Sketch";
    else // U_SPIFFS
      ftype = "Filesystem";
    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.printf("~~~~~~~~~~~~~~~~~~~~~~~~\nUpdating %s OTA\n", ftype.c_str());
  });

  ArduinoOTA.onEnd([]() {
    Serial.printf("OTA Updated !\n~~~~~~~~~~~~~~~~~~~~~~~~\n");
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.printf(" - Auth Failed\n");
    else if (error == OTA_BEGIN_ERROR) Serial.printf(" - Begin Failed\n");
    else if (error == OTA_CONNECT_ERROR) Serial.printf(" - Connect Failed\n");
    else if (error == OTA_RECEIVE_ERROR) Serial.printf(" - Receive Failed\n");
    else if (error == OTA_END_ERROR) Serial.printf(" - End Failed\n");
  });

#if defined(ESP32_RTOS) && defined(ESP32)
  xTaskCreate(
    ota_handle,          /* Task function. */
    "OTA_HANDLE",        /* String with name of task. */
    10000,               /* Stack size in bytes. */
    NULL,                /* Parameter passed as input of the task */
    1,                   /* Priority of the task. */
    NULL);               /* Task handle. */
#endif
}
