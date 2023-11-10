#include <ThingSpeak.h>  
#include <WiFiNINA.h>
#include <DHT.h>

#define DHT 13
#define DHTTYPE DHT11  

char wifi_name[] = "iPhone";     
char password[] = "abcdefghi"; 

unsigned long channelID = 2251287;         
const char *apiKey = "6HDFKMSVZH5PS0CF";   

DHT dht(DHT, DHTTYPE);

WiFiClient wifiClient;

void setup() {
  Serial.begin(9600);
  dht.begin();
  connectToWiFi();
  ThingSpeak.begin(wifiClient);
}

void connectToWiFi() {
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Establishing Connection");
    WiFi.begin(wifi_name, password);
    delay(1000);
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  delay(1000);  

  float temperature = dht.readTemperature();    
  float humidity = dht.readHumidity();           
  if (!isnan(temperature) && !isnan(humidity)) {
    sendDataToThingSpeak(temperature, humidity);
  } else {
    Serial.println("Data is flawed!");
  }
  
  delay(30000); 
}

void sendDataToThingSpeak(float temp, float humidity) {
  ThingSpeak.setField(1, temp);       
  ThingSpeak.setField(2, humidity);
  
  int httpCode = ThingSpeak.writeFields(channelID, apiKey);

  if (httpCode == 200) {
    Serial.println("Data sent to ThingSpeak");
  } else {
    Serial.println("Error sending data to ThingSpeak");
  }
}