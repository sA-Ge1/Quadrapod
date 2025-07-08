
🕷️ Quadrapod
An ESP32 cam based quadruped robot capable of walking and performing movements using servo motors and sensor input. This project demonstrates fundamental concepts in robotics, embedded systems, and mechanical design.


🚀 Features
✅ 4-legged walking mechanism with multiple degrees of freedom

✅ Controlled by ESP32 CAM

✅ Driven by 8 servo motors


✅ Modular design for easy hardware replacement and upgrades


🧰 Software
Arduino IDE (tested on version 1.8.x and 2.x)

Servo library (<Servo.h>)

Adafruit SSD1306 and GFX libraries for OLED

Custom movement control algorithms

🚀 Getting Started
⚠️ Tip: Make sure your servos are powered through a separate supply to avoid brownouts.

🔥 Upload Code
bash
git clone https://github.com/sA-Ge1/Quadrapod.git
cd Quadrapod

Open the project in Arduino IDE.
Select your board and correct COM port.
Click Upload.

🎯 Usage
After uploading, the Quadrapod will initialize and move to a standing posture.

Connect to your IP address and use the web interface for control.
