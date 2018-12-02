
#include "SIM900.h"
#include <SoftwareSerial.h>
#include "gui_tin_nhan.h"
#include "send_sms.h"

float nhiet_do = 0;
float do_am = 0;


void setup(){
    Serial.begin(115200);
    setup_sms(0);     // setup cho senset sim900a
    // setup cho senser nhiet do do am
    Serial.println("cb send");
    delay(5000);
    //send_sms("0382132912", "nguyen manh thien");
    Serial.println("da send");
    setup_GPRS();
    send_gprs();
}

void loop() {
   readSMS();
}
