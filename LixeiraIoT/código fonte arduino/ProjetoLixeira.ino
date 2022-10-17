#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Servo.h>
#include "Ultrasonic.h"
#include <ESP8266HTTPClient.h>

#ifndef STASSID
#define STASSID "jassonCell"
#define STAPSK  "ratinhas"
#endif

#define SERVO D3 // Porta Digital 6 PWM
#define TRIGGER D4
#define ECHO D5
#define FECHADO 45
#define ABERTO 140

const int ID_LIXEIRA_1 = 1;

const char *ssid = STASSID;
const char *password = STAPSK;

const int pinTrigger = D6;
const int pinEcho = D7;

bool isOpen = false;
int nivel = 0;

HC_SR04 sonar(pinTrigger,pinEcho);

HC_SR04 sensor1(TRIGGER,ECHO);

Servo s; // Variável Servo
int pos; // Posição Servo

void requestLixeiraUpdate(int id, bool isOpen, int nivel){
  if ((WiFi.status() == WL_CONNECTED)) {

    HTTPClient http;   
    String url = "https://lixeira-iot.herokuapp.com/api/lixeiras/update/";

    String open = isOpen ? "true" : "false";

    String json = "{\"id\": " + String(ID_LIXEIRA_1)  + ", \"estaAberta\": " + open + ", \"nivel\": " + String(nivel) + "}";

    Serial.println(json);
    
    http.begin(url, "B5 86 32 41 D3 FB 3D 2B 56 26 0E A3 51 14 75 E8 DC FD 4A 9B"); 
    http.addHeader("Content-Type", "application/json");
    http.setTimeout(10000);

    int httpCode = http.PUT(json);

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] POST... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK) {
          const String& payload = http.getString();
          Serial.println("received payload:\n<<");
          Serial.println(payload);
          Serial.println(">>");
        }
      } else {
        Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
    http.end(); 
  }
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

      nivel = (sonar.distance() * 100) / 30; // considerando que a lixeira tem 30cm de altura
      
      requestLixeiraUpdate(ID_LIXEIRA_1, isOpen, nivel);
    } 
    }else{
      isOpen = true;
      if(s.read()==FECHADO){
        for(pos = FECHADO; pos <= ABERTO; pos++){
          s.write(pos);
          delay(5); //Velocidade de abertura
        }

        requestLixeiraUpdate(ID_LIXEIRA_1, isOpen, nivel);
      }
      delay(2000); //Tempo de permanencia aberto minimo depois de totalmente aberto
    } 

}