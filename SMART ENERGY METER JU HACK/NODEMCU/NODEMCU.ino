#include <SoftwareSerial.h>
SoftwareSerial s(D5,D6); //Tx, Rx
#include <ArduinoJson.h>
 
void setup() {
  // Initialize Serial port
  Serial.begin(115200);
  s.begin(115200);
  while (!Serial) continue;
}
 
void loop() {
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
  int data1=root["temp"];
  Serial.println(data1);
  Serial.print("Humidity    ");
  int data2=root["hum"];
  Serial.println(data2);
  Serial.print("Heat-index  ");
  int data3=root["hi"];
  Serial.println(data3);
  Serial.print("gas sensor  ");
  int data4=root["gasv"];
  Serial.println(data4);
  Serial.println("");
  Serial.println("---------------------xxxxx--------------------");
 Serial.println("");
}
