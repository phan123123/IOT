
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
 * max send 160
 * number_root  so dien thoi mac dinh
 */
/*
 * void setup_sms(int check)
 * khoi tao qua trinh ket noi sim  
 *  - check = 0 : khong gui tin nhan check toi number_root
 *  - check = 1 : gui tin nhan check toi number_root
 *  
 * void send_sms(String number, String s) 
 * gui tin nhan s toi so number
 * length_max s = 160
 *  - number : sdt nhan tn 
 *  - s      : string muon gui lon nhat 160 ki tu 
 *  
 *  
 *  String readSMS(int del_me) 
 *  nhan tin nhan moi roi tra ve ket qua
 *  return string ket qua
 *   - del_me = 0: khong xoa tin nhan nhan duoc
 *   - del_me = 1: xoa tin nhan vua nhan duoc
 *  
 *  String nhan duoc co cau truc sdt_gui + '-' + noi_dung
 *  
 * int  kiem_tra_tai_khoan_goc(int tien_gh)
 * kiem tra tai khoan goc viettel *101#
 * return so tien co 
 * neu so tien khong lon hon tien_gh thi se gui tin nhan canh bao toi number_root
 * 
 * void  kiem_tra_tai_khoan_km()
 * kiem tra tai khoan khuyen mai  gui ve number_root
 * - kiem tra tin nhan con lai
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
  if (started == false) // neu chua ket noi thi mo cong ket noi 
  {
    
    if(gsm.begin(9600)){
        Serial.println("\nstatus=READY");
        started=true;
    }   else
        Serial.println("\nstatus=IDLE");
  }
  if(started && check){ // neu da ket noi va check = true
      Serial.print("Send message  check to " );
      Serial.println(number_root);
      sms.SendSMS(number_root, "Thiet bi dang hoat dong");   //gui tin nhan check 
  }
}
void send_sms(String number, String s) // gui sms : s toi number
{
  if(started){ // neu thiet bi da hoat dong thi gui 
  
      sms.SendSMS((char*)number.c_str(),s.c_str());   //đổi lại số của bạn nhé :D
      Serial.println("Send ok");
  }else Serial.println("Send that bai");
}
char  number[20];

String readSMS(int del_me) // nhan tin nhan - tra ve noi dung tn nhan duoc 
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
               if (del_me)sms.DeleteSMS(pos); // xoa tin nhan vua doc 
               return String(number) + "-" + smstext; //tra ve noi dung tin nhan nhan duoc 
               
           }
           
           
       }
       delay(1000);
       return "error"; // tra ve loi 
       
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
    delay(1000);//phai doi 10 giay moi gui va doc het du lieu
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
void  kiem_tra_tai_khoan_km()  // kien tra km viettel gui ve number_root
{
  Serial.println("Kiem tra tai khoan khuyen mai");
  gsm.SimpleWriteln("AT+CUSD=1,\"*102#\"");    
  int i = 0;
  while(++i < 10)
  {
    delay(1000);//phai doi 10 giay moi gui va doc het du lieu
    Serial.print("-");
  }
  Serial.println();
  char resp[101];//khoi tao xau cho noi dung tra ve
  gsm.read(resp, 100); //lay no
  Serial.println(resp);//in ra lan 2
  char ans[101];
  strncpy(ans, resp + 19, 100);
  
  send_sms(number_root, ans);// gui tin nhan bao ket qua
 
}
