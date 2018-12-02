
class file
{
  private:
    char *link;
    File f;
    static String data_default;
  public:
    void open(String link);
    String read();
    void close();
    void write(char *message);
};

String file::data_default = "{\"SSID_STA\":\"InternetHV-01\",\"PASS_STA\":\"ngococcho\",\"ipWebSocket\":\"192.168.8.105\",\"portWebSocket\":8080,\"linkWebSocket\":\"/Server/actions\",\"license\":\"qazxswedcvfrtgbn\"}";
void file::open(String link )
{

  SPIFFS.begin();
  if (SPIFFS.exists(link.c_str()) == false){
    f = SPIFFS.open(link.c_str(), "w");
    f.print(data_default);
    f.close();
    Serial.println("[FILE] Tao file thanh cong");
  }
  f = SPIFFS.open(link.c_str(), "r+w");
  if (!f) {
    Serial.println("[FILE]file open failed");
  }else
    Serial.println("[FILE]Mo file thanh cong");
}
String file::read()
{
   String message = "";
   while(f.available())
    {
      message += f.readStringUntil('\n');

    }
    Serial.printf("[FILE]Noi dung file:");
    Serial.println(message);
    return message;
}

void file::write(char *message)
{
  f.print(message);
  Serial.printf("[FILE]Ghi thanh cong:");
  Serial.println(message);
}

void file::close()
{
  f.close();
  Serial.println("[FILE] Close file");
  SPIFFS.end();
}
