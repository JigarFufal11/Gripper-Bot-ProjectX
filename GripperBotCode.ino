/// Code to operate gripper bot with mobile. Controller webpage ip:192.168.4.1
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

// Motor pin definitions
int enA = D1, in1 = D2, in2 = D3;
int enB = D4, in3 = D5, in4 = D6;

// Servo pin definitions
int servoGripperPin = D7;  // Servo for Open/Close control
int servoArmPin = D8;      // Servo for Up/Down control

ESP8266WebServer server(80);

// Access Point credentials
const char* ssid = "ProjectX";
const char* password = "areyousingle";

// Movement state variables
bool forwardPressed = false;
bool backwardPressed = false;
bool leftPressed = false;
bool rightPressed = false;

bool openPressed = false;
bool upPressed = false;

String command = "0";  
int SPEED = 500;

Servo gripperServo;
Servo armServo;

void setup() {
  Serial.begin(115200);
  Serial.println("Gripper Bot ACTIVATED");

  // Initialize motor pins
  pinMode(enA, OUTPUT); pinMode(in1, OUTPUT); pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT); pinMode(in3, OUTPUT); pinMode(in4, OUTPUT);

  analogWrite(enA, SPEED); analogWrite(enB, SPEED);
  digitalWrite(in1, LOW); digitalWrite(in2, LOW);
  digitalWrite(in3, LOW); digitalWrite(in4, LOW);

  // Attach servos to their respective pins
  gripperServo.attach(servoGripperPin);
  armServo.attach(servoArmPin);

  gripperServo.write(90);  // Default middle position
  armServo.write(90);

  WiFi.mode(WIFI_AP); // Set ESP8266 to Access Point mode
  WiFi.softAP(ssid, password); // Start the AP
  Serial.println("Wi-Fi Access Point started.");
  Serial.print("AP IP Address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", HTTP_handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
}

void loop() {
  server.handleClient();    // Handle incoming HTTP requests
  buildCommand();           // Build the command based on updated button states
  handleMovement();         // Execute movement based on the current command
  controlServos();          // Control the servos according to button states
}

void HTTP_handleRoot() {
  if (server.hasArg("Command")) {
    String receivedCommand = server.arg("Command");
    updateButtonStates(receivedCommand);
    Serial.println("Received Command: " + receivedCommand);
  }

  if (server.hasArg("Speed")) {
    SPEED = server.arg("Speed").toInt();
    Serial.println("Speed Updated: " + String(SPEED));
  }

  String htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Gripper Bot Controller</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      text-align: center;
      background-color: #111;
      color: #fff;
      margin: 0;
      padding: 0;
    }

    .controller-container {
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: center;
      height: 100vh;
    }

    h1 {
      font-size: 32px;
      margin-bottom: 20px;
    }

    .controls {
      display: flex;
      justify-content: space-between;
      width: 90%;
      max-width: 900px;
      margin-top: 20px;
    }

    .direction-buttons-left {
      display: flex;
      flex-direction: column;
      align-items: center;
      gap: 15px;
    }

    .direction-buttons-left button {
      width: 80px;
      height: 80px;
      font-size: 30px;
      cursor: pointer;
      border: 2px solid #ddd;
      border-radius: 10px;
      background-color: #333;
      color: #fff;
      box-shadow: 2px 2px 5px rgba(0, 0, 0, 0.2);
      user-select: none;  /* Prevent text selection */
    }

    .gripper-arm-controls {
      display: flex;
      justify-content: center;
      gap: 40px;
      flex-direction: row;
    }

    .gripper-arm-controls .group {
      display: flex;
      flex-direction: column;
      align-items: center;
      gap: 10px;
    }

    .gripper-arm-controls button {
      width: 120px;
      height: 60px;
      font-size: 18px;
      cursor: pointer;
      border: 2px solid #ddd;
      border-radius: 10px;
      background-color: #333;
      color: #fff;
      box-shadow: 2px 2px 5px rgba(0, 0, 0, 0.2);
      user-select: none;  /* Prevent text selection */
    }

    .heading {
      margin-bottom: 5px;
      font-size: 18px;
      font-weight: bold;
    }

    .direction-buttons-right {
      display: flex;
      flex-direction: row;
      align-items: center;
      gap: 15px;
    }

    .direction-buttons-right button {
      width: 80px;
      height: 80px;
      font-size: 30px;
      cursor: pointer;
      border: 2px solid #ddd;
      border-radius: 10px;
      background-color: #333;
      color: #fff;
      box-shadow: 2px 2px 5px rgba(0, 0, 0, 0.2);
      user-select: none;  /* Prevent text selection */
    }

    @media (max-width: 768px) {
      .controls {
        flex-direction: column;
        gap: 30px;
      }

      .direction-buttons-left {
        flex-direction: row;
        gap: 20px;
      }

      .gripper-arm-controls {
        flex-direction: column;
        gap: 30px;
      }

      .direction-buttons-right {
        flex-direction: column;
        gap: 15px;
      }
    }
  </style>
</head>
<body>
  <div class="controller-container">
    <h1>Gripper Bot Controller</h1>

    <!-- Controls -->
    <div class="controls">
      <!-- Left Controls: Forward and Backward -->
      <div class="direction-buttons-left">
        <button ontouchstart="sendCommand('FORWARD')" ontouchend="sendCommand('STOP')">&#8593;</button> <!-- Up Arrow -->
        <button ontouchstart="sendCommand('BACKWARD')" ontouchend="sendCommand('STOP')">&#8595;</button> <!-- Down Arrow -->
      </div>

      <!-- Gripper and Arm Controls in Center -->
      <div class="gripper-arm-controls">
        <!-- Gripper -->
        <div class="group">
          <div class="heading">Gripper</div>
          <button ontouchstart="sendCommand('Open')">Open</button>
          <button ontouchstart="sendCommand('Close')">Close</button>
        </div>

        <!-- Arm -->
        <div class="group">
          <div class="heading">Arm</div>
          <button ontouchstart="sendCommand('Up')">Up</button>
          <button ontouchstart="sendCommand('Down')">Down</button>
        </div>
      </div>

      <!-- Right Controls: Left and Right -->
      <div class="direction-buttons-right">
        <button ontouchstart="sendCommand('RIGHT')" ontouchend="sendCommand('STOP')">&#8592;</button> <!-- Left Arrow -->
        <button ontouchstart="sendCommand('LEFT')" ontouchend="sendCommand('STOP')">&#8594;</button> <!-- Right Arrow -->
      </div>
    </div>
  </div>

  <script>
    function sendCommand(command) {
      fetch('/?Command=' + command)
        .then(response => console.log('Sent: ' + command))
        .catch(err => alert('Failed to send'));
    }
  </script>
</body>
</html>)rawliteral";

  server.send(200, "text/html", htmlPage);
}

void handleNotFound() {
  server.send(404, "text/plain", "404: Not Found");
}

void updateButtonStates(String command) {
  if (command == "FORWARD") forwardPressed = true;
  else if (command == "BACKWARD") backwardPressed = true;
  else if (command == "LEFT") leftPressed = true;
  else if (command == "RIGHT") rightPressed = true;
  else if (command == "STOP") {
    forwardPressed = false;
    backwardPressed = false;
    leftPressed = false;
    rightPressed = false;
  }
  else if (command == "Open") openPressed = true;
  else if (command == "Close") openPressed = false;
  else if (command == "Up") upPressed = true;
  else if (command == "Down") upPressed = false;
}

void buildCommand() {
  if (forwardPressed && rightPressed) command = "P";
  else if (backwardPressed && rightPressed) command = "Q";
  else if (forwardPressed && leftPressed) command = "Z";
  else if (backwardPressed && leftPressed) command = "S";
  else if (forwardPressed) command = "F";
  else if (backwardPressed) command = "B";
  else if (leftPressed) command = "L";
  else if (rightPressed) command = "R";
  else command = "0";
}

void handleMovement() {
  if (command == "F") Forward();
  else if (command == "B") Backward();
  else if (command == "L") TurnLeft();
  else if (command == "R") TurnRight();
  else if (command == "P") ForwardRight();
  else if (command == "Q") BackwardRight();
  else if (command == "Z") ForwardLeft();
  else if (command == "S") BackwardLeft();
  else Stop();
}

void controlServos() {
  if (openPressed == true) {
    gripperServo.write(180);  // Move to 180 degrees
    delay(50);
  }
  if (openPressed == false) {
    gripperServo.write(70);    // Move to 0 degrees
    delay(50);
  }
  if (upPressed == true) {
    armServo.write(0);      // Move up to 0 degrees
    delay(50);
  }
  if (upPressed == false) {
    armServo.write(180);        // Move down to 180 degrees
    delay(50);
  }
}
void Forward() {
  Serial.println("Moving Forward");
  digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
}

void Backward() {
  Serial.println("Moving Backward");
  digitalWrite(in1, LOW); digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW); digitalWrite(in4, HIGH);
}

void TurnLeft() {
  Serial.println("Moving Left");
  digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
  digitalWrite(in3, LOW); digitalWrite(in4, HIGH);
}

void TurnRight() {
  Serial.println("Moving Right");
  digitalWrite(in1, LOW); digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
}

void ForwardLeft() {
  Serial.println("Moving Forward Left");
  digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
  analogWrite(enB, SPEED / 2);
}

void ForwardRight() {
  Serial.println("Moving Forward Right");
  digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
  analogWrite(enA, SPEED / 2);
}

void BackwardLeft() {
  Serial.println("Moving Backward Left");
  digitalWrite(in1, LOW); digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW); digitalWrite(in4, HIGH);
  analogWrite(enB, SPEED / 2);
}

void BackwardRight() {
  Serial.println("Moving Backward Right");
  digitalWrite(in1, LOW); digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW); digitalWrite(in4, HIGH);
  analogWrite(enA, SPEED / 2);
}

void Stop() {
  Serial.println("Stopped");
  digitalWrite(in1, LOW); digitalWrite(in2, LOW);
  digitalWrite(in3, LOW); digitalWrite(in4, LOW);
}
