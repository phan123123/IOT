/****************************************************************************
*                                                                           *
*                             Doc du lieu tu senser                          *
*                                                                           *
****************************************************************************/

/* chan pin
VCC  -> VCC
GDN  -> GDN
DATA -> 26
*/

#define DHTPIN 26  // khai bao chan pin nhan du lieu
#define DHTTYPE DHT21
#include "DHT.h"
DHT dht(DHTPIN, DHTTYPE);

float get_nhiet_do()
{
  float t = dht.readTemperature();
  if (isnan(t)){
    Serial.println("Khong doc duoc nhiet do!");
    return 0;
  }
  return t;
}

float get_do_am()
{
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Khong doc duoc do am!");
    return 0;
  }
  return h;
}


void send_nhiet_do()
{

  delay(60000);
  float t = get_nhiet_do();
  float h = get_do_am();


  // Kiểm tra quá trình đọc thành công hay không
  if (isnan(h) || isnan(t) ) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  String s= "Nhiet do:" + String(t) + ".C Do am:" + String(h) +  "%.";
  Serial.println(s);
  delay(1000);

  if (t > 30.0){
      String s= "Nhiet do:" + String(t) + ".C Do am:" + String(h) +  "%.";
      Serial.println(s);
      sms.SendSMS("0373326268", s.c_str());
     sms.SendSMS("0982535203", s.c_str());
    }





}
void setup_nhiet_do(String num)
{
  delay(5000);
  //Đọc giá trị độ ẩm
  float h = dht.readHumidity();
  // Đọc giá trị nhiệt độ C (mặc định)
  float t = dht.readTemperature();
  float nhiet_do = t;
  float do_am = h;
  String s= "Nhiet do:" + String(nhiet_do) + ".C Do am:" + String(do_am) +  "%.";
  if (num != '0') send_sms(num, s);
 
  Serial.println(s);

}
