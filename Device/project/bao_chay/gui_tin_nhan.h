
/******************************************************************************
*                             gui tin nhan                                    *
******************************************************************************/
/*
 * su dung thi vien sms.h va "SIM900.h" trong goi thu vien GSMSHIELD
 * 
 */
// ham gui nhan tn sim900a
// arduino mega 2560
/* 
 *  make_port
 *  sim900a  | mega 2560
 *  tx       |  50 
 *  rx       |  51
 *  5v       |  5v
 *  GND      |  GND
 */

/*
 * max receive 160 
 * number_root  so dien thoi mac dinh
 */
#include "sms.h"
SMSGSM sms;

int numdata;
boolean started=false; //trạng thái modul sim
char smstext[160];// nội dung tin nhắn
char *number_root = "0382132912"; // numbet root 
char *number_send = "0382132912";

void setup_sms(int check) // khoi tao gui sms
{
  delay(3000);
  Serial.println("Gui va nhan tin nhan");
  if (gsm.begin(9600)){
      Serial.println("\nstatus=READY");
      started=true;
  }   else
      Serial.println("\nstatus=IDLE");

  if(started && check){
      Serial.print("Send message  check to " );
      Serial.println(number_root);
      sms.SendSMS(number_root, "Thiet bi dang hoat dong");   //đổi lại số của bạn nhé :D
  }
}
void send_sms(String number, String s) // gui sms
{
  if(started){
  
      sms.SendSMS((char*)number.c_str(),s.c_str());   //đổi lại số của bạn nhé :D
      Serial.println("Send ok");
  }else Serial.println("Send that bai");
}
char  number[20];

String readSMS() // nhan tin nhan - tra ve noi dung tn nhan duoc 
{
  if(started){
       int pos; //địa chỉ bộ nhớ sim (sim luu tối đa 40 sms nên max pos = 40)
       pos = sms.IsSMSPresent(SMS_UNREAD); // kiểm tra tin nhắn chưa đọc trong bộ nhớ
       //hàm này sẽ trả về giá trị trong khoảng từ 0-40
     
       if(pos){//nêu có tin nhắn chưa đọc
           if(sms.GetSMS(pos, number, smstext, 160)){
               Serial.print("So dien thoại: ");
               Serial.println(number);
               Serial.print("Noi dung tin nhan: ");
               Serial.println(smstext);
               sms.DeleteSMS(pos);
               return String(number) + "-" + smstext;
               
           }
           
           
       }
       delay(1000);
       return "error";
       
   } else {
    Serial.println("Offline");
    return "error";
   }
}


int kiem_tra_tai_khoan_goc(int tien_gh) // kiem tra tai khoan goc viettel
{
  Serial.println("Kiem tra tai khoan");
  gsm.SimpleWriteln("AT+CUSD=1,\"*101#\"");    
  int i = 0;
  while(++i < 10)
  {
    delay(1000);//phai doi 5 giay moi gui va doc het du lieu
    Serial.print("-");
  }
  Serial.println();
  char resp[101];//khoi tao xau cho noi dung tra ve
  gsm.read(resp, 100); //lay no
  Serial.println(resp);//in ra lan 2
  char ans[101];
  strncpy(ans, resp + 34, 100);
  
  Serial.println(ans);
  int  money = 0;
  for(int i= 0; i<100; i++) 
    if(ans[i] == 'd')break;
    else money = money*10 + int(ans[i]) - int('0');
  if(money <= tien_gh)
  {
    send_sms(number_root, "Tai khoan cua quy khach sap het");
    Serial.println("Het tai khoan");
  }
  return money;
}
void  kiem_tra_tai_khoan_km(int tien_gh)  // kien tra km viettel gui ve number_root
{
  Serial.println("Kiem tra tai khoan khuyen mai");
  gsm.SimpleWriteln("AT+CUSD=1,\"*102#\"");    
  int i = 0;
  while(++i < 10)
  {
    delay(1000);//phai doi 5 giay moi gui va doc het du lieu
    Serial.print("-");
  }
  Serial.println();
  char resp[101];//khoi tao xau cho noi dung tra ve
  gsm.read(resp, 100); //lay no
  Serial.println(resp);//in ra lan 2
  char ans[101];
  strncpy(ans, resp + 19, 100);
  
  send_sms(number_root, ans);
 
}
