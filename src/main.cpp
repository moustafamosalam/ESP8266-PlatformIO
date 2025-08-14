#include "DebugManager.h"
#include <Arduino.h>
#include "WiFiManager.h"
#include "MQTTManager.h"
#include "SharedFunctions.h"

WiFiManager wifiManager;
MQTTManager mqttManager;

String currentVersion = "1.0.1";

bool setup_mood = false;

bool cancelReceiving = false;
String tempParams = "";
bool enterReceiving = false;

void setup() {
  Serial.begin(115200);
  sharedFunctions.initRGB();
  
  // Start Captive Portal to get credentials
  DEBUG_PRINTLN("Starting WiFi Manager");
  
  if (wifiManager.start()) {
    setup_mood = false;
    mqttManager.start(onMessageReceived);
  }
  else{
    setup_mood = true;
    sharedFunctions.setRGBColor(255, 0, 0);  // RED 
  }
}

void loop() {
  if(setup_mood)
  {
    wifiManager.loop(); 
  }
  else{
    while(true){
      if(!mqttManager.isConnected()){
        
        if(!wifiManager.isConnected()) wifiManager.reconnect(); //Reconnect to Wifi
          
        mqttManager.reconnect();  //Reconnect to MQTT
      }
      else{
        mqttManager.loop();
        checkFactoryResetButton();
      }
    }   
  }
}