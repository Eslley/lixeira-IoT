#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Servo.h>
#include "Ultrasonic.h"
#include <HTTPClient.h>

#ifndef STASSID
#define STASSID "jassonCell"
#define STAPSK  "ratinhas"
#endif

#define SERVO D3 // Porta Digital 6 PWM
#define TRIGGER D4
#define ECHO D5
#define FECHADO 45
#define ABERTO 140
#define ID_LIXEIRA_1 1

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

  // int distancia = sonar.distance();
  
  // int nivel = (distancia * 100) / 30; // considerando que a lixeira tem 30cm de altura

void requestLixeiraUpdate(int id, bool isOpen, int nivel){
  HTTPClient http;   
  String url = "https://lixeira-iot.herokuapp.com/api/";
  
  DynamicJsonDocument data(2048);
  data["id"] = id;
  data["estaAberta"] = isOpen;
  data["nivel"] = nivel;

  String json;
  serializeJson(data, json);
  
  http.begin(url); 
  
  int httpResponseCode = http.POST(json);   
  
  if(httpResponseCode > 0){ //Verifica codigo RESPONSE

    String response = http.getString(); // Recebe o retorno da requisicao

    Serial.println("\n-------------------------------------------------------------");
    Serial.println("HTTP Response Code:"+(String)httpResponseCode);
    Serial.println("Response: "+response);     
  }else{
    Serial.print("Erro na requisicao: ");
    Serial.println(httpResponseCode);
  }
  http.end(); 
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