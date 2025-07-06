#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

// Wi-Fi credentials
const char* ssid = "Hello_World";
const char* password = "12345678";

// Web server on port 80
WebServer server(80);
String fireStatus = "✅ No Fire";

// Pin Definitions
#define SERVO_PIN 23
#define TRIG_PIN 21
#define ECHO_PIN 19
#define FLAME1_PIN 32
#define FLAME2_PIN 35
#define FLAME3_PIN 34
#define IN1 25
#define IN2 26
#define IN3 27
#define IN4 33
#define PUMP_PIN 4

Servo pipeServo;

void setup() {
  Serial.begin(115200);

  // Motor Pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Flame Sensors
  pinMode(FLAME1_PIN, INPUT);
  pinMode(FLAME2_PIN, INPUT);
  pinMode(FLAME3_PIN, INPUT);

  // Water Pump
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW); // Pump off initially

  // Servo
  pipeServo.attach(SERVO_PIN);
  pipeServo.write(70);  // Center position

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Web Server Route
server.on("/", []() {
  String html;

  // Decide color for the button-like box
  if (fireStatus == "FIRE DETECTED") {
    html = R"rawliteral(
<html>
<head>
<meta http-equiv='refresh' content='1'>
</head>
<html>
<body style='height: 100vh;background:black;display: flex;align-items: center;justify-content: center;font-family:Georgia; '>
<div style='background-color: black;border: 5px solid cyan;padding: 40px;border-radius: 15px;box-shadow: 0 0 35px cyan;width: 600px;text-align: center;'>
  <p style='margin:35px; font-size:40px; color: white; text-align:center;'><b>Fire Fighter Car Status</b></p>

      <div style="background-color: crimson; color: black; padding: 20px; margin: 40px; border-radius: 20px; text-align: center; font-size: 20px ;box-shadow: 0 0 25px red;">
        <b>FIRE HAS BEEN DETECTED.</b>
      </div>
      <div style="background-color: black; color: white; padding: 20px; margin: 40px; text-align: center;border-radius:20px; box-shadow: 0 0 20px white; font-size: 20px;border :4px solid white;">
        <b>FIRE EXTINGUISHING.</b>
      </div>
      <div style="background-color: black; color: white; padding:20px; margin: 40px; text-align: center; font-size: 20px;border:4px solid white; border-radius:20px; box-shadow: 0 0 20px white;">
        <b>NO FIRE DETECTED. </b>
      </div></div>
  </body>
</html>
    )rawliteral";
  } else if (fireStatus == "FIRE EXTINGUISHING") {
    html = R"rawliteral(
<html>
<head>
<meta http-equiv='refresh' content='1'>
</head>
<html>
  <body style='height: 100vh;background: black;display: flex;align-items: center;justify-content: center;font-family: Georgia; '>
<div style='background-color: black;border: 5px solid cyan;padding: 40px;border-radius: 15px;box-shadow: 0 0 35px cyan;width: 600px;text-align: center;'>
  <p style='margin:35px; font-size:40px; color: white; text-align:center;'><b>Fire Fighter Car Status</b></p>
      <div style='background-color: black; color: white; padding:20px; margin: 40px; text-align: center; font-size: 20px;border:4px solid white; border-radius:20px; box-shadow: 0 0 20px white;'>
        <b>FIRE HAS BEEN DETECTED.</b>
      </div>
      <div style='background-color: orange; color: black; padding: 20px; margin: 40px; border-radius: 20px; text-align: center; font-size: 20px ;box-shadow: 0 0 25px orange;'>
        <b>FIRE EXTINGUISHING.</b>
      </div>
      <div style='background-color: black; color: white; padding:20px; margin: 40px; text-align: center; font-size: 20px;border:4px solid white; border-radius:20px; box-shadow: 0 0 20px white;'>
        <b>NO FIRE DETECTED. </b>
      </div></div>
  </body>
</html>
    )rawliteral";
  } else {  // ✅ NO FIRE page
    html = R"rawliteral(
<html>
<head>
<meta http-equiv='refresh' content='1'>
</head>
<html>
<body style='height: 100vh;background: black;display: flex;align-items: center;justify-content: center;font-family: Georgia; '>
<div style='background-color: black;border: 5px solid cyan;padding: 40px;border-radius: 15px;box-shadow: 0 0 35px cyan;width: 600px;text-align: center;'>
  <p style='margin:35px; font-size:40px; color: white; text-align:center;'><b>Fire Fighter Car Status</b></p>
      <div style="background-color: black; color: white; padding:20px; margin: 40px; text-align: center; font-size: 20px;border:4px solid white; border-radius:20px; box-shadow: 0 0 20px white;">
        <b>FIRE HAS BEEN DETECTED.</b>
      </div>
      <div style="background-color: black; color: white; padding: 20px; margin: 40px; text-align: center;border-radius:20px; box-shadow:  0 0 20px white; font-size: 20px;border :4px solid white;">
        <b>FIRE EXTINGUISHING.</b>
      </div>
      <div style="background-color: lime; color: black; padding: 20px; margin: 40px; border-radius: 20px; text-align: center; font-size: 20px ;box-shadow: 0 0 25px lime;">
        <b>NO FIRE DETECTED. </b>
      </div></div>
  </body>
</html>
    )rawliteral";
  }

  server.send(200, "text/html", html);
});

  server.begin();
  Serial.println("Web server started.");
}

void loop() {
  server.handleClient();  // Check web requests

  int flameLeft = digitalRead(FLAME1_PIN);
  int flameCenter = digitalRead(FLAME2_PIN);
  int flameRight = digitalRead(FLAME3_PIN);

  Serial.print("Left: ");
  Serial.print(flameLeft);
  Serial.print(" | Center: ");
  Serial.print(flameCenter);
  Serial.print(" | Right: ");
  Serial.println(flameRight);

  // Update fire status
  if (flameLeft == LOW || flameCenter == LOW || flameRight == LOW) {
    fireStatus = "FIRE DETECTED";
  } else {
    fireStatus = "NO FIRE DETECTED";
  }

  // Movement Decision
  if (flameLeft == LOW) {
    moveLeft();
  } else if (flameRight == LOW) {
    moveRight();
  } else if (flameCenter == LOW) {
    moveForward();
  } else {
    stopMotors(); // No flame detected
  }

  // All sensors detect flame
  if (flameLeft == LOW && flameCenter == LOW && flameRight == LOW) {
    moveBackward();
    delay(150);
    stopMotors();
    fireStatus = "FIRE EXTINGUISHING";
    server.handleClient();// HANDLE CLIENT USED 2nd TIME
    sprayWater();

    delay(200); // Delay before rechecking
    flameLeft = digitalRead(FLAME1_PIN);
    flameCenter = digitalRead(FLAME2_PIN);
    flameRight = digitalRead(FLAME3_PIN);

    if (flameLeft == LOW && flameCenter == LOW && flameRight == LOW) {
      fireStatus = "FIRE EXTINGUISHING";
      server.handleClient(); // HANDLE CLIENT USED 3RD TIME
      sprayWater();      
    } 
    
  }

  delay(100); // Small delay between cycles
}

// Movement Functions
void moveLeft() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("Moving Left");
}

void moveRight() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Moving Right");
}

void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Moving Forward");
}

void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("Moving Backward");
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  Serial.println("Motors Stopped");
}

// Water Spray Function
void sprayWater() {
  Serial.println("Spraying Water...");
  digitalWrite(PUMP_PIN, HIGH);  // Turn on pump

  // Sweep from left to right
  for (int angle = 30; angle <= 110; angle += 2) {
    pipeServo.write(angle);
    delay(30);
  }

  // Sweep back to center
  for (int angle = 110; angle >= 70; angle -= 2) {
    pipeServo.write(angle);
    delay(30);
  }

  digitalWrite(PUMP_PIN, LOW);  // Turn off pump
  Serial.println("Water Spray Complete");
}
