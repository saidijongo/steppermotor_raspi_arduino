// Pin definition
const int STEP_PIN = 2;
const int DIR_PIN = 3;    // Direction signal
const int CW_LIMIT_PIN = 4;  //CW Limit switch
const int CCW_LIMIT_PIN = 5; // CCW Limit switch

int angle = 0;
int direction = 0;  

void setup() {
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(CW_LIMIT_PIN, INPUT_PULLUP);
  pinMode(CCW_LIMIT_PIN, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "CW") {
      rotateMotor(30, 1);
    } else if (command == "CCW") {
      rotateMotor(30, -1);
    } else if (command == "STOP") {
      stopMotor();
    } else {
      //do nothing
    }
  }

  // Check for limit switches and update direction
  if (digitalRead(CW_LIMIT_PIN) == LOW) {
    direction = -1;  // CCW
    angle = 30;      // Reset angle to 30 degrees CCW
  } else if (digitalRead(CCW_LIMIT_PIN) == LOW) {
    direction = 1;   // CW
    angle = 30;      // Reset angle to 30 degrees CW
  }
}

void rotateMotor(int targetAngle, int rotateDirection) {
  int stepsToMove = targetAngle * 4096 / 360; // 4096 steps per revolution
  int stepsMoved = 0;

  digitalWrite(DIR_PIN, rotateDirection == 1 ? HIGH : LOW);

  while (stepsMoved < stepsToMove) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(500);  // Adjust speed here
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(500);  // Adjust speed here

    angle += rotateDirection;
    stepsMoved++;
    Serial.print("Angle: ");
    Serial.print(angle);
    Serial.print(" degrees, Direction: ");
    Serial.println(rotateDirection == 1 ? "CW" : "CCW");
  }
}

void stopMotor() {
  digitalWrite(STEP_PIN, LOW);
  direction = 0;
  Serial.println("Motor stopped");
}
