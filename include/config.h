#pragma once
#include <Arduino.h>
constexpr int DHT_PIN=33, POT_PIN=32, ACT_PIN=23, BLUE_PIN=2;
constexpr int SDA_PIN=21, SCL_PIN=22;
constexpr uint8_t OLED_ADDR=0x3C;
constexpr bool ACT_ACTIVE_HIGH=true;
constexpr float ALERT_HIGH=2.50f, ALERT_RESET=2.30f;
constexpr uint32_t SENSOR_MS=2500, POT_MS=100, CLOUD_MS=20000;
constexpr uint32_t MAX_RELAY_SECONDS=3600;
