#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid     = "R&D-LABS"; // Your ssid
const char* password = "12345678"; // Your Password

const char* clientid = "ESP8266";


const char* mqtt_server = "172.16.16.53";

WiFiClient espClient;
PubSubClient client(espClient);
int t = 0;
void setup_wifi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void reconnect() {
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(clientid))
    {
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected())
  {
    reconnect(   );
  }
  client.loop();
  t = random(11, 99);
  
  client.publish("sensors/livingroom/temp",String(t).c_str());
  Serial.print("SUCCESS, rc=");
  Serial.print(client.state());

  Serial.print(" Published t = ");
  Serial.println(t);
  delay(1000);
}



















//https://randomnerdtutorials.com/esp8266-nodemcu-mqtt-publish-ds18b20-arduino/
