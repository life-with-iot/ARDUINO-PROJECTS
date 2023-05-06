
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"


#define trigPin D1
#define echoPin D2
long T;
float distanceCM;

#define WLAN_SSID       "R&D LABS"             // Your SSID
#define WLAN_PASS       ""      //mention Password

/************************* Adafruit.io Setup *********************************/

#define IO_SERVER      "io.adafruit.com"
#define IO_SERVERPORT  1883                   // use 8883 for SSL

#define IO_USERNAME  "IoTLabAdafruit"
#define IO_KEY       "be73eff8fc8e44ccba9e209cfbeb6cda"

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, IO_SERVER, IO_SERVERPORT, IO_USERNAME, IO_KEY);

/****************************** Feeds ***************************************/


// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Publish DIST = Adafruit_MQTT_Publish(&mqtt, IO_USERNAME "/feeds/DIST");


void MQTT_connect();

void setup() {

  Serial.begin(115200);

  // Connect to WiFi access point.
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
 
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  Serial.begin(115200);  
}

void loop() {
 
  MQTT_connect();
  digitalWrite(trigPin, LOW);
  delay(1);
  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin, LOW);
  T=pulseIn(echoPin,HIGH);
 distanceCM= T * 0.034/2;
  Serial.print("distance in CM ");
Serial.println(distanceCM);
DIST.publish(distanceCM);
  }

  void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");  
}
