// Reciever
#include <RCSwitch.h>
#include <Servo.h>

RCSwitch mySwitch = RCSwitch();
Servo gripperservo;
Servo armservo;

// Car Motor Pins
#define IN1 D1 // Motor 1 control
#define IN2 D2
#define IN3 D3 // Motor 2 control
#define IN4 D4

// Servo Control Pins
#define GRIPPER_SERVO_PIN D5
#define ARM_SERVO_PIN D6

const int transmitterID = 12345;  // Unique transmitter ID

void setup() {
  mySwitch.enableReceive(D7); // RF Receiver DATA pin

  // Motor pins setup
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Attach servos
  gripperservo.attach(GRIPPER_SERVO_PIN);
  gripperservo.write(0); // Start at closed position
  armservo.attach(ARM_SERVO_PIN);
  armservo.write(180); // Start at neutral position

  // Stop motors initially
  stopMotors();
}

void loop() {
  if (mySwitch.available()) {
    int receivedCode = mySwitch.getReceivedValue();
    mySwitch.resetAvailable();

    switch (receivedCode) {
      case 1111:
        moveForward();
        break;
      case 2222:
        moveBackward();
        break;
      case 3333:
        turnLeft();
        break;
      case 4444:
        turnRight();
        break;
      case 9999:
        moveForwardLeft();
        break;
      case 1010:
        moveForwardRight();
        break;
      case 8888:
        moveBackwardLeft();
        break;
      case 7777:
        moveBackwardRight();
        break;
      case 5555:
        gripperservo.write(180); // Open gripper
        break;
      case 6666:
        gripperservo.write(0); // Close gripper
        break;
      case 7777:
        armservo.write(0); // Move arm up
        break;
      case 8888:
        armservo.write(180); // Move arm down
        break;
      case 0000:
        stopMotors();
        break;
      default:
        stopMotors();
        break;
    }
  }
}

// Motor control functions
void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void moveForwardLeft() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void moveForwardRight() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackwardLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackwardRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
