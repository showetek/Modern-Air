#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID ""
#define STAPSK  ""
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

int fanpin = 12;
int co2 = 12;
ESP8266WebServer server(80);

const int led = LED_BUILTIN;

const String postForms = "<html>\
  <head>\
    <title>ESP8266 Web Server POST handling</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Hier co2 werte eintragen</h1><br>\
    <form method=\"post\" enctype=\"application/x-www-form-urlencoded\" action=\"/postform/\">\
      <input type=\"text\" name=\"co2\" ><br>\
      <input type=\"submit\" value=\"Submit\">\
    </form>\
  </body>\
</html>";

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/html", postForms);
  digitalWrite(led, 0);
}


void handleForm() {
  if (server.method() != HTTP_POST) {
    digitalWrite(led, 1);
    server.send(405, "text/plain", "Method Not Allowed");
    digitalWrite(led, 0);
  } else {
    digitalWrite(led, 1);
    String message = "";
    for (uint8_t i = 0; i < server.args(); i++) {
      message += " " + server.argName(i) + ": " +"<"+ server.arg(i)+">"+ + "\n";
    }
    server.send(200, "text/plain", message);
    Serial.println(message);
    digitalWrite(led, 0);
  }
}


void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  

  server.on("/postform/", handleForm);

  
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  analogWrite(fanpin, co2);
}
