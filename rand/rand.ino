#include <ESP8266WiFi.h>
#include <String.h>

////////////////////////////////////////

#define WLAN_SSID       "R&D LABS"      //mention SSID 
//#define WLAN_PASS       "12345678"      //mention Password

//////////////////////////////////////////

#define AIO_SERVER      "10.10.110.1" // server name
#define AIO_SERVERPORT  3306

////////////////////////////////////////////       
       
#define IO_USERNAME  "IoTLabAdafruit"
#define IO_KEY       "be73eff8fc8e44ccba9e209cfbeb6cda"

/////////////////////////////////////////

void setup()
{
  Serial.begin(9600);
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);
//  WiFi.begin(WLAN_SSID, WLAN_PASS);
  WiFi.begin(WLAN_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP());  
}

void loop()
{
//  Serial.print("Random Number: ");
  int d1 = random(25, 50);
  int d2 = random(60, 100);
  Serial.print("Temperature is = ");
  Serial.println(d1);
  Serial.print(" Humidity is = ");
  Serial.println(d2);
  delay(1000);
}
