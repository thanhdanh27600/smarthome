#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID "Bay"
#define STAPSK "77777777"
#endif

#define led 2

const char index_html[] PROGMEM = {"<!DOCTYPE html>\n<html>\n<head>\n\t<script src=\"https://combinatronics.com/thanhdanh27600/smarthome/main/src/jquery.min.js\"></script>\n\t<script src=\"https://combinatronics.com/thanhdanh27600/smarthome/main/src/myfunction.js\"></script>\n</head>\n<body>\n\n<img id=\"myImage\" onclick=\"changeOutput()\" src=\"https://github.com/thanhdanh27600/smarthome/blob/main/bulb/pic_bulboff.gif?raw=true\" width=\"100\" height=\"180\">\n\n<p>Click the light bulb to turn on/off the light.</p>\n\n</body>\n</html>\n"};
//<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js"></script>
const char *ssid = STASSID;
const char *password = STAPSK;

ESP8266WebServer server(80);

void handleRoot()
{
  digitalWrite(led, 1);
  server.send_P(200, "text/html", index_html);
  digitalWrite(led, 0);
}

// void handleBulbOn(){
//   server.send_P(200, "image/gif", bulb_on_gif, sizeof(bulb_on_gif));
// }

// void handleBulbOff()
// {
//   server.send_P(200, "image/gif", bulb_off_gif, sizeof(bulb_off_gif));
// }

// void handlejQuery(){
//   server.send_P(200, "application/js", jquery_js); 
// }

// void handleMyfuntions(){
//   server.send_P(200, "application/js", myFunction_js);
// }

void handleRelay(){
  String response = "";
  digitalWrite(led, !digitalRead(led));
  response += digitalRead(led);
  server.send(200, "text/plain", response);
}

void handleNotFound()
{
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void)
{
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  // server.on("/pic_bulbon.gif", handleBulbOn);
  // server.on("/pic_bulboff.gif", handleBulbOff);
  // server.on("/js/jquery.min.js", handlejQuery);
  // server.on("/js/myJsFunctions.js", handleMyfuntions);
  //server.on("/digital_output/toggle", handleRelay);
  server.on("/switch1", handleRelay);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void)
{
  server.handleClient();
}