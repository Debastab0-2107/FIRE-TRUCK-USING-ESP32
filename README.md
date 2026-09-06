# 🚒 Fire Fighting Robot Using ESP32

> An ESP32-based autonomous fire-fighting robot that detects the direction of a flame, navigates toward it, and activates a water pump to extinguish the fire.

[![ESP32](https://img.shields.io/badge/Controller-ESP32-blue?logo=espressif)](https://www.espressif.com/en/products/socs/esp32)
[![Arduino](https://img.shields.io/badge/Platform-Arduino_IDE-00979D?logo=arduino)](https://www.arduino.cc/)
[![C++](https://img.shields.io/badge/Language-C%2B%2B-blue?logo=cplusplus)](https://isocpp.org/)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)

---

## 📌 Project Overview

The **Fire Fighting Robot Using ESP32** is a small-scale autonomous robotic vehicle designed to detect and respond to a fire source without requiring direct human control.

The robot uses multiple flame sensors to determine the approximate direction of a fire. Based on the sensor readings, the ESP32 controls the motors and moves the vehicle toward the detected flame.

When the robot reaches a position where the fire is detected by all three flame sensors, it stops, activates a water pump, and uses a servo motor to sweep the water nozzle across the target area.

The ESP32 also hosts a lightweight **web-based monitoring interface over Wi-Fi**, allowing the current fire-fighting status to be viewed from a device connected to the same network.

---

## 🎯 Objectives

The main objectives of this project are:

* 🔥 Detect the presence of a flame.
* 🧭 Determine the approximate direction of the fire.
* 🚗 Automatically move toward the detected fire.
* 💧 Activate a water pump when the fire is reached.
* 🎯 Sweep the water nozzle using a servo motor.
* 📡 Provide real-time status through an ESP32 web server.
* 🤖 Demonstrate the integration of sensors, actuators, embedded programming, and IoT connectivity.

---

## ✨ Features

### 🔥 Fire Detection

Three flame sensors are positioned to detect fire from:

* Left
* Center
* Right

The ESP32 continuously reads the sensors and determines the direction of the flame.

### 🧭 Autonomous Movement

Depending on the detected flame position:

| Flame Detection   | Robot Action                                  |
| ----------------- | --------------------------------------------- |
| Left sensor       | Turn left                                     |
| Center sensor     | Move forward                                  |
| Right sensor      | Turn right                                    |
| No flame          | Stop                                          |
| All three sensors | Move backward briefly and begin extinguishing |

The movement logic is implemented directly using the motor driver control pins.

### 💧 Automatic Fire Extinguishing

When all three flame sensors detect the fire, the robot:

1. Stops its movement.
2. Moves backward slightly.
3. Activates the water pump.
4. Rotates the servo-mounted nozzle.
5. Sprays water across the target.
6. Returns the nozzle toward its center position.
7. Turns the pump off.

### 📡 ESP32 Web Monitoring

The ESP32 runs a web server on port `80`.

After connecting to Wi-Fi, the ESP32 displays its local IP address in the Serial Monitor.

Opening that IP address in a browser provides a simple fire-fighting status dashboard.

The dashboard displays:

* 🔴 Fire Detected
* 🟠 Fire Extinguishing
* 🟢 No Fire Detected

The page automatically refreshes periodically so that the displayed status can change according to the sensor readings.

---

# 🧩 System Architecture

```text
                    ┌──────────────────────┐
                    │       ESP32          │
                    │   Main Controller    │
                    └──────────┬───────────┘
                               │
             ┌─────────────────┼─────────────────┐
             │                 │                 │
             ▼                 ▼                 ▼
      ┌─────────────┐   ┌─────────────┐   ┌─────────────┐
      │ Flame       │   │ Motor       │   │ Water       │
      │ Sensors     │   │ Driver      │   │ Pump        │
      │ L / C / R   │   │             │   │             │
      └─────────────┘   └──────┬──────┘   └─────────────┘
                                │
                                ▼
                         ┌─────────────┐
                         │ DC Motors   │
                         │ 4-Wheel     │
                         │ Movement    │
                         └─────────────┘

                               │
                               ▼
                         ┌─────────────┐
                         │   Servo     │
                         │ Water Nozzle│
                         └─────────────┘

                               │
                               ▼
                         ┌─────────────┐
                         │ Web Server  │
                         │ Wi-Fi Status│
                         └─────────────┘
```

---

# 🛠️ Hardware Components

| Component                  |    Quantity | Purpose                   |
| -------------------------- | ----------: | ------------------------- |
| ESP32 Development Board    |           1 | Main controller           |
| Flame Sensor               |           3 | Detect fire direction     |
| L298N Motor Driver         |           1 | Control DC motors         |
| DC BO Motors               |           4 | Robot movement            |
| Servo Motor                |           1 | Control water nozzle      |
| DC Water Pump              |           1 | Pump water                |
| Water Tank                 |           1 | Store extinguishing water |
| TIP122 / Switching Circuit |           1 | Drive the water pump      |
| Robot Chassis              |           1 | Mechanical structure      |
| Battery Pack               |           1 | Power supply              |
| Jumper Wires               | As required | Electrical connections    |

---

# 🔌 Pin Configuration

The current Arduino sketch uses the following ESP32 pin configuration:

| Component           | ESP32 GPIO |
| ------------------- | ---------: |
| Servo Motor         |    GPIO 23 |
| Ultrasonic Trigger  |    GPIO 21 |
| Ultrasonic Echo     |    GPIO 19 |
| Left Flame Sensor   |    GPIO 32 |
| Center Flame Sensor |    GPIO 35 |
| Right Flame Sensor  |    GPIO 34 |
| Motor Driver IN1    |    GPIO 25 |
| Motor Driver IN2    |    GPIO 26 |
| Motor Driver IN3    |    GPIO 27 |
| Motor Driver IN4    |    GPIO 33 |
| Water Pump          |     GPIO 4 |

> **Note:** The current code defines the ultrasonic sensor pins, but the present control logic primarily uses the flame sensors for fire-directed movement.

---

# ⚙️ Working Principle

The robot follows a simple sensor-driven decision process.

### Step 1 — Initialize the System

When powered on, the ESP32:

* Initializes the motor control pins.
* Initializes the flame sensors.
* Initializes the water pump.
* Attaches the servo motor.
* Connects to Wi-Fi.
* Starts the web server.

The servo initially moves to approximately its center position.

---

### Step 2 — Monitor Flame Sensors

The ESP32 continuously reads the three flame sensors.

```text
             FRONT OF ROBOT

        LEFT      CENTER      RIGHT
          │          │           │
          ▼          ▼           ▼
       Sensor 1   Sensor 2    Sensor 3
          │          │           │
          └──────────┼───────────┘
                     │
                     ▼
                  ESP32
```

---

### Step 3 — Determine Fire Direction

The robot uses the sensor readings to determine its movement.

```text
Flame on LEFT
      ↓
Turn LEFT

Flame in CENTER
      ↓
Move FORWARD

Flame on RIGHT
      ↓
Turn RIGHT

No Flame
      ↓
STOP
```

---

### Step 4 — Approach the Fire

The robot continues adjusting its direction according to the flame sensor readings.

The goal is to bring the fire source into the detection range of all three sensors.

---

### Step 5 — Fire Extinguishing Mode

When all three flame sensors detect the flame:

```text
Left   = FIRE
Center = FIRE
Right  = FIRE
```

the robot enters the extinguishing sequence.

```text
           FIRE DETECTED
                 │
                 ▼
          Stop the robot
                 │
                 ▼
       Move backward briefly
                 │
                 ▼
          Activate pump
                 │
                 ▼
        Move servo nozzle
        from 30° → 110°
                 │
                 ▼
        Return nozzle toward
             center
                 │
                 ▼
          Stop water pump
```

The current implementation performs the water-nozzle sweep using the servo while the pump is active.

---

# 🌐 Web-Based Monitoring

One of the important features of this project is the built-in ESP32 web server.

The ESP32 connects to the configured Wi-Fi network and starts a server on:

```text
Port: 80
```

The IP address assigned to the ESP32 is printed in the Serial Monitor.

Example:

```text
Connected to WiFi
IP Address: 192.168.1.100
Web server started.
```

Enter the displayed IP address into a browser:

```text
http://192.168.1.100
```

The dashboard provides a visual indication of the current robot state.

### Status States

🟢 **NO FIRE DETECTED**

The flame sensors are not detecting a fire.

---

🔴 **FIRE HAS BEEN DETECTED**

At least one flame sensor has detected a fire.

---

🟠 **FIRE EXTINGUISHING**

The robot has entered the extinguishing sequence and activated the water spray mechanism.

The web interface is implemented directly inside the ESP32 sketch using the `WebServer` library.

---

# 💻 Software Requirements

### Arduino IDE

Install the latest suitable version of Arduino IDE.

### ESP32 Board Package

Add ESP32 board support to Arduino IDE and select the appropriate ESP32 development board.

### Required Libraries

The current project uses:

```cpp
#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
```

| Library        | Purpose                  |
| -------------- | ------------------------ |
| `WiFi.h`       | ESP32 Wi-Fi connectivity |
| `WebServer.h`  | Embedded HTTP server     |
| `ESP32Servo.h` | Servo motor control      |

These dependencies are visible in the current project sketch.

---

# 🚀 Getting Started

## 1. Clone the Repository

```bash
git clone https://github.com/Debastab0-2107/FIRE-TRUCK-USING-ESP32.git
```

Move into the project directory:

```bash
cd FIRE-TRUCK-USING-ESP32
```

---

## 2. Open the Project

Open:

```text
ALL_TOTAL.ino
```

using Arduino IDE.

---

## 3. Configure Wi-Fi

Before uploading, change the Wi-Fi credentials in the source code:

```cpp
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
```

**Do not upload real Wi-Fi credentials to a public GitHub repository.**

For a public repository, use placeholder credentials or move the credentials into a separate configuration file that is excluded from Git.

---

## 4. Select the ESP32 Board

In Arduino IDE:

```text
Tools
   ↓
Board
   ↓
ESP32
   ↓
Your ESP32 Board
```

Then select the correct COM port.

---

## 5. Upload

Connect the ESP32 through USB and upload the sketch.

Open Serial Monitor:

```text
Baud Rate: 115200
```

You should see Wi-Fi connection information and the ESP32 IP address.

---

# 🧪 Testing Procedure

For safe testing, use a controlled flame source and keep appropriate fire-safety equipment nearby.

### Test 1 — No Flame

Expected result:

```text
No Flame
    ↓
Robot Stops
    ↓
Web Status: NO FIRE DETECTED
```

### Test 2 — Flame on Left

Expected result:

```text
Left Sensor
     ↓
Fire Detected
     ↓
Robot Turns Left
```

### Test 3 — Flame in Center

Expected result:

```text
Center Sensor
      ↓
Fire Detected
      ↓
Robot Moves Forward
```

### Test 4 — Flame on Right

Expected result:

```text
Right Sensor
      ↓
Fire Detected
      ↓
Robot Turns Right
```

### Test 5 — All Sensors Detect Flame

Expected result:

```text
All Sensors Detect Flame
          ↓
Robot Moves Backward Briefly
          ↓
Robot Stops
          ↓
Pump ON
          ↓
Servo Sweeps Nozzle
          ↓
Water Spraying
          ↓
Pump OFF
```

---

# 🧠 Control Logic

The current control algorithm can be summarized as:

```text
START
  │
  ▼
Initialize ESP32
  │
  ▼
Connect to Wi-Fi
  │
  ▼
Start Web Server
  │
  ▼
Read Flame Sensors
  │
  ├───────────────┐
  │               │
  ▼               ▼
Flame Detected   No Flame
  │               │
  ▼               ▼
Determine        Stop Motors
Direction
  │
  ├── Left ───────► Turn Left
  │
  ├── Center ─────► Move Forward
  │
  └── Right ──────► Turn Right
  │
  ▼
All Sensors Detect Flame?
  │
  ├── NO ──► Continue Tracking
  │
  └── YES
       │
       ▼
  Move Backward
       │
       ▼
   Stop Motors
       │
       ▼
    Pump ON
       │
       ▼
 Servo Nozzle Sweep
       │
       ▼
    Pump OFF
       │
       ▼
   Continue Monitoring
```

---

# 📂 Repository Structure

```text
FIRE-TRUCK-USING-ESP32/
│
├── ALL_TOTAL.ino
│
└── README.md
```

The primary implementation is currently contained in `ALL_TOTAL.ino`.

---

# 🔧 Possible Improvements

This project can be extended significantly in future versions.

### 🤖 Better Autonomous Navigation

Add multiple ultrasonic sensors to provide:

* Front obstacle detection
* Left obstacle detection
* Right obstacle detection
* Rear obstacle detection

This would allow the robot to combine **fire tracking with obstacle avoidance**.

### 🌡️ Temperature Monitoring

Integrate a temperature sensor such as:

* MLX90614
* DS18B20

This would provide another environmental signal alongside the flame sensors.

### 📷 ESP32-CAM Integration

An ESP32-CAM could be added to provide:

* Live video
* Remote monitoring
* Fire-scene visualization
* Web-based camera streaming

### 📱 Mobile-Friendly Interface

The existing web dashboard could be expanded into a responsive control panel with:

* Live status
* Robot controls
* Camera stream
* Sensor readings
* Pump status
* Battery voltage
* Emergency stop

### 📊 Sensor Dashboard

Display real-time values such as:

```text
Flame Left      : DETECTED
Flame Center    : DETECTED
Flame Right     : CLEAR
Pump            : OFF
Servo Position  : 70°
Robot State     : SEARCHING
```

### 🔋 Battery Monitoring

Add a voltage-divider circuit and ADC-based monitoring to display the remaining battery voltage.

### 🧠 Improved Fire Localization

Instead of using only basic conditional logic, future versions could use weighted sensor readings or a more sophisticated state machine to improve fire localization and reduce unnecessary movement.

---

# ⚠️ Safety Notice

This project is a **prototype for educational and experimental purposes**.

It should **not be considered a certified fire-safety or emergency-response system**.

Never test the robot around uncontrolled fires, flammable materials, people, animals, or environments where water/electrical interaction could create a hazard.

Always perform experiments in a controlled environment with suitable fire-safety precautions.

---

# 📚 Learning Outcomes

This project provides practical experience in:

* Embedded systems
* ESP32 programming
* Arduino/C++ programming
* Digital sensor interfacing
* Motor-driver control
* Servo control
* DC pump control
* Autonomous robotics
* IoT and Wi-Fi communication
* Embedded web servers
* Hardware-software integration
* Real-time sensor-based decision making

---

# 👨‍💻 Author

### Debastab Das

Computer Science / MCA Student



---

# ⭐ Support the Project

If you find this project useful or interesting:

* ⭐ Star the repository
* 🍴 Fork the repository
* 🐛 Report issues
* 💡 Suggest improvements
* 🔧 Contribute enhancements

---

# 📜 License

This project is intended for educational and research purposes.

If you choose to distribute or modify the project, please provide appropriate attribution to the original author.

---

## 🚒 Project Summary

The **Fire Fighting Robot Using ESP32** demonstrates how an inexpensive microcontroller can combine **fire detection, autonomous movement, water-based extinguishing, servo control, and Wi-Fi monitoring** into a single robotic platform.

The project serves as a practical demonstration of embedded systems, robotics, IoT, and real-time sensor-based control.

**Built with ESP32 • Arduino/C++ • Sensors • Motors • Servo • Water Pump • Wi-Fi**
