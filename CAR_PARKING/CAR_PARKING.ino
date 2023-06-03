#include <Wire.h>
#include "DHT.h"
#include <ESP8266WiFi.h>
#include "ESPAsyncWebServer.h"

// Replace with your network credentials
const char* ssid = "R&D-LABS";
const char* password = "12345678";

// Uncomment one of the lines below for whatever DHT sensor type you're using!
//#define DHTTYPE DHT11 // DHT 11
//#define DHTTYPE DHT21 // DHT 21 (AM2301)
#define DHTTYPE DHT22 // DHT 22 (AM2302), AM2321
//DHT Sensor;
uint8_t DHTPin = 12;
DHT dht(DHTPin, DHTTYPE);

float temperature_Celsius;
float temperature_Fahrenheit;
float Humidity;

AsyncWebServer server(80);
AsyncEventSource events("/events");

unsigned long lastTime = 0;  
unsigned long timerDelay = 3000;  // send readings timer

void getDHTReadings(){
 
   Humidity = random(40,90);
  // Read temperature as Celsius (the default)
  temperature_Celsius = random(40,90);
  // Read temperature as Fahrenheit (isFahrenheit = true)
  temperature_Fahrenheit= random(40,90);

//  Humidity = dht.readHumidity();
//  // Read temperature as Celsius (the default)
//  temperature_Celsius = dht.readTemperature();
//  // Read temperature as Fahrenheit (isFahrenheit = true)
//  temperature_Fahrenheit= dht.readTemperature(true);
}

String processor(const String& var){
  getDHTReadings();
  //Serial.println(var);
  if(var == "TEMPERATURE_C"){
    return String(temperature_Celsius);
  }
  else if(var == "TEMPERATURE_F"){
    return String(temperature_Fahrenheit);
  }
   else if(var == "HUMIDITY"){
    return String(Humidity);
  }
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>DHT Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    p {  font-size: 1.2rem;}
    body {  margin: 0;}
    .topnav { overflow: hidden; background-color: #4B1D3F; color: white; font-size: 1.7rem; }
    .content { padding: 20px; }
    .card { background-color: white; box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5); }
    .cards { max-width: 700px; margin: 0 auto; display: grid; grid-gap: 2rem; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr)); }
    .reading { font-size: 2.8rem; }
    .card.temperature { color: #0e7c7b; }
    .card.humidity { color: #17bebb; }
  </style>
</head>
<body>
  <div class="topnav">
    <h3>DHT WEB SERVER</h3>
  </div>
  <div class="content">
    <div class="cards">
      <div class="card 1">
        <h4><i class="fas fa-thermometer-half"></i> TEMPERATURE</h4><p><span class="reading"><span id="temp_celcius">%TEMPERATURE_C%</span> &deg;C</span></p>
      </div>
      <div class="card 2">
        <h4><i class="fa-solid fa-garage-car"></i> TEMPERATURE</h4><p><span class="reading"><span id="temp_fahrenheit">%TEMPERATURE_F%</span> &deg;F</span></p>
      </div>
      <div class="card 3">
        <h4><i class="fa-solid fa-car-garage"></i> HUMIDITY</h4><p><span class="reading"><span id="hum">%HUMIDITY%</span> &percnt;</span></p>
      </div>
      <div class="card 4">
        <h4><i class="fa-solid fa-car-on"></i> HUMIDITY</h4><p><span class="reading"><span id="hum">%HUMIDITY%</span> &percnt;</span></p>
      </div>
    </div>
  </div>
<script>
if (!!window.EventSource) {
 var source = new EventSource('/events');
 
 source.addEventListener('open', function(e) {
  console.log("Events Connected");
 }, false);
 source.addEventListener('error', function(e) {
  if (e.target.readyState != EventSource.OPEN) {
    console.log("Events Disconnected");
  }
 }, false);
 
 source.addEventListener('message', function(e) {
  console.log("message", e.data);
 }, false);
 
 source.addEventListener('temperature_Celsius', function(e) {
  console.log("temperature", e.data);
  document.getElementById("temp_celcius").innerHTML = e.data;
 }, false);
 
 source.addEventListener('temperature_Fahrenheit', function(e) {
  console.log("temperature", e.data);
  document.getElementById("temp_fahrenheit").innerHTML = e.data;
 }, false);
 source.addEventListener('humidity', function(e) {
  console.log("humidity", e.data);
  document.getElementById("hum").innerHTML = e.data;
 }, false);
 
}
</script>
</body>
</html>)rawliteral";

void setup() {
  Serial.begin(115200);
  pinMode(DHTPin, INPUT);
  dht.begin();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Setting as a Wi-Fi Station..");
  }
  Serial.print("Station IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  
  // Handle Web Server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Handle Web Server Events
  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);
  server.begin();
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    
    getDHTReadings();
    Serial.printf("Temperature = %.2f ºC \n", temperature_Celsius);
    Serial.printf("Temperature = %.2f ºF \n", temperature_Fahrenheit);
    Serial.printf("Humidity= %f %\n", Humidity);
    Serial.println();

    // Send Events to the Web Server with the Sensor Readings
    events.send("ping",NULL,millis());
    events.send(String(temperature_Celsius).c_str(),"temperature_Celsius",millis());
    events.send(String(temperature_Fahrenheit).c_str(),"temperature_Fahrenheit",millis());
    events.send(String(Humidity).c_str(),"humidity",millis());
    
    lastTime = millis();
  }
}
