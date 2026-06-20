/*******************************************************************************
 * SELF-BALANCING ROBOT - OPTIMIZED FOR YOUR DIMENSIONS
 * 
 * Robot Specs: 17x14cm base, 9cm tower height (top-heavy design)
 * 
 * Features:
 * - Optimized PID for tall robots
 * - Fixed timestep control (100Hz)
 * - Removed asymmetric gain (caused oscillation)
 * - Proper motor dead-zone compensation
 * - HC-SR04 ultrasonic sensor (optional obstacle detection)
 * 
 * Hardware:
 * - Arduino UNO
 * - MPU6050 (GY-521)
 * - L298N Motor Driver  
 * - 2x DC Motors
 * - Battery (7-12V)
 * - HC-SR04 Ultrasonic (optional)
 * 
 * Wiring:
 * MPU6050:     SDA->A4, SCL->A5, VCC->5V, GND->GND
 * L298N:       ENA->10, IN1->8, IN2->9, IN3->6, IN4->7, ENB->5
 * HC-SR04:     TRIG->12, ECHO->11, VCC->5V, GND->GND (optional)
 ******************************************************************************/

#include <Wire.h>
#include <TimerOne.h>

// ============================================================================
// PIN CONFIGURATION
// ============================================================================

// Motor Pins (CORRECTED for your wiring: OUT1/OUT2=Right, OUT3/OUT4=Left)
#define ENA 10    // Right motor PWM
#define IN1 8     // Right motor direction
#define IN2 9
#define IN3 6     // Left motor direction  
#define IN4 7
#define ENB 5     // Left motor PWM

// Ultrasonic Sensor Pins (optional - can be disabled)
#define TRIG_PIN 12
#define ECHO_PIN 11
#define ENABLE_ULTRASONIC true  // Set false to disable ultrasonic sensor

// MPU6050 I2C address
#define MPU6050_ADDR 0x68

// ============================================================================
// CONFIGURATION - OPTIMIZED FOR YOUR ROBOT (17x14cm, 9cm tall)
// ============================================================================

// PID Settings - TUNED FOR TALL, TOP-HEAVY ROBOTS
float Kp = 50.0;    // Strong response (tall robots need this!)
float Ki = 200.0;   // Eliminates steady lean
float Kd = 3.2;     // High damping (critical for tall robots!)

// Motor Settings
#define MOTOR_DEAD_ZONE 35      // Minimum PWM to overcome friction
#define MOTOR_MIN_SPEED 20      // Dead-band threshold
#define MOTOR_MAX_SPEED 255     // FIXED: Was 355, now proper 255!

// Safety Settings
#define MAX_ANGLE 40.0          // Max tilt before giving up (degrees)
#define FALL_TIMEOUT 2000       // Recovery timeout (ms)

// Ultrasonic Settings (optional)
#define OBSTACLE_DISTANCE 15.0  // Stop if obstacle closer than 15cm

// Control Loop Timing
#define CONTROL_FREQ 100        // 100Hz control loop
#define CONTROL_PERIOD (1000000 / CONTROL_FREQ)  // microseconds

// ============================================================================
// GLOBAL VARIABLES
// ============================================================================

// Sensor raw values
int16_t ax, ay, az;
int16_t gx, gy, gz;

// Angle calculation
float currentAngle = 0;
float accAngle = 0;
float angleOffset = 0;

// PID variables
float targetAngle = 0;
float error = 0;
float lastAngle = 0;        // For derivative on measurement
float integral = 0;
float derivative = 0;
float derivativeFiltered = 0;
float pidOutput = 0;

// Motor control
int motorPWM = 0;

// Ultrasonic
float distance = 999;
bool obstacleDetected = false;
unsigned long lastUltrasonicRead = 0;

// Timing
volatile bool controlFlag = false;
unsigned long lastMicros = 0;
float dt = 0.01;  // 10ms for 100Hz

// Safety
bool isFallen = false;
unsigned long fallStartTime = 0;
bool systemActive = true;

// ============================================================================
// SETUP
// ============================================================================

void setup() {
  Serial.begin(115200);
  
  Serial.println(F("=== Self-Balancing Robot ==="));
  Serial.println(F("Optimized for: 17x14cm base, 9cm tower"));
  Serial.println();
  
  // Initialize I2C
  Wire.begin();
  Wire.setClock(400000);  // 400kHz Fast I2C
  
  // Initialize MPU6050
  Serial.println(F("Initializing MPU6050..."));
  initMPU6050();
  
  // Setup motor pins
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
  
  // Setup ultrasonic pins (if enabled)
  if (ENABLE_ULTRASONIC) {
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    Serial.println(F("Ultrasonic sensor: ENABLED"));
  } else {
    Serial.println(F("Ultrasonic sensor: DISABLED"));
  }
  
  // Stop motors
  stopMotors();
  
  // Calibrate
  Serial.println(F("\nCalibrating..."));
  Serial.println(F("Keep robot UPRIGHT and STILL!"));
  delay(2000);
  calibrate();
  
  Serial.println(F("Calibration complete!"));
  Serial.print(F("Angle offset: "));
  Serial.println(angleOffset, 2);
  
  // Print PID gains
  Serial.println(F("\n=== PID Configuration ==="));
  Serial.print(F("Kp: ")); Serial.println(Kp, 1);
  Serial.print(F("Ki: ")); Serial.println(Ki, 1);
  Serial.print(F("Kd: ")); Serial.println(Kd, 1);
  
  Serial.println(F("\nStarting in 3 seconds..."));
  delay(3000);
  
  // Start timer interrupt for fixed-frequency control
  Timer1.initialize(CONTROL_PERIOD);  // 10ms = 100Hz
  Timer1.attachInterrupt(controlLoopISR);
  
  lastMicros = micros();
  systemActive = true;
  
  Serial.println(F("=== SYSTEM ACTIVE ===\n"));
}

// ============================================================================
// MAIN LOOP
// ============================================================================

void loop() {
  // Wait for control loop flag from timer interrupt
  if (controlFlag) {
    controlFlag = false;
    
    // Calculate actual dt
    unsigned long currentMicros = micros();
    dt = (currentMicros - lastMicros) / 1000000.0;
    lastMicros = currentMicros;
    
    // Constrain dt (safety check)
    if (dt > 0.05) dt = 0.01;
    
    // Read sensors
    readMPU6050();
    calculateAngle();
    
    // Read ultrasonic (every 100ms to avoid interference)
    if (ENABLE_ULTRASONIC) {
      unsigned long currentMillis = millis();
      if (currentMillis - lastUltrasonicRead > 100) {
        distance = readUltrasonic();
        lastUltrasonicRead = currentMillis;
        
        // Check for obstacle
        obstacleDetected = (distance > 0 && distance < OBSTACLE_DISTANCE);
      }
    }
    
    // Safety check - fallen
    if (!checkSafety()) {
      stopMotors();
      return;
    }
    
    // Stop if obstacle detected
    if (obstacleDetected) {
      stopMotors();
      static unsigned long lastWarning = 0;
      if (millis() - lastWarning > 500) {
        Serial.print(F("OBSTACLE at "));
        Serial.print(distance, 1);
        Serial.println(F("cm - STOPPED"));
        lastWarning = millis();
      }
      return;
    }
    
    // Calculate PID
    calculatePID();
    
    // Apply motor compensation
    motorPWM = applyMotorCompensation(pidOutput);
    
    // Drive motors
    driveMotors(motorPWM);
    
    // Debug output
    printDebug();
  }
  
  // Background tasks can go here
}

// ============================================================================
// TIMER INTERRUPT
// ============================================================================

void controlLoopISR() {
  controlFlag = true;
}

// ============================================================================
// INITIALIZE MPU6050
// ============================================================================

void initMPU6050() {
  // Wake up MPU6050
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0x00);  // Clear sleep bit
  Wire.endTransmission(true);
  delay(100);
  
  // Configure accelerometer (±2g)
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x1C);
  Wire.write(0x00);
  Wire.endTransmission(true);
  
  // Configure gyroscope (±250°/s)
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x1B);
  Wire.write(0x00);
  Wire.endTransmission(true);
  
  // Configure DLPF (low-pass filter)
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x1A);
  Wire.write(0x03);  // ~43Hz bandwidth
  Wire.endTransmission(true);
  
  Serial.println(F("MPU6050 initialized!"));
}

// ============================================================================
// READ MPU6050
// ============================================================================

void readMPU6050() {
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x3B);  // ACCEL_XOUT_H register
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_ADDR, 14, true);
  
  // Read accelerometer
  ax = Wire.read() << 8 | Wire.read();
  ay = Wire.read() << 8 | Wire.read();
  az = Wire.read() << 8 | Wire.read();
  
  // Skip temperature
  Wire.read();
  Wire.read();
  
  // Read gyroscope
  gx = Wire.read() << 8 | Wire.read();
  gy = Wire.read() << 8 | Wire.read();
  gz = Wire.read() << 8 | Wire.read();
}

// ============================================================================
// READ ULTRASONIC SENSOR (HC-SR04)
// ============================================================================

float readUltrasonic() {
  // Send 10us pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Read echo (timeout 30ms)
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  
  // Calculate distance in cm
  if (duration == 0) return 999;  // Out of range
  
  float dist = duration * 0.01715;  // Speed of sound conversion
  return dist;
}

// ============================================================================
// CALIBRATION
// ============================================================================

void calibrate() {
  float sum = 0;
  int samples = 500;
  
  Serial.print(F("Calibrating"));
  
  for (int i = 0; i < samples; i++) {
    readMPU6050();
    
    // Calculate angle from accelerometer
    float angle = atan2(ay, az) * 180.0 / PI;
    sum += angle;
    
    if (i % 50 == 0) Serial.print(F("."));
    delay(5);
  }
  
  Serial.println();
  angleOffset = sum / samples;
  currentAngle = 0;
  lastAngle = 0;
}

// ============================================================================
// CALCULATE ANGLE (COMPLEMENTARY FILTER)
// ============================================================================

void calculateAngle() {
  // Accelerometer angle (pitch)
  accAngle = atan2(ay, az) * 180.0 / PI - angleOffset;
  
  // Gyroscope integration
  float gyroRate = gx / 131.0;  // Convert to °/s
  float gyroAngle = currentAngle + gyroRate * dt;
  
  // Complementary filter (98% gyro, 2% accel)
  currentAngle = 0.98 * gyroAngle + 0.02 * accAngle;
}

// ============================================================================
// PID CALCULATION - OPTIMIZED FOR TALL ROBOTS
// ============================================================================

void calculatePID() {
  // Calculate error
  error = targetAngle - currentAngle;
  
  // Proportional term
  float pTerm = Kp * error;
  
  // Integral term with conditional integration (anti-windup)
  if (abs(error) < 20.0 && abs(pidOutput) < MOTOR_MAX_SPEED * 0.9) {
    integral += error * dt;
    integral = constrain(integral, -25.0, 25.0);
  }
  
  // Back-calculation anti-windup
  if (abs(motorPWM) >= MOTOR_MAX_SPEED) {
    float excess = pidOutput - motorPWM;
    integral -= 0.5 * excess * dt;
  }
  
  float iTerm = Ki * integral;
  
  // Derivative term (on measurement, not error - prevents kick)
  float rawDerivative = -(currentAngle - lastAngle) / dt;
  
  // Low-pass filter on derivative (critical for noise reduction!)
  derivativeFiltered = 0.7 * derivativeFiltered + 0.3 * rawDerivative;
  
  float dTerm = Kd * derivativeFiltered;
  
  // Calculate total PID output
  pidOutput = pTerm + iTerm + dTerm;
  
  // REMOVED ASYMMETRIC GAIN - was causing oscillation!
  // Old code had: if (pidOutput < 0) pidOutput *= 1.4;
  
  // Clamp output to valid PWM range
  pidOutput = constrain(pidOutput, -MOTOR_MAX_SPEED, MOTOR_MAX_SPEED);
  
  // Save for next iteration
  lastAngle = currentAngle;
}

// ============================================================================
// MOTOR COMPENSATION (DEAD-ZONE)
// ============================================================================

int applyMotorCompensation(float rawOutput) {
  int compensatedOutput;
  
  // Apply dead-zone compensation with active dead-band
  if (rawOutput > MOTOR_MIN_SPEED) {
    compensatedOutput = (int)rawOutput + MOTOR_DEAD_ZONE;
  } else if (rawOutput < -MOTOR_MIN_SPEED) {
    compensatedOutput = (int)rawOutput - MOTOR_DEAD_ZONE;
  } else {
    compensatedOutput = 0;  // Active dead-band for stability
  }
  
  // Final clamping
  compensatedOutput = constrain(compensatedOutput, -MOTOR_MAX_SPEED, MOTOR_MAX_SPEED);
  
  return compensatedOutput;
}

// ============================================================================
// SAFETY CHECK
// ============================================================================

bool checkSafety() {
  // Check if fallen
  if (abs(currentAngle) > MAX_ANGLE) {
    if (!isFallen) {
      fallStartTime = millis();
      isFallen = true;
      Serial.println(F("FALLEN! Attempting recovery..."));
    }
    
    // Give up if fallen too long
    if (millis() - fallStartTime > FALL_TIMEOUT) {
      if (systemActive) {
        Serial.println(F("FALLEN TOO LONG - SYSTEM SHUTDOWN"));
        Serial.println(F("Please reset Arduino to restart"));
        systemActive = false;
      }
      return false;
    }
    
    return true;  // Still trying to recover
  } else {
    // Recovered!
    if (isFallen) {
      Serial.println(F("Recovered!"));
      integral = 0;  // Reset integral
    }
    isFallen = false;
    return true;
  }
}

// ============================================================================
// DRIVE MOTORS
// ============================================================================

void driveMotors(int pwm) {
  bool forward = (pwm >= 0);
  int absPWM = abs(pwm);
  
  if (forward) {
    // Move forward
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else {
    // Move backward
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
  
  // Apply PWM to both motors
  analogWrite(ENA, absPWM);
  analogWrite(ENB, absPWM);
}

// ============================================================================
// STOP MOTORS
// ============================================================================

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  integral = 0;  // Reset integral when stopping
}

// ============================================================================
// DEBUG OUTPUT
// ============================================================================

void printDebug() {
  static unsigned long lastPrint = 0;
  
  // Print every 200ms (5Hz)
  if (millis() - lastPrint > 200) {
    lastPrint = millis();
    
    Serial.print(F("Angle: "));
    Serial.print(currentAngle, 2);
    Serial.print(F(" | PID: "));
    Serial.print(pidOutput, 0);
    Serial.print(F(" | PWM: "));
    Serial.print(motorPWM);
    
    if (ENABLE_ULTRASONIC && distance < 100) {
      Serial.print(F(" | Dist: "));
      Serial.print(distance, 1);
      Serial.print(F("cm"));
    }
    
    Serial.println();
  }
}

// ============================================================================
// END OF CODE
// ============================================================================
