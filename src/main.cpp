#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID "Bay"
#define STAPSK "77777777"
#endif

#define led 2

const char index_html[] PROGMEM = {"<!DOCTYPE html>\n<html>\n<title>W3.CSS</title>\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n<link rel=\"stylesheet\" href=\"https://www.w3schools.com/w3css/4/w3.css\">\n<link rel=\"stylesheet\" href=\"https://www.w3schools.com/lib/w3-theme-teal.css\">\n<link rel=\"stylesheet\" href=\"https://fonts.googleapis.com/css?family=Roboto\">\n<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css\">\n<link rel=\"stylesheet\" href=\"https://combinatronics.com/thanhdanh27600/smarthome/main/src/mystyle.css\">\n\n<body>\n\n    <nav class=\"w3-sidebar w3-bar-block w3-collapse w3-animate-left w3-card\" style=\"z-index:3;width:250px;\"\n        id=\"mySidebar\">\n        <a class=\"w3-bar-item w3-button w3-border-bottom w3-large\" href=\"#\"><img\n                src=\"https://www.w3schools.com/images/w3schools.png\" style=\"width:50%;\"></a>\n        <a class=\"w3-bar-item w3-button w3-hide-large w3-large\" href=\"javascript:void(0)\" onclick=\"w3_close()\">Close\n            <i class=\"fa fa-remove\" style=\"font-size:15px\"></i></a>\n        <a class=\"w3-bar-item w3-button w3-teal\" href=\"#\">Home</a>\n        <a class=\"w3-bar-item w3-button\" href=\"#\">Link 1</a>\n        <a class=\"w3-bar-item w3-button\" href=\"#\">Link 2</a>\n        <a class=\"w3-bar-item w3-button\" href=\"#\">Link 3</a>\n        <a class=\"w3-bar-item w3-button\" href=\"#\">Link 4</a>\n        <a class=\"w3-bar-item w3-button\" href=\"#\">Link 5</a>\n        <div>\n            <a class=\"w3-bar-item w3-button\" onclick=\"myAccordion('demo')\" href=\"javascript:void(0)\">Dropdown <i\n                    class=\"fa fa-caret-down\"></i></a>\n            <div id=\"demo\" class=\"w3-hide\">\n                <a class=\"w3-bar-item w3-button\" href=\"#\">Link</a>\n                <a class=\"w3-bar-item w3-button\" href=\"#\">Link</a>\n                <a class=\"w3-bar-item w3-button\" href=\"#\">Link</a>\n            </div>\n        </div>\n    </nav>\n\n    <div class=\"w3-overlay w3-hide-large w3-animate-opacity\" onclick=\"w3_close()\" style=\"cursor:pointer\" id=\"myOverlay\">\n    </div>\n\n    <div class=\"w3-main\" style=\"margin-left:250px;\">\n\n        <div id=\"myTop\" class=\"w3-container w3-top w3-theme w3-large\">\n            <p><i class=\"fa fa-bars w3-button w3-teal w3-hide-large w3-xlarge\" onclick=\"w3_open()\"></i>\n                <span id=\"myIntro\" class=\"w3-hide\">W3.CSS: Introduction</span>\n            </p>\n        </div>\n\n        <header class=\"w3-container w3-theme\" style=\"padding:64px 32px\">\n            <h1 class=\"w3-xxxlarge\">W3.CSS</h1>\n        </header>\n\n        <div class=\"w3-container\" style=\"padding:32px\">\n\n            <h2>What is W3.CSS?</h2>\n\n            <p>W3.CSS is a modern CSS framework with built-in responsiveness:</p>\n\n            <ul class=\"w3-leftbar w3-theme-border\" style=\"list-style:none\">\n                <li>Smaller and faster than other CSS frameworks.</li>\n                <li>Easier to learn, and easier to use than other CSS frameworks.</li>\n                <li>Uses standard CSS only (No jQuery or JavaScript library).</li>\n                <li>Speeds up mobile HTML apps.</li>\n                <li>Provides CSS equality for all devices. PC, laptop, tablet, and mobile:</li>\n            </ul>\n            <br>\n            <img src=\"https://raw.githubusercontent.com/thanhdanh27600/STM32-Nucleo/main/lab5/F103RB.jpg\"\n                style=\"width:100%\" alt=\"Responsive\">\n            <div style=\"padding: 1em 0em 0em 1em\" <span> Toggle </span>\n                <label class=\"switch\">\n                    <input type=\"checkbox\" id=\"switch1\" onclick=\"checkbox()\">\n                    <span class=\"slider round\"></span>\n                </label>\n            </div>\n            <hr>\n            <h2>W3.CSS is Free</h2>\n            <p>W3.CSS is free to use. No license is necessary.</p>\n\n            <hr>\n            <h2>Easy to Use</h2>\n            <div class=\"w3-container w3-sand w3-leftbar\">\n                <p><i>Make it as simple as possible, but not simpler.</i><br>\n                    Albert Einstein</p>\n            </div>\n\n            <hr>\n            <h2>W3.CSS Web Site Templates</h2>\n\n            <p>We have created some responsive W3CSS templates for you to use.</p>\n            <p>You are free to modify, save, share, use or do whatever you want with them</p>\n\n        </div>\n\n        <footer class=\"w3-container w3-theme\" style=\"padding:32px\">\n            <p>Footer information goes here</p>\n        </footer>\n\n    </div>\n\n    <script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js\"></script>\n    <script src=\"https://combinatronics.com/thanhdanh27600/smarthome/main/src/myfunction.js\"></script>\n\n</body>\n\n</html>"};
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