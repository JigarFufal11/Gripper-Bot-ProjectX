//Transmitter
#include<RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

#define FORWARD_PIN D0
#define BACKWARD_PIN D1
#define LEFT_PIN D2
#define RIGHT_PIN D3
#define OPEN_PIN D4
#define CLOSE_PIN D6
#define UP_PIN D7
#define DOWN_PIN D8

void setup() {
  // Initialize RC-Switch transmitter
  mySwitch.enableTransmit(D5); // GPIO pin for the RF transmitter's DATA pin    

  // Set button pins as input
  pinMode(FORWARD_PIN, INPUT_PULLUP);
  pinMode(BACKWARD_PIN, INPUT_PULLUP);
  pinMode(LEFT_PIN, INPUT_PULLUP);
  pinMode(RIGHT_PIN, INPUT_PULLUP);
  pinMode(OPEN_PIN, INPUT_PULLUP);
  pinMode(CLOSE_PIN, INPUT_PULLUP);
  pinMode(UP_PIN, INPUT_PULLUP);
  pinMode(DOWN_PIN, INPUT_PULLUP);
}

void loop() {
  // Read button states
  bool forwardPressed = (digitalRead(FORWARD_PIN) == LOW);
  bool backwardPressed = (digitalRead(BACKWARD_PIN) == LOW);
  bool leftPressed = (digitalRead(LEFT_PIN) == LOW);
  bool rightPressed = (digitalRead(RIGHT_PIN) == LOW);
  bool openPressed = (digitalRead(OPEN_PIN) == LOW);
  bool closePressed = (digitalRead(CLOSE_PIN) == LOW);
  bool upPressed = (digitalRead(UP_PIN) == LOW);
  bool downPressed = (digitalRead(DOWN_PIN) == LOW);

  // Check for combined commands first
  if (forwardPressed && leftPressed) {
    mySwitch.send(9999, 24); // Unique code for "Forward-Left"
    delay(200); // Add delay to debounce the buttons
  } else if (forwardPressed && rightPressed) {
    mySwitch.send(1010, 24); // Unique code for "Forward-Right"
    delay(200);
  } else if (backwardPressed && leftPressed) {
    mySwitch.send(8888, 24); // Unique code for "Backward-Left"
    delay(200);
  } else if (backwardPressed && rightPressed) {
    mySwitch.send(7777, 24); // Unique code for "Backward-Right"
    delay(200);
  } else if (forwardPressed) {
    mySwitch.send(1111, 24); // Unique code for "Forward"
    delay(200);
  } else if (backwardPressed) {
    mySwitch.send(2222, 24); // Unique code for "Backward"
    delay(200);
  } else if (leftPressed) {
    mySwitch.send(3333, 24); // Unique code for "Left"
    delay(200);
  } else if (rightPressed) {
    mySwitch.send(4444, 24); // Unique code for "Right"
    delay(200);
  } else if (openPressed) {
    mySwitch.send(5555, 24); // Unique code for "Open"
    delay(200);
  } else if (closePressed) {
    mySwitch.send(6666, 24); // Unique code for "Close"
    delay(200);
  } else if (upPressed) {
    mySwitch.send(7777, 24); // Unique code for "Up"
    delay(200);
  } else if (downPressed) {
    mySwitch.send(8888, 24); // Unique code for "Down"
    delay(200);
  } else {
    mySwitch.send(0000, 24); // Default "Stop" command
    delay(200);
  }
}
