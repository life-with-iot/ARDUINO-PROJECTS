#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#define WLAN_SSID       "Water tank"
#define WLAN_PASS       "123405678"
const int trigPin = D1;
const int echoPin = D2;
const char *host = "iotbitsathy.ga";  
float l = 6.28;
float b = 3.11;
float volume;
long duration;
float distance;
float distance1;

/************************* Adafruit.io Setup *********************************/
#define IO_SERVER      "io.adafruit.com"
#define IO_SERVERPORT  1883                   // use 8883 for SSL
#define IO_USERNAME  "IoTRameshravi"
#define IO_KEY       "c4f6a970bc264e36807024afe17ad141"
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, IO_SERVER, IO_SERVERPORT, IO_USERNAME, IO_KEY);
Adafruit_MQTT_Publish d = Adafruit_MQTT_Publish(&mqtt, IO_USERNAME "/feeds/watercm");
Adafruit_MQTT_Publish v = Adafruit_MQTT_Publish(&mqtt, IO_USERNAME "/feeds/waterlit");
void MQTT_connect();
void setup() {
  volume=constrain(volume,0,40000);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  HTTPClient http;  
  String dist, postData;
  http.begin("http://iotbitsathy.ga/volume/upload.php");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  MQTT_connect();
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance =(( duration * 0.034 / 2)-20);
  Serial.print("Distance: ");
  Serial.println(distance);
  distance1 =( ( distance)/100);
  volume = (((2-distance1) * l * b) * 1000);
  Serial.println("volume=");
  Serial.println(volume);
  d.publish(distance);
  v.publish(volume);
  String vol="";
  vol+=volume;
  String dis="";
  dis+=distance;
  postData = "vol=" + vol+"&dis="+dis;
  int httpCode = http.POST(postData);  
  String payload = http.getString();    
  Serial.println(payload);
  http.end();
  delay(5000);
}
void MQTT_connect() {
  int8_t ret;
  if (mqtt.connected()) {
    return;
  }
  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);
    retries--;
    if (retries == 0) {
     while (1);
    }
  }
  Serial.println("MQTT Connected!");
}
