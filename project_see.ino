/*
   This is a 2 week long project aims to provide quick and cheap way for blind people to navigate through space.
*/

//Global---------------------------------------------------------------------

#include <Servo.h>

//setup field of measure at 16 degree interval
long measurements[176];
long averages[11];

//servo
Servo myServo;
const int servoPin = 13; //MUST CHANGE THIS TO MATCH BOARD

//set the distance sensor limit to the height of the person
int personHeight = 180; //MUST CHANGE THIS TO MATCH HEIGHT

// ultrasonic distance measuring sensor
const int trigPin = 12;
const int echoPin = 11;

//buzzer
int buzzerPin[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
int buzzerCount = 11;
float buzzerIntensity; //used for power buzzer in analogwrite
int buzzerFactor = 200; //adjustable multiplier


//Functions------------------------------------------------------------------

// sweep 176 degrees and populate the measurements array
void sweepAndMeasure() {
  for (int degree = 0; degree < 176; degree++) {
    myServo.write(degree);
    delay(10);
    measurements[degree] = measureDistance();
  }
}

// Take a measurement using the ultrasonic discance
// measuring sensor and return the distance in cm
// returns -1 if an error occurrs

long measureDistance() {
  long myDuration, distance;

  // measure how far anything is from us
  // send the pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2); // low for 2 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // high for 10 microseconds
  digitalWrite(trigPin, LOW);
  myDuration = pulseIn(echoPin, HIGH); // measure the time to the echo
  distance = (myDuration / 2) / 29.1; // calculate the distance in cm

  // if reading is out of range, make it -1

  if (distance <= 0 || distance >= personHeight ) {
    distance = -1;
  }
  return distance;
}

void averageInSixteenDegreeChunks() {

  for (int degree = 0; degree < 176; degree = degree + 16) {
    averages[degree / 16] = averageTheseEleven(degree);
  }
}

int averageTheseEleven ( int startHere ) {
  int sum = 0;
  int validMeasurements = 16;
  int average;

  for (int degree = startHere; degree < startHere + 16; degree ++) {
    Serial.print(measurements[degree]);
    Serial.print("\t");
    if (-1 == measurements[degree] ) {
      // skip this measuremnt
      validMeasurements--;
    } else {
      sum = sum + measurements[degree];
    }
  }

  average = sum / validMeasurements;
  buzzerIntensity = buzzerFactor * (1 / log10(average));

  Serial.print("average:  ");
  Serial.print(average);
  Serial.print("\t");
  Serial.print("valid measurements:  ");
  Serial.print(validMeasurements);
  Serial.print("\t");
  Serial.print("\t");
  Serial.print("buzzerIntensity:   ");
  Serial.print(buzzerIntensity);
  Serial.println("");

  return buzzerIntensity;
}

// End of Functions----------------------------------------------------



// setup and loop------------------------------------------------------
void setup() {

  Serial.begin (9600);

  // pins for the ultrasonic distance measuring sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //attach servo to pin
  myServo.attach(servoPin);

  //pins for buzzer
  for (int thisPin = 0; thisPin <= buzzerCount; thisPin++) {
    pinMode(buzzerPin[thisPin], OUTPUT);
  }
}

void loop() {
  
  sweepAndMeasure();//get distance
  averageInSixteenDegreeChunks();//get average
  myServo.write(0);//return servo to 0
  analogWrite(buzzerPin[11], buzzerIntensity);
  
}





