#define pingPin D1

#include "ThingSpeak.h"
#include <ESP8266WiFi.h>

#define SECRET_SSID "MySSID"     // replace MySSID with your WiFi network name
#define SECRET_PASS "MyPassword" // replace MyPassword with your WiFi password

#define SECRET_CH_ID 000000       // replace 0000000 with your channel number
#define SECRET_WRITE_APIKEY "XYZ" // replace XYZ with your channel write API Key

char ssid[] = SECRET_SSID; // your network SSID (name)
char pass[] = SECRET_PASS; // your network password
int keyIndex = 0;          // your network key Index number (needed only for WEP)
WiFiClient client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char *myWriteAPIKey = SECRET_WRITE_APIKEY;
unsigned int myFieldNumber = 1;

void setup()
{
 Serial.begin(115200); // Initialize serial

 WiFi.mode(WIFI_STA);
 ThingSpeak.begin(client); // Initialize ThingSpeak
}

void loop()
{

 // Connect or reconnect to WiFi
 if (WiFi.status() != WL_CONNECTED)
 {
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(SECRET_SSID);
  while (WiFi.status() != WL_CONNECTED)
  {
   WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
   Serial.print(".");
   delay(5000);
  }
  Serial.println("\nConnected.");
 }

 int cm = getDistance();

 // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
 // pieces of information in a channel.  Here, we write to field 1.
 int httpConnectionStatusCode = ThingSpeak.writeField(myChannelNumber, myFieldNumber, cm, myWriteAPIKey);
 if (httpConnectionStatusCode == 200)
 {
  Serial.println("Channel update successful.");
 }
 else
 {
  Serial.println("Problem updating channel. HTTP error code " + String(httpConnectionStatusCode));
 }

 delay(20000); // Wait 20 seconds to update the channel again
}

int getDistance() {
 long duration;
 int cm;

 pinMode(pingPin, OUTPUT);

 digitalWrite(pingPin, LOW);
 delayMicroseconds(2);
 digitalWrite(pingPin, HIGH);
 delayMicroseconds(10);
 digitalWrite(pingPin, LOW);

 pinMode(pingPin, INPUT);
 duration = pulseIn(pingPin, HIGH);

 cm = microsecondsToCentimeters(duration);
 
 Serial.println("Distance: " + String(cm) + " cm");
 return cm;
}

int microsecondsToCentimeters(long microseconds)
{
 return microseconds / 29 / 2;
}
