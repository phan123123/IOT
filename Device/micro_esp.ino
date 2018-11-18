#include "serial_v1.h"
#include <Keyboard.h>
Serial_make myserial;

int d_delay = 0;
void open_cmd()
{
  delay(1000);
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.releaseAll();
  delay(3000);
  Keyboard.print("cmd");
  delay(500);
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press(KEY_RETURN);
  Keyboard.releaseAll();
}
void resetkey()
{

  myserial.change();
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  myserial.open_receive(2, 3);
  attachInterrupt(myserial.get_fre(), resetkey, CHANGE);
}

void loop() {

  if (myserial.available() > 0)
  {
   
   char p = myserial.read();
   if (d_delay = true){
      delay(int(p)*10);
      Serial.println(int(p)*10);
   }
   if ((int)p == 3) {
    d_delay = true; 
     Serial.print("\nDELAY ");
   }
   else 
   {
     if (int(p) ==4 ){
        Keyboard.releaseAll();
        Serial.println("Giai phong"); 
     }else{
        if ((int)p > 128){
          Keyboard.press(p);
        }else 
        {
          Serial.print(p); 
          Keyboard.write(p);
        }
     }
   }
   
 
  
 }
}
