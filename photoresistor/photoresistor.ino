#include <ESP8266WiFi.h>

//Variables for WiFi
char ssid[] = "better-better-eduroam";
char pass[] = "kyleisdum";

const int buzzer = 0;
int sensorValue = 0;
float voltage = 0;

void setup() {
  // put your setup code here, to run once:
  beginWiFi();
  Serial.begin(2400);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  sensorValue = analogRead(A0);
  voltage = sensorValue * (5.0/1023.0);
  Serial.println(voltage);
  if(voltage < 0.4){
    digitalWrite(buzzer, HIGH);
    delay(sensorValue);
    digitalWrite(buzzer, LOW);
    delay(sensorValue);
  }
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
