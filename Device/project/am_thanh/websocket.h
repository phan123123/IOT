void send_data(WebSocketsClient  &webSocket, String link_file, int byte_h, int size_of_text);
void get_data(WebSocketsClient &webSocket, String message);
bool wifiDisconneted = true;
void send_test_speed();
void check(String s);
void send_am_thanh()
{
  Serial.println("send_code ok ");
  webSocket.sendTXT("ADD_D");
}
void webSocketEvent( WStype_t type, uint8_t * payload, size_t length) {
  //Serial.println(type);
  switch(type) {
    case WStype_DISCONNECTED:
      if (wifiDisconneted == true)  Serial.printf("[WSc] Disconnected!\n");
      wifiDisconneted = false;
      break;
    case WStype_CONNECTED: {
      Serial.printf("[WSc] Connected to url: \n");
      wifiDisconneted = true;
      // send message to server when Connected
      //encode_json_send_start(webSocket);
      //send_test_speed(); //test speed send to internet
      send_am_thanh();  //test am thanh 
      break;
      }

    case WStype_TEXT:
      Serial.printf("[WSc] get text: %s\n", payload);
      //Serial.println(sizeof(payload));
      //check((char*)payload);
      //send_am_thanh();
      //Serial.printf("vua nhan duoc mot tin nhan");
      //decode_json_receive(webSocket,(char*) payload);

      break;
    case WStype_BIN:
      Serial.printf("[WSc] get binary length: %u\n", length);
      hexdump(payload, length);
      
      
      //Serial.println("Xuat am thanh");
      player.playChunk(payload, length);
      // send data to server
      // webSocket.sendBIN(payload, length);
      break;
  }

}
int i= 0;
void check(String s)
{
  Serial.println(s);
  Serial.println(s.length());
  Serial.println(i++);
  delay(1000);
}

void send_test_speed()
{
  int num = 0;
  while(num <= 1000)
  {
    num++; 
    //delay(100);
    DynamicJsonBuffer JSONbuffer;
    JsonObject& JSONencoder = JSONbuffer.createObject();
    JSONencoder["action"] = "text";
    JSONencoder["kind"] = "receive";
    String data = "a";
    for(int i= 1; i<= 5000; i++) data += 'a';
    JSONencoder["test"] = data;
    char JSONmessageBuffer[1200];
    JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    Serial.println("[KEY]Gui du lieu bat tay");
    Serial.println(JSONmessageBuffer);
  
    webSocket.sendTXT(JSONmessageBuffer);
  }
}

// bat tay 2 thiet bi
// gui lan 1

void encode_json_send_start(WebSocketsClient  &webSocket)
{
  DynamicJsonBuffer JSONbuffer;
  JsonObject& JSONencoder = JSONbuffer.createObject();
  JSONencoder["action"] = "add";
  JSONencoder["kind"] = "receive";
  JSONencoder["name"] = name_thiet_bi;
  JSONencoder["license"] = license;
  JSONencoder["type"] = "123";
  JSONencoder["description"] = "asdfsa";
  char JSONmessageBuffer[500];
  JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  Serial.println("[KEY]Gui du lieu bat tay");
  Serial.println(JSONmessageBuffer);

  webSocket.sendTXT(JSONmessageBuffer);
}

void send_json(WebSocketsClient  &webSocket)
{
  DynamicJsonBuffer JSONbuffer;
  JsonObject& JSONencoder = JSONbuffer.createObject();
  JSONencoder["action"] = "post";
  JSONencoder["data"] = "receive";
  JSONencoder["id"] = name_thiet_bi;

  char JSONmessageBuffer[500];
  JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  Serial.println("[DATA]Gui du lieu");
  Serial.println(JSONmessageBuffer);

  webSocket.sendTXT(JSONmessageBuffer);
}

void decode_json_receive(WebSocketsClient &webSocket, String message)
{
  DynamicJsonBuffer JSONBuffer;                     //Memory pool
  JsonObject& parsed = JSONBuffer.parseObject(message); //Parse message

  if (!parsed.success()) {   //Check for errors in parsing
    Serial.println("[KEY]Parsing failed");
    return;
  }
  char* action = strdup(parsed["action"]);           //Get sensor type value
  if (strcmp(action,"setLicense") == 0)
  {
    Serial.println("[KEY]Nhan duoc key moi");
    license =  strdup(parsed["license"]);
    write_new_license(license);
    Serial.printf("[KEY]Da doi key moi: %s\n", license);
    //send_data(webSocket, "text.txt",0, 300);
    return;
  }
  if (strcmp(action, "get") == 0)
  {
     Serial.println("[DATA]Phan hoi");
     send_json(webSocket);
  }
  if (strcmp(action, "error") == 0)
  {
      status_wf = strdup(parsed["data"]);
      strcat(status_wf, " error");
      webSocket.setReconnectInterval(10000000);
  }
  if (strcmp(action,"TEXT") ==0)
  {
    Serial.println("[DATA] receive");
    get_data(webSocket, message);
  }

}
void send_data(WebSocketsClient  &webSocket, String link_file, int byte_h, int size_of_text)
{
  DynamicJsonBuffer JSONbuffer;
  JsonObject& JSONencoder = JSONbuffer.createObject();
  JSONencoder["action"] = "GETTEXT";
  JSONencoder["file"] = link_file;
  JSONencoder["byte"] = byte_h;
  JSONencoder["sizetext"] = size_of_text;

  char JSONmessageBuffer[MAXJSON+1];
  JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  Serial.println("[DATA]Gui du lieu");
  Serial.println(JSONmessageBuffer);
  webSocket.sendTXT(JSONmessageBuffer);
}


int end_data = 0;
void open_file_write_data(String link, String message) // viet tiep vao file
{
  SPIFFS.begin();
  File f = SPIFFS.open(("/" + link).c_str(), "a");
  if (!f) {
    Serial.println("[FILE]file open failed: /" + link);
  }else{
      Serial.println("[FILE]Mo file ghi thanh cong/" + link);
      f.print(message);
      Serial.printf("[FILE]Ghi thanh cong:");
      Serial.println(message);
      f.close();
      Serial.println("[FILE] Close file");
    }
  SPIFFS.end();
}

void transport_file(String text)
{
  mySerial.write(text);
  Serial.println("[SEND] Send data: " + text);
}

void get_data(WebSocketsClient &webSocket, String message)
{
  DynamicJsonBuffer JSONBuffer;                     //Memory pool
  JsonObject& parsed = JSONBuffer.parseObject(message); //Parse message
   if (!parsed.success()) {   //Check for errors in parsing
    Serial.println("[DATA]Parsing failed");
    return;
  }
  char *link_file = strdup(parsed["file"]);
  int byte_h = parsed["byte"];
  int size_of_text = parsed["sizetext"];
  String text = String(strdup(parsed["text"]));
  int end_string = parsed["end"];
  
  Serial.println("[DATA] File: " + String(link_file));
  Serial.println("[DATA] text: " + text);

  //open_file_write_data(link_file, text);
  transport_file(text);  
  if (end_string == 0) 
  {
    delay(10);
    send_data(webSocket, link_file, byte_h + 1, size_of_text);
    Serial.println("[DATA] Da gui yeu cau");
  }else {
    Serial.println("[DATA] Ket thuc gui data");
  }
}



void setup_websocket()
{
  if (status_wifi())
  {
    // websocket
    Serial.printf("%s\n %d\n %s\n", ipWebSocket,portWebSocket,linkWebSocket);
    webSocket.begin(ipWebSocket,portWebSocket,linkWebSocket);
    // event handler
    webSocket.onEvent(webSocketEvent);
    webSocket.setReconnectInterval(5000);
  }
}



void write_new_license(char* message)
{
      // ma hoa json
       DynamicJsonBuffer file_json;
       JsonObject& file_json_encode = file_json.createObject();
       file_json_encode["NAME_DRIVER"] = name_thiet_bi;
       file_json_encode["SSID_STA"] = SSID_STA;
       file_json_encode["PASS_STA"] = PASS_STA;
       file_json_encode["ipWebSocket"] = ipWebSocket;
       file_json_encode["portWebSocket"] = portWebSocket;
       file_json_encode["linkWebSocket"] = linkWebSocket;
       file_json_encode["license"] = message;

      char JSONmessageBuffer[MAXJSON + 3];// chuyen ve string de gui
      file_json_encode.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
      write_data_wifi(JSONmessageBuffer);
}
