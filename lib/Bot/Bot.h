#pragma once
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <time.h>
#include <Cloud.h>
class Bot {
  WiFiClientSecure client;
  UniversalTelegramBot bot{BOT_TOKEN,client};
  QueueHandle_t commands;
  Cloud& cloud;
  uint32_t pollAt=0,alertAt=0,deliveredAlarm=0;
  bool waitingUpload=false;
  void reply(const String& text) {bot.sendMessage(ALLOWED_CHAT_ID,text,"");}
  bool enqueue(const String& text) {
    Command c{};text.toCharArray(c.text,sizeof(c.text));
    if(xQueueSend(commands,&c,0)!=pdTRUE) {reply("Cola ocupada. Reintenta.");return false;}
    return true;
  }
  void handle(String text,const Reading& r) {
    text.trim();
    if(text.length()>=96) {reply("Comando demasiado largo.");return;}
    int space=text.indexOf(' '),at=text.indexOf('@');
    if(at>=0 && (space<0 || at<space)) text.remove(at,(space<0?text.length():space)-at);
    if(text=="/start") {
      String keyboard=R"json([[{"text":"Ver clima","callback_data":"/clima"}]])json";
      bot.sendMessageWithInlineKeyboard(ALLOWED_CHAT_ID,"TPI02: elegi una accion.","",keyboard);
    } else if(text=="/clima") {
      if(!r.valid) reply("DHT22: todavia no hay una lectura valida.");
      else reply("Temperatura: "+String(r.temperature,1)+" C\nHumedad: "+String(r.humidity,1)+
        " %\nEdad de lectura: "+String(uint32_t(millis()-r.sampleAt)/1000)+" s"+
        (elapsed(millis(),r.sampleAt,10000)?"\nAVISO: datos antiguos":""));
    }

    else reply("Comando desconocido. Usa /start.");
  }
public:
  Bot(QueueHandle_t q,Cloud& c):commands(q),cloud(c) {}
  void begin() {
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
    client.setTimeout(3000);client.setHandshakeTimeout(5);
    bot.longPoll=0;bot.waitForResponse=3000;
    configTime(0,0,"pool.ntp.org","time.nist.gov");
  }
  void tick(const Reading& r) {
    if(time(nullptr)<1700000000) return; // TLS requires valid date.
    if(!elapsed(millis(),pollAt,1000)) return;
    pollAt=millis();
    int count=bot.getUpdates(bot.last_message_received+1);
    for(int i=0;i<count;++i) {
      auto& m=bot.messages[i];
      if(m.chat_id!=String(ALLOWED_CHAT_ID)) continue;
      if(m.type=="callback_query") bot.answerCallbackQuery(m.query_id);
      handle(m.text,r);
    }

  }

};
