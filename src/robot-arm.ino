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
  WRIST,
  BASE
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

  // int speed = analogRead(POT_PIN);
  // speed = 2;
  // Serial.println(speed);
  // int wait = map(speed, 1, 1024, 30, 200);


  //axisStep(axis, steps, clockwise, wait)

  jack(100);
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
//  Serial.println(axis);
//  Serial.println(steps);
//  Serial.println(clockwise);
//  Serial.println(wait);
  if (axis == "shoulder")
    moveShoulder(steps, clockwise, wait);
  if (axis == "elbow")
    moveElbow(steps, clockwise, wait);
  if (axis == "forearm")
    moveForearm(steps, clockwise, wait);
  if (axis == "wrist")
    moveWrist(steps, clockwise, wait);
  if (axis == "base")
    moveBase(steps, clockwise, wait);
}


void jack(int speed){
  int steps = 1000;
  speed = 100;

  for (int i = 0; i < steps; i++)
  {
    axisStep("elbow", 1, 1, 1);
    delayMicroseconds(speed);

    axisStep("wrist", 1, 1, 1);
    axisStep("wrist", 1, 1, 1);

    delayMicroseconds(30);

  }

  for (int i = 0; i < steps; i++)
  {
    axisStep("elbow", 1, 0, 1);
    delayMicroseconds(speed);
    axisStep("wrist", 1, 0, 1);
    axisStep("wrist", 1, 0, 1);

    delayMicroseconds(30);

  }


}