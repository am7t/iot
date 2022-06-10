#define ldrPin A0
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

int testValue = 0;
int nt = 0;
unsigned long tick = millis();

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  if ((char)payload[0] == '1') {
    digitalWrite(ledPin, LOW);
  }
  else {
    digitalWrite(ledPin, HIGH);
  }
  Serial.println();
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
  pinMode (ldrPin, INPUT);
  client.ubidotsSubscribe("nodemcu", "light"); //Insert the dataSource and Variable's Labels
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!client.connected()) {
    client.reconnect();
    client.ubidotsSubscribe("nodemcu", "light"); //Insert the dataSource and Variable's Labels
  }

  if (millis() - tick > 10000) {
    client.add("ldr-value", readLDRValue());
    client.ubidotsPublish("nodemcu");
    tick = millis();
  }
  client.loop();
}

int readLDRValue() {
  int ldrValue = analogRead(ldrPin);
  Serial.println("LDR Value :" + String(ldrValue));
  return ldrValue;
}
