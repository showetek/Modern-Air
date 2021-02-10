//Libs
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

//Define
#ifndef STASSID
#define STASSID "SSID"
#define STAPSK  "PASS"
#endif

//Const Var
const char* ssid     = STASSID;
const char* password = STAPSK;

//Var
String co2_in = "";
String tmp_in = "";
String result = "";
int pwm = 0;

//SoftwareSerial
SoftwareSerial komm(D5, D6);

//Webserver erstellen
ESP8266WebServer server(80);

//Minimierte Version der Page auf root-Verzeichnis anzeigen
void handleRoot() {
  server.send(200, "text/html", "<!DOCTYPE html><html lang=\"de\"><head><meta charset=\"UTF-8\"><!--<meta http-equiv=\"refresh\" content=\"5; URL=http://localhost/Modern%20Air/src/\" /> --><meta name=\"viewport\" content=\"width=device-width,initial-scale=1\"><link href=\"../Logo_min.svg\" rel=\"icon\" type=\"image/svg\"><link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.1/css/all.min.css\"><script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js\"></script><title>Modern Air - Lüften neu definiert | Webserver</title><style>body{margin:0;font-family:Arial,Helvetica,sans-serif}.navbar{background-color:#29abe2;overflow:hidden;padding:0 240px 0 240px}.dropdown{text-align:center;float:left;overflow:hidden}.dropbtn{font-size:17px;border:none;outline:0;color:#fff;padding:14px 16px;background-color:inherit;font-family:inherit;margin:0;font-weight:700}.dropdown-content{display:none;position:absolute;background-color:#f9f9f9;min-width:200px;box-shadow:0 8px 16px 0 rgba(0,0,0,.2);z-index:1}.dropdown-content a{float:none;color:#000;padding:12px 16px;text-decoration:none;display:block;text-align:left}.dropdown-content a:hover{background-color:#ddd;color:#000}.dropdown:hover .dropdown-content{display:block}.banner{padding:0 240px 0 240px;background-color:#f5f5f5}.footer{position:fixed;bottom:0;width:100%;background-color:#f5f5f5;color:#000;align-items:center;display:flex;justify-content:space-evenly}.footer img{max-width:50px;max-height:50px}.footer a{color:#0366d6;text-decoration:none}.footer i{padding-right:2px}.footer a:hover{text-decoration:underline}.box{margin:20px 240px 20px 240px}.box-header{padding:16px;margin:-1px -1px 0;border:1px solid;border-top-left-radius:6px;border-top-right-radius:6px;background-color:#f1f8ff;border-color:#c1ddff}.box-footer{padding:16px;margin:-1px -1px 0;border:1px solid;border-bottom-left-radius:6px;border-bottom-right-radius:6px;background-color:#fff;border-color:#e0e3e7}.container{margin:20px 240px 20px 240px;display:flex;flex-direction:row;justify-content:space-between;flex-wrap:wrap}.active_on{padding-top:20px;padding-bottom:20px;padding-left:100px;padding-right:100px;background-color:#4caf50;color:#fff;border-radius:18px}.inactive_on{padding-top:20px;padding-bottom:20px;padding-left:100px;padding-right:100px;margin:10px;background-color:#f44336;color:#fff;align-items:center;border-radius:18px}.active_off{padding-top:20px;padding-bottom:20px;padding-left:100px;padding-right:100px;margin:10px;background-color:#4caf50;color:#fff;opacity:.6;cursor:not-allowed;border-radius:18px}.inactive_off{padding-top:20px;padding-bottom:20px;padding-left:100px;padding-right:100px;margin:10px;background-color:#f44336;color:#fff;align-items:center;opacity:.6;cursor:not-allowed;border-radius:18px}.progresslin{width:100%;background-color:grey;border-radius:18px}.barlin{width:60%;height:30px;background-color:#29abe2;border-radius:12px;text-align:center;line-height:30px;color:#fff}.card{background-color:#fff;box-shadow:0 8px 16px 0 rgba(0,0,0,.2);padding:10px;border-radius:10px;min-width:45%}.heading{text-align:center;font-size:17px;font-weight:700;padding-bottom:10px}</style></head><body><!--Navbar--><div class=\"navbar\"><div class=\"dropdown\"><button class=\"dropbtn\"><i class=\"fas fa-fan\"></i> Lüfter-Steuerung <i class=\"fa fa-caret-down\"></i></button><div class=\"dropdown-content\"><a href=\"/fan_on\">Aktivieren</a> <a href=\"/fan_off\">Deaktivieren</a></div></div><div class=\"dropdown\"><button class=\"dropbtn\"><i class=\"fas fa-code\"></i> Demo <i class=\"fa fa-caret-down\"></i></button><div class=\"dropdown-content\"><a href=\"#\">CO2-Eintragen</a> <a href=\"#\">Tmp-Eintragen</a></div></div></div><!--Banner--><div class=\"banner\"><img src=\"../Logo.svg\" alt=\"Logo-Banner\"></div><!--Farblegende--><div class=\"box\"><div class=\"box-header\"><i class=\"fas fa-paint-roller\"></i> Farblegende</div><div class=\"box-footer\" style=\"display:flex;flex-direction:row\"><div class=\"barlin\" style=\"background-color:#29abe2;width:10%;margin-right:10px\">Normal</div><div class=\"barlin\" style=\"background-color:#ff9800;width:10%;margin-right:10px\">Hoch</div><div class=\"barlin\" style=\"background-color:#f44336;width:10%;margin-right:10px\">Kritisch</div></div></div><!--Lüfter--><div class=\"box\"><div class=\"box-header\"><i class=\"fas fa-fan\"></i> Lüfter</div><div class=\"box-footer\" style=\"display:flex;flex-direction:row;justify-content:space-between;align-items:stretch\"><div class=\"card\" style=\"display:flex;flex-direction:row;align-items:center;justify-content:space-between\"><img src=\"../Fan.svg\" alt=\"Fan\" style=\"max-height:200px;max-width:200px\"><div><div class=\"active_on\">🠔 Active 🠖</div><div class=\"inactive_on\">Inactive</div></div><img src=\"../Fan.svg\" alt=\"Fan\" style=\"max-height:200px;max-width:200px\"></div><div class=\"card\" style=\"display:flex;flex-direction:column;justify-content:center\"><div class=\"heading\"><i class=\"fas fa-redo\"></i> Lüfterumderhungen</div><div class=\"progresslin\"><div class=\"barlin\" style=\"width:60%\">836 RPM</div></div></div></div></div><!--Container--><div class=\"container\"><!--CO2--><div style=\"width:49%\"><div class=\"box-header\"><i class=\"fas fa-cloud\"></i> CO2-Gehalt</div><div class=\"box-footer\" style=\"display:flex;flex-direction:row;justify-content:space-around;align-items:center\"><div class=\"card\"><div class=\"heading\"><i class=\"fas fa-home\"></i> In-CO2</div><div class=\"progresslin\"><div class=\"barlin\" style=\"width:60%;background-color:#ff9800\">"+co2_in+" PPM</div></div></div><div class=\"card\"><div class=\"heading\"><i class=\"fas fa-leaf\"></i> Out-CO2</div><div class=\"progresslin\"><div class=\"barlin\" style=\"width:24%\">421 PPM</div></div></div></div></div><!--Tmp--><div style=\"width:49%\"><div class=\"box-header\"><i class=\"fas fa-thermometer-half\"></i> Temperatur</div><div class=\"box-footer\" style=\"display:flex;flex-direction:row;justify-content:space-around;align-items:center\"><div class=\"card\"><div class=\"heading\"><i class=\"fas fa-home\"></i> In-Tmp</div><div class=\"progresslin\"><div class=\"barlin\" style=\"width:100%;background-color:#f44336\">"+tmp_in+" °C</div></div></div><div class=\"card\"><div class=\"heading\"><i class=\"fas fa-leaf\"></i> Out-Tmp</div><div class=\"progresslin\"><div class=\"barlin\" style=\"width:30%\">10 °C</div></div></div></div></div></div><!--Footer--><div class=\"footer\"><a href=\"#\"><i class=\"fas fa-info-circle\"></i>Impressum</a> <img src=\"../Logo_min.svg\" alt=\"Logo_min\"> <a href=\"https://github.com/showetek/Modern-Air\"><i class=\"fab fa-github\"></i>GitHub</a></div><script>function updateDiv(){}window.setInterval(updateDiv(),1e3)</script></body></html>");
}

//Post-request verarbeiten
void handleForm() {
  result = "";
  int pos = 0;
  if (server.method() != HTTP_POST) {
    server.send(405, "text/plain", "Method Not Allowed");
  } else {
    String message = "";
    for (uint8_t i = 0; i < server.args(); i++) {
      message =  server.arg(i)+"\n";
    }
    for (int i =0; i < message.length(); i++){ 
      if (message.charAt(i) == ','){
        pos = pos +1;
        switch (pos){
           case 1: co2_in = result;
           case 2: tmp_in = result;
        }
       
        result = "";
        
      }else{
      result = result +  message[i];  
      }
      
    }
    server.send(200, "text/plain", message);
    Serial.println(message);

    //PWM werte senden
    pwm = map(co2_in.toInt(), 400, 2000, 75, 255);
    komm.print(pwm);
   
  }
}

//Setup
void setup(void) {
  komm.begin(115200);

  Serial.begin(9600);//Serielle Verbindung mit Monitor
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

  server.on("/", handleRoot);                     //Page auf root-Verzeichnis anzeigen
  server.on("/postform/", handleForm);            //Post-request verarbeiten
  
  server.begin();
  Serial.println("HTTP server started");
}

//Mainloop des Hauptprogrammes
void loop(void) {
  server.handleClient();
}