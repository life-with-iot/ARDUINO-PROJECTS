#include <ESP8266WiFi.h>
#include <time.h>
#include <SPI.h>
#include <DMD2.h>
#include <fonts/Arial14.h>

const char* ssid = "Gokul";
const char* password = "asdfghjk";

int ledPin =LED_BUILTIN ;
SPIDMD dmd(1,1);  // DMD controls the entire display
DMD_TextBox box(dmd, 0, 2);  // "box" provides a text box to automatically write to/scroll the display


int timezone = 19800;
int dst = 0;
String h;
String m;
int i=1;
void setup() {
  dmd.setBrightness(255);
  dmd.selectFont(Arial14);
 
  
  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin,LOW);

  Serial.begin(115200);
  Serial.println();
  Serial.print("Wifi connecting to ");
  Serial.println( ssid );

  WiFi.begin(ssid,password);

  Serial.println();
  
  Serial.print("Connecting");

  while( WiFi.status() != WL_CONNECTED ){
      delay(500);
      Serial.print(".");        
  }

  digitalWrite( ledPin , HIGH);
  Serial.println();

  Serial.println("Wifi Connected Success!");
  Serial.print("NodeMCU IP Address : ");
  Serial.println(WiFi.localIP() );

  configTime(timezone, dst, "pool.ntp.org","time.nist.gov");
  Serial.println("\nWaiting for Internet time");

  while(!time(nullptr)){
     Serial.print("*");
     delay(1000);
  }
  Serial.println("\nTime response....OK");  
  dmd.begin(); 
}

void loop() {
  
  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);

  Serial.print(p_tm->tm_mday);
  Serial.print("/");
  Serial.print(p_tm->tm_mon + 1);
  Serial.print("/");
  Serial.print(p_tm->tm_year + 1900);
  
  Serial.print(" ");
  
 
  Serial.println(p_tm->tm_hour);
  Serial.println(p_tm->tm_min);
  Serial.println(p_tm->tm_sec);
 
  Serial.println(F("..."));
  box.print(' ');
  box.print(p_tm->tm_hour);
  box.print(';');
  box.print(p_tm->tm_min);
  box.println(F(""));
  delay(1000);
  
  

}
