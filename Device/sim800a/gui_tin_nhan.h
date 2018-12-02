
/******************************************************************************
*                             gui tin nhan                                    *
******************************************************************************/


#include "sms.h"
SMSGSM sms;

int numdata;
boolean started=false; //trạng thái modul sim
char smstext[160];// nội dung tin nhắn
char *number_root = "0382132912";
char *number_send = "0382132912";

void setup_sms(int check)
{
  delay(3000);
  Serial.println("Gui va nhan tin nhan");
  if (gsm.begin(9600)){
      Serial.println("\nstatus=READY");
      started=true;
  }   else
      Serial.println("\nstatus=IDLE");

  if(started && check){
      Serial.println("Send tn");
      sms.SendSMS(number_root, "Thiet bi dang hoat dong");   //đổi lại số của bạn nhé :D
  }
}
void send_sms(String number, String s)
{
  if(started){
  
      sms.SendSMS((char*)number.c_str(),s.c_str());   //đổi lại số của bạn nhé :D
      Serial.println("Send ok");
  }else Serial.println("Send that bai");
}
char  number[20];

void readSMS()
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
           }
       }
       delay(1000);
   } else Serial.println("Offline");
}
