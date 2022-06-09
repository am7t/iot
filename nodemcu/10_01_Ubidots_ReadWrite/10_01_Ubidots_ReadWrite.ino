#define ledPin D4

/****************************************
   Include Libraries
 ****************************************/
#include "UbidotsESPMQTT.h"

/****************************************
   Define Constants
 ****************************************/
#define TOKEN "BBFF-ljWQKFtDZUp04Wy" // Your Ubidots TOKEN
#define WIFINAME "MyWiFiHotSpot" //Your SSID
#define WIFIPASS "MyWiFiHotSpotPassword" // Your Wifi Pass

Ubidots client(TOKEN);

/****************************************
   Auxiliar Functions
 ****************************************/

int testValue = 0;
unsigned long tick = millis();

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  /*
    for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
    }
  */
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
  client.ubidotsSubscribe("testdevice", "light"); //Insert the dataSource and Variable's Labels
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!client.connected()) {
    client.reconnect();
    client.ubidotsSubscribe("testdevice", "light"); //Insert the dataSource and Variable's Labels
  }
  if (millis() - tick > 2000) {
    testValue++;
    if (testValue > 10) {
      testValue = 0;
    }
    client.add("sensor_value", testValue);
    client.ubidotsPublish("testdevice");
    client.loop();
    tick = millis();
  }
}
