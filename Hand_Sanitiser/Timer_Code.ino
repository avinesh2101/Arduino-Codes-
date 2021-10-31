/*
    Code for hand wash timer. Developed by The Tech Lab
    Copyright (C) 2020  The Tech Lab

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    Facebook page - www.facebook.com/thetechlab
    Website - www.thetechlab.website
    Feedback/suggestions - shayonkhaled@thetheclab.website
    Feel free to modify the code according to your need.
*/

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
