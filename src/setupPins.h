void setupPins()
{
    pinMode(SHOULDER_STEP_PIN, OUTPUT);
    pinMode(SHOULDER_DIR_PIN, OUTPUT);
    pinMode(SHOULDER_ENABLE_PIN, OUTPUT);
    digitalWrite(SHOULDER_ENABLE_PIN, LOW);

    pinMode(REVERSE_SHOULDER_STEP_PIN, OUTPUT);
    pinMode(REVERSE_SHOULDER_DIR_PIN, OUTPUT);
    pinMode(REVERSE_SHOULDER_ENABLE_PIN, OUTPUT);
    digitalWrite(REVERSE_SHOULDER_ENABLE_PIN, LOW);

    pinMode(ELBOW_STEP_PIN, OUTPUT);
    pinMode(ELBOW_DIR_PIN, OUTPUT);
    pinMode(ELBOW_ENABLE_PIN, OUTPUT);
    digitalWrite(ELBOW_ENABLE_PIN, LOW);

    pinMode(FOREARM_STEP_PIN, OUTPUT);
    pinMode(FOREARM_DIR_PIN, OUTPUT);
    pinMode(FOREARM_ENABLE_PIN, OUTPUT);
    digitalWrite(FOREARM_DIR_PIN, LOW);

    pinMode(WRIST_STEP_PIN, OUTPUT);
    pinMode(WRIST_DIR_PIN, OUTPUT);
    pinMode(WRIST_ENABLE_PIN, OUTPUT);
    digitalWrite(WRIST_ENABLE_PIN, LOW);

    pinMode(BASE_STEP_PIN, OUTPUT);
    pinMode(BASE_DIR_PIN, OUTPUT);
    pinMode(BASE_ENABLE_PIN, OUTPUT);
    digitalWrite(BASE_ENABLE_PIN, LOW);
}