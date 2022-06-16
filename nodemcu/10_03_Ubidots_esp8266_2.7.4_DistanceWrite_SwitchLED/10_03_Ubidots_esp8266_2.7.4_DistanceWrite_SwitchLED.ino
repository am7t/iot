#define ledPin D4
#define triggerPin D1
#define echoPin D2

/****************************************
   Include Libraries
 ****************************************/
#include "UbidotsESPMQTT.h"

/****************************************
   Define Constants
 ****************************************/
#define TOKEN "BBFF-ljWQKFtDZU4WydBFq0aco2kqJqrT" // Your Ubidots TOKEN
#define WIFINAME "MyWiFiSSID" //Your SSID
#define WIFIPASS "MyWiFiPassword" // Your Wifi Pass

Ubidots client(TOKEN);

/****************************************
   Auxiliar Functions
 ****************************************/

unsigned long t1 = millis();
unsigned long t2 = t1;
int cm = 0;

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
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  client.ubidotsSubscribe("nodemcu", "light"); //Insert the dataSource and Variable's Labels
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!client.connected()) {
    client.reconnect();
    client.ubidotsSubscribe("nodemcu", "light"); //Insert the dataSource and Variable's Labels
  }
  
  if (millis() - t1 > 200 ) {
    cm = getDistance();
    t1 = millis();
  }

  if (millis() - t2 > 10000) {

    client.add("distance", cm);
    client.ubidotsPublish("nodemcu");
    t2 = millis();
  }
  client.loop();
}



int getDistance() {
  long duration;
  int cm;

  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  cm = microsecondsToCentimeters(duration);

  Serial.println("Distance: " + String(cm) + " cm");
  return cm;
}

int microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
}
