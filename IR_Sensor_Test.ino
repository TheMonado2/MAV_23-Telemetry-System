#define sensorPin 3

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(sensorPin) == HIGH)
  {
    Serial.println("Sensor Pin High");
  }
  else if (digitalRead(sensorPin) == LOW)
  {
    Serial.println("Sensor Pin Low");
  }
  else
  {
    Serial.println("No data collected");
  }

  delay(1000);
}
