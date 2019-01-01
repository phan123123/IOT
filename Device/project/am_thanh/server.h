// server ----------------------------------------------------

//
String anchars = "abcdefghijklmnopqrstuvwxyz0123456789" ;// truong key
String sessioncookie = "";  // sessionkie duy tri dang nhap
void make_session() {
  sessioncookie = "";
  for (int  i = 0; i < 32; i++) sessioncookie += anchars[random(0, anchars.length())]; // random key co trong anchars
}

//
bool check_user_login()
{
  if (server.hasHeader("Cookie")) {
    String cookie = server.header("Cookie"), authk = "c=" + sessioncookie;
    if (cookie.indexOf(authk) != -1) return true;
  }
  return false;
}
void func_header()
{
  if (!check_user_login()) // check server login
  {
    server.sendHeader("Location", "/", true);  //Redirect to our html web page
    server.send(302, "text/plane", "");
  }
  server.send(200, "text/html", "<h1>Welcome, " + server.arg("username") + "!</h1><p>" + status_wf + "</p><form action=\"/writefile\" method=\"POST\"><input type=\"text\" name=\"name_thiet_bi\" placeholder=\"Ten thiet bi\"></br><input type=\"text\" name=\"ssid_sta\" placeholder=\"SSID Station\"></br><input type=\"text\" name=\"pass_sta\" placeholder=\"PASS Station\"></br><input type=\"text\" name=\"ip_web_socket\" placeholder=\"IP WebSocket\"></br><input type=\"text\" name=\"port_web_socket\" placeholder=\"Port WebSocket\"></br><input type=\"text\" name=\"link_web_socket\" placeholder=\"Link WebSocket\"></br><input type=\"text\" name=\"license\" placeholder=\"License\"></br><input type=\"submit\" value=\"Save\"></form>");


}
void func_login()
{

  String web = "             <!DOCTYPE html>"
               "             <html>"
               "             <head>"
               "             <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
               "             <style>"
               "             body {font-family: Arial, Helvetica, sans-serif; background-color: #262525}"
               "             form {border: 3px solid #262525;}"
               "             input[type=text], input[type=password] {"
               "                 width: 100%;"
               "                 padding: 12px 20px;"
               "                 margin: 10px 6px 15px;"
               "                 display: inline-block;"
               "                 border: 1px solid #ccc;"
               "                 box-sizing: border-box; "
               "             }"
               "             .container {"
               "               margin-left: 25%;"
               "               margin-top: 10%;"
               "               width: 40%;"
               "                 padding: 30px 60px 20px;"
               "               box-shadow: 3px 4px 5px 5px rgba(0, 0, 0, 0.2);"
               "               background-color: #bc0505;"
               "               border-radius: 10px;"
               "             }"
               "             button{"
               "               width: 90px;"
               "               height: 35px;"
               "                 box-shadow: 1px 1px 1px 1px rgba(0, 0, 0, 0.2);"
               "             }"
               "               .h2{"
               "                 margin-top:30%;"
               "               }"
               "             </style>"
               "             </head>"
               "             <body>"
               "             <center>"
               "               <h2>&nbsp;</h2>"
               "             </center>"
               "             <form action = \"/login\" \"method=\"POST\">"
               "               <div class=\"container\">"
               "                 <center><h2>LOGIN</h2></center>"
               "                 <label><strong>Username</strong></label>"
               "                 <input type=\"text\"  placeholder=\"Enter Username\" name =\"username\" >"
               "                 <label><b>Password</b></label>"
               "                 <input type=\"password\"  placeholder=\"Enter Password\" name=\"password\" >"
               "                 <center><input type=\"submit\" value=\"Login\"></center>"
               "               </div>"
               "             </form>"
               "             </body>"
               "             </html>";





  String msg = "";
  if ( server.hasArg("username") && server.hasArg("password")
       && server.arg("username") != NULL && server.arg("password") != NULL)
  {
    if (server.arg("username") == "admin" && server.arg("password") == "admin")
    {
      String header = "HTTP/1.1 301 OK\r\nSet-Cookie: c=" + sessioncookie + "\r\nLocation: /\r\nCache-Control: no-cache\r\n\r\n"; //if above values are good, send 'Cookie' header with variable c, with format 'c=sessioncookie'
      server.sendContent(header);
      return ;
    } else {
      msg = "<center><br>Wrong username/password</center>";
    }
  }

  server.send(200, "text/html", web + msg);
}

void func_send_data()
{
  String ss = "Khong co du lieu";
  if ( server.hasArg("data") && server.arg("data") != NULL)
  {
    String s = server.arg("data");
    mySerial.write(s);
    Serial.println("[SEND] Send data: " + s);
    ss = s;
  }
  server.send(200, "text/html", ss);
}
void func_index()
{


}
void func_write_file()
{
  if (  ! server.hasArg("ssid_sta")) { // If the POST request doesn't have username and password data
    server.send(400, "text/plain", "400: Invalid Request");         // The request is invalid, so send HTTP status 400
    return;
  }

  StaticJsonBuffer<MAXJSON> file_json;

  JsonObject& file_json_encode = file_json.createObject();
  if (server.arg("name_thiet_bi") == NULL) file_json_encode["NAME_DRIVER"] = name_thiet_bi;
  else file_json_encode["NAME_DRIVER"] = server.arg("name_thiet_bi");
  
  if (server.arg("ssid_sta") == NULL) file_json_encode["SSID_STA"] = SSID_STA;
  else file_json_encode["SSID_STA"] = server.arg("ssid_sta");

  if (server.arg("pass_sta") == NULL) file_json_encode["PASS_STA"] = PASS_STA;
  else file_json_encode["PASS_STA"] = server.arg("pass_sta");

  if (server.arg("ip_web_socket") == NULL) file_json_encode["ipWebSocket"] = ipWebSocket;
  else file_json_encode["ipWebSocket"] = server.arg("ip_web_socket");

  if (server.arg("port_web_socket") == NULL) file_json_encode["portWebSocket"] = portWebSocket;
  else file_json_encode["portWebSocket"] = server.arg("port_web_socket");

  if (server.arg("link_web_socket") == NULL) file_json_encode["linkWebSocket"] = linkWebSocket;
  else file_json_encode["linkWebSocket"] = server.arg("link_web_socket");

  if (server.arg("license") == NULL) file_json_encode["license"] = license;
  else file_json_encode["license"] = server.arg("license");



  char JSONmessageBuffer[MAXJSON];
  file_json_encode.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  Serial.println("[WIFI]" + String(JSONmessageBuffer));
  write_data_wifi(JSONmessageBuffer);
  server.send(200, "text/html", "<p>Da save da lieu</p><p>Restart ESP8266</p>");
  ESP.restart();
}
void func_not_found()
{

  server.send(404, "text/plain", "404: Not found");
}

void func_test()
{
  Serial.println("test am thanh");
  player.playChunk(sampleMp3, sizeof(sampleMp3));
  server.send(200, "text/html", "ghi file am thanh");
}

void func_status()
{ 
  String data_file = "";
  data_file += "Trang thai wifi: " + String(status_wf)  + "\r\n";
  data_file += "Ten ESP8266:" +  String(name_thiet_bi) +  "\r\n";
  data_file += "Ten wifi truy cap: " + String(SSID_STA) + "\r\n";
  data_file += "Mat khau truy cap: " + String(PASS_STA) + "\r\n";
  data_file += "Dia chi websocket: " + String(ipWebSocket) + "\r\n";
  data_file += "Cong websocket: " + String(portWebSocket) + "\r\n"; 
  data_file += "Link websocket: " + String(linkWebSocket) + "\r\n";
  data_file += "Key ket noi " + String(license) + "\r\n";

  server.send(200, "text/html", data_file);
  
}
void setup_server()
{
  server.on("/", func_header);
  server.on("/login",  func_login);
  server.on("/index", func_index) ;         // sau khi login vao ham dang nhap
  server.on("/writefile", func_write_file);
  server.on("/send", func_send_data);
  server.on("/status", func_status);
  server.on("/test", func_test);
  server.onNotFound(func_not_found);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"
  make_session();
  Serial.println("Set sission");
  const char * headerkeys[] = {"User-Agent", "Cookie"} ;
  size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);
  server.collectHeaders(headerkeys, headerkeyssize );
  Serial.println("Connect session");
  server.begin();                           // Actually start the server
  Serial.println("HTTP server started");

}
