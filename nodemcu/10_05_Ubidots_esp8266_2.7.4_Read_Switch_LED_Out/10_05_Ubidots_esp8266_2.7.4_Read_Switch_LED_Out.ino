/****************************************
   Include Libraries
 ****************************************/
#include "UbidotsESPMQTT.h"

/****************************************
   Define Constants
 ****************************************/
#define TOKEN "BBFF-ljWQKFtp04WydBFq0aco2kqJqrT" // Your Ubidots TOKEN
#define WIFINAME "Nokia 3.1" //Your SSID
#define WIFIPASS "1267890" // Your Wifi Pass

Ubidots client(TOKEN);
#define ledPin D4

/****************************************
   Auxiliar Functions
 ****************************************/

void callback(char* topic, byte* payload, unsigned int length) {
  String data = "";
  for (int i = 0; i < length; i++) {
    data += (char)payload[i];
  }
  Serial.println("Message arrived!,\n Topic: " + String(topic) + ",\n Data : " + data);
  if (data == "1.0") {
    Serial.println("Switch ON");
    digitalWrite(ledPin, LOW);
  } else if (data == "0.0") {
    Serial.println("Switch OFF");
    digitalWrite(ledPin, HIGH);
  }
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
  client.ubidotsSubscribe("nodemcu", "light");
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!client.connected()) {
    client.reconnect();
    client.ubidotsSubscribe("nodemcu", "light");
  }
  client.loop();
}
