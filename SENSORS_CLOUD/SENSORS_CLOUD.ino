#include "DHTesp.h"
DHTesp dht;

#define DHTPIN 10
const int ldrPin = A0;
int gas_sensor  = A1;
const int trigger = 8;
const int echo = 9;
const int ledPin = 13;

long T;
float distanceCM;
float m = -0.318;
float b = 1.133;  
float R0 = 11.820; 

void setup() {
    
    Serial.begin(9600);
    dht.setup(DHTPIN, DHTesp::DHT11);
       
    pinMode(ldrPin, INPUT);
    pinMode(echo, INPUT);
    pinMode(trigger, OUTPUT);
    pinMode(ledPin, OUTPUT);    
    pinMode(gas_sensor, INPUT);

}

void loop() 
{
  
  int ldrStatus = analogRead(ldrPin);
  
  if (ldrStatus <= 200) 
  {
  
      digitalWrite(ledPin, LOW);
      
      Serial.print("Its BRIGHT, Turn off the LED : ");
      
      Serial.println(ldrStatus);
  
  } 
  else 
  {
  
  digitalWrite(ledPin, HIGH);
  
  Serial.print("Its DARK, Turn on the LED : ");
  
  Serial.println(ldrStatus);
  
  }

  float h = dht.getHumidity();
  float t = dht.getTemperature();
  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print("\t");
  Serial.print("Humidity: ");
  Serial.println(h);

  digitalWrite(trigger, LOW);
  delay(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  T = pulseIn(echo, HIGH);
  distanceCM = T * 0.034 / 2;
  Serial.print("Distance in cm: ");
  Serial.println(distanceCM);

  float sensor_volt; 
  float RS_gas;
  float ratio; 
  float sensorValue = analogRead(gas_sensor);
  sensor_volt = sensorValue * (5.0 / 1023.0); 
  RS_gas = ((5.0 * 10.0) / sensor_volt) - 10.0;
  ratio = RS_gas / R0;  

   
  double ppm_log = (log10(ratio) - b) / m;
  double ppm = pow(10, ppm_log);
  double percentage = ppm / 10000;
  Serial.print("ppm= ");
  Serial.print(percentage);
  Serial.println(" %");
  delay(2000);

}

 

 
