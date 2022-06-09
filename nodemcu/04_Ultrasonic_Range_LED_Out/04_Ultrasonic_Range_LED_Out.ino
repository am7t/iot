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
 delayMicroseconds(2);
 duration = pulseIn(pingPin, HIGH);

 cm = microsecondsToCentimeters(duration);
 if (cm > 20) {
  digitalWrite(ledPin, LOW);
 } else {
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
