//PROCEDURE IS AVILABLE IN THIS WEBSITE--->https://gist.github.com/pudelosha/5a42092bbcf0bfc9ddab20e6093e68b7
#include <DHT.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#define sensorPin1 D1

//#define sensorPin2 D2
#define typeDHT DHT11
DHT dht(sensorPin1, typeDHT);

char ssid[] = "R&D LABS";                 // Network Name
//char pass[] = "xxx";                 // Network Password
byte mac[6];

WiFiServer server(80);
IPAddress ip(10, 10, 188, 223);
IPAddress gateway(10, 10, 160, 1);
IPAddress subnet(255, 255, 224, 0);

WiFiClient client;
MySQL_Connection conn((Client *)&client);

char INSERTSQL[] = "INSERT INTO iot.iotcloud(time) VALUES (curtime())";
char INSERT_SQL[] = "INSERT INTO iot.iotcloud(data) VALUES (%d)";
//char INSERT_SQL[] = "INSERT INTO officeto_plants.TBL_READINGS(ID_PLANT, AIR_HUMIDITY, AIR_TEMPERATURE, SOIL_MOISTURE_1, SOIL_MOISTURE_2) VALUES (1, NULL, NULL, %d, %d)";
char query[128];

IPAddress server_addr(10, 10 ,110, 5);          // MySQL server IP
char user[] = "iot";           // MySQL user
char password[] = "iot";       // MySQL password

void setup() {

  Serial.begin(115200);

  pinMode(sensorPin1, INPUT);
//  pinMode(sensorPin2, INPUT);

  Serial.println("Initialising connection");
  Serial.print(F("Setting static ip to : "));
  Serial.println(ip);

  Serial.println("");
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.config(ip, gateway, subnet); 
  WiFi.begin(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi Connected");

  WiFi.macAddress(mac);
  Serial.print("MAC: ");
  Serial.print(mac[5],HEX);
  Serial.print(":");
  Serial.print(mac[4],HEX);
  Serial.print(":");
  Serial.print(mac[3],HEX);
  Serial.print(":");
  Serial.print(mac[2],HEX);
  Serial.print(":");
  Serial.print(mac[1],HEX);
  Serial.print(":");
  Serial.println(mac[0],HEX);
  Serial.println("");
  Serial.print("Assigned IP: ");
  Serial.print(WiFi.localIP());
  Serial.println("");

  Serial.println("Connecting to database");

  while (conn.connect(server_addr, 3306, user, password) != true) {
    delay(200);
    Serial.print ( "." );
  }

  Serial.println("");
  Serial.println("Connected to SQL Server!");  

}

void loop() {

//  int soil_hum = 1024 - analogRead(sensorPin1);
  float t = dht.readTemperature();

  Serial.println(t);

  delay(100); 
  sprintf(query, INSERT_SQL, t);
  //sprintf(query, INSERT_SQL, soil_hum, t);

  Serial.println("Recording data.");
  Serial.println(query);
  
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  
  cur_mem->execute(query);

  if(cur_mem->execute(INSERTSQL)==1)
    Serial.println("execute");

  delete cur_mem;

}
