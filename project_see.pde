import processing.serial.*;

Serial myPort;        // The serial port
int Pos = 0;       // input position from sensor
float inByte = 0;


void setup () {
  size (1200, 800);
  background(255);
  println(Serial.list());
  
  myPort = new Serial(this, Serial.list()[1], 9600);
  myPort.bufferUntil('\n');
}

void draw () {

  float a = 2; // modifier for draw size
  float drawWidth = 600*a;
  float drawHeight = 400*a;

  for (float degreeRadian = 0.0; degreeRadian <=PI; degreeRadian = degreeRadian + (PI/12)) {
    strokeWeight(1);
    line (drawWidth/2, drawHeight-100*a, drawWidth/2-((drawWidth/2-100*a)*cos(degreeRadian)), drawWidth/2-((drawWidth/2-100*a)*sin(degreeRadian)));
  }

  for (float arcRadian = 0.0; arcRadian <=PI; arcRadian = arcRadian + (PI/12)) {
    noFill();
    strokeWeight(3);
    arc(drawWidth/2, drawHeight-100*a, Pos+inByte, Pos+inByte, PI, PI+ arcRadian);
  }
}

void serialEvent (Serial myPort) {
  // get the ASCII string:
  String inString = myPort.readStringUntil('\n');

  if (inString != null) {
    // trim off any whitespace:
    inString = trim(inString);
    // convert to an int and map to the screen height:
    inByte = float(inString);
    println(inByte);
    inByte = map(inByte, 0, 1023, 0, height);
  }
}