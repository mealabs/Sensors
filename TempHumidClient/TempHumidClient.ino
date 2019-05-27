#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include "DHT.h"

//Variables for WiFi
char ssid[] = "Animal-Shelter";
char pass[] = "kyleisdum";

//Variables for Servers
String serverIP = "192.168.137.1";
String serverPort = "80";
String serverURL = "http://"+serverIP + ":" +serverPort;

//Variables for Client
HTTPClient http;
int httpCode = 0;
String payload = "";

//Variables for DHT11 
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
float humidValue;
float tempValue;

void setup() {
  Serial.begin(115200);
  Serial.println("Serial Begun");

  //Begin the dht Sensor
  dht.begin();  

  //begin the WiFi
  beginWiFi();
  http.begin(serverURL);

}

void loop() {
  // put your main code here, to run repeatedly:
  
  readtempHumidData();
  
  if(WiFi.status()== WL_CONNECTED){
    sendData();
  }else{
    Serial.println("Error in WiFi connection");  
  }
  delay(1000);
}

void beginWiFi(){
  //Connect to the WiFi network
  
  WiFi.begin(ssid, pass);    
  
  while (WiFi.status() != WL_CONNECTED) {  //Wait for connection
    delay(500);
    Serial.println("Waiting to connect");
  }  

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //Print the local IP  
  
}

void readtempHumidData(){
  humidValue = dht.readHumidity(); 
  tempValue = dht.readTemperature(); 
  
  if (isnan(humidValue) || isnan(tempValue)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }   
    Serial.print("Humidity: " + String(humidValue) + " %\t");
    Serial.println("Temperature: " + String(tempValue) + "*C"+ "\t");     
}

void sendData(){
   http.begin("http://ff770eb3.ngrok.io/");
   //Specify content-type header
   http.addHeader("Content-Type", "application/json");  
    //Send the request
   httpCode = http.POST("{ \"Temperature\": \"" + String(tempValue) + "\", \"Humidity\":\"" + String(humidValue) + "\"}");   
   //Get the response payload
   payload = http.getString(); 
                    
   Serial.println("Return Code: " + String(httpCode));   //Print HTTP return code
   Serial.println("Payload: " + String(payload));
 
   http.end();  //Close connection
}
