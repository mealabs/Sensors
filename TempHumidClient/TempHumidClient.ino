#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include "DHT.h"
#include <WiFiUdp.h>

//Variables for WiFi
char ssid[] = "Animal-Shelter";
char pass[] = "kyleisdum";

//Variables for Servers
String serverIP = "192.168.137.1";
String serverPort = "80";
//String serverURL = "http://"+serverIP + ":" +serverPort;
String serverURL = "http://ff770eb3.ngrok.io/"

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

//Variables for currentTime
const long utcOffsetInSeconds = -18000;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
String currentTime;

void setup() {
  Serial.begin(115200);
  Serial.println("Serial Begun");

  //Begin the dht Sensor
  dht.begin();  

  //begin the WiFi
  beginWiFi();

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
  http.begin(serverURL);
  //Specify content-type header
  http.addHeader("Content-Type", "application/json");  
  //Send the request
  data = "{\"Device Type\": \"ESP8266\",\"Device Name\": \"Temperature_Humidity_Sensor\",\"Connection Type\": \"HTTP\",\"Sensor Data\": {\"Temperature\": \""+ String(tempValue)+"\",\"Humidity\": \""+String(humidValue)+"\"},\"Time Stamp\": \""+currentTime+"\"}"
  httpCode = http.POST(data);   
  //Get the response payload
  payload = http.getString(); 
                  
  Serial.println("Return Code: " + String(httpCode));   //Print HTTP return code
  Serial.println("Payload: " + String(payload));

  http.end();  //Close connection
}

// {
//   "Device Type": "ESP8266",
//   "Device Name": "Temperature_Humidity_Sensor",
//   "Connection Type": "HTTP",
//   "Sensor Data": {
//     "Temperature": "21*C",
//     "Humidity": "50%"
//   },
//   "Time Stamp": "5-28-2019;2:09PM"
// }