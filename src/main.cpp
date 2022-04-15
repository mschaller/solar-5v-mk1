#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>
#include "PRIVATE.h" 

const char* ssid = WLAN_SSID;
const char* password = WLAN_PASS;
int status = WL_IDLE_STATUS;

ESP8266WebServer server(80);

void blink(int count, int time) {
  for(int i = 0; i < count; i++) {
    digitalWrite(LED_BUILTIN, 0);
    delay(time);
    digitalWrite(LED_BUILTIN, 1);
    delay(time);
  }
}

void handleRoot() {
  int reading = analogRead(A0);
  float voltage = reading * 0.004666666667; // cascaded voltage divider - measured by faire MultiMeter-Measuring (220k/100k by NodeMCU and 5K potentiometer to shift Voltage under 3V)

  String response = F("{\"voltage\":");
  response += voltage;
  response += F(",\"a0\":");
  response += reading;
  response += F("}");

  server.send(200, F("text/html"), response);
  blink(2,200);
}

void handleNotFound() {
  String message = F("File Not Found\n\n");
  message += F("URI: ");
  message += server.uri();
  message += F("\nMethod: ");
  message += (server.method() == HTTP_GET) ? F("GET") : F("POST");
  message += F("\nArguments: ");
  message += server.args();
  message += F("\n");
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, F("text/plain"), message);
}

void WIFI_Connect()
{
  bool ipisset = WiFi.localIP().isSet();
  
  if ( WiFi.status() != WL_CONNECTED || !ipisset) {
    WiFi.disconnect();
    Serial.print("Connecting to ");
    Serial.println(ssid);
  
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
	  wifi_station_set_auto_connect(true); // TODO: check if this method makes a selfcoded reconnect obsolete
	  wifi_station_set_hostname(WLAN_NAME);

      // Wait for connection
    for (int i = 0; i < 25; i++) {
      if ( WiFi.status() != WL_CONNECTED ) {
        delay ( 500 );
      }
    }
    Serial.println(WiFi.localIP());
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  blink(8,200);

  Serial.begin(115200);
  delay(10);

     
  WIFI_Connect();
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());

  // server
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);

  ArduinoOTA.setHostname(WLAN_NAME);
  ArduinoOTA.setPassword(WLAN_OTA_PASS);
  ArduinoOTA.begin();

  
  Serial.println(F("solar-5v-mk1"));
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    server.handleClient();
    ArduinoOTA.handle();
  } else {
    WIFI_Connect();
    delay(5000);
  }
}