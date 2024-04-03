#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>



const char *SSID = "redmi";
const char *PWD = "12345678";



const int adcPin1 = 34;
const int adcPin2 = 35;
const int adcPin3 = 36;
const int adcPin4 = 39;




WebServer server(80);
 

StaticJsonDocument<250> jsonDocument;
char buffer[250];

float temperature = 30.0;
float humidity = 10.0;
float pressure = 20.0;
 
void setup_routing() {     
  server.on("/temperature", getTemperature);     
  server.on("/pressure", getPressure);     
  server.on("/humidity", getHumidity);     
  server.on("/data", getData); 
  server.on("/adc",handleGetADCValues);    
  
          
  server.begin();    
}
 
void create_json(char *tag, float value, char *unit) {  
  jsonDocument.clear();  
  jsonDocument["type"] = tag;
  jsonDocument["value"] = value;
  jsonDocument["unit"] = unit;
  serializeJson(jsonDocument, buffer);
}
 
void add_json_object(char *tag, float value, char *unit) {
  JsonObject obj = jsonDocument.createNestedObject();
  obj["type"] = tag;
  obj["value"] = value;
  obj["unit"] = unit; 
}


 
void getTemperature() {
  Serial.println("Get temperature");
  create_json("temperature", temperature, "°C");
  server.send(200, "application/json", buffer);
}
 
void getHumidity() {
  Serial.println("Get humidity");
  create_json("humidity", humidity, "%");
  server.send(200, "application/json", buffer);
}
 
void getPressure() {
  Serial.println("Get pressure");
  create_json("pressure", pressure, "hPa");
  server.send(200, "application/json", buffer);
}
 
void getData() {
  Serial.println("Get BME280 Sensor Data");
  jsonDocument.clear();
  add_json_object("temperature", temperature, "°C");
  add_json_object("humidity", humidity, "%");
  add_json_object("pressure", pressure, "hPa");
  serializeJson(jsonDocument, buffer);
  server.send(200, "application/json", buffer);
}
void handleGetADCValues() {
  int adcValue1 = analogRead(adcPin1);
  int adcValue2 = analogRead(adcPin2);
  int adcValue3 = analogRead(adcPin3);
  int adcValue4 = analogRead(adcPin4);
  jsonDocument.clear();
  add_json_object("adcValue1", adcValue1,"v");
  add_json_object("adcValue2", adcValue2,"v");
  add_json_object("adcValue3",adcValue3, "v");
  add_json_object("adcValue4",adcValue4, "v");
  serializeJson(jsonDocument, buffer);
  server.send(200, "application/json", buffer);
}



void setup() {     
  Serial.begin(115200); 

 

  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(SSID, PWD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
 
  Serial.print("Connected! IP Address: ");
  Serial.println(WiFi.localIP());
  
  setup_routing();     
   
}    
       
void loop() {    
  server.handleClient();  
  Serial.println(WiFi.localIP());   
}
