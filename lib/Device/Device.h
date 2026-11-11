#pragma once
#include <Arduino.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_SH110X.h>
#include <Control.h>
#include "config.h"

struct Reading {
  float temperature=0,humidity=0,voltage=0,eventVoltage=0;
  uint32_t sampleAt=0,alarmCount=0;
  bool valid=false,relay=false;
};

struct Command { char text[96]; };

class Device {
  DHT dht{DHT_PIN,DHT22};
  Adafruit_SH1106G oled{128, 64, &Wire, -1};
  RelayTimer timer;
  AlarmLatch alarm;
  Reading state;
  uint32_t sensorAt=0,potAt=0,screenAt=0;
  String screen="clima";
  bool oledOK=false;

  void output() { digitalWrite(ACT_PIN,timer.on==ACT_ACTIVE_HIGH ? HIGH:LOW); state.relay=timer.on; }

public:
  void begin() {
    pinMode(ACT_PIN,OUTPUT); output();
    pinMode(BLUE_PIN,OUTPUT); digitalWrite(BLUE_PIN,LOW);
    analogReadResolution(12); analogSetPinAttenuation(POT_PIN,ADC_11db);
    dht.begin(); Wire.begin(SDA_PIN,SCL_PIN);
    Wire.setTimeOut(50);
    oledOK=oled.begin(OLED_ADDR, true);
    if(!oledOK) Serial.println("ERROR OLED: revisar cableado/direccion");
    sensorAt=millis();
  }

  Reading read() const {return state;}

  void command(const String& text) {
    if(text=="/releoff") {timer.stop(); output();}
    else if(text.startsWith("/releon ")) {
      uint32_t seconds;
      if(parseSeconds(text.substring(8).c_str(),seconds,MAX_RELAY_SECONDS)) {
        timer.start(millis(),seconds); output();
      }
    } else if(text.startsWith("/display")) {
      String cmd=text.substring(8); cmd.trim();
      if(cmd=="led") screen="led";
      else if(cmd=="pot"||cmd=="potenciometro") screen="pot";
      else if(cmd=="dht22"||cmd=="dth22"||cmd=="clima") screen="clima";
      else screen="error";
    }
    screenAt=millis()-500;
  }

  void tick(bool wifi,bool enableAlarm) {
    uint32_t now=millis();
    if(timer.tick(now)) {output(); Serial.println("Actuador: apagado automatico");}
    digitalWrite(BLUE_PIN,wifi?HIGH:LOW);
    if(elapsed(now,potAt,POT_MS)) {
      potAt=now; state.voltage=analogRead(POT_PIN)*3.3f/4095.0f;
      if(enableAlarm && alarm.update(state.voltage,ALERT_HIGH,ALERT_RESET)) {
        ++state.alarmCount; state.eventVoltage=state.voltage;
        Serial.println("Alerta de voltaje pendiente");
      }
    }
    if(elapsed(now,sensorAt,SENSOR_MS)) {
      sensorAt=now; float t=dht.readTemperature(),h=dht.readHumidity();
      if(!isnan(t)&&!isnan(h)) {state.temperature=t;state.humidity=h;state.sampleAt=now;state.valid=true;}
      else Serial.println("DHT22: lectura invalida; conservando ultima valida");
    }
    if(oledOK && elapsed(now,screenAt,500)) {
      screenAt=now; oled.clearDisplay();oled.setTextSize(1);oled.setTextColor(SH110X_WHITE);oled.setCursor(0,0);
      oled.println("TPI02 - ESP32");oled.println(wifi?"WiFi: conectado":"WiFi: desconectado");oled.println();
      if(screen=="led") oled.println(state.relay?"Led encendido":"Led apagado");
      else if(screen=="pot") {oled.print("Voltaje: ");oled.print(state.voltage,2);oled.println(" V");}
      else if(screen=="error") oled.println("Comando de display\nno identificado");
      else if(!state.valid) oled.println("DHT22: sin datos");
      else {oled.printf("T: %.1f C\nHR: %.1f %%\n",state.temperature,state.humidity);
        if(elapsed(now,state.sampleAt,10000)) oled.println("DATOS ANTIGUOS");}
      oled.display();
    }
  }
};