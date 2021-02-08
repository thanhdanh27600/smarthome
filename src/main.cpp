#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "DHT/DHT.h"

#ifndef STASSID
#define STASSID "Bay"
#define STAPSK "77777777"
#endif

#define DHTPIN 14
#define DHTTYPE DHT22
#define led 2
#define relay1 15
#define light_pin A0
#define motion_pin 4
#define light_second 10
#define timeSeconds 10

/* DHT 22 */
DHT dht(DHTPIN, DHTTYPE);
float temp = 0.0;
float humid = 0.0;
float hic = 0.0; // heat index

/*Timer and variable for motion sensor*/
unsigned long now = millis();
unsigned long lastTrigger = 0;
boolean startTimer = false;
boolean statusMotion = false;

const char index_html[] PROGMEM = {"<!DOCTYPE html>\n<html>\n<title>W3.CSS</title>\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n<link rel=\"stylesheet\" href=\"https://www.w3schools.com/w3css/4/w3.css\">\n<link rel=\"stylesheet\" href=\"https://www.w3schools.com/lib/w3-theme-teal.css\">\n<link rel=\"stylesheet\" href=\"https://fonts.googleapis.com/css?family=Roboto\">\n<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css\">\n<link rel=\"stylesheet\" href=\"https://combinatronics.com/thanhdanh27600/smarthome/main/src/mystyle.css\">\n<style>\n    .d-none{\n        display: none;\n    }\n    .d-block{\n        display: block;\n    }\n</style>\n\n<body>\n\n    <nav class=\"w3-sidebar w3-bar-block w3-collapse w3-animate-left w3-card\" style=\"z-index:3;width:250px;\"\n        id=\"mySidebar\">\n        <a class=\"w3-bar-item w3-button w3-border-bottom w3-large\" href=\"#\"><img\n                src=\"https://www.w3schools.com/images/w3schools.png\" style=\"width:80%;\"></a>\n        <a class=\"w3-bar-item w3-button w3-hide-large w3-large\" href=\"javascript:void(0)\" onclick=\"w3_close()\">Close <i\n                class=\"fa fa-remove\"></i></a>\n        <a class=\"w3-bar-item w3-button w3-teal\" href=\"javascript:void(0)\" onclick=\"home()\">Home</a>\n        <a class=\"w3-bar-item w3-button\" href=\"javascript:void(0)\" onclick=\"link1()\">Link 1</a>\n        <div>\n            <a class=\"w3-bar-item w3-button\" onclick=\"myAccordion('demo')\" href=\"javascript:void(0)\">Dropdown <i\n                    class=\"fa fa-caret-down\"></i></a>\n            <div id=\"demo\" class=\"w3-hide\">\n                <a class=\"w3-bar-item w3-button\" href=\"#\">Link</a>\n                <a class=\"w3-bar-item w3-button\" href=\"#\">Link</a>\n                <a class=\"w3-bar-item w3-button\" href=\"#\">Link</a>\n            </div>\n        </div>\n    </nav>\n\n    <div class=\"w3-overlay w3-hide-large w3-animate-opacity\" onclick=\"w3_close()\" style=\"cursor:pointer\" id=\"myOverlay\">\n    </div>\n\n    <div class=\"w3-main\" style=\"margin-left:250px;\">\n\n        <div id=\"myTop\" class=\"w3-container w3-top w3-theme w3-large\">\n            <p><i class=\"fa fa-bars w3-button w3-teal w3-hide-large w3-xlarge\" onclick=\"w3_open()\"></i>\n                <span id=\"myIntro\" class=\"w3-hide\">W3.CSS: Introduction</span>\n            </p>\n        </div>\n\n        <header class=\"w3-container w3-theme\" style=\"padding:64px 32px\">\n            <h1 class=\"w3-xxxlarge\">W3.CSS</h1>\n        </header>\n\n        <div id=\"home\" class=\"w3-container\" style=\"padding:32px\">\n\n            <h2>What is W3.CSS?</h2>\n\n            <p>W3.CSS is a modern CSS framework with built-in responsiveness:</p>\n\n            <ul class=\"w3-leftbar w3-theme-border\" style=\"list-style:none\">\n                <li>Smaller and faster than other CSS frameworks.</li>\n                <li>Easier to learn, and easier to use than other CSS frameworks.</li>\n                <li>Uses standard CSS only (No jQuery or JavaScript library).</li>\n                <li>Speeds up mobile HTML apps.</li>\n                <li>Provides CSS equality for all devices. PC, laptop, tablet, and mobile:</li>\n            </ul>\n            <br>\n            <div>\n                <img src=\"https://raw.githubusercontent.com/thanhdanh27600/STM32-Nucleo/main/lab5/F103RB.jpg\" width: 50%\n                    height: 50% alt=\"My images\">\n            </div>\n            <div style=\"padding: 1em 0em 0em 1em\"> </div><span> Toggle </span>\n            <label class=\"switch\">\n                <input type=\"checkbox\" id=\"switch1\" onclick=\"checkbox()\" checked>\n                <span class=\"slider round\"></span>\n            </label>\n        </div>\n        <hr>\n        \n        <div id=\"link1\">\n            <h2>W3.CSS is Free</h2>\n            <p>W3.CSS is free to use. No license is necessary.</p>\n            <button id=\"button\" onclick=\"dht()\"> DHT </button>\n            <hr>\n            <h2>Easy to Use</h2>\n            <div class=\"w3-container w3-sand w3-leftbar\">\n                <p><i>Make it as simple as possible, but not simpler.</i><br>\n                    Albert Einstein</p>\n            </div>\n\n            <hr>\n            <h2>W3.CSS Web Site Templates</h2>\n\n            <p>We have created some responsive W3CSS templates for you to use.</p>\n            <p>You are free to modify, save, share, use or do whatever you want with them</p>\n        </div>\n\n    </div>\n\n    <footer class=\"w3-container w3-theme\" style=\"padding:32px\">\n        <p>Footer information goes here</p>\n    </footer>\n\n    </div>\n\n    <script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js\"></script>\n    <script src=\"https://combinatronics.com/thanhdanh27600/smarthome/main/src/myfunction.js\"></script>\n    <script>\n        function link1() {\n            reset();\n            document.getElementById(\"home\").classList.add('d-none');\n            document.getElementById(\"link1\").classList.add('d-block');\n        }\n        function home() {\n            reset();\n            document.getElementById(\"home\").classList.add('d-block');\n            document.getElementById(\"link1\").classList.add('d-none');\n        }\n        function reset() {\n            document.getElementById(\"home\").classList.remove('d-none','d-block');\n            document.getElementById(\"link1\").classList.remove('d-none','d-block');\n        }\n        function init(){\n            var url_switch1 = \"/checkstatus\";\n            $.ajax({\n                url: url_switch1,\n                dataType: \"text\",\n                error: function() {\n                alert(\"Error! Check your connection and try again.\");\n            },\n                success: function (response) {\n                if (response == '0') {\n                 document.getElementById(\"switch1\").removeAttribute(\"checked\");\n                    }\n            }\n            })\n        }\n        function dht() {\n             // for check dht\n             url_dht = \"/dht\";\n             $.ajax({\n                url: url_dht,\n                dataType: \"text\",\n                error: function() {\n                alert(\"Error! Check your connection and try again.\");\n            },\n                success: function () {\n                 alert(\"DHT Read!!\");\n                }\n            })\n        }\n        init();\n    </script>\n\n</body>\n\n</html>"};
//<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js"></script>
                                   const char *ssid = STASSID;
const char *password = STAPSK;

ESP8266WebServer server(80);

ICACHE_RAM_ATTR void detectsMovement(){
  if (!startTimer){
    Serial.println("MOTION DETECTED!!!");
    digitalWrite(led, HIGH);
    startTimer = true;
    lastTrigger = millis();
  }
    
}

void handleRoot(){
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
  digitalWrite(relay1, !digitalRead(relay1));
  server.send(200, "text/plain", String(digitalRead(relay1)));
}

void handleDHT(){
  humid = dht.readHumidity();
  temp = dht.readTemperature();
  if (isnan(humid) || isnan(temp)){
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  hic = dht.computeHeatIndex(temp, humid, false);
  Serial.print(F("Humidity: "));
  Serial.print(humid);
  Serial.print(F("%  Temperature: "));
  Serial.print(temp);
  Serial.print(F("°C "));
  Serial.print(F("Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
}

void turnOnLightSensor(){
  statusMotion = !statusMotion;
  server.send(200, "text/plain", "sucessfully");
}

void handleLightsensor(){
  Serial.print(analogRead(light_pin));
}

void handleMotion(){
  Serial.println(digitalRead(motion_pin));
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

void handleCheckStatus(){
  server.send(200, "text/plain", String(digitalRead(relay1)));
}

void setup(void)
{
  pinMode(led, OUTPUT);
  pinMode(relay1, OUTPUT);
  pinMode(motion_pin, INPUT);

  digitalWrite(led, LOW);
  digitalWrite(relay1, LOW);
  dht.begin();

  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(motion_pin), detectsMovement, RISING);
  WiFi.mode(WIFI_STA);
  //WiFi.begin(ssid, password);
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
  server.on("/checkstatus", handleCheckStatus);
  server.on("/dht", handleDHT);
  server.on("/turnonlightsensor", turnOnLightSensor());
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  //server.handleClient();
  //handleMotion();
  now = millis();
  // Turn off the LED after the number of seconds defined in the timeSeconds variable
  if (startTimer && (now - lastTrigger > (timeSeconds * 1000))){
    Serial.println("Motion stopped...");
    digitalWrite(led, LOW);
    startTimer = false;
  }
}