
// make connect wifi and access point
/*
 * Hearder 
 * int status_wifi()
 *    get status wifi 
 *    +return 1: wifi connected
 *    +return 0: wifi not connect
 * 
 *void read_data_wifi()    
 *    read data wifi to SPIFFS memory
 *    structure: file json
 *    read by json v5 use StaticJsonBuffer
 *    SSID_STA:  name wifi connect
 *    PASS_STA: passwork wifi connect
 *    name_thiet_bi: name device
 *    ipWebSocket: ip connect to server websocket
 *    portWebSocket: port connect to server websocket
 *    linkWebSocket: link connect to server wwbscoket 
 *    license: key connect with server 
 *    
 *int Scan_wifi()
 *    Scan wifis near around
 *    return 1 if SSID in near else 0
 * 
 
 */


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

void read_data_wifi() // read  file data and write new data 

{
  file login;  
  login.open("/data_wifi.tg"); // open file 
  String message = login.read();  // read file 
  login.close(); // close file 
    StaticJsonBuffer<MAXJSON> file_json; // new json 
    JsonObject& file_json_encode = file_json.parseObject(message); // assign to file_json by data get to file 
    if (file_json_encode.success()){   //file_json_encode.success: correct to json
      SSID_STA = strdup(file_json_encode["SSID_STA"]);
      name_thiet_bi = strdup(file_json_encode["NAME_DRIVER"]);
      PASS_STA = strdup(file_json_encode["PASS_STA"]);
      ipWebSocket = strdup(file_json_encode["ipWebSocket"]);
      portWebSocket = file_json_encode["portWebSocket"];
      linkWebSocket = strdup(file_json_encode["linkWebSocket"]);
      license = strdup(file_json_encode["license"]);
      Serial.printf("[WIFI]SSID: %s \n     PASS: %s \n", SSID_STA, PASS_STA);
    }else Serial.println("[WIFI] Sai cau truc");

    // data in file read and assign value
}
int Scan_wifi(String ssid_default) // scan wifi near around
{
  Serial.println("[WIFI] Search: " +  ssid_default);
  int sl_wf =  WiFi.scanNetworks();   // scan wifi 
  Serial.printf("[WIFI] Number of networks: %d \n",sl_wf);
  for (int i= 0 ; i< sl_wf; i++){ // find wifi in list wifi
    Serial.println(WiFi.SSID(i));
    if (WiFi.SSID(i) == ssid_default) {
      Serial.println("[WIFI] Search completed");
      return 1;
    }
  }
  return 0;
}

void write_data_wifi(char* message) // read file 
{
  file login;
  login.open("/data_wifi.tg");
  login.write(message);
  login.close();
}


bool  setup_wifi() // setup wifi main 
{
  read_data_wifi();
  WiFi.mode(WIFI_AP_STA);
  Serial.printf("[WIFI]SSID: %s \n     PASS: %s \n", SSID_STA, PASS_STA);
  SSID_scan = Scan_wifi(SSID_STA);
  Serial.printf("[WIFI]SSID_Scan: %d \n", SSID_scan);
  delay(1000);
  if (SSID_scan)
    WiFi.begin(SSID_STA, PASS_STA); // start connet wifi 
  WiFi.softAP(SSID_AP, PASS_AP); // start access point
  
  return SSID_scan;
}
