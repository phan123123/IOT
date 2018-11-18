#define MAXJSON 500
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <FS.h>   // Include the SPIFFS library
#include <WebSocketsClient.h>
#include "serial_v1.h"
#include "file.h"

#include "data.h"
#include "setup_wifi.h"  // setup wifi
#include "server.h"
#include "websocket.h"

// Create a webserver object that listens for HTTP request on port 80


String getContentType(String filename);
bool handleFileRead(String path);
void handleFileUpload();




//code --------------------------------------------
void setup()
{
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  for(int i = 1; i<= 3; i++)
  {
    Serial.printf("%d ", i);
    delay(1000);

  }
  setup_wifi();
  mySerial.open_send(12 ,14, 1); //D6 D5

  Serial.println('\n');

  // server
  setup_server();
  setup_websocket();
}

void loop() {
  webSocket.loop();
  server.handleClient();
  if (WiFi.status() != WL_CONNECTED) status_wf = "connect error";
}
