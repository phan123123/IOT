
#include "SIM900.h"
#include <SoftwareSerial.h>
#include "gui_tin_nhan.h"
#include "DHT_read.h"


float nhiet_do = 0;
float do_am = 0;


void setup(){
    Serial.begin(115200);
    setup_sms(0);     // setup cho senset sim900a
    //send_sms("0382132912", "nguyen manh thien");
    //setup_nhiet_do("0382132912"); // setup cho senser nhiet do do am
    String p = "";
    kiem_tra_tai_khoan_goc(0);
    kiem_tra_tai_khoan_km();
    
    
}

void loop()
{
   String ans= readSMS(0);
   if (ans != "error")
   {
    Serial.println("Tin nhan nhan duoc: \n" + ans);
    send_sms("0382132912", ans);
   }
   //send_nhiet_do();
}
