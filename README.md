# 🚀 LAM Research Challenge Bot

An autonomous high-speed PID line follower robot with a smart arena automation system built for the **LAM Research Challenge**.

---

## 🤖 Features

### Line Follower Robot
- High-speed PID line following
- Dynamic speed control
- Obstacle detection using ultrasonic sensor
- QTR sensor calibration
- TaskScheduler multitasking
- L298N motor control

### Smart Arena System
- IR-based gate detection
- Automatic fluid dispensing
- Relay-controlled LED activation
- HX711 load cell weighing
- TFT live status display

---

# 🛠️ Hardware Used

## Robot
- Arduino Nano/Uno
- QTR-8A Sensor Array
- L298N Motor Driver
- HC-SR04 Ultrasonic Sensor
- DC Motors

## Arena
- ESP32
- ST7735 TFT Display
- HX711 + Load Cell
- IR Sensors
- Relay Modules
- Water Pump

---

# 📚 Libraries Used

```cpp
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

# ⚙️ PID Settings

```cpp
Kp = 0.12
Ki = 0.00
Kd = 8.00
```

---

# 🔌 Arena Pin Mapping

| Component | GPIO |
|---|---|
| Gate 1 IR | 4 |
| Gate 2 IR | 13 |
| Gate 3 IR | 14 |
| Pump Relay | 16 |
| LED Relay | 17 |
| HX711 DT | 19 |
| HX711 SCK | 25 |

---

# 🧠 Arena Workflow

### Gate 1
- Detect robot
- Dispense fluid

### Gate 2
- Turn ON LEDs

### Gate 3
- Measure weight
- Display result on TFT

---

# 📸 Demo

```md
![Robot](images/robot.jpg)
![Arena](images/arena.jpg)
```

---

# 👨‍💻 Team
**Hardware Hustle**

Built for the **LAM Research Challenge**
