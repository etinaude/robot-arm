#include <SoftwareSerial.h>
#include "step.h"
#include "pins.h"
#include "setupPins.h"
SoftwareSerial portOne(10, 11);
SoftwareSerial portTwo(8, 9);
enum AXIS
{
  SHOULDER,
  ELBOW,
  FOREARM,
  WRIST
};

int speed = 100;
int del = 100;
int stepsToTake = 5000;
bool clockwise = false;

void setup()
{
  Serial.begin(9600);
  portOne.begin(9600);
  setupPins();

}

void loop()
{
  // axisStep("shoulder", 1000, 1, 100);
  // axisStep("shoulder", 1000, 0, 100);
  
  axisStep("elbow", 1000, 1, 100);
  axisStep("elbow", 1000, 0, 100);

  axisStep("wrist", 1000, 1, 100);
  axisStep("wrist", 1000, 0, 100);
}

void listen(){
  portOne.listen();
  String input = Serial.readString();
  if (portOne.isListening())
  {  
    if(input == "") return;
    int index;
    int i =0 ;
    String axis;
    while (index = input.indexOf(";") > -1)
    {
      String current = input.substring(0, input.indexOf(";"));
      input = input.substring(input.indexOf(";")+1);
      if(i == 0) axis = current;
      if(i == 1) stepsToTake = current.toInt();
      if(i == 2) clockwise = current.toInt();
      if(i == 3) speed = current.toInt();
      if(i > 3)break;
    }
      axisStep(axis, stepsToTake, clockwise, speed);
  }
  else
  {
    Serial.println("Port One is not listening!");
  }
}

void axisStep(String axis, int steps, boolean clockwise, int wait)
{
  Serial.println(axis);
  Serial.println(steps);
  Serial.println(clockwise);
  Serial.println(wait);
  if (axis == "shoulder")
    moveShoulder(steps, clockwise, wait);
  if (axis == "elbow")
    moveElbow(steps, clockwise, wait);
  if (axis == "forearm")
    moveForearm(steps, clockwise, wait);
  if (axis == "wrist")
    moveWrist(steps, clockwise, wait);
}

