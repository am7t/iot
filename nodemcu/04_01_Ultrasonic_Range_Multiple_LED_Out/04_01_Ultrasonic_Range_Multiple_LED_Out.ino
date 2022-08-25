#define triggerPin D0
#define echoPin D1

#define ledAPin D2
#define ledBPin D3
#define ledCPin D4

void setup()
{
 Serial.begin(115200);
 pinMode(ledAPin, OUTPUT);
 pinMode(ledBPin, OUTPUT);
 pinMode(ledCPin, OUTPUT);

 pinMode(triggerPin, OUTPUT);
 pinMode(echoPin, INPUT);
}

void loop()
{
 digitalWrite(triggerPin, LOW);
 delayMicroseconds(2);
 digitalWrite(triggerPin, HIGH);
 delayMicroseconds(10);
 digitalWrite(triggerPin, LOW);
 delayMicroseconds(2);

 long durationMicroseconds = pulseIn(echoPin, HIGH);

 int distanceCentimeters = microsecondsToCentimeters(durationMicroseconds);
 
 if (distanceCentimeters < 10)
 {
  ledLevel(0, 0, 0);
 }

 else if (distanceCentimeters >= 10 && distanceCentimeters < 20)
 {
  ledLevel(1, 0, 0);
 }

 else if (distanceCentimeters >= 20 && distanceCentimeters < 30)
 {
  ledLevel(1, 1, 0);
 }

 else
 {
  ledLevel(1, 1, 1);
 }

 Serial.println("Distance = " + String(distanceCentimeters) + " cm");
 delay(200);
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

void ledLevel(bool stateA, bool stateB, bool stateC)
{
 digitalWrite(ledAPin, stateA);
 digitalWrite(ledBPin, stateB);
 digitalWrite(ledCPin, stateC);
}
