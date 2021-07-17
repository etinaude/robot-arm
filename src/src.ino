#include "step.h"
#include "pins.h"
#include "setupPins.h"

//                    bse, shl, elb, wri
int currentSteps[] = {0, 0, 0, 0};

int speed = 100;
int del = 100;
int stepsToTake = 5000;
bool clockwise = false;

/*Arm dimensions(mm) */
#define BASE_HGT 270                                            //base height
#define HUMERUS 220                                             //shoulder-to-elbow "bone"
#define ULNA 220                                                //elbow-to-wrist "bone"
#define HAND 50                                                 //end
#define ftl(x) ((x) >= 0 ? (long)((x) + 0.5) : (long)((x)-0.5)) //float to long conversion

/*pre-calculations */
float hum_sq = HUMERUS * HUMERUS;
float uln_sq = ULNA * ULNA;

int loopCounter = 0;
int pulseWidth = 6.6;
int microsecondsToDegrees;

void setup()
{
  setupPins();

  Serial.begin(9600);
  Serial.flush();
  Serial.println("Start ====================");

  //      x  y    z
  //set_arm(1, 250, 550, 1, 100);
  //set_arm(float x, float y, float z, float grip_angle_d, int servoSpeed);

  //angleSet(0,0,0,30);
  int steps = 2600;
  speed = 300;

  for (int i = 0; i < steps; i++)
  {
    moveElbow(0);
    //moveShoulder(0);
    // moveWrist(0);
    // moveBase(0);
    delayMicroseconds(speed);
  }

  delay(0);
  steps = 2600;
  for (int i = 0; i < steps; i++)
  {
    moveElbow(1);
    //moveShoulder(1);
    // moveWrist(1);
    // moveBase(1);
    delayMicroseconds(speed);
  }
}

void loop()
{
  /*
  int steps = 1500;
  speed = 200;

  for (int i = 0; i < steps; i++)
  {
    // moveElbow(0);
    moveShoulder(0);
    // moveWrist(0);
    // moveBase(0);
    delayMicroseconds(speed);
  }

  delay(2000);

  for (int i = 0; i < steps; i++)
  {
    // moveElbow(1);
    moveShoulder(1);
    // moveWrist(1);
    // moveBase(1);
    delayMicroseconds(speed);
  }
  */
  /*
  loopCounter += 1;
  //set_arm(-300, 0, 100, 0 ,10); //
  //delay(7000);
  //zero_x();
  //line();
  //circle();

  delay(4000);

  if (loopCounter > 1)
  {
    park();
    //set_arm(0, 0, 0, 0 ,10);  // park
    delay(5000);
    exit(0);
  } //pause program - hit reset to continue

  //exit(0);
  */
}

/*arm positioning routine utilizing inverse kinematics */
/*z is height, y is distance from base center out, x is side to side. y,z can only be positive */
//void set_arm(uint16_t x, uint16_t y, uint16_t z, uint16_t grip_angle)
void set_arm(float x, float y, float z, float grip_angle_d, int servoSpeed)
{
  float grip_angle_r = radians(grip_angle_d); //grip angle in radians for use in calculations
  /*Base angle and radial distance from x,y coordinates */
  float bas_angle_r = atan2(x, y);
  float bas_angle_d = degrees(bas_angle_r);
  float rdist = sqrt((x * x) + (y * y));
  /*rdist is y coordinate for the arm */
  y = rdist;
  /*Grip offsets calculated based on grip angle */
  float grip_off_z = (sin(grip_angle_r)) * HAND;
  float grip_off_y = (cos(grip_angle_r)) * HAND;
  /*Wrist position */
  float wrist_z = (z - grip_off_z) - BASE_HGT;
  float wrist_y = y - grip_off_y;
  /*Shoulder to wrist distance (AKA sw) */
  float s_w = (wrist_z * wrist_z) + (wrist_y * wrist_y);
  float s_w_sqrt = sqrt(s_w);
  /*s_w angle to ground */
  float a1 = atan2(wrist_z, wrist_y);
  /*s_w angle to humerus */
  float a2 = acos(((hum_sq - uln_sq) + s_w) / (2 * HUMERUS * s_w_sqrt));
  /*shoulder angle */
  float shl_angle_r = a1 + a2;
  float shl_angle_d = degrees(shl_angle_r);
  /*elbow angle */
  float elb_angle_r = acos((hum_sq + uln_sq - s_w) / (2 * HUMERUS * ULNA));
  float elb_angle_d = degrees(elb_angle_r);
  float elb_angle_dn = -(180.0 - elb_angle_d);
  /*wrist angle */
  float wri_angle_d = (grip_angle_d - elb_angle_dn) - shl_angle_d;

  /*Set steppers */
  Serial.println("Base angle degrees: " + String(bas_angle_d));
  Serial.println("Shoulder angle degrees: " + String(shl_angle_d));
  Serial.println("Elbow angle degrees: " + String(elb_angle_d));
  Serial.println("Wrist angle degrees: " + String(wri_angle_d));
}

void angleSet(int baseAngle, int shoulderAngle, int elbowAngle, int wristAngle)
{
  //calculate how many steps to move based on current position and desired angles
  int stepsToMove[] = {angleToSteps(baseAngle) - currentSteps[0], angleToSteps(shoulderAngle) - currentSteps[1], angleToSteps(elbowAngle) - currentSteps[2], angleToSteps(wristAngle) - currentSteps[3]};

  //move
  speed = 100;
  byte axisFinishedMovingCount = 0;
  while (axisFinishedMovingCount < 4)
  {
    axisFinishedMovingCount = 0;
    for (int axis = 0; axis < 4; axis++)
    {
      if (stepsToMove[axis] == 0)
      { //if there are no steps left to move, don't move
        axisFinishedMovingCount += 1;
      }
      else
      { //if there are steps left to move
        Serial.println("STEP");
        Serial.println(axis);
        Serial.println(stepsToMove[axis]);
        if (stepsToMove[axis] > 0)
        { //move clockwise
          //          axisStep(axis, 1, 1, 1);
          stepsToMove[axis] = stepsToMove[axis] - 1;
        }
        if (stepsToMove[axis] < 0)
        { //move counter clockwise
          //          axisStep(axis, 0);
          stepsToMove[axis] = stepsToMove[axis] + 1;
        }
        delayMicroseconds(speed);
      }
    }
  }
  Serial.println("END");
}

int angleToSteps(int angle)
{
  return int(angle / 1.8);
}

int stepsToAngle(int steps)
{
  return int(steps * 1.8);
}

/*move steppers to park position */
void park()
{
}

void zero_x()
{
  for (double yaxis = 250.0; yaxis < 400.0; yaxis += 1)
  {
    Serial.print(" yaxis= : ");
    Serial.println(yaxis);
    set_arm(0, yaxis, 200.0, 0, 10);
    delay(10);
  }
  for (double yaxis = 400.0; yaxis > 250.0; yaxis -= 1)
  {
    set_arm(0, yaxis, 200.0, 0, 10);
    delay(10);
  }
}

/*moves arm in a straight line */
void line()
{
  for (double xaxis = -100.0; xaxis < 100.0; xaxis += 0.5)
  {
    set_arm(xaxis, 250, 120, 0, 10);
    delay(10);
  }
  for (float xaxis = 100.0; xaxis > -100.0; xaxis -= 0.5)
  {
    set_arm(xaxis, 250, 120, 0, 10);
    delay(10);
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
    set_arm(0, yaxis, zaxis, 0, 50);
    delay(10);
  }
}
