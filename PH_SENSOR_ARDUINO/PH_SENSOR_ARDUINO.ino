#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include <stdlib.h>
//#include <SoftwareSerial.h>
//SoftwareSerial nodemcu(2,3);

#define SensorPin 0          //pH meter Analog output to Arduino Analog Input 0
unsigned long int avgValue;  //Store the average value of the sensor feedback
float b;
int buf[10],temp;
float phValue;

// for float value to string converstion
int f;
  float val; // also works with double. 
  char buff2[10];
//  String valueString = "";
//  String Value = ""; 

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "KGNDf7yrz0MeV4NFsd3a_upwXlwU9XqL";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Redmi";
char pass[] = "jothimani";




void setup()
{
  pinMode(13,OUTPUT);
    pinMode(D1,OUTPUT);  
  Serial.begin(9600);  
//  nodemcu.begin(9600); 
  Blynk.begin(auth, ssid, pass); 
}
void loop()
{
  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(SensorPin);
    delay(10);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
    avgValue+=buf[i];
  phValue=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
  phValue=3.5*phValue;                      //convert the millivolt into pH value
  Serial.println(phValue);
      pinMode(D1,phValue);  
      Blynk.virtualWrite(V1,phValue);
  
//     Value =  dtostrf(phValue, 4, 2, buff2);  //4 is mininum width, 6 is precision
//   valueString = valueString + Value +","; 
//   Serial.println(valueString);
//   nodemcu.println(valueString);
//  valueString = "";

   Blynk.run();
 delay(1000);


}
