#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include "DHT/DHT.h"

#ifndef STASSID
#define STASSID "Bay"
#define STAPSK "77777777"
#endif

#define DHTPIN 5
#define DHTTYPE DHT22
#define led 2
#define relay1 16 //relay from D5-D8
#define relay2 14
#define relay3 12
#define relay4 13
#define light_pin A0
#define motion_pin 4
//#define touch_pin 3

#define light_second 10
#define timeMotion 7000 //time for detect a motion
#define timeAlarm 200 //time for detect a motion

/* Software timer */
extern "C"{
#include "user_interface.h"
}
os_timer_t myTimer;
boolean tickOccured = false;
int hour = -1;
int minute = -1;

/* DHT 22 */
DHT dht(DHTPIN, DHTTYPE);
float temp = 0.0;
float humid = 0.0;
float hic = 0.0; // heat index

/*Timer and variable for motion sensor*/
unsigned long now = 0;
unsigned long lastTrigger = 0;
boolean startTimer = false;
boolean motionStatus = true;
boolean statusLight = true;
unsigned long countMotion = 0;

/* Variable for distance */
#define trig_pin 0
#define echo_pin 15
#define THRESHOLD_DISTANCE 100
unsigned long duration;    // variable for the duration of sound wave travel
unsigned int distance = 0; // variable for the distance measurement
unsigned int count_distance = 0;

/* Timer and variable for alarm*/
boolean statusAlarm = true;

const char index_html[] PROGMEM = {"<!DOCTYPE html>\n<html>\n<title>SmartHome - Control Center</title>\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n<link rel=\"stylesheet\" href=\"https://www.w3schools.com/w3css/4/w3.css\">\n<link rel=\"stylesheet\" href=\"https://www.w3schools.com/lib/w3-theme-teal.css\">\n<link rel=\"stylesheet\" href=\"https://fonts.googleapis.com/css?family=Roboto\">\n<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css\">\n<link rel=\"stylesheet\" href=\"https://combinatronics.com/thanhdanh27600/smarthome/main/src/mystyle.css\">\n<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css\"\n    integrity=\"sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm\" crossorigin=\"anonymous\">\n<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/weather-icons/2.0.10/css/weather-icons.min.css\">\n\n<style>\n    body {\n        position: relative;\n        height: 100vh;\n    }\n\n    .d-none {\n        display: none;\n    }\n\n    .d-block {\n        display: block;\n    }\n\n    .d-flex {\n        display: flex;\n    }\n</style>\n\n<body onload=\"startTime()\">\n    <nav class=\"w3-sidebar w3-bar-block w3-collapse w3-animate-left w3-card\" style=\"z-index:3;width:250px;\"\n        id=\"mySidebar\">\n        <a class=\"w3-bar-item w3-button w3-border-bottom w3-large\" href=\"#\"><img\n                src=\"https://raw.githubusercontent.com/thanhdanh27600/smarthome/main/esp8266-nodemcu-wifi-module-500x500.jpg\"\n                style=\"width:80%;\"></a>\n        <a class=\"w3-bar-item w3-button w3-hide-large w3-large\" href=\"javascript:void(0)\" onclick=\"w3_close()\">Close <i\n                class=\"fa fa-remove\"></i></a>\n        <a id=\"option1\" class=\"w3-bar-item w3-button w3-teal\" href=\"javascript:void(0)\" onclick=\"option1()\"></a>\n        <a id=\"option2\" class=\"w3-bar-item w3-button\" href=\"javascript:void(0)\" onclick=\"option2()\"></a>\n        <a id=\"option3\" class=\"w3-bar-item w3-button\" href=\"javascript:void(0)\" onclick=\"option3()\"></a>\n        <a id=\"option4\" class=\"w3-bar-item w3-button\" href=\"javascript:void(0)\" onclick=\"option4()\"></a>\n    </nav>\n\n    <div class=\"w3-overlay w3-hide-large w3-animate-opacity\" onclick=\"w3_close()\" style=\"cursor:pointer\" id=\"myOverlay\">\n    </div>\n\n    <div class=\"w3-main\" style=\"margin-left:250px;\">\n\n        <div id=\"myTop\" class=\"w3-container w3-top w3-theme w3-large\">\n            <p><i class=\"fa fa-bars w3-button w3-teal w3-hide-large w3-xlarge\" onclick=\"w3_open()\"></i>\n                <span id=\"myIntro\" class=\"w3-hide w3-xlarge\" style=\"padding-top: 10px;\"></span>\n            </p>\n        </div>\n\n        <header class=\"w3-container w3-theme\" style=\"padding:50px 0px 0px 1em\">\n            <h1 class=\"w3-xxlarge\">Smarthome <span\n                    style=\"float: right; margin-right: 0.5em; font-size: x-large; padding-top: 10px;\" id=\"clock\"></span>\n            </h1>\n        </header>\n        <div style=\"float: right;margin-right: 1em;font-style: italic;\" id=\"last_update\">Last update: </div>\n        <div id=\"general_control\" class=\"w3-container\" style=\"padding:0px 32px;\">\n            <hr>\n            <h2 id=\"heading_option1\"></h2>\n            <p style=\"padding-top: 1em;\">\n                <label class=\"switch\">\n                    <input type=\"checkbox\" id=\"switch1\" onclick=\"relay(1)\">\n                    <span class=\"slider round\"></span>\n                </label>\n                <span id=\"relay1\" style=\"padding: 0.5em;\"></span>\n            </p>\n            <p>\n                <label class=\"switch\">\n                    <input type=\"checkbox\" id=\"switch2\" onclick=\"relay(2)\">\n                    <span class=\"slider round\"></span>\n                </label>\n                <span id=\"relay2\" style=\"padding-left: 0.5em;\"></span>\n            </p>\n            <p>\n                <label class=\"switch\">\n                    <input type=\"checkbox\" id=\"switch3\" onclick=\"relay(3)\">\n                    <span class=\"slider round\"></span>\n                </label>\n                <span id=\"relay3\" style=\"padding-left: 0.5em;\"></span>\n            </p>\n            <p>\n                <label class=\"switch\">\n                    <input type=\"checkbox\" id=\"switch4\" onclick=\"relay(4)\">\n                    <span class=\"slider round\"></span>\n                </label>\n                <span id=\"relay4\" style=\"padding-left: 0.5em;\"></span>\n            </p>\n            <hr>\n        </div>\n        <div id=\"dht_22\" style=\"padding:32px;\">\n            <hr>\n            <h2 id=\"heading_option2\" style=\"padding-bottom: 0.5em;\"></h2>\n            <div class=\"d-flex\">\n                <span id=\"real_temp\" style=\"font-size: 80px; padding-left: 0.4em;\">Update</span>\n                <div style=\"margin: auto 0em 0em 0em ;\">\n                    <i class=\"wi wi-celsius\" style=\"font-size: 100px;\"></i>\n                </div>\n            </div>\n            <span>\n                <i class=\"wi wi-thermometer\" style=\"font-size: x-large;\"></i>\n                <span id=\"temparature\" style=\"font-size: large; font-weight: bold; padding: 2em 5em 0em 1em;\"> :\n                    Update</span>\n            </span>\n            <span>\n                <i class=\"wi wi-raindrops\" style=\"font-size: xx-large;\"></i>\n                <span id=\"relative_humid\" style=\"font-size: large; font-weight: bold; padding: 2em 0em 0em 1em;\"> :\n                    Update</span>\n            </span>\n\n            <div style=\"padding-top: 2em;\">\n                <button class=\"btn btn-info\" id=\"refresh_button\" onclick=\"dht()\"></button>\n            </div>\n            <div style=\"padding-top: 2em;\">\n                <button class=\"btn btn-info\" id=\"light_button\" onclick=\"light()\">light</button>\n            </div>\n        </div>\n        <div id=\"general_setting\" style=\"padding:32px;\">\n            <hr>\n            <h2 id=\"heading_option3\"></h2>\n            <p>\n                <label class=\"switch\">\n                    <input type=\"checkbox\" onclick=\"toggleMotionSensor()\">\n                    <span class=\"slider round\"></span>\n                </label>\n                <span id=\"motion_sensor\" style=\"padding-left: 0.5em;\"></span>\n            </p>\n\n            <p>\n                <label class=\"switch\">\n                    <input type=\"checkbox\" onclick=\"toggleAlarmStatus()\">\n                    <span class=\"slider round\"></span>\n                </label>\n                <span id=\"alarm_status\" style=\"padding-left: 0.5em;\"></span>\n            </p>\n            <p>\n                <label class=\"switch\">\n                    <input type=\"checkbox\" onclick=\"handleLanguage()\">\n                    <span class=\"slider round\"></span>\n                </label>\n                <span id=\"change_language\" value=\"vie\" style=\"padding-left: 0.5em;\"></span>\n            </p>\n            <div>\n                <img src=\"https://raw.githubusercontent.com/thanhdanh27600/smarthome/main/ESP-12E-Development-Board-ESP8266-NodeMCU-Pinout.png\"\n                    style=\"width: 100%; height: auto;\" alt=\"My images\">\n            </div>\n            <hr>\n        </div>\n        <div id=\"security_camera\" style=\"padding:32px;\">\n            <hr>\n            <h2 id=\"heading_option4\">Camera</h2>\n            <button class=\"btn btn-info\" id=\"config_camera\" onclick=\"refreshCamera()\">Config</button>\n            <div>\n                <iframe id=\"iframe_camera\" src=\"http://thanhdanh2000.cameraddns.net/doc/page/config.asp\"\n                    style=\"width: 100%;height: 100vh;\" title=\"description\"></iframe>\n            </div>\n        </div>\n    </div>\n\n    <footer class=\"w3-container w3-theme\" style=\"padding: 1em;width: 100%;position: absolute;\">\n        <p style=\"margin-top: 5px; margin-bottom: 5px;\">Pham Thanh Danh, Spring 2021</p>\n    </footer>\n\n    </div>\n\n    <script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js\"></script>\n    <script src=\"https://combinatronics.com/thanhdanh27600/smarthome/main/src/myfunction.js\"></script>\n    <script>\n        /* Function send to Node MCU */\n\n        function light() {\n            // for check dht\n            url_dht = \"/light\";\n            $.ajax({\n                url: url_dht,\n                dataType: \"text\",\n                error: function () {\n                    alert(\"Error! Check your connection and try again.\");\n                },\n                success: function (response) {\n                    if (response == \"failed\") {\n                        alert(\"Cannot read Light\");\n                        return;\n                    } else {\n                        alert(response);\n                    }\n                }\n            })\n        }\n\n        function configCamera() {\n            document.getElementById('iframe_camera').src = 'http://thanhdanh2000.cameraddns.net/doc/page/config.asp';\n        }\n\n        function refreshCamera() {\n            //setTimeout(()=>{document.getElementById('iframe_camera').style.display=\"none\",1000});\n            //setInterval(() =>{document.getElementById('image_camera').src=\"http://thanhdanh2000.cameraddns.net/Streaming/channels/1/picture?\" +  new Date().getTime();},3000)\n            document.getElementById('iframe_camera').src =\n                'http://thanhdanh2000.cameraddns.net/ISAPI/Streaming/channels/102/httpPreview';\n        }\n\n        function refreshTime() {\n            var now = new Date();\n            url = \"/refreshtime?hour=\" + now.getHours() + \"&minute=\" + now.getMinutes();\n            $.ajax({\n                url: this.url,\n                dataType: \"text\",\n                error: function () {\n                    alert(\"Error! Check your connection and try again.\");\n                },\n                success: function (response) {\n                    document.getElementById('last_update').innerText = \"Last update: \" + response;\n                }\n            })\n        }\n\n        function init() {\n            option1();\n            updateTextVIE();\n        }\n\n        function update() {\n            var url_switch1 = \"/checkstatus\";\n            $.ajax({\n                url: url_switch1,\n                dataType: \"text\",\n                error: function () {\n                    alert(\"Error! Check your connection and try again.\");\n                    return;\n                },\n                success: function (response) {\n                    if (response == '1') {\n                        document.getElementById(\"switch1\").setAttribute(\"checked\", \"\");\n                    }\n                }\n            })\n            refreshTime();\n        }\n\n        function toggleMotionSensor() {\n            url = \"/motionsensor?toggle=true\";\n            $.ajax({\n                url: this.url,\n                dataType: \"text\",\n                error: function () {\n                    alert(\"Error! Check your connection and try again.\");\n                },\n                success: function (response) {\n                    alert(\"Toggle Motion Sensor sucessfully, State \" + response);\n                }\n            })\n        }\n\n        function toggleAlarmStatus() {\n            url = \"/alarmstatus?toggle=true\";\n            $.ajax({\n                url: this.url,\n                dataType: \"text\",\n                error: function () {\n                    alert(\"Error! Check your connection and try again.\");\n                },\n                success: function (response) {\n                    alert(\"Toggle Alarm status sucessfully, State \" + response);\n                }\n            })\n        }\n        update();\n        init();\n    </script>\n\n</body>\n\n</html>"};

const char *ssid = STASSID;
const char *password = STAPSK;

ESP8266WebServer server(80);

// start of timerCallback
void timerCallback(void* pArg){
  tickOccured = true;
} // End of timerCallback


ICACHE_RAM_ATTR void detectsMovement()
{
  if (!startTimer)
  {
    countMotion++;
    Serial.print("MOTION DETECTED!!!  count = " + String(countMotion));
    startTimer = true;
    lastTrigger = millis();
    /* Light control */
    if (statusLight)
    {
      if (digitalRead(relay3) == LOW)
        digitalWrite(relay3, LOW);
    }
    /* Alarm control */
    if (statusAlarm)
    {
      if (digitalRead(relay4) == LOW)
        digitalWrite(relay4, LOW);
    }
  }
}

void user_init(void) {
  os_timer_setfn(&myTimer, timerCallback, NULL);
  os_timer_arm(&myTimer, 10000, true);
  pinMode(led, OUTPUT);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(motion_pin, INPUT);
  //pinMode(touch_pin, INPUT);
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);

  digitalWrite(led, HIGH);
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  dht.begin();

  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(motion_pin), detectsMovement, RISING);
}

void handleRoot(){
  digitalWrite(led, 1);
  server.send_P(200, "text/html", index_html);
  digitalWrite(led, 0);
}

// void handlejQuery(){
//   server.send_P(200, "application/js", jquery_js); 
// }

// void handleMyfuntions(){
//   server.send_P(200, "application/js", myFunction_js);
// }

void toggleRelay(char pin){
  String response = "";
  switch(pin){
    case '1':
      digitalWrite(relay1, !digitalRead(relay1));
      Serial.println("toggle relay1");
      response += !digitalRead(relay1);
      break;
    case '2':
      digitalWrite(relay2, !digitalRead(relay2));
      Serial.println("toggle relay2");
      response += !digitalRead(relay2);
      break;
    case '3': //light with motion
      digitalWrite(relay3, !digitalRead(relay3));
      Serial.println("toggle relay3");
      response += !digitalRead(relay3);
      break;
    case '4': //alarm
      digitalWrite(relay4, !digitalRead(relay4));
      Serial.println("toggle relay4");
      response += !digitalRead(relay4);
      break;
  }
  server.send(200, "text/plain", response);
}

void handleRelay()
{
  toggleRelay(server.arg(0)[0]);
}

void handleDHT(){
  humid = dht.readHumidity();
  temp = dht.readTemperature();
  if (isnan(humid) || isnan(temp)){
    server.send(200, "text/plain", "failed");
    return;
  }
  hic = dht.computeHeatIndex(temp, humid, false);
  char buffer[50];
  sprintf(buffer, "{\"humid\":%5.2f,\"temp\":%5.2f,\"hic\":%5.2f}",humid,temp,hic);
  server.send(200, "text/plain", buffer);
}

void handleLight(){
  server.send(200, "text/plain", String(analogRead(light_pin)));
}

    void
    handleMotionSensor()
{
  statusLight = !statusLight;
  server.send(200, "text/plain", String(statusLight));
}

void handleAlarmStatus(){
    statusAlarm = !statusAlarm;
    server.send(200, "text/plain", String(statusAlarm));
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
  server.send(200, "text/plain", String(!digitalRead(relay1)));
}

void handleTime(){
  char buffer[20];
  hour = (int)strtol(server.arg(0).c_str(), (char **)NULL, 10);
  minute = (int)strtol(server.arg(1).c_str(), (char **)NULL, 10);
  sprintf(buffer, "Updated Time: %d:%d", hour, minute);
  server.send(200, "text/plain", server.arg(0) + ':' + server.arg(1));
}

void updateMotionSensor(){
  now = millis();
  // Turn off the LED after the number of seconds defined in the timeSeconds variable
  if (startTimer)
  {
    if (now - lastTrigger > (timeMotion))
    {
      Serial.println("Motion stopped...");
      if (statusLight) digitalWrite (relay3, HIGH);
      startTimer = false;
    }
    if (now - lastTrigger > (timeAlarm))
    {
      if (statusAlarm) digitalWrite(relay4, HIGH);
    }
  }
}

void handleDistance(){
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echo_pin, HIGH);
  // Calculating the distance
  distance += duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  if (++count_distance > 100)
  {
    Serial.print("Distance: ");
    Serial.print(distance / 100);
    Serial.println(" cm");
    count_distance = 0;
    distance = 0;
  }
}

void setup(void)
{
  user_init();
  WiFi.mode(WIFI_STA);
  //WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  /* OTA */
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
    {
      type = "sketch";
    }
    else
    { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
    {
      Serial.println("Auth Failed");
    }
    else if (error == OTA_BEGIN_ERROR)
    {
      Serial.println("Begin Failed");
    }
    else if (error == OTA_CONNECT_ERROR)
    {
      Serial.println("Connect Failed");
    }
    else if (error == OTA_RECEIVE_ERROR)
    {
      Serial.println("Receive Failed");
    }
    else if (error == OTA_END_ERROR)
    {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  /* End OTA */

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
  server.on("/relay", handleRelay);
  server.on("/checkstatus", handleCheckStatus);
  server.on("/dht", handleDHT);
  server.on("/light", handleLight);
  server.on("/motionsensor", handleMotionSensor);
  server.on("/alarmstatus", handleAlarmStatus);
  server.on("/refreshtime", handleTime);
  server.onNotFound(handleNotFound);

  server.begin();
}

void loop(void){
  server.handleClient();
  ArduinoOTA.handle();
  if (motionStatus) 
    updateMotionSensor();

  // if(now%2000)
  //   Serial.println(analogRead(light_pin));
  //handleDistance();
  // if (tickOccured == true){
  //   Serial.println("Tick Occurred");
  //   tickOccured = false;
  // }
}