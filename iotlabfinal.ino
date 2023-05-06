#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h> 
const char* ssid = "tamil";
const char* pwd = "12345678";
const char* host = "http://35.226.144.150/switchs/";
String get_host = "http://35.226.144.150/switchs/";
WiFiServer server(80); 
#define Relay1 D1
#define Relay2 D2
#define Relay3 D3
#define Relay4 D4
#define Relay5 D5
#define Relay6 D6
#define Relay7 D7
#define Relay8 D8
void setup() {
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);
  pinMode(Relay5, OUTPUT);
  pinMode(Relay6, OUTPUT);
  pinMode(Relay7, OUTPUT);
  pinMode(Relay8, OUTPUT);
  delay(500);
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  
  WiFi.begin(ssid,pwd);     //Connect to your WiFi router
//  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(400);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
}

void loop()
{
  WiFiClient client = server.available();
  HTTPClient http;
  int i=1;
  String y=String(i);

  String url1 = get_host+"value.php?id=1";
  http.begin(url1);
  int httpCode1 = http.GET();
  String payload1 = http.getString();
  if((payload1=="ON")||(payload1=="")){
     digitalWrite(Relay1, LOW);
    for(i=2;i<=8;i++){
  String url = get_host+"value.php?id="+i;
        http.begin(url);
        int httpCode = http.GET();
        String payload = http.getString();
        if((i==2)&&(payload=="ON")){
          digitalWrite(Relay2, LOW);
        }
        if((i==2)&&(payload=="OFF")){
            digitalWrite(Relay2, HIGH);
          }
        if((i==3)&&(payload=="ON")){
          digitalWrite(Relay3, LOW);
        }
        if((i==3)&&(payload=="OFF")){
            digitalWrite(Relay3, HIGH);
          }
        if((i==4)&&(payload=="ON")){
          digitalWrite(Relay4, LOW);
        }
        if((i==4)&&(payload=="OFF")){
            digitalWrite(Relay4, HIGH);
          }
        if((i==5)&&(payload=="ON")){
          digitalWrite(Relay5, LOW);
        }
        if((i==5)&&(payload=="OFF")){
            digitalWrite(Relay5, HIGH);
          }
        if((i==6)&&(payload=="ON")){
          digitalWrite(Relay6, LOW);
        }
        if((i==6)&&(payload=="OFF")){
            digitalWrite(Relay6, HIGH);
          }
        if((i==7)&&(payload=="ON")){
          digitalWrite(Relay7, LOW);
        }
        if((i==7)&&(payload=="OFF")){
            digitalWrite(Relay7, HIGH);
          }
        if((i==8)&&(payload=="ON")){
          digitalWrite(Relay8, LOW);
        }
        if((i==8)&&(payload=="OFF")){
            digitalWrite(Relay8, HIGH);
          }
       }
  }
  else
  {
    digitalWrite(Relay1, HIGH);
    digitalWrite(Relay2, HIGH);
    digitalWrite(Relay3, HIGH);
    digitalWrite(Relay4, HIGH);
    digitalWrite(Relay5, HIGH);
    digitalWrite(Relay6, HIGH);
    digitalWrite(Relay7, HIGH);
    digitalWrite(Relay8, HIGH);
    
    }
   
        http.end();
}
