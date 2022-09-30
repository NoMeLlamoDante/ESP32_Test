// Libraries 
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h> //MQTT
#include <NTPClient.h>  //Time
#include <WiFiUdp.h>

//Data
//Wifi Data
const char* ssid = "IZZI-0CC8";
const char* password = "704FB8270CC8";
//Broker Data
const char* mqttServer = "broker.emqx.io";
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";
//Time Data
const long utcOffsetInSeconds = -18000;
char daysOfTheWeek[7][10] = {"Domingo","Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado"};

//Data Lib
WiFiClient espClient;
PubSubClient client(espClient);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "mx.pool.ntp.org",utcOffsetInSeconds);


void setup(){
  Serial.begin(9600);
  //Conectar Wifi
  WiFi.begin(ssid, password);
  Serial.println("...................................");

  Serial.print("Intentando Conectar Wifi.");
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".") ;
  }

  Serial.println("Conectado a Wifi");
  client.setServer(mqttServer, mqttPort);
  timeClient.begin();
  //Conectar Broker
  while (!client.connected()){
    Serial.println("Conectando al broker MQTT...");
    if (client.connect("mqttx_54d756f3", mqttUser, mqttPassword ))
      Serial.println("Conectado");
    else{
      Serial.print("Fallo con estado:");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void loop() {
  timeClient.update();
  client.loop();

  Serial.println(timeClient.getFormattedTime());
    

  client.publish("dante_prueba/esp_32",String(millis()).c_str());
  delay(1000);
}