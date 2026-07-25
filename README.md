# 🚀 LAM Research Challenge 2025 - Hardware Hustle Bot

<p align="center">

### Autonomous High-Speed PID Line Follower Robot with Smart Arena Automation System

</p>

---

## 📌 Overview

This project was developed for the **LAM Research Challenge 2025 - Hardware Hustle** as part of Team **Tech Tacklers (LRC-C-16576-9)**.

The system consists of two major subsystems:

1. **Advanced Line Follower Robot (ALFR)**  
   - Autonomous high-speed line following
   - PID-based motion control
   - Obstacle detection
   - Precision navigation

2. **Smart Arena Automation System**
   - Automated fluid dispensing
   - LED activation control
   - Robot weight verification
   - Real-time TFT display feedback

The design focuses on **reliability, manufacturing efficiency, and optimized mechanical performance** through custom fabrication and intelligent control algorithms.

---

# 🎥 Project Demo

🔗 **Demo Video:**  
(Add your YouTube/Drive link here)

---

# 🏗️ System Architecture

```
                    SMART ARENA SYSTEM

                         ESP32
                           |
        --------------------------------------
        |                 |                  |
     Gate 1            Gate 2             Gate 3
        |                 |                  |
    IR Sensor        IR Sensor        Load Cell
        |                 |                  |
 Peristaltic Pump    LED Relay       HX711 Module
        |                 |                  |
 Fluid Delivery     LED Activation    Weight Check
                                      |
                                  TFT Display


                    AUTONOMOUS ROBOT

                    Arduino Controller
                           |
        ----------------------------------
        |                |               |
    QTR Sensors     Ultrasonic       Motor Driver
        |                |               |
   Line Detection  Obstacle Check    DC Motors
                           |
                    PID Controller
```

---

# 🤖 Robot Features

## Advanced Line Follower Robot (ALFR)

### ⚡ Autonomous Navigation
- High-speed PID based line tracking
- Smooth cornering and stable motion
- Dynamic motor speed adjustment
- QTR sensor calibration

### 🧠 Control Algorithm

Instead of simple ON/OFF line following logic, a PID controller was implemented.

The controller continuously calculates:

- **Proportional (P):** Immediate correction based on current error
- **Integral (I):** Removes accumulated error
- **Derivative (D):** Prevents overshooting and oscillation

This enables smooth and accurate navigation even at high speeds.

### PID Parameters

```
Kp = 0.12
Ki = 0.00
Kd = 8.00
```

---

# 🏭 Smart Arena Automation System

The arena consists of three automated checkpoints controlled using an ESP32.

---

## Gate 1 - Automated Fluid Delivery

### Function:
Automatically dispenses 125 ml fluid after detecting the robot.

### Working:

1. IR sensor detects robot arrival
2. ESP32 triggers motor driver
3. Peristaltic pump dispenses calibrated fluid quantity
4. System resets after completion


### Pump Design

A custom peristaltic pump was developed using:

- Silicone tube
- 3D printed roller mechanism
- DC motor drive

Advantages:

✅ Precise fluid delivery  
✅ No contamination between fluid and mechanism  
✅ Instant stop after motor cutoff  

---

## Gate 2 - LED Activation

### Function:
Activates LAM LED array when the robot crosses the checkpoint.

Implementation:

- IR sensor detection
- Relay-based switching
- Electrical isolation between ESP32 and LED load

---

## Gate 3 - Weight Verification

### Function:
Verifies final robot weight after completing the arena task.

Components:

- 5kg Load Cell
- HX711 Amplifier
- ST7735 TFT Display


Workflow:

```
Robot Parking
      |
      ↓
Weight Measurement
      |
      ↓
Tolerance Check
      |
      ↓
Display Result
```

Display Output:

```
Team Name

Simulation Completed!
```

or

```
Weight Mismatch
```

---

# 🔩 Hardware Components

## Robot Hardware

| Component | Purpose |
|---|---|
| Arduino Nano/Uno | Main controller |
| QTR-8A Sensor Array | Line detection |
| L298N Motor Driver | Motor control |
| DC Motors | Robot movement |
| HC-SR04 Ultrasonic Sensor | Obstacle detection |


## Arena Hardware

| Component | Purpose |
|---|---|
| ESP32-WROOM-32 | Central controller |
| IR Sensors | Gate detection |
| Relay Modules | Pump & LED control |
| Peristaltic Pump | Fluid dispensing |
| HX711 + Load Cell | Weight measurement |
| ST7735 TFT Display | Status visualization |

---

# 🔌 Arena Pin Mapping

| Component | ESP32 GPIO |
|---|---|
| Gate 1 IR Sensor | GPIO 4 |
| Gate 2 IR Sensor | GPIO 13 |
| Gate 3 IR Sensor | GPIO 14 |
| Pump Relay | GPIO 16 |
| LED Relay | GPIO 17 |
| HX711 DT | GPIO 19 |
| HX711 SCK | GPIO 25 |

---

# 💻 Software Stack

### Programming

- Embedded C/C++

### Controllers

- Arduino Nano
- ESP32

### Libraries Used

```
QTRSensors
QuickPID
TaskScheduler
JLed
HX711
Adafruit_GFX
Adafruit_ST7735
SPI
```

---

# 🛠️ Mechanical Innovations

## Custom Wooden Chassis

Instead of conventional 3D printed chassis:

Advantages:

- Increased structural rigidity
- Better vibration damping
- Faster prototyping
- Improved stability during lifting operations


## Differential Drive System

Selected over mecanum wheels because:

- Higher traction
- Better stability
- Reduced wheel slip
- Improved obstacle manipulation accuracy


## Topology Optimized Canopy

A custom protective canopy was designed with:

- Reduced material usage
- Improved PCB protection
- Lightweight structure
- Enhanced aesthetics

---

# 📂 Repository Structure

```
LAM-Hardware-Hustle/
│
├── Robot/
│   ├── PID_LineFollower_Code/
│   └── Libraries/
│
├── Arena/
│   ├── ESP32_Control_Code/
│   └── Pin_Mapping.md
│
├── CAD/
│   ├── Robot_Chassis/
│   └── Pump_Design/
│
├── Images/
│
├── Documentation/
│
└── README.md
```

---

# 🚀 Future Improvements

- ROS2 integration for autonomous navigation
- Camera-based lane detection
- Wireless monitoring dashboard
- Advanced motion planning algorithms
- AI-assisted obstacle recognition

---

# 🏆 Achievement

## LAM Research Challenge 2025

**Team:** Tech Tacklers  
**Project:** Hardware Hustle - Robotic Simulation Essentials

---

# 👨‍💻 Team

Developed by:

**Team Tech Tacklers**

LAM Research Challenge 2025

---

⭐ If you found this project interesting, feel free to explore the implementation!
