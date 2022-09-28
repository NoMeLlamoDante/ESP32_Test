// Libraries 
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

//Data
const char* ssid = "IZZI-0CC8";
const char* password = "704FB8270CC8";
const char* mqttServer = "broker.emqx.io";
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";

//Data Lib
WiFiClient espClient;
PubSubClient client(espClient);



void setup()
{ Serial.begin(9600);
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
  client.loop();
    

  client.publish("dante_prueba/esp_32",String(millis()).c_str());
  delay(500);
}