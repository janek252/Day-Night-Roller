#include <Keypad.h>

// Keypa Configuration
const byte ROWS = 4; // four rows
const byte COLS = 4; // four columns
char keys[ROWS][COLS] = {
  {'4', '8', 'C', 'G'},
  {'3', '7', 'B', 'F'},
  {'2', '6', 'A', 'E'},
  {'1', '5', '9', 'D'}
};

// Pins assignment
byte rowPins[ROWS] = {9, 8, 7, 6}; // connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; // connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Motor control
#define DIR A2
#define STP A3
#define MAX 750

bool motorRunning = false; // Flag to track if the motor is running
bool stopMotor = false; // Flag to track if the motor should be stopped

void setup() {
  pinMode(DIR, OUTPUT);
  pinMode(STP, OUTPUT);
  digitalWrite(DIR, LOW);
  Serial.begin(9600);
}

void loop() {
  char key = keypad.getKey();
  if (key) {
    Serial.println(key);
    if (key == '1' && !stopMotor) {
      digitalWrite(DIR, HIGH); // Set direction for clockwise rotation
      motorRunning = true;
      tone(STP, MAX); // Set maximum rotation speed

    } else if (key == '2' && !stopMotor) {
      digitalWrite(DIR, HIGH); // Set direction for anti-clockwise rotation
      motorRunning = true;
      tone(STP, MAX * 0.75); // Set maximum rotation speed
    } else if (key == '3' && !stopMotor) {
      digitalWrite(DIR, HIGH); // Set direction for anti-clockwise rotation
      motorRunning = true;
      tone(STP, MAX * 0.5); // Set maximum rotation speed
    } else if (key == '4' && !stopMotor) {
      digitalWrite(DIR, HIGH); // Set direction for anti-clockwise rotation
      motorRunning = true;
      tone(STP, MAX * 0.25); // Set maximum rotation speed
    } else if (key == '5' && !stopMotor) {
      digitalWrite(DIR, LOW); // Set direction for clockwise rotation
      motorRunning = true;
      tone(STP, MAX);
    } else if (key == '6' && !stopMotor) {
      digitalWrite(DIR, LOW); // Set direction for anti-clockwise rotation
      motorRunning = true;
      tone(STP, MAX * 0.75);
    } else if (key == '7' && !stopMotor) {
      digitalWrite(DIR, LOW); // Set direction for anti-clockwise rotation
      motorRunning = true;
      tone(STP, MAX * 0.5);
    } else if (key == '8' && !stopMotor) {
      digitalWrite(DIR, LOW); // Set direction for anti-clockwise rotation
      motorRunning = true;
      tone(STP, MAX * 0.25);
    } else if (key == '9' && !stopMotor) {
      digitalWrite(DIR, LOW); // Set direction for anti-clockwise rotation
      motorRunning = true;
      tone(STP, MAX * 0.25);
      delay(400);
      stopMotor = true; // Set the stopMotor flag
      delay(20);
    } else if (key == 'A' && !stopMotor) {
      digitalWrite(DIR, LOW); // Set direction for anti-clockwise rotation
      motorRunning = true;
      tone(STP, MAX * 0.25);
      delay(200);
      stopMotor = true; // Set the stopMotor flag
      delay(20);
    }  else if (key == 'B' && !stopMotor) {
      digitalWrite(DIR, LOW); // Set direction for anti-clockwise rotation
      motorRunning = true;
      tone(STP, MAX * 0.25);
      delay(100);
      stopMotor = true; // Set the stopMotor flag
      delay(20);
    }




    else if (key == 'G') {
      stopMotor = true; // Set the stopMotor flag
    }
  }

  // Stop the motor if no key is pressed or '9' is pressed
  if (!motorRunning || stopMotor) {
    if (motorRunning) {
      noTone(STP); // Stop the motor
      motorRunning = false;
    }
    stopMotor = false; // Reset the stopMotor flag
  }
}
