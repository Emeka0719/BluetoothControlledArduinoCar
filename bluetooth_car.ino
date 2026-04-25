/*
 * ============================================================
 *  Bluetooth Controlled Arduino Car
 *  Motor Driver : L298N
 *  Control      : HC-05 / HC-06 Bluetooth Module
 *  Features     : PWM Speed Control (3 speeds)
 * ============================================================
 *
 * WIRING SUMMARY
 * -----------------------------------------------
 * L298N  IN1  → Arduino pin 2
 * L298N  IN2  → Arduino pin 3
 * L298N  IN3  → Arduino pin 4
 * L298N  IN4  → Arduino pin 5
 * L298N  ENA  → Arduino pin 9  (PWM)
 * L298N  ENB  → Arduino pin 10 (PWM)
 * L298N  GND  → Arduino GND
 * L298N  +5V  → Arduino 5V (logic supply)
 * L298N  +12V → Battery (+)
 *
 * HC-05/06 TX  → Arduino pin 11 (SoftwareSerial RX)
 * HC-05/06 RX  → Arduino pin 12 via 1kΩ+2kΩ voltage divider
 * HC-05/06 VCC → Arduino 5V
 * HC-05/06 GND → Arduino GND
 *
 * -----------------------------------------------
 * BLUETOOTH COMMAND SET (send via Serial terminal / app)
 * -----------------------------------------------
 *  'F' – Forward          'B' – Backward
 *  'L' – Turn Left        'R' – Turn Right
 *  'S' – Stop
 *  '1' – Speed Low  (40%)
 *  '2' – Speed Med  (70%)  ← default
 *  '3' – Speed High (100%)
 * ============================================================
 */

#include <SoftwareSerial.h>

// ── Bluetooth serial ──────────────────────────────────────
SoftwareSerial btSerial(11, 12);   // RX, TX

// ── L298N motor pins ──────────────────────────────────────
// Left motor
const uint8_t IN1 = 2;
const uint8_t IN2 = 3;
const uint8_t ENA = 9;   // PWM

// Right motor
const uint8_t IN3 = 4;
const uint8_t IN4 = 5;
const uint8_t ENB = 10;  // PWM

// ── Speed presets (PWM 0-255) ─────────────────────────────
const uint8_t SPEED_LOW  = 102;   // ~40 %
const uint8_t SPEED_MED  = 178;   // ~70 %
const uint8_t SPEED_HIGH = 255;   // 100 %

uint8_t currentSpeed = SPEED_MED;

// ─────────────────────────────────────────────────────────
void setup() {
  // Motor direction pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // PWM enable pins (HIGH by default; speed set via analogWrite)
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  stopMotors();

  Serial.begin(9600);       // USB debug
  btSerial.begin(9600);     // HC-05 / HC-06 default baud rate

  Serial.println(F("Bluetooth Car Ready."));
  Serial.println(F("Waiting for commands..."));
}

// ─────────────────────────────────────────────────────────
void loop() {
  if (btSerial.available()) {
    char cmd = (char)btSerial.read();
    handleCommand(cmd);
  }
}

// ── Command dispatcher ────────────────────────────────────
void handleCommand(char cmd) {
  Serial.print(F("CMD: "));
  Serial.println(cmd);

  switch (cmd) {
    case 'F': case 'f': moveForward();  break;
    case 'B': case 'b': moveBackward(); break;
    case 'L': case 'l': turnLeft();     break;
    case 'R': case 'r': turnRight();    break;
    case 'S': case 's': stopMotors();   break;
    case '1': setSpeed(SPEED_LOW);      break;
    case '2': setSpeed(SPEED_MED);      break;
    case '3': setSpeed(SPEED_HIGH);     break;
    default: break;   // ignore unknown commands
  }
}

// ── Motor control functions ───────────────────────────────
void moveForward() {
  setMotorSpeed(currentSpeed, currentSpeed);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void moveBackward() {
  setMotorSpeed(currentSpeed, currentSpeed);
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
}

void turnLeft() {
  // Right motor forward, left motor backward (pivot)
  setMotorSpeed(currentSpeed, currentSpeed);
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void turnRight() {
  // Left motor forward, right motor backward (pivot)
  setMotorSpeed(currentSpeed, currentSpeed);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
}

void stopMotors() {
  setMotorSpeed(0, 0);
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}

// ── Helpers ───────────────────────────────────────────────
void setMotorSpeed(uint8_t leftSpeed, uint8_t rightSpeed) {
  analogWrite(ENA, leftSpeed);
  analogWrite(ENB, rightSpeed);
}

void setSpeed(uint8_t speed) {
  currentSpeed = speed;
  Serial.print(F("Speed set to: "));
  Serial.println(speed);
}
