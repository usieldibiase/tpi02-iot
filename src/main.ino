#include <Arduino.h>
#include <Device.h>
#include <Cloud.h>
#include <Bot.h>

Device device;
QueueHandle_t readings,commands;
void networkTask(void*) {
  Cloud cloud;cloud.begin();
  Bot bot(commands,cloud);bot.begin();
  Reading latest;
  for(;;) {
    xQueuePeek(readings,&latest,0);
    if(cloud.connected()) {
      bot.tick(latest);
      int result=cloud.tick(latest);
      bot.uploadResult(result);
    }
    vTaskDelay(pdMS_TO_TICKS(20));
  }
}
void setup() {
  Serial.begin(115200);device.begin();
  readings=xQueueCreate(1,sizeof(Reading));commands=xQueueCreate(8,sizeof(Command));
  if(!readings||!commands) {Serial.println("ERROR colas");while(true) delay(1000);}
  Reading initial=device.read();xQueueOverwrite(readings,&initial);
  if(xTaskCreatePinnedToCore(networkTask,"network",16384,nullptr,1,nullptr,0)!=pdPASS) {
    Serial.println("ERROR tarea de red");while(true) delay(1000);
  }
}
void loop() {
  device.tick(WiFi.status()==WL_CONNECTED,true);
  Command command;
  if(xQueueReceive(commands,&command,0)==pdTRUE) device.command(String(command.text));
  Reading latest=device.read();xQueueOverwrite(readings,&latest);
  delay(5);
}
