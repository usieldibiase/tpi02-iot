#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Device.h>
#include "secrets.h"
class Cloud {
  uint32_t wifiAt=0,writeAt=0;
  bool attempted=false;
public:
  bool pendingVoltage=false;
  float requestedVoltage=0;
  void begin() {
    WiFi. disconnect(true);
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    Serial.printf("\nConectando a WiFi: %s...\n", WIFI_SSID);
    WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
    wifiAt=millis();
  }
  bool connected() {
    if(WiFi.status()==WL_CONNECTED) return true;
    if(elapsed(millis(),wifiAt,10000)) {
      wifiAt=millis();
      Serial.printf("Reintentando WiFi (estado=%d)...\n", WiFi.status());
      WiFi.disconnect();
      WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
    }
    return false;
  }
  int tick(const Reading& r) {
    if(!connected()) return 0;
    if(attempted && !elapsed(millis(),writeAt,CLOUD_MS)) return 0;
    if(!r.valid && !pendingVoltage) return 0;
    attempted=true;
    WiFiClient client;HTTPClient http;
    http.setConnectTimeout(3000);http.setTimeout(3000);
    String data="api_key="+String(THINGSPEAK_WRITE_KEY);
    if(r.valid && !elapsed(millis(),r.sampleAt,10000)) {
      data+="&field1="+String(r.temperature,2)+"&field2="+String(r.humidity,2);
    } else if(!pendingVoltage) {writeAt=millis();return 0;}
    if(pendingVoltage) data+="&field3="+String(requestedVoltage,3);
    int status=-1;String body;
    if(http.begin(client,"http://api.thingspeak.com/update")) {
      http.addHeader("Content-Type","application/x-www-form-urlencoded");
      status=http.POST(data);body=http.getString();http.end();
    }
    writeAt=millis();
    bool ok=status==200 && body.toInt()>0;
    Serial.printf("ThingSpeak: HTTP %d, %s\n",status,ok?"aceptado":"rechazado/error");
    if(ok) pendingVoltage=false;
    return ok?1:-1;
  }
};