// ===== Human Following Robot (Optimized Version) =====
// Libraries Required: AFMotor, NewPing, Servo

#include <NewPing.h>
#include <Servo.h>
#include <AFMotor.h>

// -------- PIN DEFINITIONS --------
#define RIGHT_IR A2
#define LEFT_IR  A3
#define TRIGGER_PIN A1
#define ECHO_PIN    A0
#define MAX_DISTANCE 200

// -------- THRESHOLDS --------
#define IR_THRESHOLD 500   // Adjust based on IR sensor
#define MIN_DIST 2
#define MAX_DIST 15

// -------- GLOBAL VARIABLES --------
unsigned int distance;
int Right_Value, Left_Value;

// -------- OBJECTS --------
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
Servo myservo;

AF_DCMotor Motor1(1, MOTOR12_1KHZ);
AF_DCMotor Motor2(2, MOTOR12_1KHZ);
AF_DCMotor Motor3(3, MOTOR34_1KHZ);
AF_DCMotor Motor4(4, MOTOR34_1KHZ);

// -------- FUNCTION DECLARATIONS --------
void moveForward(int spd);
void turnLeft();
void turnRight();
void stopRobot();

// -------- SETUP --------
void setup() {
  Serial.begin(9600);

  pinMode(RIGHT_IR, INPUT);
  pinMode(LEFT_IR, INPUT);

  myservo.attach(10);
  myservo.write(90);   // Center position
  delay(500);

  Serial.println("Human Following Robot Ready");
}

// -------- LOOP --------
void loop() {

  // ----- Read Ultrasonic -----
  distance = sonar.ping_cm();
  if (distance == 0) distance = MAX_DISTANCE;

  // ----- Read IR Sensors -----
  Right_Value = analogRead(RIGHT_IR);
  Left_Value  = analogRead(LEFT_IR);

  // ----- Debug -----
  Serial.print("D: "); Serial.print(distance);
  Serial.print(" | R: "); Serial.print(Right_Value);
  Serial.print(" | L: "); Serial.println(Left_Value);

  // -------- MAIN LOGIC (PRIORITY BASED) --------

  // Turn Left
  if (Right_Value < IR_THRESHOLD && Left_Value > IR_THRESHOLD) {
    turnLeft();
  }

  // Turn Right
  else if (Right_Value > IR_THRESHOLD && Left_Value < IR_THRESHOLD) {
    turnRight();
  }

  // Move Forward (Human detected)
  else if (distance > MIN_DIST && distance < MAX_DIST) {
    int speedVal = map(distance, MIN_DIST, MAX_DIST, 120, 180);
    speedVal = constrain(speedVal, 120, 180);
    moveForward(speedVal);
  }

  // Stop
  else {
    stopRobot();
  }

  delay(30);  // small delay for stability
}

// -------- MOVEMENT FUNCTIONS --------

void moveForward(int spd) {
  Motor1.setSpeed(spd); Motor1.run(FORWARD);
  Motor2.setSpeed(spd); Motor2.run(FORWARD);
  Motor3.setSpeed(spd); Motor3.run(FORWARD);
  Motor4.setSpeed(spd); Motor4.run(FORWARD);
}

void turnLeft() {
  Motor1.setSpeed(150); Motor1.run(FORWARD);
  Motor2.setSpeed(150); Motor2.run(FORWARD);
  Motor3.setSpeed(150); Motor3.run(BACKWARD);
  Motor4.setSpeed(150); Motor4.run(BACKWARD);
}

void turnRight() {
  Motor1.setSpeed(150); Motor1.run(BACKWARD);
  Motor2.setSpeed(150); Motor2.run(BACKWARD);
  Motor3.setSpeed(150); Motor3.run(FORWARD);
  Motor4.setSpeed(150); Motor4.run(FORWARD);
}

void stopRobot() {
  Motor1.run(RELEASE);
  Motor2.run(RELEASE);
  Motor3.run(RELEASE);
  Motor4.run(RELEASE);
}
