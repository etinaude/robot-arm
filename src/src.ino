#include "step.h"
#include "pins.h"
#include "setupPins.h"
#include <AccelStepper.h>
#include <MultiStepper.h>

int stepsPerDegree[] = {43,23,100,40}; //still need to calibrate [0](base)
//int speeds[] = {1000,1500,700,500};
int speeds[] = {1000,750,0,0};

int speed = 500;

/*Arm dimensions(mm) */
#define BASE_HGT 270                                            //base height
#define HUMERUS 220                                             //shoulder-to-elbow "bone"
#define ULNA 220                                                //elbow-to-wrist "bone"
#define HAND 50                                                 //end
#define ftl(x) ((x) >= 0 ? (long)((x) + 0.5) : (long)((x)-0.5)) //float to long conversion

//Length of links in cm
#define a1 22.5
#define a2 24
#define a3 6

/*pre-calculations */
float hum_sq = HUMERUS * HUMERUS;
float uln_sq = ULNA * ULNA;

int loopCounter = 0;
int pulseWidth = 6.6;
int microsecondsToDegrees;

String sdata="";  // Initialised to nothing

AccelStepper shoulderStepper(1, SHOULDER_STEP_PIN, SHOULDER_DIR_PIN);
AccelStepper reverseShoulderStepper(1, REVERSE_SHOULDER_STEP_PIN, REVERSE_SHOULDER_DIR_PIN);
AccelStepper elbowStepper(1, ELBOW_STEP_PIN, ELBOW_DIR_PIN);
AccelStepper forearmStepper(1, FOREARM_STEP_PIN, FOREARM_DIR_PIN);
AccelStepper wristStepper(1, WRIST_STEP_PIN, WRIST_DIR_PIN);
AccelStepper baseStepper(1, BASE_STEP_PIN, BASE_DIR_PIN);
MultiStepper steppers;

void setup()
{
  setupPins();

  baseStepper.setMaxSpeed(2000);
  shoulderStepper.setMaxSpeed(1500);
  reverseShoulderStepper.setMaxSpeed(1500);
  elbowStepper.setMaxSpeed(2500);
  wristStepper.setMaxSpeed(2500);


  steppers.addStepper(baseStepper);
  steppers.addStepper(reverseShoulderStepper);
  steppers.addStepper(shoulderStepper);
  steppers.addStepper(elbowStepper);
  steppers.addStepper(wristStepper);


  

  Serial.begin(9600);
  Serial.flush();
  Serial.println("Start ====================");


  //angleSet(0,-57,-90,45);
  //delay(1000);
  //home();

  //      x  y    z
  //set_arm(0, 250, 600, 1);
  //delay(1000);
  //home();
  //set_arm(float x, float y, float z, float grip_angle_d, int servoSpeed);

  //angleSet(40,45,-90,90);
  //delay(1000);
  //home();

  /*
  set_arm(30,20,90);
  delay(500);
  set_arm(40,20,90);
  delay(500);
  set_arm(40,30,90);
  delay(500);
  set_arm(30,30,90);
  delay(500);
  home();

  /*

  angleSet(90,0,0,0);
  angleSet(90,-90,0,0);
  angleSet(90,-90,90,0);
  for(int i = 0; i < 3; i++){
    angleSet(90,-90,90,-75);
    angleSet(90,-90,90,-90);
  }
  angleSet(-45,-90,0,0);
  angleSet(0,0,0,0);
  */

/*
  long positions[4] = {-4000,0,9000,3000}; // Array of desired stepper positions
  
  steppers.moveTo(positions);
  steppers.runSpeedToPosition(); // Blocks until all are in position
  delay(1000);
  
  // Move to a different coordinate
  positions[0] = 0;
  positions[1] = 0;
  positions[2] = 0;
  positions[3] = 0;
  steppers.moveTo(positions);
  steppers.runSpeedToPosition(); // Blocks until all are in position
  delay(1000);

*/

  

  /*
  int steps = 2000;
  speed = 400;
  for (int i = 0; i < steps; i++)
  {
    //moveElbow(0);
    //moveShoulder(0);
     moveWrist(0);
    // moveBase(0);
    delayMicroseconds(speed);
  }

  for (int i = 0; i < steps; i++)
  {
    //moveElbow(1);
    //moveShoulder(1);
     moveWrist(1);
    // moveBase(1);
    delayMicroseconds(speed);
  }
*/
  
  
}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void loop()
{
  
  if (Serial.available()) { //wait for command
    byte ch = Serial.read();
    sdata += (char)ch;
    if (ch=='\r') {  // command recieved
      sdata.trim();

      Serial.print("X Z Y endAngle - ");
      Serial.println(sdata);

      if(sdata == "home"){
        home();
      }
      else{
  
        int commands[] = {getValue(sdata, ';', 0).toInt(),getValue(sdata, ';', 1).toInt(),getValue(sdata, ';', 2).toInt(),getValue(sdata, ';', 3).toInt()}; //x,z,y,endAngle (need to add forarm angle)  
        set_arm(commands[1],commands[2],commands[3]); //z,y,endAngle
      }
      
      sdata = ""; // clr string
    }
  }

  //jack
  angleSet(0, 75, 0, 0);
  angleSet(0, 55, 0, 0);

  
  //set_arm(10,40,90);
  //set_arm(18,40,90);
  //line(0,40,90,0,30,90);
  
}

/*arm positioning utilising inverse kinematics */
//void set_arm(uint16_t x, uint16_t y, uint16_t z, uint16_t grip_angle_d)
void set_arm(uint16_t x, uint16_t y, uint16_t angle)
{
  //Desired Position of End effector
  float px = x;
  float py = y;

  float phi = angle;
  phi = radians(phi);

  //Equations for Inverse kinematics
  float wx = px - a3*cos(phi);
  float wy = py - a3*sin(phi);
 
    

  float delta = sq(wx) + sq(wy);
  float c2 = ( delta -sq(a1) -sq(a2))/(2*a1*a2);
  float s2 = sqrt(1-sq(c2));  //elbow down
  float theta_2 = atan2(s2, c2);

  float s1 = ((a1+a2*c2)*wy - a2*s2*wx)/delta;
  float c1 = ((a1+a2*c2)*wx + a2*s2*wy)/delta;
  float theta_1 = atan2(s1,c1);
  float theta_3 = phi-theta_1-theta_2;

  Serial.println(degrees(theta_1));
  Serial.println(degrees(theta_2));
  Serial.println(degrees(theta_3));

  //angleSet(bas_angle_d, shl_angle_d, elb_angle_d, wri_angle_d);
  angleSet(0, degrees(theta_1), degrees(theta_2), degrees(theta_3));
}

void moveSteppers(int base, int shoulder, int elbow, int wrist){ //move steppers desired number of steps (negative for reverse)
  long positions[5] = {base*-1,shoulder,shoulder*-1,elbow,wrist*-1}; // Array of desired stepper positions
  
  steppers.moveTo(positions);
  steppers.runSpeedToPosition(); // Blocks until all are in position
}

void angleSet(int baseAngle, int shoulderAngle, int elbowAngle, int wristAngle)
{
  //calculate how many steps to move based on current position and desired angles
  int stepsToMove[] = {angleToSteps(0,baseAngle), angleToSteps(1,shoulderAngle), angleToSteps(2,elbowAngle), angleToSteps(3,wristAngle)};

  //move
  moveSteppers(stepsToMove[0],stepsToMove[1],stepsToMove[2],stepsToMove[3]);
  
}

void axisStep(int axis, int clockwise){
  if(axis == 0){
    moveBase(clockwise);
  }
  if(axis == 1){
    moveShoulder(clockwise);
  }
  if(axis == 2){
    moveElbow(clockwise);
  }
  if(axis == 3){
    moveWrist(clockwise);
  }
}

int angleToSteps(int axis, int angle)
{
  return int(angle * stepsPerDegree[axis]);
}

int stepsToAngle(int axis, int steps)
{
  return int(steps / stepsPerDegree[axis]);
}

/*move steppers to home position */
void home()
{
  moveSteppers(0,0,0,0);
}

void zero_x()
{
  for (double yaxis = 250.0; yaxis < 400.0; yaxis += 1)
  {
    Serial.print(" yaxis= : ");
    Serial.println(yaxis);
    //set_arm(0, yaxis, 200.0, 0, 10);
    delay(10);
  }
  for (double yaxis = 400.0; yaxis > 250.0; yaxis -= 1)
  {
    //set_arm(0, yaxis, 200.0, 0, 10);
    delay(10);
  }
}

/*moves arm in a straight line */
void line(int z1, int y1, int ang1, int z2, int y2, int ang2)
{

  int segments = 5;
  float currentSegment = 0;
  for(int i = 0; i < segments+1; i++){
    int y = y1 + (y2-y1) * currentSegment;
    int z = z1 + (z2-z1) * currentSegment;
    int ang = ang1 + (ang2-ang1) * currentSegment;
    set_arm(y,z,ang);
    Serial.println(z);
    currentSegment = currentSegment + 0.2;
    
  }

  
}

void circle()
{
#define RADIUS 50.0
  //float angle = 0;
  float zaxis, yaxis;
  for (float angle = 0.0; angle < 360.0; angle += 1.0)
  {
    yaxis = RADIUS * sin(radians(angle)) + 300;
    zaxis = RADIUS * cos(radians(angle)) + 200;
    //set_arm(0, yaxis, zaxis, 0, 50);
    delay(10);
  }
}
