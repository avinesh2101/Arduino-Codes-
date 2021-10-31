//Subarna Das for hacktoberfest2021
int echoPin = 11;
int trigPin = 12;
long duration;
int distance;

void setup() {
  for (int i = 2; i <= 7; i++) {
    pinMode (i, OUTPUT);
  }
  pinMode (trigPin, OUTPUT);
  pinMode (echoPin, INPUT);
}
void loop() {
  // This part of the code tells the sensor to measure distance and reads the measured data
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration * .0343) / 2;

  // This "If Statement" detects if there is any object within 30 centimeters
  if (distance > 0 && distance <= 30) {
    ledtimer();
  }
}

void ledtimer() {
  digitalWrite (7, HIGH);
  delay(4000);
  for (int i = 6; i >= 2; i--) {
    digitalWrite (i, HIGH);
    if (i == 2) {
      digitalWrite (7, LOW);
    }
    delay(4000);
  }
  for (int i = 7; i >= 2; i--) {
    digitalWrite (i, LOW);

  }
}
