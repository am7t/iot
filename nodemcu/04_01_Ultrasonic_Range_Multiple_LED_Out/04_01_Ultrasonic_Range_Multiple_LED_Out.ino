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
 if (cm < 10)
 {
  ledLevel(0, 0, 0);
 }

 else if (cm >= 10 && cm < 20)
 {
  ledLevel(1, 0, 0);
 }

 else if (cm >= 20 && cm < 30)
 {
  ledLevel(1, 1, 0);
 }

 else
 {
  ledLevel(1, 1, 1);
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

void ledLevel(bool stateA, bool stateB, bool stateC)
{
 digitalWrite(ledAPin, stateA);
 digitalWrite(ledBPin, stateB);
 digitalWrite(ledCPin, stateC);
}
