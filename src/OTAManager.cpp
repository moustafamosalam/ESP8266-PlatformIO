#include "DebugManager.h"
#include "OTAManager.h"
#include "SharedFunctions.h"
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ArduinoJson.h>

#define OTA_PASSWORD "123456" // Set your OTA password here
WiFiClientSecure otaWifiClient;  // Secure client for HTTPS

OTAManager::OTAManager(){}

void OTAManager::initArduinoOTA(){

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    DEBUG_PRINTLN("Start updating " + type);
  });

  ArduinoOTA.onEnd([]() {
    DEBUG_PRINTLN("\nEnd");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    DEBUG_PRINTF("Progress: %u%%\r", (progress / (total / 100)));
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) DEBUG_PRINTLN("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) DEBUG_PRINTLN("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) DEBUG_PRINTLN("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) DEBUG_PRINTLN("Receive Failed");
    else if (error == OTA_END_ERROR) DEBUG_PRINTLN("End Failed");
  });

  ArduinoOTA.setHostname("TinyTenants"); // Set your device name here
  ArduinoOTA.setPassword(OTA_PASSWORD);
  ArduinoOTA.begin();
  
}

void OTAManager::OTALoop(){
  ArduinoOTA.handle();
}
