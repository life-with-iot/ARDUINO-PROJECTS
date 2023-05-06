#include <SoftwareSerial.h>
#include <ArduinoJson.h>
SoftwareSerial s(5,6);   // Tx, Rx
#include "DHT.h"
#define DHTPIN 2     
#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE);
 
void setup() {
  s.begin(115200);
//  Serial.begin(115200);
  pinMode(A0,INPUT);
  dht.begin();
}
StaticJsonBuffer<1000> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();
void loop() {
 
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
 
  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
if (isnan(h) || isnan(t) || isnan(f)) {
    return;
  }
  // If the DHT-11 is not connected to correct pin or if it doesnot
//work no data will be sent
  root["temp"] = t;
  root["hum"] = h;
  root["hi"] = hic;
  root["gasv"]= analogRead(A0);

//    Serial.print("t ");
//    Serial.println(t);
//    Serial.print("h ");
//    Serial.println(h);
//    Serial.print("hic ");
//    Serial.println(hic);
//    Serial.print("analog ");
//    Serial.println(analogRead(A0));

    delay(1000);
    
 
if(s.available()>0)
{
 root.printTo(s);
}
}
