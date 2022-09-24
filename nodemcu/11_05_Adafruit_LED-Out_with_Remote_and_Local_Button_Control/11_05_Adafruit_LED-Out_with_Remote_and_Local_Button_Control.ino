#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883  // use 8883 for SSL
#define AIO_USERNAME "mediumtechnica"
#define AIO_KEY "aio_XYfI12m3mpcDulaNj6h6cHjX556O"

#define WIFI_SSID "ZTE-QhjYur"
#define WIFI_KEY "october15"

#define ledPin D4

bool lightState;
bool lightStateChanged;
void publishLightState(void);

#include "SwitchBounceTimerSettings.h"

String msg = "";
bool remoteSwitchState;

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiClientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Setup a feed called 'light' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish lightPub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/light");

// Setup a feed called 'light' for subscribing to changes.
Adafruit_MQTT_Subscribe lightSub = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/light");

/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

void setup() {
  pinMode(ledPin, OUTPUT);
  initSwitchBounceSettings();

  Serial.begin(115200);
  while (!Serial)
    ;

  delay(200);

  // Connect to WiFi access point.
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_KEY);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&lightSub);
}

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &lightSub) {
      msg = String((char *)lightSub.lastread);
      Serial.println("Message Arrived: " + msg);
      if (msg == "1") {
        remoteSwitchState = true;
        Serial.println("Remote Switch ON");
      } else {
        remoteSwitchState = false;
        Serial.println("Remote Switch OFF");
      }
      digitalWrite(ledPin, !remoteSwitchState);
    }
  }
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) {  // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(2000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1)
        ;
    }
  }
  Serial.println("MQTT Connected!");
}

void publishLightState() {
  lightState = !digitalRead(ledPin);
  if (! lightPub.publish(lightState)) {
    Serial.println("Publish " + String(lightState) + " Failed");
  } else {
    Serial.println("Publish " + String(lightState) + " Done!");
  }
}