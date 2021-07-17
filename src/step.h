#include "pins.h"

void moveShoulder(boolean clockwise)
{
    digitalWrite(SHOULDER_DIR_PIN, clockwise);
    digitalWrite(REVERSE_SHOULDER_DIR_PIN, !clockwise);
    digitalWrite(SHOULDER_STEP_PIN, HIGH);
    digitalWrite(REVERSE_SHOULDER_STEP_PIN, HIGH);
    digitalWrite(SHOULDER_STEP_PIN, LOW);
    digitalWrite(REVERSE_SHOULDER_STEP_PIN, LOW);
}

void moveElbow(boolean clockwise)
{
    digitalWrite(ELBOW_DIR_PIN, clockwise);
    digitalWrite(ELBOW_STEP_PIN, HIGH);
    digitalWrite(ELBOW_STEP_PIN, LOW);
}

void moveForearm(int steps, boolean clockwise, int wait)
{
    digitalWrite(FOREARM_DIR_PIN, clockwise);
    digitalWrite(FOREARM_STEP_PIN, HIGH);
    digitalWrite(FOREARM_STEP_PIN, LOW);
}

void moveWrist(boolean clockwise)
{
    digitalWrite(WRIST_DIR_PIN, clockwise);
    digitalWrite(WRIST_STEP_PIN, HIGH);
    digitalWrite(WRIST_STEP_PIN, LOW);
}

void moveBase(boolean clockwise)
{
    digitalWrite(BASE_DIR_PIN, clockwise);
    digitalWrite(BASE_STEP_PIN, HIGH);
}
