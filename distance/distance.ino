#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
const char *ssid = "R&D LABS";  
//const char *password = "yourPassword";
const char *host = "iotbitsathy.ga";  

const int trigPin = 2;
const int echoPin =0;
long duration;
int distance;

void setup() {
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  Serial.begin(9600);
  WiFi.mode(WIFI_OFF);        
  delay(1000);
  WiFi.mode(WIFI_STA);        
  WiFi.begin(ssid);     
  Serial.println("");
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
   Serial.print(".");
  }
}
void loop() {
HTTPClient http;   
String dist, postData;
http.begin("http://iotbitsathy.ga/savepost.php");
http.addHeader("Content-Type", "application/x-www-form-urlencoded"); 
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
distance= duration*0.034/2;
Serial.print("Distancein cm: ");
Serial.println(distance);
dist = String(distance); 
postData = "ran=" + dist;
int httpCode = http.POST(postData);   
String payload = http.getString();    
Serial.println(payload); 
http.end(); 
delay(5000);
}
