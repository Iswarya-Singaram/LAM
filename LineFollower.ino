#include <Arduino.h>
#include <QTRSensors.h>
#include <TaskScheduler.h>
#include <QuickPID.h>
#include <jled.h>

/* * ADVANCED PID BOT v6.3 (HYPER SPEED MODE)
 * - Behavior: Max speed on straights, FAST cornering.
 * - Note: If it flies off the track, lower SPEED_TURN back down.
 */

// --- 1. PIN DEFINITIONS ---
// Motor Pins
const int ENA = 10; const int IN1 = 9; const int IN2 = 8;
const int ENB = 5;  const int IN3 = 7; const int IN4 = 6;

// Ultrasonic Sensor Pins
const int TRIG_PIN = 3; 
const int ECHO_PIN = 4;

auto led = JLed(13).Breathe(200).Forever(); // Fast breathe for fast mode

// --- 2. TUNING PARAMETERS (HYPER SPEED) ---

// SPEED SETTINGS
const uint8_t SPEED_STRAIGHT = 255; // Absolute Max
const uint8_t SPEED_TURN = 180;     // INCREASED: Was 90. Less braking on corners.
const uint8_t MAX_SPEED = 255;      

// OBSTACLE SETTINGS
const int OBSTACLE_DISTANCE_CM = 25; // Increased slightly for higher speed safety

// MOTOR DEADZONE
const uint8_t MOTOR_MIN_PWM = 45; 

// PID GAINS (AGGRESSIVE)
float Kp = 0.12;   // INCREASED: Was 0.08. Needs to turn harder now.
float Ki = 0.00;   
float Kd = 8.00;   // DECREASED: Was 10.5. Less resistance to turning.

// --- 3. OBJECTS & VARIABLES ---
QTRSensors qtr;
const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];

float setPoint = 3500; 
float input = 0;
float output = 0;

// Obstacle Flag
bool isBlocked = false;

QuickPID myPID(&input, &output, &setPoint, Kp, Ki, Kd, QuickPID::Action::direct);
Scheduler runner;

// --- 4. FUNCTION PROTOTYPES ---
void readSensors();
void computePID();
void driveMotors();
void checkObstacle(); 
void setL298N(int left, int right);

// --- 5. TASKS ---
Task taskRead(2, TASK_FOREVER, &readSensors, &runner, true); // Faster read (2ms)
Task taskPID(2, TASK_FOREVER, &computePID, &runner, true);   // Faster compute (2ms)
Task taskMotor(4, TASK_FOREVER, &driveMotors, &runner, true);
Task taskSonar(30, TASK_FOREVER, &checkObstacle, &runner, true); 

void setup() {
  Serial.begin(9600);
  
  // Motor Pins
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);

  // Ultrasonic Pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5, A6, A7}, SensorCount);

  delay(1000); 

  // --- CALIBRATION ---
  Serial.println("Calibrating... SWEEP ROBOT FAST!");
  led.Blink(30, 30).Forever();
  for (uint16_t i = 0; i < 250; i++) {
    qtr.calibrate();
    led.Update();
    delay(20);
  }
  Serial.println("Done.");
  led.Breathe(1000).Forever();

  myPID.SetOutputLimits(-MAX_SPEED, MAX_SPEED); 
  myPID.SetMode(QuickPID::Control::automatic);
}

void loop() {
  runner.execute(); 
  led.Update();     
}

// --- TASK 1: READ LINE SENSORS ---
void readSensors() {
  input = qtr.readLineBlack(sensorValues);
}

// --- TASK 2: READ ULTRASONIC ---
void checkObstacle() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 4000); 
  int distance = duration * 0.034 / 2;

  if (distance > 0 && distance < OBSTACLE_DISTANCE_CM) {
    isBlocked = true;
  } else {
    isBlocked = false;
  }
}

// --- TASK 3: PID CALCULATION ---
void computePID() {
  myPID.Compute();
}

// --- TASK 4: DRIVE MOTORS ---
void driveMotors() {
  
  if (isBlocked) {
    setL298N(0, 0); 
    return;         
  }

  // --- FASTER DRIVE LOGIC ---
  float turnSeverity = abs(output) / MAX_SPEED; 
  turnSeverity = constrain(turnSeverity, 0.0, 1.0);
  
  // Interpolates between 255 (Straight) and 180 (Turn)
  // This keeps the robot moving much faster on curves.
  int dynamicBaseSpeed = SPEED_STRAIGHT - (turnSeverity * (SPEED_STRAIGHT - SPEED_TURN));

  int leftSpeed = dynamicBaseSpeed + output;
  int rightSpeed = dynamicBaseSpeed - output;

  setL298N(leftSpeed, rightSpeed);
}

// --- HELPER: L298N ---
void setL298N(int left, int right) {
  left = constrain(left, -MAX_SPEED, MAX_SPEED);
  right = constrain(right, -MAX_SPEED, MAX_SPEED);

  if (left > 0 && left < MOTOR_MIN_PWM) left = MOTOR_MIN_PWM;
  if (left < 0 && left > -MOTOR_MIN_PWM) left = -MOTOR_MIN_PWM;
  if (right > 0 && right < MOTOR_MIN_PWM) right = MOTOR_MIN_PWM;
  if (right < 0 && right > -MOTOR_MIN_PWM) right = -MOTOR_MIN_PWM;

  if (left >= 0) {
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); analogWrite(ENA, left);
  } else {
    digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH); analogWrite(ENA, abs(left));
  }

  if (right >= 0) {
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); analogWrite(ENB, right);
  } else {
    digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH); analogWrite(ENB, abs(right));
  }
}
