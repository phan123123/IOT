#include <SoftwareSerial.h>

SoftwareSerial dataSerial(12, 14);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dataSerial.begin(115200);
 
}

void loop() {
  
  if (dataSerial.available())
    Serial.write(dataSerial.read());
  if (Serial.available())
   {
    char x= Serial.read();
    dataSerial.write(x);
    Serial.write(x);
   }
  
}
