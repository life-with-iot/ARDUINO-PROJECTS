#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
SoftwareSerial s(D5, D6); //Tx, Rx
#include <ArduinoJson.h>
#define WLAN_SSID       "R&D LABS"             // Your SSID
#define WLAN_PASS       ""      //mention Password

/************************* Adafruit.io Setup *********************************/

#define IO_SERVER      "io.adafruit.com"
#define IO_SERVERPORT  1883                   // use 8883 for SSL

#define IO_USERNAME  "adventures2701"
#define IO_KEY       "d80c6678f66c4b2a835ea439b9f3aeaf"

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, IO_SERVER, IO_SERVERPORT, IO_USERNAME, IO_KEY);

/****************************** Feeds ***************************************/


// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Publish temp = Adafruit_MQTT_Publish(&mqtt, IO_USERNAME "/feeds/temp");
Adafruit_MQTT_Publish hum = Adafruit_MQTT_Publish(&mqtt, IO_USERNAME "/feeds/hum");
Adafruit_MQTT_Publish ldr = Adafruit_MQTT_Publish(&mqtt, IO_USERNAME "/feeds/ldr");
Adafruit_MQTT_Publish dist = Adafruit_MQTT_Publish(&mqtt, IO_USERNAME "/feeds/dist");
Adafruit_MQTT_Publish gas = Adafruit_MQTT_Publish(&mqtt, IO_USERNAME "/feeds/gas");



void MQTT_connect();

void setup() {
  // Initialize Serial port
  Serial.begin(115200);
  s.begin(115200);
  while (!Serial) continue;
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  //    WiFi.begin(WLAN_SSID);

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
  MQTT_connect();
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(s);

  if (root == JsonObject::invalid())
  {
    return;
  }
  //Print the data in the serial monitor
  Serial.println("JSON received and parsed");
  root.prettyPrintTo(Serial);
  Serial.println("");
  Serial.print("Temperature ");
  int data1 = root["temp"];
  Serial.println(data1);
  Serial.print("Humidity    ");
  int data2 = root["hum"];
  Serial.println(data2);
  Serial.print("Heat-index  ");
  int data3 = root["hi"];
  Serial.println(data3);
  Serial.print("gas sensor  ");
  int data4 = root["gasv"];
  Serial.println(data4);
  Serial.print("ldrStatus");
  int data5 = root["ldr"];
  Serial.println(data5);
  Serial.print("distance");
  int data6 = root["dist"];
  Serial.println(data6);
  Serial.print("analogSensor");
  int data7 = root["gas"];
  Serial.println(data7);

  Serial.println("");
  Serial.println("---------------------xxxxx--------------------");
  Serial.println("");
  temp.publish(data1);
 hum.publish(data2);
  ldr.publish(data5);
  dist.publish(data6);
  gas.publish(data7);

delay(10000);


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
