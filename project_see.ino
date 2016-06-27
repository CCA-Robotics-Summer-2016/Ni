/*
   This is a 2 week long project aims to provide quick and cheap way for blind people to navigate through space.
*/

//Global---------------------------------------------------------------------

#include <Servo.h>

//setup field of measure at 18 degree interval
long measurements[180];
long averages[10];

//servo
Servo myServo;
const int servoPin = 13; //MUST CHANGE THIS TO MATCH BOARD

//set the distance sensor limit to the height of the person
int personHeight = 300; //MUST CHANGE THIS TO MATCH HEIGHT

// ultrasonic distance measuring sensor
const int trigPin = 22;
const int echoPin = 12;

//buzzer
//Be careful with wiring vibrating motors, the first one starting on right hand side goes into pin#2
int buzzerPin[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
int buzzerCount = 10;
int buzzerMod = 60;


//Functions------------------------------------------------------------------

// sweep 180 degrees and populate the measurements array
void sweepAndMeasure() {
  for (int degree = 0; degree < 180; degree++) {
    myServo.write(degree);
    delay(5);
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
  delayMicroseconds(1); // low for 1 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5); // high for 5 microseconds
  digitalWrite(trigPin, LOW);
  myDuration = pulseIn(echoPin, HIGH); // measure the time to the echo
  distance = (myDuration / 2) / 29.1; // calculate the distance in cm

  // if reading is out of range, make it -1

  if (distance <= 0 || distance >= personHeight ) {
    distance = -1;
  }
  return distance;
}

//get average of distance, convert it into a factor to be used for buzzer
float averageThese ( int startHere ) {
  int sum = 0;
  int validMeasurements = 18;
  int average;
  float buzzerIntensity;
  float buzzerFactor;

  for (int degree = startHere; degree < startHere + 18; degree ++) {
    //Serial.print(measurements[degree]);
    //Serial.print("\t");
    if (-1 == measurements[degree] ) {
      // skip this measuremnt
      validMeasurements--;
    } else {
      sum = sum + measurements[degree];
    }
  }
  average = sum / validMeasurements;
  buzzerFactor = (int)(buzzerMod * (1 / log10(average))); //value expected within 43 to 333
  buzzerIntensity = 2 * (map(buzzerFactor, 43 , 333, 50, 255)); //tested analog output power best within 50 to 250 for buzzer
  /*
      Serial.print("average:  ");
      Serial.print(average);
      Serial.print("\t");
      Serial.print("valid measurements:  ");
      Serial.print(validMeasurements);
      Serial.print("\t");
      Serial.print("buzzerFactor:  ");
      Serial.print(buzzerFactor);
      Serial.print("\t");
      Serial.print("buzzerIntensity:   ");
      Serial.print(buzzerIntensity);
      Serial.println("");
  */
  Serial.print(buzzerIntensity);
  Serial.print(",");
  Serial.print("0");

  return buzzerIntensity;
}


/*
  void averageInSixteenDegreeChunks() {
  for (int degree = 0; degree < 176; degree = degree + 16) {
    averages[degree / 16] = averageThese(degree);
  }
  }
*/

//turn on buzzer. buzzer power range needs to be 50-250
void buzzerOn() {
  for (int degree = 0; degree < 180; degree = degree + 18) {
    analogWrite ( buzzerPin[degree / 18], averageThese(degree));
  }
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
  Serial.println("");
  myServo.write(0);//return servo to 0
  buzzerOn();
}
