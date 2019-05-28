#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
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

//Variables for Photoresistor
int sensorValue = 0;
float voltage = 0;
bool status = false;

//Variables for currentTime
const long utcOffsetInSeconds = -18000;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
String currentTime;

void setup() {
  // put your setup code here, to run once:
  //Begin Serial
  Serial.begin(115200);
  Serial.println("Serial Begun"); 
  
  //begin the WiFi
  beginWiFi();
}

void loop() {
  // put your main code here, to run repeatedly:
  readPhotoresistorData();

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

void readPhotoresistorData(){
  sensorValue = analogRead(A0);
  voltage = sensorValue * (5.0/1023.0);
  if(voltage > 80){
    status = true;
  }else{
    status = false;
  }
}

void sendData(){
  http.begin(serverURL);
  //Specify content-type header
  http.addHeader("Content-Type", "application/json");  
  //Send the request
  data = "{\"Device Type\": \"ESP8266\",\"Device Name\": \"Laser Trip Wire\",\"Connection Type\": \"HTTP\",\"Sensor Data\": \""+status+"\",\"Time Stamp\": \""+currentTime+"\"}"
  httpCode = http.POST(data);   

  //Get the response payload
  payload = http.getString();                  
  Serial.println(httpCode);   //Print HTTP return code

  http.end();  //Close connection
}