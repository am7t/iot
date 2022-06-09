#define pingPin D1
#define ledPin D4

void setup()
{
 Serial.begin(115200);
 pinMode(ledPin, OUTPUT);
}

void loop()
{
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

 Serial.println("Distance = " + String(cm) + " cm");

 if (cm < 15)
 {
  digitalWrite(ledPin, LOW);
 }
 else
 {
  digitalWrite(ledPin, HIGH);
 }

 delay(500);
}

int microsecondsToCentimeters(long microseconds)
{
 // The speed of sound is 340 m/s or 29 microseconds per centimeter.
 // The ping from the sensor travels out and back, so to find the distance of the object we
 // take half of the distance travelled.
 return microseconds / 29 / 2;
}
