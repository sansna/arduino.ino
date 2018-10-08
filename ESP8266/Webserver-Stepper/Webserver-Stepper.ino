#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "wifissid";
const char* password = "password";

ESP8266WebServer server(80);

const int led = 13;
const int Pin1 = D1;
const int Pin2 = D2;
const int Pin3 = D3;
const int Pin4 = D4;
const int delaytime = 1;
const int recursivetime = 1000/(delaytime*8);
boolean locked = true;

int i = 0;

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from esp8266!");
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void stop_motor() {
     digitalWrite(Pin1, LOW);  
     digitalWrite(Pin2, LOW); 
     digitalWrite(Pin3, LOW); 
     digitalWrite(Pin4, LOW);
}

void setup(void) {
  pinMode(led, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  digitalWrite(led, 0);
  digitalWrite(D1, LOW); //init
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(D4, LOW);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
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

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
    digitalWrite(D1, HIGH);
    digitalWrite(D2, HIGH);
    digitalWrite(D3, LOW);
    digitalWrite(D4, LOW);
  });

  server.on("/unlock", []() {
    if (!locked) {
      return;
    }
    locked = false;
    server.send(200, "text/plain", "door opening");
	// Rotate
  for (i = 0; i < recursivetime; i++) {
       digitalWrite(Pin1, LOW);  
       digitalWrite(Pin2, LOW); 
       digitalWrite(Pin3, LOW);
       digitalWrite(Pin4, HIGH);
       delay(delaytime);
       digitalWrite(Pin1, HIGH);  
       digitalWrite(Pin2, LOW); 
       digitalWrite(Pin3, LOW); 
       digitalWrite(Pin4, HIGH);
       delay(delaytime);
       digitalWrite(Pin1, HIGH);  
       digitalWrite(Pin2, LOW); 
       digitalWrite(Pin3, LOW); 
       digitalWrite(Pin4, LOW);
       delay(delaytime);
       digitalWrite(Pin1, HIGH);  
       digitalWrite(Pin2, HIGH); 
       digitalWrite(Pin3, LOW); 
       digitalWrite(Pin4, LOW);
       delay(delaytime);
       digitalWrite(Pin1, LOW);  
       digitalWrite(Pin2, HIGH); 
       digitalWrite(Pin3, LOW); 
       digitalWrite(Pin4, LOW);
       delay(delaytime);
       digitalWrite(Pin1, LOW);  
       digitalWrite(Pin2, HIGH); 
       digitalWrite(Pin3, HIGH); 
       digitalWrite(Pin4, LOW);
       delay(delaytime);
       digitalWrite(Pin1, LOW);  
       digitalWrite(Pin2, LOW); 
       digitalWrite(Pin3, HIGH); 
       digitalWrite(Pin4, LOW);
       delay(delaytime);
       digitalWrite(Pin1, LOW);  
       digitalWrite(Pin2, LOW); 
       digitalWrite(Pin3, HIGH); 
       digitalWrite(Pin4, HIGH);
       delay(delaytime);
    }
    stop_motor();
  });

  server.on("/lock", []() {
    if (locked) {
      return;
    }
    locked = true;
    server.send(200, "text/plain", "door closing");
	// Reverse
    for (i = 0; i < recursivetime; i++) {
       digitalWrite(Pin1, LOW);  
       digitalWrite(Pin2, LOW); 
       digitalWrite(Pin3, LOW); 
       digitalWrite(Pin4, HIGH);
       delay(delaytime);
       digitalWrite(Pin1, LOW);  
       digitalWrite(Pin2, LOW); 
       digitalWrite(Pin3, HIGH); 
       digitalWrite(Pin4, HIGH);
       delay(delaytime);
       digitalWrite(Pin1, LOW);  
       digitalWrite(Pin2, LOW); 
       digitalWrite(Pin3, HIGH); 
       digitalWrite(Pin4, LOW);
       delay(delaytime);
       digitalWrite(Pin1, LOW);  
       digitalWrite(Pin2, HIGH); 
       digitalWrite(Pin3, HIGH); 
       digitalWrite(Pin4, LOW);
       delay(delaytime);
       digitalWrite(Pin1, LOW);  
       digitalWrite(Pin2, HIGH); 
       digitalWrite(Pin3, LOW); 
       digitalWrite(Pin4, LOW);
       delay(delaytime);
       digitalWrite(Pin1, HIGH);  
       digitalWrite(Pin2, HIGH); 
       digitalWrite(Pin3, LOW); 
       digitalWrite(Pin4, LOW);
       delay(delaytime);
       digitalWrite(Pin1, HIGH);  
       digitalWrite(Pin2, LOW); 
       digitalWrite(Pin3, LOW); 
       digitalWrite(Pin4, LOW);
       delay(delaytime);
       digitalWrite(Pin1, HIGH);  
       digitalWrite(Pin2, LOW); 
       digitalWrite(Pin3, LOW); 
       digitalWrite(Pin4, HIGH);
       delay(delaytime);
    }
    stop_motor();
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}
