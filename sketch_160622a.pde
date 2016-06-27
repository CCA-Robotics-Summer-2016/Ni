import processing.serial.*;
Serial myPort;        // The serial port
float Pos=100;       // initial position
float PosMod[];
boolean PosModIsValid=false;

void serialEvent (Serial myPort) {
  // get the ASCII string:
  String inString = myPort.readStringUntil('\n');
  if (inString != null) {
    inString = trim(inString);
    PosMod= float(split(inString, ','));
    if (PosMod.length != 11 ) {
      return;
    }
    for (int i = 0; i < PosMod.length; i++) {
      print(PosMod[i] + "\t");
    }
    println();
  }
  PosModIsValid=true;
}

void setup () {
  size (1200, 800);
  myPort = new Serial(this, Serial.list()[0], 9600);
  myPort.bufferUntil('\n');
}

void draw () {
  int a = 2; // modifier for draw size
  int drawWidth = 600*a;
  int drawHeight = 400*a;
  background(255);
  if (PosModIsValid==false) {
      return;
    }
  for (float degreeRadian = 0.0; degreeRadian <=PI+PI/10; degreeRadian = degreeRadian + (PI/10)) {
    strokeWeight(1);
    line (drawWidth/2, drawHeight-100*a, drawWidth/2-((drawWidth/2-100*a)*cos(degreeRadian)), drawWidth/2-((drawWidth/2-100*a)*sin(degreeRadian)));
  }

  for (float arcRadian = 0.0; arcRadian <=PI; arcRadian = arcRadian + (PI/10)) {
    noFill();
    strokeWeight(3);
    //arc(drawWidth/2, drawHeight-100*a, 300, 300, PI+arcRadian, PI+arcRadian+(PI/10));
    //print((int)(arcRadian/(PI/10)));
    //print(PosMod[1]);
    //print (PosMod[(int)(arcRadian/(PI/10))]);
    //println ("");
    arc(drawWidth/2, drawHeight-100*a, Pos+PosMod[(int)(arcRadian/(PI/10))], Pos+PosMod[(int)(arcRadian/(PI/10))], PI+arcRadian, PI+arcRadian+(PI/10));
  }
}