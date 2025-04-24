#include <Arduino.h>

#define WIFI_SSID "pc-class-310"
#define WIFI_PASS "SGgw66=123a"
#include "GyverStepper2.h"
GStepper2<STEPPER2WIRE> stepper1(2048, 15, 14, 5);// перепроверить
GStepper2<STEPPER2WIRE> stepper2(2048, 5, 4, 2); // перепроверить
int pos0t = 0;
int pos100t = 0;
int poscurt = 0;
#include <GyverDBFile.h>
#include <LittleFS.h>
#include <SettingsGyver.h>
// база данных для хранения настроек
// будет автоматически записываться в файл при изменениях
GyverDBFile db(&LittleFS, "/data.db");

// указывается заголовок меню, подключается база данных
SettingsGyver sett("My Settings", &db);
int muve=0;
// имена ячеек базы данных
DB_KEYS(
    kk,
    myText,
    pos0,
    pos100,
    poscur,
    sw1);

// билдер! Тут строится наше окно настроек
void build(sets::Builder& b) {
    while  (b.ButtonHold()) { // кнпка вверх
      
    muve=1;
    
}

   while (b.ButtonHold()) { // кнопка вниз
    
    muve=2;
   }
}

void setup() {

    Serial.begin(115200);
    Serial.println();

    WiFi.mode(WIFI_AP_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.print("Connected: ");
    Serial.println(WiFi.localIP());

#ifdef ESP32
    LittleFS.begin(true);
#else
    LittleFS.begin();
#endif

    sett.begin();
    sett.onBuild(build);

    // запуск и инициализация полей БД
    db.begin();
    
    db.init(kk::myText,"42" );
    db.init(kk::pos0,0);
    db.init(kk::pos100,4096);
    db.init(kk::poscur,0);
    db.init(kk::sw1,0); // if 0 = manula control; if 1 = automatic control



}

void loop(){
  ;
    // тикер, вызывать в лупе
    sett.tick();
    if (muve ==1){
      Serial.print(muve);
      stepper1.tick();
      stepper2.tick();
      muve=0;
      }
    if (muve ==2){
      Serial.print(muve);
      stepper1.reverse(1);
      stepper2.reverse(1);
      stepper1.tick();
      stepper2.tick();
      muve=0;
    }
        
    pos0t = db[kk::pos0].toInt();
    pos100t = db[kk::pos100].toInt();
    poscurt = db[kk::poscur].toInt();
}
