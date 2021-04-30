#include "pins.h"

void moveShoulder(int steps, boolean clockwise, int wait)
{
    digitalWrite(SHOULDER_DIR_PIN, clockwise);
    digitalWrite(REVERSE_SHOULDER_DIR_PIN, !clockwise);
    for (int i = 0; i < steps; i++)
    {
        digitalWrite(SHOULDER_STEP_PIN, HIGH);
        digitalWrite(REVERSE_SHOULDER_STEP_PIN, HIGH);
        delayMicroseconds(wait);
        digitalWrite(SHOULDER_STEP_PIN, LOW);
        digitalWrite(REVERSE_SHOULDER_STEP_PIN, LOW);
        delayMicroseconds(wait);
    }
    digitalWrite(SHOULDER_DIR_PIN, LOW);
    digitalWrite(SHOULDER_STEP_PIN, LOW);
    digitalWrite(REVERSE_SHOULDER_DIR_PIN, LOW);
    digitalWrite(REVERSE_SHOULDER_STEP_PIN, LOW);
    digitalWrite(SHOULDER_ENABLE_PIN, LOW);
    digitalWrite(REVERSE_SHOULDER_ENABLE_PIN, LOW);
}

void moveElbow(int steps, boolean clockwise, int wait)
{
    digitalWrite(ELBOW_DIR_PIN, clockwise);
    for (int i = 0; i < steps; i++)
    {
        digitalWrite(ELBOW_STEP_PIN, HIGH);
        delayMicroseconds(wait);
        digitalWrite(ELBOW_STEP_PIN, LOW);
        delayMicroseconds(wait);
    }
}

void moveForearm(int steps, boolean clockwise, int wait)
{
    digitalWrite(FOREARM_DIR_PIN, clockwise);
    for (int i = 0; i < steps; i++)
    {
        digitalWrite(FOREARM_STEP_PIN, HIGH);
        delayMicroseconds(wait);
        digitalWrite(FOREARM_STEP_PIN, LOW);
        delayMicroseconds(wait);
    }
}

void moveWrist(int steps, boolean clockwise, int wait)
{
    digitalWrite(WRIST_DIR_PIN, clockwise);
    for (int i = 0; i < steps; i++)
    {
        digitalWrite(WRIST_STEP_PIN, HIGH);
        delayMicroseconds(wait);
        digitalWrite(WRIST_STEP_PIN, LOW);
        delayMicroseconds(wait);
    }
}