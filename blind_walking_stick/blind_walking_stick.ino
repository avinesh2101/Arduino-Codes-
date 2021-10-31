//For Hacktoberfest2021 by Sukanyadas

const int trigPin = 3;
const int echoPin = 2;
const int buzzer = 5;
const int motorPin = 6;


long duration;
int distance;
int safeDistance;


void setup() {
pinMode(trigPin, OUTPUT); 
pinMode(echoPin, INPUT); 
pinMode(buzzer, OUTPUT);
pinMode(motorPin, OUTPUT);
Serial.begin(9600); 
}


void loop() {

digitalWrite(trigPin, LOW);
delayMicroseconds(2);


digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);


duration = pulseIn(echoPin, HIGH);


distance= duration*0.034/2;

safeDistance = distance;
if (safeDistance <= 30){
  digitalWrite(buzzer, HIGH);
  digitalWrite(motorPin, HIGH);
}
else{
  digitalWrite(buzzer, LOW);
  digitalWrite(motorPin, LOW);
}


Serial.print("Distance: ");
Serial.println(distance);
}
