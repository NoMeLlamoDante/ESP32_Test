// Libraries 
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h> //MQTT
#include <NTPClient.h>  //Time
#include <WiFiUdp.h>  //Wifi - Time
#include <FS.h>
#include <SD.h>
#include <SPI.h>

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

//Data Lib
WiFiClient espClient;
PubSubClient client(espClient);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "mx.pool.ntp.org",utcOffsetInSeconds);


void setup(){
  Serial.begin(9600);
  //Conectar Wifi
  WiFi.begin(ssid, password);
  Serial.print("Intentando Conectar Wifi.");
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".") ;
  }
  Serial.println("Conectado a Wifi");
  //Time
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
  //SD Mount
  Serial.println("Verificando Micro SD");
  if(!SD.begin(5)){
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();
  
  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if(cardType == CARD_MMC){
    Serial.println("MMC");
  } else if(cardType == CARD_SD){
    Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);

}

void loop() {
  //MQTT
  client.loop();
  client.publish("dante_prueba/esp_32",String(millis()).c_str());
  //Time
  timeClient.update();
  Serial.println(timeClient.getFormattedTime());
  delay(1000);
  //SD
  File file = SD.open("/datalog.txt", FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.println(timeClient.getDay()+" ,Hora: "+timeClient.getFormattedTime())){
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}