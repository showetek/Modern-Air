//Libs
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

//Define
#ifndef STASSID
#define STASSID "WIFI-SSID"
#define STAPSK  "WIFI-Pass"
#endif

//Const Var
const char* ssid     = STASSID;
const char* password = STAPSK;

//Var
int fanpin = 12;
int co2 = 12;

//Webserver erstellen
ESP8266WebServer server(80);

//Minimierte Version der Page
const String postForms = "<!DOCTYPE html><html lang=\"de\"><head><meta charset=\"UTF-8\"><!--<meta http-equiv=\"refresh\" content=\"5; URL=http://localhost/Modern%20Air/src/\" /> --><meta name=\"viewport\" content=\"width=device-width,initial-scale=1\"><link href=\"../Logo_min.svg\" rel=\"icon\" type=\"image/svg\"><link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.1/css/all.min.css\"><script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js\"></script><title>Modern Air - Lüften neu definiert | Webserver</title><style>body{margin:0;font-family:Arial,Helvetica,sans-serif}.navbar{background-color:#29abe2;overflow:hidden;padding:0 240px 0 240px}.dropdown{text-align:center;float:left;overflow:hidden}.dropbtn{font-size:17px;border:none;outline:0;color:#fff;padding:14px 16px;background-color:inherit;font-family:inherit;margin:0;font-weight:700}.dropdown-content{display:none;position:absolute;background-color:#f9f9f9;min-width:200px;box-shadow:0 8px 16px 0 rgba(0,0,0,.2);z-index:1}.dropdown-content a{float:none;color:#000;padding:12px 16px;text-decoration:none;display:block;text-align:left}.dropdown-content a:hover{background-color:#ddd;color:#000}.dropdown:hover .dropdown-content{display:block}.banner{padding:0 240px 0 240px;background-color:#f5f5f5}.footer{position:fixed;bottom:0;width:100%;background-color:#f5f5f5;color:#000;align-items:center;display:flex;justify-content:space-evenly}.footer img{max-width:50px;max-height:50px}.footer a{color:#0366d6;text-decoration:none}.footer i{padding-right:2px}.footer a:hover{text-decoration:underline}.box{margin:20px 240px 20px 240px}.box-header{padding:16px;margin:-1px -1px 0;border:1px solid;border-top-left-radius:6px;border-top-right-radius:6px;background-color:#f1f8ff;border-color:#c1ddff}.box-footer{padding:16px;margin:-1px -1px 0;border:1px solid;border-bottom-left-radius:6px;border-bottom-right-radius:6px;background-color:#fff;border-color:#e0e3e7}.container{margin:20px 240px 20px 240px;display:flex;flex-direction:row;justify-content:space-between;flex-wrap:wrap}</style></head><body><!--Navbar--><div class=\"navbar\"><div class=\"dropdown\"><button class=\"dropbtn\"><i class=\"fas fa-fan\"></i> Lüfter-Steuerung <i class=\"fa fa-caret-down\"></i></button><div class=\"dropdown-content\"><a href=\"/fan_on\">Aktivieren</a> <a href=\"/fan_off\">Deaktivieren</a></div></div><div class=\"dropdown\"><button class=\"dropbtn\"><i class=\"fas fa-code\"></i> Demo <i class=\"fa fa-caret-down\"></i></button><div class=\"dropdown-content\"><a href=\"#\">CO2-Eintragen</a> <a href=\"#\">Tmp-Eintragen</a></div></div></div><!--Banner--><div class=\"banner\"><img src=\"../Logo.svg\" alt=\"Logo-Banner\"></div><!--Lüfter--><div class=\"box\"><div class=\"box-header\"><i class=\"fas fa-fan\"></i> Lüfter</div><div class=\"box-footer\">Inhalt</div></div><!--Container--><div class=\"container\"><!--CO2--><div style=\"width:49%\"><div class=\"box-header\"><i class=\"fas fa-cloud\"></i> CO2-Gehalt</div><div class=\"box-footer\" id=\"co2inhalt\">Inhalt</div></div><!--Tmp--><div style=\"width:49%\"><div class=\"box-header\"><i class=\"fas fa-thermometer-half\"></i> Temperatur</div><div class=\"box-footer\">Inhalt</div></div></div><!--Footer--><div class=\"footer\"><a href=\"#\"><i class=\"fas fa-info-circle\"></i>Impressum</a> <img src=\"../Logo_min.svg\" alt=\"Logo_min\"> <a href=\"https://github.com/showetek/Modern-Air\"><i class=\"fab fa-github\"></i>GitHub</a></div><script>function updateDiv(){}window.setInterval(updateDiv(),1e4)</script></body></html>";

//Page auf root-Verzeichnis anzeigen
void handleRoot() {
  server.send(200, "text/html", postForms);
}

//Post-request verarbeiten
void handleForm() {
  if (server.method() != HTTP_POST) {
    server.send(405, "text/plain", "Method Not Allowed");
  } else {
    String message = "";
    for (uint8_t i = 0; i < server.args(); i++) {
      message += " " + server.argName(i) + ": " +"<"+ server.arg(i)+">"+ + "\n";
    }
    server.send(200, "text/plain", message);
    Serial.println(message);
  }
}

//Setup
void setup(void) {
  Serial.begin(9600);         //Serielle Verbindung mit Monitor
  WiFi.begin(ssid, password); //WIFI Verbinden
  Serial.println("");

  //Auf Verbindung warten
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

  server.on("/", handleRoot);           //Page auf root-Verzeichnis anzeigen
  server.on("/postform/", handleForm);  //Post-request verarbeiten
  
  server.begin();
  Serial.println("HTTP server started");
}

//Mainloop des Hauptprogrammes
void loop(void) {
  server.handleClient();
  analogWrite(fanpin, co2);
}
