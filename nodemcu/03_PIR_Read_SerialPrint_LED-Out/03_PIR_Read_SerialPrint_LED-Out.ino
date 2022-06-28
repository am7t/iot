#define ledPin D4
#define buzzerPin D2
#define pirPin D1
#define pirDelay 7

long tick = millis();

void setup()
{
 Serial.begin(115200);
 pinMode(ledPin, OUTPUT);
 pinMode(pirPin, INPUT);
 pinMode(buzzerPin, OUTPUT);
}

void loop()
{
 int pirValue = digitalRead(pirPin);
 if (pirValue)
 {
  digitalWrite(ledPin, LOW);
  digitalWrite(buzzerPin, HIGH);
  Serial.println("Motion Detected!!!...");

  for (int i = 0; i <= pirDelay; i++)
  {
   Serial.print(String(pirDelay - i) + " ");
   delay(1000);
  }
  Serial.println();
 }
 else
 {
  digitalWrite(ledPin, HIGH);
  digitalWrite(buzzerPin, LOW);
  if ((millis() - tick) > 1000)
  {
   Serial.println("Scanning...");
   tick = millis();
  }
 }
}
