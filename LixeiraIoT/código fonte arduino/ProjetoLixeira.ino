
#include <ESP32Servo.h>
#include "Ultrasonic.h"

#include <WiFi.h>

#include <HTTPClient.h>

#ifndef STASSID
#define STASSID "SSID"
#define STAPSK "PASSWORD"
#endif

#define ITV 1
#define SERVO 13 // Porta Digital 6 PWM
#define TRIGGER 12
#define ECHO 15
#define FECHADO 70
#define ABERTO 170
#define TAM 14

const int ID_LIXEIRA_1 = 1;

const char *ssid = STASSID;
const char *password = STAPSK;

const int pinTrigger = 27;
const int pinEcho = 26;

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
    Serial.println(nivel);    
    nivel = 100 - nivel;
    String json = "{\"id\": " + String(ID_LIXEIRA_1)  + ", \"estaAberta\": " + open + ", \"nivel\": " + String(nivel) + "}";

    Serial.println(json);
    
    http.begin(url); 
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
//HTTPClient http;
//WiFiClient client;
void setup() {
	// Allow allocation of all timers
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);  
  WiFi.begin(ssid, password);
	//s.setPeriodHertz(50);    // standard 50 hz servo
	s.attach(SERVO); // attaches the servo on pin 18 to the servo object
  s.write(FECHADO);  
	
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting...");
  } 
  Serial.println("Conneted");   
}

void loop(void) {
  //http.begin("https://lixeira-iot.herokuapp.com/api/lixeiras/");   
  //int httpCode = http.GET();
  //http.setTimeout(10000);  
  //Serial.println(httpCode);
  int distancia = sensor1.distance();
  //Serial.print("Distancia em cm: ");
  Serial.println(distancia);
  delay(50); // Velocidade de leitura (evita trepidações na porta)
  //Serial.println(nivel);
  if(distancia > 20){
    isOpen = false;
    if(s.read()==ABERTO-ITV){
      for(pos = ABERTO; pos >= FECHADO; pos--){
        s.write(pos);
        delay(15); //Velocidade de fechamento
      }
       // considerando que a lixeira tem 15cm de altura
      //requestLixeiraUpdate(ID_LIXEIRA_1, isOpen, nivel);      
    } 
    nivel = (sonar.distance() * 100) / TAM;
    requestLixeiraUpdate(ID_LIXEIRA_1, isOpen, nivel);    
    }else{
      isOpen = true;
      if(s.read()==FECHADO-ITV){        
        for(pos = FECHADO; pos <= ABERTO; pos++){
          s.write(pos);
          delay(5); //Velocidade de abertura
        }
        requestLixeiraUpdate(ID_LIXEIRA_1, isOpen, nivel);
      }
      delay(2000); //Tempo de permanencia aberto minimo depois de totalmente aberto
    } 

}
