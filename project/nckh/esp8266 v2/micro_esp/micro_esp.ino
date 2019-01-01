#include "serial_v1.h"
#include <Keyboard.h>
Serial_make myserial;

int d_delay = 0;
int to_hop = 0;
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

int tach_hex(char x)
{
  if (x > '9') return (int(x- 'A') + 10);
  else return int(x -'0');
  
}
int dem = 0;
int sl = 0;
int timee = millis();
void setup() {
  delay(5000);
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println(timee);
  myserial.open_receive(2, 3, 4);
  myserial.reset_stack();
  attachInterrupt(myserial.get_fre(), resetkey, CHANGE);
  //open_cmd();
  timee = millis();
  Serial.println(timee);
}

void loop() {
  
  //Serial.println(myserial.available());

  if (myserial.available() > 0)
  {
    
    char  p1 = myserial.read();
    //Serial.println(p1);
    //delay(1000);
   dem++;
   if (dem ==1){
    sl = tach_hex(p1);
   }else{
    dem = 0;
    sl = sl*16 + tach_hex(p1);
   int key = sl;
   
      
     if (d_delay == true){
      delay(key*10);
      d_delay = false;
      Serial.println(key*10);
   }else 

   switch(key){
      case 233:{
          d_delay = true; 
           delay(100);
          Serial.print("\nDELAY ");
          break;
      }
  
      case 234:
      {
          Keyboard.releaseAll();
          delay(100);
          to_hop = 0;
          Serial.println("Ket thuc to hop");     
          break;
      }

      case 235:
      {
          delay(100);
         to_hop = 1;
         Serial.println("Bat dau to hop");     
         break;
      }

      default:{

        if (to_hop ) {
          Keyboard.press(key);
          delay(100);
          Serial.print("du phim");
          Serial.println(key);
        }else{
          
         
          Keyboard.write(key);
        }
      
      
      }

  
      }
    }
  }
}
