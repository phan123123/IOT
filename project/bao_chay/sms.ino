
#include "SIM900.h"
#include <SoftwareSerial.h>
#include "gui_tin_nhan.h"
#include "DHT_read.h"


float nhiet_do = 0;
float do_am = 0;


void setup(){
    Serial.begin(115200);
    setup_sms(0);     // setup cho senset sim900a
    setup_nhiet_do(); // setup cho senser nhiet do do am

}

void loop() {
   readSMS();
}
