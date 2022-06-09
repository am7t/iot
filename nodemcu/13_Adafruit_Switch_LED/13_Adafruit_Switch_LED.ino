#include <ESP8266WiFi.h>  //library for using ESP8266 WiFi
#include <PubSubClient.h> //library for MQTT
//#include <ArduinoJson.h> //library for Parsing JSON

// defining Pins
#define LED LED_BUILTIN

#define WIFISSID "Nokia 3.1"
#define WIFIPASSWORD "234560856"

// MQTT Credentials
const char *ssid = WIFISSID;                           // setting your ap ssid
const char *password = WIFIPASSWORD;                      // setting your ap psk
const char *mqttServer = "io.adafruit.com";               // MQTT URL
const char *mqttUserName = "mediumtechnica";                   // MQTT username
const char *mqttPwd = "aio_koSs603xYHVny0mIUFaZdh1c3QZr"; // MQTT password
const char *clientID = "mediumtechnica0001";                    // client id username+0001
const char *topic = "mediumtechnica/feeds/light";              // publish topic

// parameters for using non-blocking delay
unsigned long previousMillis = 0;
const long interval = 5000;

String msgStr = ""; // MQTT message buffer

// setting up wifi and mqtt client
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi()
{
  delay(10);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect()
{
  while (!client.connected())
  {
    if (client.connect(clientID, mqttUserName, mqttPwd))
    {
      Serial.println("MQTT connected");

      if (client.subscribe(topic))
      {
        Serial.println("Topic Subscribed., Topic:" + String(topic));
      }
      else
      {
        Serial.println("Subscription Failed!, Topic:" + String(topic));
      }
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000); // wait 5sec and retry
    }
  }
}

// subscribe call back
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  String data = "";
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
    data += (char)payload[i];
  }
  Serial.println();
  Serial.print("Message size :");
  Serial.println(length);
  Serial.println();
  Serial.println("-----------------------");
  Serial.println(data);

  if (data == "0")
  {
    Serial.println("LED");
    digitalWrite(LED, HIGH);
  }
  else
  {
    digitalWrite(LED, LOW);
  }
}

void setup()
{
  Serial.begin(115200);

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  setup_wifi();

  client.setServer(mqttServer, 1883); // setting MQTT server
  client.setCallback(callback);       // defining function which will be called when message is received.
}

void loop()
{
  if (!client.connected())
  {              // if client is not connected
    reconnect(); // try to reconnect
  }
  client.loop();
}
