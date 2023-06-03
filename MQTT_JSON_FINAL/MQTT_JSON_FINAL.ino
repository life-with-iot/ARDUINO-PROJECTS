//https://techtutorialsx.com/2017/04/29/esp32-sending-json-messages-over-mqtt/
//#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
//#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "R&D-LABS";
const char* password = "12345678";
//const char* mqttServer = "m11.cloudmqtt.com";
//const int mqttPort = 12948;
char* mqttServer = "172.16.16.104";
const int mqttPort = 1883;
const char* mqttUser = "iot_lab";
const char* mqttPassword = "12345678";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {

  Serial.begin(9600);
  Serial.println();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");

  client.setServer(mqttServer, mqttPort);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect("ESP32Client", mqttUser, mqttPassword )) {

      Serial.println("connected");

    } else {

      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);

    }
  }

}

void loop() {

  StaticJsonBuffer<300> JSONbuffer;
  JsonObject& JSONencoder = JSONbuffer.createObject();

  JSONencoder["device"] = "ESP32";
  JSONencoder["sensorType"] = "Temperature";
  JsonArray& values = JSONencoder.createNestedArray("values");

  //  values.add(20);
  //  values.add(21);
  //  values.add(23);

  values.add(random(1,9));
  values.add(random(20,29));
  values.add(random(50,59));

  char JSONmessageBuffer[100];
  JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  Serial.println("Sending message to MQTT topic..");
  Serial.println(JSONmessageBuffer);

  if (client.publish("sensors/livingroom/temp", JSONmessageBuffer) == true) {
    Serial.println("Success sending message");
    Serial.print("SUCCESS, rc=");
    Serial.print(client.state());
  } else {
    Serial.println("Error sending message");
    Serial.print("Error, rc=");
    Serial.print(client.state());
  }

  client.loop();
  Serial.println("--------------------------------------------------------------");

  delay(5000);

}
