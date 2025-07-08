# 🕷️ Quadrapod

An **ESP32-CAM based quadruped robot** capable of walking and performing complex movements using servo motors and sensor input.  
This project demonstrates fundamental concepts in **robotics, embedded systems, and mechanical design**.

---

## 🚀 Features

✅ **4-legged walking mechanism** with multiple degrees of freedom  
✅ Controlled by **ESP32-CAM** (Wi-Fi + Camera module)  
✅ Driven by **8 servo motors** for coordinated leg movements  
✅ **Modular design** for easy hardware replacement and upgrades  

---

## 🧰 Software

- **Arduino IDE** (tested on version 1.8.x and 2.x)
- `Servo` library (`<Servo.h>`)
- Custom movement control algorithms

---

## 🚀 Getting Started

⚠️ **Tip:**  
Make sure your servos are powered through a **separate supply** (not from the ESP32 itself) to avoid voltage drops or brownouts.

---

### 🔥 Upload Code

```bash
git clone https://github.com/sA-Ge1/Quadrapod.git
cd Quadrapod
```

### 🎯 Usage
After uploading, the Quadrapod will initialize and move to a standing posture.

Connect to the ESP32-CAM IP address on your network and use the web interface for control.
