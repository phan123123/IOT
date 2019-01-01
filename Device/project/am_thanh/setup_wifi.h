
char* status_wf = "connetion";
char* SSID_STA = "";
char* PASS_STA = "";
char* SSID_AP = "Wifi chua";
char* PASS_AP = "12345678";
int SSID_scan = 0;

int status_wifi()
{
  if (SSID_scan == 0) {
    Serial.print("[WIFI] Not wifi connect\n" );
    return 0;
  }
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++i); Serial.print(' ');
    if (i > 10) break;
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println('\n');
    Serial.print("Connected to ");
    Serial.println(WiFi.SSID());              // Tell us what network we're connected to
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer
    return 1;
  }
  Serial.print("Not connect!!");
  return 0;
}

void read_data_wifi()
{
  file login;
  login.open("/data_wifi.tg");
  String message = login.read();
  login.close();
    StaticJsonBuffer<MAXJSON> file_json;
    JsonObject& file_json_encode = file_json.parseObject(message);
    if (file_json_encode.success()){
      SSID_STA = strdup(file_json_encode["SSID_STA"]);
      name_thiet_bi = strdup(file_json_encode["NAME_DRIVER"]);
      PASS_STA = strdup(file_json_encode["PASS_STA"]);
      ipWebSocket = strdup(file_json_encode["ipWebSocket"]);
      portWebSocket = file_json_encode["portWebSocket"];
      linkWebSocket = strdup(file_json_encode["linkWebSocket"]);
      license = strdup(file_json_encode["license"]);
      Serial.printf("[WIFI]SSID: %s \n     PASS: %s \n", SSID_STA, PASS_STA);
    }else Serial.println("[WIFI] Sai cau truc");
  

}
int Scan_wifi(String ssid_default)
{
  Serial.println("[WIFI] Search: " +  ssid_default);
  int sl_wf =  WiFi.scanNetworks();
  Serial.printf("[WIFI] Number of networks: %d \n",sl_wf);
  for (int i= 0 ; i< sl_wf; i++){
    Serial.println(WiFi.SSID(i));
    if (WiFi.SSID(i) == ssid_default) {
      Serial.println("[WIFI] Search completed");
      return 1;
    }
  }
  return 0;
}

void write_data_wifi(char* message)
{
  file login;
  login.open("/data_wifi.tg");
  login.write(message);
  login.close();
}

bool  setup_wifi()
{
  read_data_wifi();
  WiFi.mode(WIFI_AP_STA);
  Serial.printf("[WIFI]SSID: %s \n     PASS: %s \n", SSID_STA, PASS_STA);
  SSID_scan = Scan_wifi(SSID_STA);
  Serial.printf("[WIFI]SSID_Scan: %d \n", SSID_scan);
  delay(1000);
  if (SSID_scan)
    WiFi.begin(SSID_STA, PASS_STA);
    strcpy(SSID_AP, (WiFi.macAddress()).c_str());
  WiFi.softAP(SSID_AP, PASS_AP);
  return SSID_scan;
}
