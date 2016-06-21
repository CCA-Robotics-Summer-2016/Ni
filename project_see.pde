import processing.serial.*;

Serial myPort;        // The serial port
float Pos=0;       // initial position
float [] PosMod; //input array positions modifier from sensor


void setup () {
  size (1200, 800);

  println(Serial.list());

  myPort = new Serial(this, Serial.list()[1], 9600);
  myPort.bufferUntil('\n');
}

void draw () {

  float a = 2; // modifier for draw size
  float drawWidth = 600*a;
  float drawHeight = 400*a;
  background(255);
  for (float degreeRadian = 0.0; degreeRadian <=PI; degreeRadian = degreeRadian + (PI/12)) {
    strokeWeight(1);
    line (drawWidth/2, drawHeight-100*a, drawWidth/2-((drawWidth/2-100*a)*cos(degreeRadian)), drawWidth/2-((drawWidth/2-100*a)*sin(degreeRadian)));
  }

  for (float arcRadian = 0.0; arcRadian <=PI; arcRadian = arcRadian + (PI/12)) {
    for (int i = 0; i<=11; i++) {
      noFill();
      strokeWeight(3);
      arc(drawWidth/2, drawHeight-100*a, Pos+PosMod[i], Pos+PosMod[i], PI, PI+ arcRadian);
    }
  }
}

void serialEvent (Serial myPort) {
  // get the ASCII string:
  String inString = myPort.readStringUntil('\n');

  if (inString != null) {
    inString = trim(inString);
    for (int i=0; i<=11; i++){
    PosMod[i] = float(inString);
    println(PosMod);
    }
  }
}