#define triggerPin D1
#define echoPin D2
#define ledPin D4

void setup()
{
 Serial.begin(115200);
 pinMode(triggerPin, OUTPUT);
 pinMode(echoPin, INPUT);
 pinMode(ledPin, OUTPUT);
}

void loop()
{
 digitalWrite(triggerPin, LOW);
 delayMicroseconds(2);
 digitalWrite(triggerPin, HIGH);
 delayMicroseconds(10);
 digitalWrite(triggerPin, LOW);

 long durationMicroseconds = pulseIn(echoPin, HIGH);

 int distanceCentimeters = microsecondsToCentimeters(durationMicroseconds);

 Serial.println("Distance = " + String(distanceCentimeters) + " cm");

 if (distanceCentimeters < 15)
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
 // 340 meter per seconds = 0.00034 meter per microseconds = 0.034 centimeter per microseconds

 // instead of multiplying with a float value which takes relatively more computation time & memory,
 // the multiplication is replaced with division by the reciprocal of 0.034 which is approximately
 // equal to the integer 29

 // 0.034 = 1 / 29.4117647059 ≈ 1 / 29

 // The ping from the sensor travels out and back, so to find the distance of the object
 // half of the distance travelled is taken.

 return microseconds / 29 / 2;
}
