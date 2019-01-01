ESP8266WebServer server(80);
Serial_make mySerial;
WebSocketsClient webSocket;



char* status_wf = "connetion"; // Trang thai wifi
char* SSID_STA = "";         
char* PASS_STA = "";
char* SSID_AP = "Wifi chua"; // defailt name access point wifi
char* PASS_AP = "12345678";  // defailt pass access point wifi
char* license = "qazxswedcvfrtgbn";

 char* ipWebSocket = "192.168.8.105";
 int portWebSocket = 8080;
 char* linkWebSocket = "/Server/actions";
 char* name_thiet_bi = "Default driver";

 void encode_json_send_start(WebSocketsClient  &webSocket);
 void send_json(WebSocketsClient  &webSocket);
 void decode_json_receive(WebSocketsClient &webSocket, String message);
 void write_new_license(char* message);
