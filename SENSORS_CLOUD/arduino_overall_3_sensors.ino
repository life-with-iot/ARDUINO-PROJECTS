#include <SoftwareSerial.h>
#include <ArduinoJson.h>
SoftwareSerial s(5,6);   // Tx, Rx
 #include "DHT.h"
#define DHTPIN 2     
#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE);
const int ledPin = LED_BUILTIN;
const int ldrPin = A0;

 String ai;
 const int trigger = 3;
const int echo = 4;
long T;
float distanceCM;
int smokeA0 = A5;
int analogSensor = analogRead(smokeA0);
void setup() {
  s.begin(115200);
//  Serial.begin(115200);
  pinMode(A0,INPUT);
  dht.begin();
  pinMode(ledPin, OUTPUT);

pinMode(ldrPin, INPUT);
 pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(smokeA0, INPUT);
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
  int ldrStatus = analogRead(ldrPin);
  if (ldrStatus >= 200) {
     ai="dark";
digitalWrite(ledPin, HIGH);
    } else {
   ai="bright";
digitalWrite(ledPin, LOW);}

//Serial.print("Its DARK, Turn off the LED : ");

//Serial.println(ldrStatus);
 digitalWrite(trigger, LOW);
  delay(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  T = pulseIn(echo, HIGH);
  distanceCM = T * 0.034/2;
  analogSensor = analogRead(smokeA0);




  

if (isnan(h) || isnan(t) || isnan(f) || isnan(ldrStatus)|| isnan(distanceCM)|| isnan(analogSensor) ){
    return;
  }
  // If the DHT-11 is not connected to correct pin or if it doesnot
//work no data will be sent
  root["temp"] = t;
  root["hum"] = h;
  root["hi"] = hic;
  root["gasv"]= analogRead(A0);
  root["ldr"]=ai;
  root["dist"]=distanceCM;
  root["gas"]=analogSensor;
//    Serial.print("t ");
//    Serial.println(t);
//    Serial.print("h ");
//    Serial.println(h);
//    Serial.print("hic ");
//    Serial.println(hic);
//    Serial.print("analog ");
//    Serial.println(analogRead(A0));

 if(s.available()>0)
{
 root.printTo(s);
}

  delay(1000);
}
