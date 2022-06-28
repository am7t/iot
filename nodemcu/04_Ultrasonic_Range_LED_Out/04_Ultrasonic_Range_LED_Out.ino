#define triggerPin D1
#define echoPin D2

#define ledPin D4

void setup()
{
 Serial.begin(115200);
 pinMode(ledPin, OUTPUT);

 pinMode(triggerPin, OUTPUT);
 pinMode(echoPin, INPUT);
}

void loop()
{
 long duration;
 int cm;

 digitalWrite(triggerPin, LOW);
 delayMicroseconds(2);
 digitalWrite(triggerPin, HIGH);
 delayMicroseconds(10);
 digitalWrite(triggerPin, LOW);

 delayMicroseconds(2);
 duration = pulseIn(echoPin, HIGH);

 cm = microsecondsToCentimeters(duration);
 if (cm > 20)
 {
  digitalWrite(ledPin, LOW);
 }
 else
 {
  digitalWrite(ledPin, HIGH);
 }

 Serial.println("Distance = " + String(cm) + " cm");
 delay(200);
}

int microsecondsToCentimeters(long microseconds)
{
 // The speed of sound is 340 m/s or 29 microseconds per centimeter.
 // The ping from the sensor travels out and back, so to find the distance of the object we
 // take half of the distance travelled.
 return microseconds / 29 / 2;
}
