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
 long duration;
 int cm;

 digitalWrite(triggerPin, LOW);
 delayMicroseconds(2);
 digitalWrite(triggerPin, HIGH);
 delayMicroseconds(10);
 digitalWrite(triggerPin, LOW);

 durationMicroSeconds = pulseIn(echoPin, HIGH);

 cm = microSecondsToCentimeters(durationMicroSeconds);

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

int microSecondsToCentimeters(long microSeconds)
{
 // The speed of sound is 340 m/s or 29 microseconds per centimeter.
 // 340 meter per seconds -> 0.00034 meter per microseconds -> 0.034 centimeter per microseconds

 // so instead of multiplying with a float value which takes relatively more computation time & memory,
 // the multiplying factor is converted to an integer by taking the reciprocal

 // 0.034 = 1 / 29.4117647059 ≈ 1 / 29

 // The ping from the sensor travels out and back, so to find the distance of the object
 // half of the distance travelled is taken.

 return microSeconds / 29 / 2;
}
