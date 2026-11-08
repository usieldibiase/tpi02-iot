#include <Arduino.h>
#include "config.h"
void setup() {
  Serial.begin(115200);
  pinMode(ACT_PIN, OUTPUT); digitalWrite(ACT_PIN, ACT_ACTIVE_HIGH ? LOW : HIGH);
  pinMode(BLUE_PIN, OUTPUT); digitalWrite(BLUE_PIN, LOW);
  Serial.println("Base TPI02 lista");
}
void loop() { delay(10); }
