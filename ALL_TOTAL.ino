#include <ESP32Servo.h> 
 
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
} 
 
void loop() { 
  int flameLeft = digitalRead(FLAME1_PIN); 
  int flameCenter = digitalRead(FLAME2_PIN); 
  int flameRight = digitalRead(FLAME3_PIN); 
 
  Serial.print("Left: "); 
  Serial.print(flameLeft); 
  Serial.print(" | Center: "); 
  Serial.print(flameCenter); 
  Serial.print(" | Right: "); 
  Serial.println(flameRight); 
 
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
 
  // If all sensors detect flame 
  if (flameLeft == LOW && flameCenter == LOW && flameRight == LOW) { 
     
    moveBackward();
    delay(150);
    stopMotors();
    sprayWater(); // I GUESS I NEED TO ADD A LITTLE BACK WARD HERE(update previous), its running well( last update acc to 05.07.2025)

 
    // Re-check fire after spraying 
    delay(200); // Short delay before rechecking 
    flameLeft = digitalRead(FLAME1_PIN); 
    flameCenter = digitalRead(FLAME2_PIN); 
    flameRight = digitalRead(FLAME3_PIN); 
 
    if (flameLeft == LOW && flameCenter == LOW && flameRight == LOW) { 
      sprayWater();  // Spray again 
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
 
void stopMotors() { 
  digitalWrite(IN1, LOW); 
  digitalWrite(IN2, LOW); 
  digitalWrite(IN3, LOW); 
  digitalWrite(IN4, LOW); 
  Serial.println("Motors Stopped"); 
} 

void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
 
// Improved Water Spray Function with Smooth Servo Sweep 
void sprayWater() { 
  Serial.println("Spraying Water..."); 
  digitalWrite(PUMP_PIN, HIGH);  // Turn on pump 
 
  // Smooth sweep from left (30°) to right (110°) 
  for (int angle = 30; angle <= 110; angle += 2) { 
    pipeServo.write(angle); 
    delay(30); 
  } 
 
  // Return from right (110°) to center (70°) 
  for (int angle = 110; angle >= 70; angle -= 2) { 
    pipeServo.write(angle); 
    delay(30); 
  } 
 
  digitalWrite(PUMP_PIN, LOW);  // Turn off pump 
  Serial.println("Water Spray Complete"); 
} 
