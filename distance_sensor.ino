
#include <Wire.h>

const int LED_PIN = 13;
const int MAX_VALUE = 8191;

byte registers [8];
int reg = 0;
 
void setup() {
  pinMode(3, OUTPUT);
  pinMode(4, INPUT);
  pinMode(5, OUTPUT);
  pinMode(6, INPUT);
  pinMode(7, OUTPUT);
  pinMode(8, INPUT);
  pinMode(9, OUTPUT);
  pinMode(10, INPUT);
  Wire.begin(0x31);                // join i2c bus with address #31
  Wire.onRequest(requestEvent); // register event
  Wire.onReceive(receiveEvent);
}
 
long getDistance(byte echoPin, byte trigPin) {
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  long duration = pulseIn(echoPin, HIGH, MAX_VALUE);
  if (duration == 0) {
    duration = MAX_VALUE;
  }
  return duration;
}

void loop() {
   long duration1 = getDistance(4, 3);
   registers[0] = duration1 & 255;
   registers[1] = (duration1 >> 8) & 255;

   long duration2 = getDistance(6, 5);
   registers[2] = duration2 & 255;
   registers[3] = (duration2 >> 8) & 255;

   long duration3 = getDistance(8, 7);
   registers[4] = duration3 & 255;
   registers[5] = (duration3 >> 8) & 255;

   long duration4 = getDistance(10, 9);
   registers[6] = duration4 & 255;
   registers[7] = (duration4 >> 8) & 255;

   // debugging LED, if lit when any sensor register something close-by
   long distance = min(min(duration1, duration2), min(duration3, duration4));
   digitalWrite(LED_PIN, distance < 1000 ? HIGH : LOW);
}

void receiveEvent(int bytes) {
  reg = Wire.read();
  // Purge rest of the data
  while (Wire.available() > 0) Wire.read();
}

void requestEvent() {
  Wire.write(registers + reg, 8); // respond with one register
}
