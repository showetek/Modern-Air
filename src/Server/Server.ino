//Libs
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>

//Define
#ifndef STASSID
#define STASSID "SSID"
#define STAPSK  "PASS"
#endif

//Const Var
const char* ssid     = STASSID;
const char* password = STAPSK;

//Var
String co2_in = "0";
String tmp_in = "0";
String co2_out = "0";
String tmp_out = "0";
String result = "";
String fan = "on";
int pwm = 0;
String timer_out = "";
String timer_in = "";
String Link = "";
String getData = "";
String rpm = "0";
boolean rpm_check = false;
String rpm_ctrl = "";
String result_rpm_ctrl = "";


//SoftwareSerial
SoftwareSerial komm(D5, D6);

//Webserver erstellen
ESP8266WebServer server(80);

//Minimierte Version der Page auf root-Verzeichnis anzeigen
void handleRoot() {
  server.send(200, "text/html", "<!DOCTYPE html><html lang=\"de\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><link href=\"https://raw.githubusercontent.com/showetek/Modern-Air/e3f5907cb627eb1ce78387f5bfc38c61fcba2572/img/Logo_min.svg\" rel=\"icon\" type=\"image/svg\" /><link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.1/css/all.min.css\" /><link rel=\"stylesheet\" href=\"https://www.w3schools.com/w3css/4/w3.css\"> <script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js\"></script> <title>Modern Air - Lüften neu definiert | Webserver</title><style>html{height:100%}body{margin:0;font-family:Arial,Helvetica,sans-serif;min-height:100%;display:grid;grid-template-rows:1fr auto}.content{min-height:100%}.navbar{background-color:#29ABE2;overflow:hidden;padding:0px 10% 0px 10%}.dropdown{text-align:center;float:left;overflow:hidden}.dropbtn{font-size:17px;border:none;outline:none;color:white;padding:14px 16px;background-color:inherit;font-family:inherit;margin:0;font-weight:bold}.dropdown-content{display:none;position:absolute;background-color:#f9f9f9;min-width:200px;box-shadow:0px 8px 16px 0px rgba(0,0,0,0.2);z-index:1}.dropdown-content a{float:none;color:black;padding:12px 16px;text-decoration:none;display:block;text-align:left}.dropdown-content a:hover{background-color:#ddd;color:black}.dropdown:hover .dropdown-content{display:block}.banner{padding:0px 10% 0px 10%;background-color:whitesmoke}.footer{grid-row-start:2;grid-row-end:3;position:relative;bottom:0;width:100%;background-color:whitesmoke;color:black;align-items:center;display:flex;justify-content:space-evenly;flex-shrink:0}.footer img{max-width:50px;max-height:50px}.footer a{color:#0366D6;text-decoration:none}.footer i{padding-right:2px}.footer a:hover{text-decoration:underline}.box{margin:20px 10% 20px 10%}.box-header{padding:16px;margin:-1px -1px 0;border:1px solid;border-top-left-radius:6px;border-top-right-radius:6px;background-color:#F1F8FF;border-color:#c1ddff}.box-footer{padding:16px;margin:-1px -1px 0;border:1px solid;border-bottom-left-radius:6px;border-bottom-right-radius:6px;background-color:white;border-color:#e0e3e7}.box-footer a{text-decoration:none}.box-footer i{padding-right:2px}.box-footer a:hover{text-decoration:underline}.container{margin:20px 10% 20px 10%;display:block;flex-direction:row;justify-content:space-between;flex-wrap:wrap}.activebutton_on{padding-top:2%;padding-bottom:2%;margin:10px;background-color:#4CAF50;color:white;border-radius:18px;border:none;text-align:center}.inactivebutton_on{padding-top:2%;padding-bottom:2%;margin:10px;background-color:#f44336;color:white;border-radius:18px;text-align:center;border:none}.activebutton_on:hover,.activebutton_on:focus{outline:none;cursor:pointer;opacity:0.8}.inactivebutton_on:hover,.inactivebutton_on:focus{outline:none;cursor:pointer;opacity:0.8}.active_on{padding-top:2%;padding-bottom:2%;margin:10px;background-color:#4CAF50;color:white;border-radius:18px;border:none;text-align:center}.inactive_on{padding-top:2%;padding-bottom:2%;margin:10px;background-color:#f44336;color:white;border-radius:18px;text-align:center;border:none}.active_off{padding-top:2%;padding-bottom:2%;margin:10px;background-color:#4CAF50;color:white;opacity:0.6;cursor:not-allowed;border-radius:18px;text-align:center;border:none}.inactive_off{padding-top:2%;padding-bottom:2%;margin:10px;background-color:#f44336;color:white;opacity:0.6;cursor:not-allowed;border-radius:18px;text-align:center;border:none}.progresslin{width:100%;background-color:grey;border-radius:18px}.barlin{width:60%;height:30px;background-color:#29ABE2;border-radius:12px;text-align:center;line-height:30px;color:white}.card{background-color:white;box-shadow:0px 8px 16px 0px rgba(0,0,0,0.2);padding:10px;border-radius:10px;min-width:45%}.heading{text-align:center;font-size:17px;font-weight:bold;padding-bottom:10px}.modal{display:none;position:fixed;z-index:1;left:0;top:0;width:100%;height:100%;overflow:auto;background-color:rgb(0,0,0);background-color:rgba(0,0,0,0.4)}.modal-content{background-color:#fefefe;margin:15% auto;padding:20px;border:2px solid;border-color:#c1ddff;width:60%;border-radius:10px}.close{color:#aaa;float:right;font-size:28px;font-weight:bold}.close:hover,.close:focus{color:black;text-decoration:none;cursor:pointer}input[type=number]{width:100%;padding:12px;border:1px solid #ccc;border-radius:4px;box-sizing:border-box;margin-top:6px;margin-bottom:16px;resize:vertical}input[type=number]:focus,input[type=number]:hover{outline:none}</style></head><body><div class=\"content\"><div class=\"navbar\"><div class=\"dropdown\"> <button class=\"dropbtn\"><i class=\"fas fa-fan\"></i> Lüfter-Steuerung <i class=\"fa fa-caret-down\"></i></button><div class=\"dropdown-content\"> <a href=\"#fan_on\" onclick=\"showmodal('modal_on')\">Aktivieren</a> <a href=\"#fan_off\" onclick=\"showmodal('modal_off')\">Deaktivieren</a></div></div><div class=\"dropdown\"> <button class=\"dropbtn\"><i class=\"fas fa-code\"></i> Demo <i class=\"fa fa-caret-down\"></i></button><div class=\"dropdown-content\"> <a href=\"#co2_eintragen\" onclick=\"resetForm('co2_form'), showmodal('modal_co2')\">CO2-Eintragen</a> <a href=\"#tmp_eintragen\" onclick=\"resetForm('tmp_form'), showmodal('modal_tmp')\">Tmp-Eintragen</a></div></div></div><div id=\"modal_on\" class=\"modal\"><div class=\"modal-content w3-animate-bottom\"> <span class=\"close\" onclick=\"closemodal('modal_on')\">&times;</span><h2>Lüfter-Steuerung</h2><p>Lüfter wirklich <b>aktivieren?</b></p><div style=\"text-align: right;\"> <button class=\"activebutton_on\" onclick=\"closemodal('modal_on'), post('fan=on')\">JA, aktivieren</button> <button class=\"inactivebutton_on\" onclick=\"closemodal('modal_on')\">Abbrechen</button></div></div></div><div id=\"modal_off\" class=\"modal\"><div class=\"modal-content w3-animate-bottom\"> <span class=\"close\" onclick=\"closemodal('modal_off')\">&times;</span><h2>Lüfter-Steuerung</h2><p>Lüfter wirklich <b>deaktivieren?</b></p><div style=\"text-align: right;\"> <button class=\"activebutton_on\" onclick=\"closemodal('modal_off'), post('fan=off')\">JA, deaktivieren</button> <button class=\"inactivebutton_on\" onclick=\"closemodal('modal_off')\">Abbrechen</button></div></div></div><div id=\"modal_co2\" class=\"modal\"><div class=\"modal-content w3-animate-bottom\"> <span class=\"close\" onclick=\"closemodal('modal_co2')\">&times;</span><h2>CO2-Eintragen</h2><form action=\"postform/\" id=\"co2_form\" target=\"_blank\" rel=\"noopener\" enctype=\"text/plain\" method=\"post\"> <label for=\"co2_in\">Location</label><br> <input type=\"radio\" id=\"co2_in\" name=\"ort_wert\" value=\"in\" checked> <label for=\"co2_in\">In</label><br> <input type=\"radio\" id=\"co2_out\" name=\"ort_wert\" value=\"out\"> <label for=\"co2_out\">Out</label><br><br> <label for=\"co2_wert\">CO2 Wert in ppm (400-2000)</label><br> <input type=\"number\" id=\"co2_wert\" placeholder=\"CO2-Wert\" name=\"co2_wert\" min=\"400\" max=\"2000\" required> <label for=\"co2_time\">Zeit in ms (0-15000)</label><br> <input type=\"number\" id=\"time_co2\" placeholder=\"Zeit-Wert\" name=\"time_wert\" min=\"0\" max=\"15000\" required></form><div style=\"text-align: right;\"> <button class=\"activebutton_on\" onclick=\"submitForm('modal_co2', '#co2_form', 'ort_wert', 'co2_wert', 'time_co2')\">CO2 senden</button> <button class=\"inactivebutton_on\" onclick=\"closemodal('modal_co2')\">Abbrechen</button></div></div></div><div id=\"modal_tmp\" class=\"modal\"><div class=\"modal-content w3-animate-bottom\"> <span class=\"close\" onclick=\"closemodal('modal_tmp')\">&times;</span><h2>Tmp-Eintragen</h2><form action=\"postform/\" id=\"tmp_form\" target=\"_blank\" rel=\"noopener\" enctype=\"text/plain\" method=\"post\"> <label for=\"tmp_in\">Location</label><br> <input type=\"radio\" id=\"tmp_in\" name=\"ort_wert\" value=\"in\" checked> <label for=\"tmp_in\">In</label><br> <input type=\"radio\" id=\"tmp_out\" name=\"ort_wert\" value=\"out\"> <label for=\"tmp_out\">Out</label><br><br> <label for=\"tmp_wert\">Tmp Wert in °C (-40-30)</label><br> <input type=\"number\" id=\"tmp_wert\" placeholder=\"Tmp-Wert\" name=\"tmp_wert\" min=\"-40\" max=\"30\" required> <label for=\"tmp_time\">Zeit in ms (0-15000)</label><br> <input type=\"number\" id=\"time_tmp\" placeholder=\"Zeit-Wert\" name=\"time_wert\" min=\"0\" max=\"15000\" required></form><div style=\"text-align: right;\"> <button class=\"activebutton_on\" onclick=\"submitForm('modal_tmp', '#tmp_form', 'ort_wert', 'tmp_wert', 'time_tmp')\">Tmp senden</button> <button class=\"inactivebutton_on\" onclick=\"closemodal('modal_tmp')\">Abbrechen</button></div></div></div><div class=\"banner\"> <img src=\"https://raw.githubusercontent.com/showetek/Modern-Air/e3f5907cb627eb1ce78387f5bfc38c61fcba2572/img/Logo.svg\" alt=\"Logo-Banner\"></div><div class=\"box\"><div class=\"box-header\" style=\"display: flex; flex-direction: row;\"><div><i class=\"fas fa-paint-roller\"></i> Farblegende</div></div><div class=\"box-footer\" style=\"display: flex; flex-direction: row;\" ><div class=\"barlin\" style=\"background-color: #29ABE2; width: 20%; margin-right: 10px;\" >Normal</div><div class=\"barlin\" style=\"background-color: #ff9800; width: 20%; margin-right: 10px;\" >Hoch</div><div class=\"barlin\" style=\"background-color: #f44336; width: 20%; margin-right: 10px;\" >Kritisch</div> <a href=\"#update\" onclick=\"update()\" class=\"barlin\" style=\"background-color: #29ABE2; width: 20%; margin-left: 10px;\" ><i class=\"fas fa-redo\"></i>Refresh</a></div></div><div class=\"box\"><div class=\"box-header\"> <i class=\"fas fa-fan\"></i> Lüfter</div><div class=\"box-footer\" style=\"display: flex; flex-direction: row; justify-content: space-between; align-items: stretch;\"><div class=\"card\" ><div id=\"active_button\" class=\"active_on\">Active</div><div id=\"inactive_button\" class=\"inactive_off\">Inactive</div></div><div class=\"card\" style=\"display: flex; flex-direction: column; justify-content: center;\"><div class=\"heading\"><i class=\"fas fa-redo\"></i> Lüfterumdrehungen</div><div class=\"progresslin\"><div class=\"barlin\" id=\"rpm_display\" style=\"width: 100%;\"> Default</div></div></div></div></div><div class=\"container\"><div style=\"width: 100%; margin-bottom: 20px;\"><div class=\"box-header\"> <i class=\"fas fa-cloud\"></i> CO2-Gehalt</div><div class=\"box-footer\" style=\"display: flex; flex-direction: row; justify-content: space-around; align-items: center;\" ><div class=\"card\"><div class=\"heading\"><i class=\"fas fa-home\"></i> In-CO2</div><div class=\"progresslin\"><div class=\"barlin\" id=\"co2_in_display\" style=\"width: 100%;\">Default</div></div></div><div class=\"card\"><div class=\"heading\"><i class=\"fas fa-leaf\"></i> Out-CO2</div><div class=\"progresslin\"><div class=\"barlin\" id=\"co2_out_display\" style=\"width: 100%;\">Default</div></div></div></div></div><div style=\"width: 100%;\"><div class=\"box-header\"> <i class=\"fas fa-thermometer-half\"></i> Temperatur</div><div class=\"box-footer\" style=\"display: flex; flex-direction: row; justify-content: space-around; align-items: center;\" ><div class=\"card\"><div class=\"heading\"><i class=\"fas fa-home\"></i> In-Tmp</div><div class=\"progresslin\"><div class=\"barlin\" id=\"tmp_in_display\" style=\"width: 100%;\">Default</div></div></div><div class=\"card\"><div class=\"heading\"><i class=\"fas fa-leaf\"></i> Out-Tmp</div><div class=\"progresslin\"><div class=\"barlin\" id=\"tmp_out_display\" style=\"width: 100%;\">Default</div></div></div></div></div></div><div id=\"modal_impressum\" class=\"modal\"><div class=\"modal-content w3-animate-bottom\"> <span class=\"close\" onclick=\"closemodal('modal_impressum')\">&times;</span><h2>Impressum</h2> <i>Dies ist der Webserver des Jufo-Projektes \"Modern Air - Lüften neu definiert\" zur Wettbewerbsrunde 2021</i><h3>Aktuell entwickelt von:</h3><ul><li>Torben Heine</li><li>Paul Paysan</li></ul></div></div></div><div style=\"padding-left: 10%; padding-right: 10%; background-color: whitesmoke;\"><div class=\"footer\"> <a href=\"#modal_impressum\" onclick=\"showmodal('modal_impressum')\"><i class=\"fas fa-info-circle\"></i>Impressum</a> <img src=\"https://raw.githubusercontent.com/showetek/Modern-Air/e3f5907cb627eb1ce78387f5bfc38c61fcba2572/img/Logo_min.svg\" alt=\"Logo_min\" > <a href=\"https://github.com/showetek/Modern-Air\" target=\"_blank\" rel=\"noopener\"><i class=\"fab fa-github\"></i>GitHub</a></div></div> <script>function showmodal(id){modal=document.getElementById(id);modal.style.display=\"block\";} function closemodal(id){modal=document.getElementById(id);modal.style.display=\"none\";} window.onclick=function(event){if(event.target==modal){modal.style.display=\"none\";}};function validateForm(form_id,name_wert,name_time){var result=true;var messsage=\"\";var x=$(\"#\"+name_wert);if(x.val()==\"\"){messsage=messsage+\"Das Feld \"+x.attr(\"placeholder\")+\" muss gefüllt sein!n\";result=false;} if(parseInt(x.val())>parseInt(x.attr(\"max\"))||parseInt(x.val())<parseInt(x.attr(\"min\"))){messsage=messsage+\"Das Feld \"+x.attr(\"placeholder\")+\" muss eine Zahl zwischen \"+x.attr(\"min\")+\" und \"+x.attr(\"max\")+\" sein!n\";result=false;} var x=$(\"#\"+name_time);if(x.val()==\"\"){messsage=messsage+\"Das Feld \"+x.attr(\"placeholder\")+\" muss gefüllt sein!n\";result=false;} if(parseInt(x.val())>parseInt(x.attr(\"max\"))||parseInt(x.val())<parseInt(x.attr(\"min\"))){messsage=messsage+\"Das Feld \"+x.attr(\"placeholder\")+\" muss eine Zahl zwischen \"+x.attr(\"min\")+\" und \"+x.attr(\"max\")+\" sein!n\";result=false;} if(result==false){alert(\"Folgende Fehler liegen vor:n\"+messsage);} return result;} function generatePostString(form_id,name_ort,name_wert,name_time){var answer=\"\";var arg_wert=\"\";var arg_time=\"\";var ort=$(\"input:radio[name=\"+name_ort+\"]:checked\");var wert=$(\"#\"+name_wert);var time=$(\"#\"+name_time);var x=$(form_id).serializeArray;for(let i=0;i<name_wert.length;i++){arg_wert=arg_wert+name_wert[i];if(name_wert[i]=='_'){break;}} for(let i=0;i<name_time.length;i++){arg_time=arg_time+name_time[i];if(name_time[i]=='_'){break;}} answer=arg_wert+ort.val()+\"=\"+wert.val()+\",\"+arg_time+ort.val()+\"=\"+time.val()+\",\";return answer;} function submitForm(modal_id,form_id,name_ort,name_wert,name_time){if(validateForm(form_id,name_wert,name_time)){post(generatePostString(form_id,name_ort,name_wert,name_time));closemodal(modal_id);}} function resetForm(id){form=document.getElementById(id);form.reset();} function post(arg){var ip=location.host;var xhttp=new XMLHttpRequest();xhttp.open(\"POST\",\"http://\"+ip+\"/postform/\",true);xhttp.setRequestHeader(\"Content-type\",\"text/plain\");xhttp.send(arg);} function get(arg){var ip=location.host;var xhttp=new XMLHttpRequest();xhttp.open(\"GET\",\"http://\"+ip+\"/getform/?data=\"+arg,false);xhttp.send(null);return xhttp.responseText;} function updateDisplay(get_arg,display,max,colorswitch1,colorswitch2){var data=get(get_arg);var element=document.getElementById(display);var width=((parseInt(data)/max)*100).toString();var color='';if(get_arg==\"co2_out\"||get_arg==\"co2_in\"){element.innerHTML=data+\" PPM\";} if(get_arg==\"tmp_out\"||get_arg==\"tmp_in\"){element.innerHTML=data+\" °C\";} if(get_arg==\"rpm\"){element.innerHTML=data+\" RPM\";} if(parseInt(data)<colorswitch1){color='#29ABE2';} if(parseInt(data)>=colorswitch1&&parseInt(data)<colorswitch2){color='#ff9800';} if(parseInt(data)>=colorswitch2){color='#f44336';} element.setAttribute(\"style\",\"width:\"+width+\"%; background-color:\"+color+\";\");} function updateStatus(get_arg,active_button,inactive_button){var data=get(get_arg);var active_element=document.getElementById(active_button);var inactive_element=document.getElementById(inactive_button);if(data==\"on\"){active_element.classList.remove('active_off');inactive_element.classList.remove('inactive_on');active_element.classList.add('active_on');inactive_element.classList.add('inactive_off');} if(data==\"off\"){active_element.classList.remove('active_on');inactive_element.classList.remove('inactive_off');active_element.classList.add('active_off');inactive_element.classList.add('inactive_on');}} function update(){updateDisplay(\"co2_in\",\"co2_in_display\",3500,1200,1800);updateDisplay(\"co2_out\",\"co2_out_display\",2000,1200,1500);updateDisplay(\"tmp_in\",\"tmp_in_display\",30,25,28);updateDisplay(\"tmp_out\",\"tmp_out_display\",30,25,28);updateDisplay(\"rpm\",\"rpm_display\",5500,4000,5000);updateStatus('fan','active_button','inactive_button');} $('document').ready(update());</script> </body></html>");
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
      message =  server.arg(i)+"";
    }
    for (int i =0; i < message.length(); i++){ 
      
      if (message.charAt(i) == '='){
        
        if (result == "co2_in") 
        {pos = 1;}
        if (result == "tmp_in") 
        {pos = 2;}
        if (result == "co2_out") 
        {pos = 3;}
        if (result == "tmp_out") 
        {pos = 4;}
        if (result == "fan") 
        {pos = 5;}
        if (result == "wert_in") 
        {pos = 6;}
        if (result == "timer_out")
        {pos = 7;}

        result = "";
      }
      
      if (message.charAt(i) == ','){
        switch (pos){
           case 1: co2_in = result;
           case 2: tmp_in = result;
           case 3: co2_out = result;
           case 4: tmp_out = result;
           case 5: fan = result;
           case 7: timer_in = result;
        }
       
        result = "";
        
      }else{
        if (message.charAt(i) != '=') { 
      result = result +  message[i];  
      }

      }
    }
    server.send(200, "text/plain", message);
    Serial.println(message);


    //PWM werte senden
    pwm = map(co2_in.toInt(), 400, 3500, 75, 255);
    //if (fan =="on"){
    //delay(timer_in.toInt());
    //timer_in = "0";
    komm.print("pwm="+String(pwm)+",");  
    //}
    //else{
    //komm.print(0);
    //}
    
   
  }
}

void handleGet(){
 if (server.method() != HTTP_GET) {
    server.send(405, "text/plain", "Method Not Allowed");
  } else {
  String message = "";
    for (uint8_t i = 0; i < server.args(); i++) {
      message =  server.arg(i)+"";
    }   
    if (message == "rpm"){
      if (rpm_check == true) {
      Serial.println("rpm="+rpm+",");
      server.send(200, "text/plain", rpm);  
    }
    }
    if (message == "co2_in"){
      server.send(200, "text/plain", co2_in);  
    }
    if (message == "tmp_in"){
      server.send(200, "text/plain", tmp_in);  
    }
    if (message == "co2_out"){
      server.send(200, "text/plain", co2_out);  
    }
    if (message == "tmp_out"){
      server.send(200, "text/plain", tmp_out);  
    }
    if (message == "fan") {
      server.send(200, "text/plain", fan);
    }
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
  server.on("/postform/", handleForm); //Post-request verarbeiten
  server.on("/getform/", handleGet); //Get-request verarbeiten
  
  server.begin();
  Serial.println("HTTP server started");
}

//Mainloop des Hauptprogrammes
void loop(void) {
  server.handleClient();
   result_rpm_ctrl = "";
      
      rpm_ctrl = komm.readString();
      Serial.println(rpm_ctrl);
      for (int i =0; i < rpm_ctrl.length(); i++){
        
      if (rpm_ctrl.charAt(i) == '=') { 
        
        if (result_rpm_ctrl == "rpm") {
         rpm_check = true; 
         result_rpm_ctrl = "";
        }
        
      }
      if (rpm_ctrl.charAt(i) == ','){
        rpm = result_rpm_ctrl;
        Serial.println("Result RPM= "+rpm);
        result_rpm_ctrl = "";
      }
      else{
        if (rpm_ctrl.charAt(i) != '=') {
        result_rpm_ctrl = result_rpm_ctrl + rpm_ctrl[i] ;
      }
      }
      }
}
