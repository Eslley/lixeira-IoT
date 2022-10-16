#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Servo.h>
#include <NewPing.h>
#include "Ultrasonic.h"

#ifndef STASSID
#define STASSID "jassonCell"
#define STAPSK  "ratinhas"
#endif

#define SERVO D3 // Porta Digital 6 PWM
#define TRIGGER D4
#define ECHO D5
#define FECHADO 45
#define ABERTO 140

const char *ssid = STASSID;
const char *password = STAPSK;

const int pinTrigger = D6;
const int pinEcho = D7;

bool isOpen = false;

ESP8266WebServer server(80);

HC_SR04 sonar(pinTrigger,pinEcho);

HC_SR04 sensor1(TRIGGER,ECHO);

Servo s; // Variável Servo
int pos; // Posição Servo

void handleRoot() {
  char temp[600];
  int distancia = sonar.distance();
  
  int nivel = (distancia * 100) / 30; // considerando que a lixeira tem 30cm de altura

  // char status[] = "fechada";

  // if (isOpen) {
  //   status[] = "aberta";
  // } else {
  //   status[] = "fechada";
  // }


  snprintf(temp, 600,

           "<html>\
            <head>\
              <title>Projeto Lixeira Inteligente</title>\
              <meta charset='UTF-8'>\
              <style>\
                body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
              </style>\
            </head>\
            <body>\
              <h1>Dashboard</h1>\
              <p>Estado da Lixeira: %s</p>\
              <p>Nível da Lixeira: %02d %%</p>\
            </body>\
          </html>",

           isOpen ? "aberta" : "fechada", nivel
          );
  server.send(200, "text/html", temp);
}

void handleNotFound() {
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
}

void setup(void) {
  s.attach(SERVO);
  s.write(FECHADO); // Inicia motor posição zero

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting...");
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
  });
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  int distancia = sensor1.distance();
  Serial.print("Distancia em cm: ");
  Serial.println(distancia);
  delay(50); // Velocidade de leitura (evita trepidações na porta)

  if(distancia > 20){
    isOpen = false;
    if(s.read()==ABERTO){
      for(pos = ABERTO; pos >= FECHADO; pos--){
        s.write(pos);
        delay(15); //Velocidade de fechamento
      }
    } 
    }else{
      isOpen = true;
      if(s.read()==FECHADO){
        for(pos = FECHADO; pos <= ABERTO; pos++){
          s.write(pos);
          delay(5); //Velocidade de abertura
        }
      }
      delay(2000); //Tempo de permanencia aberto minimo depois de totalmente aberto
    } 

  server.handleClient();
  MDNS.update();
}