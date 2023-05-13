#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
/************************* WiFi Access Point *********************************/
#define WLAN_SSID       "R&D LABS"
#define WLAN_PASS       "12345678"
/********************a***** Adafruit.io Setup *********************************/
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "vd19splab"
#define AIO_KEY         "aio_zaff123ZMPJwAPxATDivXcJsEIOG"
void send_event(const char *event);
const char *host = "maker.ifttt.com";
const char *privateKey = "blFXb8wdFvJZ6o4EALzFHV";
WiFiServer server(80);
/************* DHT11 Setup ********************************************/
const int trigPin = 2;  //D4
const int echoPin = 0;  //D3
long duration;
int distance;
float sensorValue = A0;
float sensorVoltage;
float level;
int sensor = D2;
int gas_value;
/************ Global State (you don't need to change this!) ******************/
// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;
// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
/****************************** Feeds ***************************************/
// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish distance1 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/dist");
Adafruit_MQTT_Publish SensorVoltage = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/methane");
Adafruit_MQTT_Publish Gas_value = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/carbon");
/*************************** Sketch Code ************************************/
// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();
void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(115200); // Starts the serial communication
  Serial.println(F("Adafruit MQTT demo"));
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  Serial.println("Server started");
}
uint32_t x = 0;
void loop() {
  MQTT_connect();
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.println(distance);
  level = ((14 - distance) / 14.0) * 100;
  Serial.println("level");
  Serial.println(level);
  delay(1000);
  sensorValue = analogRead(A0);
  sensorVoltage = sensorValue / 1024 * 5.0;
  Serial.print("Methane  = ");
  Serial.print(sensorVoltage);
  Serial.println("mg/l");
  gas_value = digitalRead(sensor);
  Serial.print("co2=");
  Serial.print(gas_value);
  Serial.println("ppm");
  Serial.println("Lattitude:11.4983");
  Serial.println("Lattitude:77.2452");
  delay(1000);
  distance1.publish(distance);
  SensorVoltage.publish(sensorVoltage);
  Gas_value.publish(47);
  delay(5000);
  if ( level <= 10) {
    send_event("jar_event");
  }
}
void send_event(const char *event)
{
  Serial.print("Connecting to ");
  Serial.println(host);
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("Connection failed");
    return;
  }
  // We now create a URI for the request
  String url = "/trigger/";
  url += event;
  url += "/with/key/";
  url += privateKey;
  Serial.print("Requesting URL: ");
  Serial.println(url);
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  while (client.connected())
  {
    if (client.available())
    {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    } else {
      // No data yet, wait a bit
      delay(50);
    };
  }
  Serial.println();
  Serial.println("closing connection");
  client.stop();
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
