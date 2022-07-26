#include "DHT.h"

#define DHTPIN D1     // Digital pin connected to the DHT sensor

#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

#define ledPin D4

/****************************************
   Include Libraries
 ****************************************/
#include "UbidotsESPMQTT.h"

/****************************************
   Define Constants
 ****************************************/
#define TOKEN "BBFF-ljWQKFtDZUp04WydBFq0aco2kqJqrT" // Your Ubidots TOKEN
#define WIFINAME "Nokia 3.1" //Your SSID
#define WIFIPASS "1234567890" // Your Wifi Pass

Ubidots client(TOKEN);

/****************************************
   Auxiliar Functions
 ****************************************/
unsigned long tick = millis();

void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println("Message arrived [" + topic + "] = " + String(message));
}

/****************************************
   Main Functions
 ****************************************/

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  client.setDebug(true); // Pass a true or false bool value to activate debug messages
  client.wifiConnection(WIFINAME, WIFIPASS);
  client.begin(callback);
  pinMode(ledPin, OUTPUT);
  Serial.println("DHT11 test!");

  dht.begin();
  client.ubidotsSubscribe("nodemcu", "light"); //Insert the dataSource and Variable's Labels
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!client.connected()) {
    client.reconnect();
    client.ubidotsSubscribe("nodemcu", "light"); //Insert the dataSource and Variable's Labels
  }

  if (millis() - tick > 10000) {
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t))
    {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }

    Serial.println("Humidity: " + String(h) + "%, Temperature: " + String(t) + "°C ");

    client.add("temperature", t);
    client.add("humidity", h);
    client.ubidotsPublish("nodemcu");
    tick = millis();
  }
  client.loop();
}
