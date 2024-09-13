#include <WiFi.h>
#include <WebServer.h>
#include "esp_camera.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "index_html.h"  // Include the HTML file

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN  150 // Minimum pulse length count (out of 4096) for SG90
#define SERVOMAX  600 // Maximum pulse length count (out of 4096) for SG90   
int STEP_SIZE = 2; // Default step size for smooth movement
int currentAngles[16] = {90, 90, 90, 90, 90, 90, 90, 90 ,90, 90, 90, 90, 90, 90, 90, 90}; // Store current angles of each servo
int zeroError[16] = {0, 0, 0, 0, 0, 0, 0, 0, -10, 0, -20, 0, 0, 0, 0, 0}; // Store zero error for each servo
int a[8] = {4, 5, 6, 7, 8, 9, 10, 11};  // Servo pins

// Function prototypes
void smoothMoveSimultaneously(int* servos, int* targetAngles, int numServos);
void smoothMove(int servoNum, int targetAngle);
int angleToPulse(int angle, int zeroError);
int signum(int value);
int dtime = 10;

// Replace with your network credentials
const char* ssid = "Aspider";
const char* password = "11111111";

// Create a WebServer object on port 80
WebServer server(80);

char data[100];  // Store the received text command
String latestCommand = "";  // Store the latest command
String prevCommand = "";

// Camera configuration for AI-Thinker model
void setupCamera() 
{
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = 5;
  config.pin_d1 = 18;
  config.pin_d2 = 19;
  config.pin_d3 = 21;
  config.pin_d4 = 36;
  config.pin_d5 = 39;
  config.pin_d6 = 34;
  config.pin_d7 = 35;
  config.pin_xclk = 0;
  config.pin_pclk = 22;
  config.pin_vsync = 25;
  config.pin_href = 23;
  config.pin_sscb_sda = 26;
  config.pin_sscb_scl = 27;
  config.pin_pwdn = 32;
  config.pin_reset = -1;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  // Init with high specs to pre-allocate larger buffers
  if (psramFound()) 
  {
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 6;
    config.fb_count = 2;
  } 

  else 
  {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 6;
    config.fb_count = 1;
  }

  // Camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) 
  {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
}


// Stream video over HTTP
void handleStream() 
{
  camera_fb_t * fb = NULL;
  fb = esp_camera_fb_get();
  if (!fb) 
  {
    Serial.println("Camera capture failed");
    server.send(500);
    return;
  }
  server.sendHeader("Content-Type", "image/jpeg");
  server.send_P(200, "image/jpeg", (const char *)fb->buf, fb->len);
  esp_camera_fb_return(fb);
}

void setup() 
{
  // Start Serial Monitor
  Wire.begin(14, 15);
  Serial.begin(115200);

  pinMode(4, OUTPUT);
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos like the SG90 run at ~60 Hz updates

  // Initialize all servos to 90 degrees
  int servos[] = {a[0], a[2], a[4],a[6]};
  int angles[] = {90, 90, 90, 90};
  smoothMoveSimultaneously(servos, angles, 4);

  int servos2[] = {a[1], a[3], a[5], a[7]};
  int angles2[] = {90, 90, 90, 90};
  smoothMoveSimultaneously(servos2, angles2, 4);

  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("Access Point IP Address: ");
  Serial.println(IP);

  // Initialize the camera
  setupCamera();

  // Serve the HTML page
  server.on("/", HTTP_GET, []() 
  {
    server.send_P(200, "text/html", index_html);
  });

  // Handle the video stream
  server.on("/stream", HTTP_GET, handleStream);

  // Handle the AJAX text command
  server.on("/sendText", HTTP_GET, []() {
    if (server.hasArg("data")) {
      latestCommand = server.arg("data");
      prevCommand = "";
      latestCommand.toCharArray(data, 100);
      Serial.println("Received: " + latestCommand);  // You can process the data here
    }
    server.send(200, "text/plain", "OK");
  });
  
  // Start the server
  server.sendHeader("Content-Type", "text/html; charset=UTF-8");

  server.begin();
  Serial.println("Server started");
}


void loop() 
{
  // Handle client requests
  server.handleClient();

  STEP_SIZE=2;
  if(prevCommand!=latestCommand)
  {
    prevCommand=latestCommand;
    String command = latestCommand;

    if (command == "g") 
    {
      int servos[] = {a[0], a[2], a[4],a[6]};
      int angles[] = {120, 60, 120, 60};
      smoothMoveSimultaneously(servos, angles, 4);

      int servos2[] = {a[1], a[3], a[5], a[7]};
      int angles2[] = {0, 180, 0, 180};
      smoothMoveSimultaneously(servos2, angles2, 4);

      Serial.println("Servo angles set to predefined positions:");

    } 
    
    else if (command == "s") 
    {
      int servos[] = {a[0], a[2], a[4],a[6]};
      int angles[] = {90, 90, 90, 90};
      smoothMoveSimultaneously(servos, angles, 4);

      int servos2[] = {a[1], a[3], a[5], a[7]};
      int angles2[] = {90, 90, 90, 90};
      smoothMoveSimultaneously(servos2, angles2, 4);

      Serial.println("All servos set to 90 degrees");

    } 
    
    else if (command == "y") 
    {
      int servos1[] = {a[0],a[2]};
      int angles1[] = {170,10};
      smoothMoveSimultaneously(servos1, angles1, 2);
      smoothMove(a[1],180);
      smoothMove(a[1], 120);
      smoothMove(a[1], 180);
      smoothMove(a[1], 120);
      smoothMove(a[1], 180);
      smoothMove(a[1], 0);
      delay(100);

      int servos3[] = {a[0],a[2]};
      int angles3[] = {130,50};
      smoothMoveSimultaneously(servos3, angles3, 2);
      
      smoothMove(a[2], 50);

      Serial.println("Sequence y executed");

    }
  
    else if(command=="n")
    {
      int servos1[] = {a[0],a[2]};
      int angles1[] = {180,10};
      smoothMoveSimultaneously(servos1, angles1, 2);
      smoothMove(a[1],180);
      smoothMove(a[0],140 );
      delay(100);
      smoothMove(a[0], 170);
      delay(100);
      smoothMove(a[0],140 );
      delay(100);
      smoothMove(a[0], 170);
      smoothMove(a[1], 0);
      delay(100);

      int servos3[] = {a[0],a[2]};
      int angles3[] = {130,50};
      smoothMoveSimultaneously(servos3, angles3, 2);
      
      smoothMove(a[2], 50);

      Serial.println("Sequence n executed");

    }

    else if(command=="u")
    {
      int servos1[] = {a[0], a[2], a[4],a[6]};
      int angles1[] = {180,0,180,0};
      smoothMoveSimultaneously(servos1, angles1, 4);
      int servos3[] = {a[1], a[3], a[5], a[7]};
      int angles3[] = {0,180,90,90};
      smoothMoveSimultaneously(servos3, angles3, 4);
      Serial.println("Sequence u executed");

    }

    else if(command=="d")
    {
      int servos1[] = {a[0], a[2], a[4],a[6]};
      int angles1[] = {180,0,180,0};
      smoothMoveSimultaneously(servos1, angles1, 4);
      int servos3[] = {a[1], a[3], a[5], a[7]};
      int angles3[] = {90,90,0,180};
      smoothMoveSimultaneously(servos3, angles3, 4);
      Serial.println("Sequence u executed");

    }

    else if(command=="l")
    {
      int servos1[] = {a[3],a[7],a[2],a[6]};
      int angles1[] = {150,150,90,90};
      smoothMoveSimultaneously(servos1, angles1, 4);
      int angles1r[] = {170,170,90,90};
      smoothMoveSimultaneously(servos1, angles1r, 4);
      int servos2[] = {a[1],a[5],a[0],a[4]};
      int angles2[] = {30,30,170,170};
      smoothMoveSimultaneously(servos2, angles2, 4);
      int angles2r[] = {10,10,170,170};
      smoothMoveSimultaneously(servos2, angles2r, 4);
      
      int servos[] = {a[0], a[2], a[4], a[6]};
      int angles[] = {130, 50, 130, 50};
      smoothMoveSimultaneously(servos, angles, 4);

      int servos3[] = {a[1], a[3], a[5], a[7]};
      int angles3[] = {0, 180, 0, 180};
      smoothMoveSimultaneously(servos3, angles3, 4);
      Serial.println("Sequence l executed");

    }

    else if(command=="r")
    {
      int servos1[] = {a[3],a[7],a[2],a[6]};
      int angles1[] = {150,150,10,10};
      smoothMoveSimultaneously(servos1, angles1, 4);
      int angles1r[] = {170,170,10,10};
      smoothMoveSimultaneously(servos1, angles1r, 4);
      int servos2[] = {a[1],a[5],a[0],a[4]};
      int angles2[] = {30,30,90,90};
      smoothMoveSimultaneously(servos2, angles2, 4);
      int angles2r[] = {10,10,90,90};
      smoothMoveSimultaneously(servos2, angles2r, 4);
      
      int servos[] = {a[0], a[2], a[4], a[6]};
      int angles[] = {130, 50, 130, 50};
      smoothMoveSimultaneously(servos, angles, 4);

      int servos3[] = {a[1], a[3], a[5], a[7]};
      int angles3[] = {0, 180, 0, 180};
      smoothMoveSimultaneously(servos3, angles3, 4);
      Serial.println("Sequence l executed");

    }

    else if (command == "f") 
    {
      for(int i=0;i<2;i++)
      {
        {   
          int a1=map(30,0,100,90,0);
          int a3=map(30,0,100,90,180);
          int servos1[] = {a[1],a[3],a[5],a[7]};
          int angles1[] = {a1,a3,a1,a3};
          smoothMoveSimultaneously(servos1, angles1, 4);
        }

        int servosr[] = {a[4], a[6], a[0], a[2]};
        int anglesr[] = {120, 60, 120, 60};
        smoothMoveSimultaneously(servosr, anglesr, 4);
        // First set of movements
        smoothMove(a[1], 90);
        smoothMove(a[0], 140);
        smoothMove(a[1], 62);
        // Third set of movements
        smoothMove(a[7], 90);
        smoothMove(a[6], 80);
        smoothMove(a[7], 118);

        smoothMove(a[5], 90);
        smoothMove(a[4], 100);
        smoothMove(a[5], 62);

        // Fourth set of movements
        smoothMove(a[3], 90);
        smoothMove(a[2], 40);
        smoothMove(a[3], 118);
      {
        int a1=map(30,0,100,90,0);
        int a3=map(30,0,100,90,180);
        int servos1[] = {a[1],a[3],a[5],a[7]};
        int angles1[] = {a1,a3,a1,a3};
        smoothMoveSimultaneously(servos1, angles1, 4);
      } 
    }
}

else if (command == "b") 
{
  for(int i=0;i<2;i++)
  {
    {
        int a1=map(30,0,100,90,0);
        int a3=map(30,0,100,90,180);
        int servos1[] = {a[1],a[3],a[5],a[7]};
        int angles1[] = {a1,a3,a1,a3};
        smoothMoveSimultaneously(servos1, angles1, 4);
    }
    int servosr[] = {a[4], a[6], a[0], a[2]};
    int anglesr[] = {120, 60, 120, 60};
    smoothMoveSimultaneously(servosr, anglesr, 4);
    // First set of movements
    smoothMove(a[1], 90);
    smoothMove(a[0], 100);
    smoothMove(a[1], 62);

    smoothMove(a[7], 90);
    smoothMove(a[6], 40);
    smoothMove(a[7], 118);

    smoothMove(a[5], 90);
    smoothMove(a[4], 140);
    smoothMove(a[5], 62);

    smoothMove(a[3], 90);
    smoothMove(a[2], 80);
    smoothMove(a[3], 118);
    {
    int a1=map(30,0,100,90,0);
        int a3=map(30,0,100,90,180);
        int servos1[] = {a[1],a[3],a[5],a[7]};
        int angles1[] = {a1,a3,a1,a3};
        smoothMoveSimultaneously(servos1, angles1, 4);
    }
  }

}

else if(command=="wf")
{
  STEP_SIZE=2;
  for(int i=0;i<1;i++)
  {
    int servosr[] = {a[4], a[6], a[0], a[2]};
    int anglesr[] = {120, 60, 120, 60};
    smoothMoveSimultaneously(servosr, anglesr, 4);
    // First set of movements
    smoothMove(a[1], 30);
    smoothMove(a[0], 140);
    smoothMove(a[1], 10);

    smoothMove(a[5], 30);
    smoothMove(a[4], 100);
    smoothMove(a[5], 10);


    smoothMove(a[3], 150);
    smoothMove(a[2], 40);
    smoothMove(a[3], 170);

    smoothMove(a[7], 150);
    smoothMove(a[6], 80);
    smoothMove(a[7], 170);

     {
      int servos[] = {a[0], a[2], a[4],a[6]};
      int angles[] = {120, 60, 120, 60};
      smoothMoveSimultaneously(servos, angles, 4);

      int servos2[] = {a[1], a[3], a[5], a[7]};
      int angles2[] = {0, 180, 0, 180};
      smoothMoveSimultaneously(servos2, angles2, 4);
    } 
    smoothMoveSimultaneously(servosr, anglesr, 4);
    // First set of movements
    smoothMove(a[3], 150);
    smoothMove(a[2], 40);
    smoothMove(a[3], 170);

    smoothMove(a[7], 150);
    smoothMove(a[6], 80);
    smoothMove(a[7], 170);

    smoothMove(a[5], 30);
    smoothMove(a[4], 100);
    smoothMove(a[5], 10);

    smoothMove(a[1], 30);
    smoothMove(a[0], 140);
    smoothMove(a[1], 10);
     {
      int servos[] = {a[0], a[2], a[4],a[6]};
      int angles[] = {120, 60, 120, 60};
      smoothMoveSimultaneously(servos, angles, 4);

      int servos2[] = {a[1], a[3], a[5], a[7]};
      int angles2[] = {0, 180, 0, 180};
      smoothMoveSimultaneously(servos2, angles2, 4);
    } 
  }
}

else if(command=="stand")
{
    smoothMove(a[3], 150);
    smoothMove(a[2], 50);
    smoothMove(a[3], 170);

    smoothMove(a[7], 150);
    smoothMove(a[6], 140);
    smoothMove(a[7], 170);

    smoothMove(a[5], 30);
    smoothMove(a[4], 40);
    smoothMove(a[5], 10);

    smoothMove(a[1], 30);
    smoothMove(a[0], 130);
    smoothMove(a[1], 10);
     {
      int servos[] = {a[0], a[2], a[4],a[6]};
      int angles[] = {120, 60, 120, 60};
      smoothMoveSimultaneously(servos, angles, 4);

      int servos2[] = {a[1], a[3], a[5], a[7]};
      int angles2[] = {0, 180, 0, 180};
      smoothMoveSimultaneously(servos2, angles2, 4);
    } 
}
else if(command=="jump")
{
    STEP_SIZE=2;
    smoothMove(a[3], 150);
    smoothMove(a[2], 50);
    smoothMove(a[3], 170);

    smoothMove(a[7], 150);
    smoothMove(a[6], 140);
    smoothMove(a[7], 170);

    smoothMove(a[5], 30);
    smoothMove(a[4], 40);
    smoothMove(a[5], 10);

    smoothMove(a[1], 30);
    smoothMove(a[0], 130);
    smoothMove(a[1], 10);

    delay(2000);
    smoothMove(a[0], 170);
    smoothMove(a[2], 10);
    smoothMove(a[5], 40);
    smoothMove(a[7], 140);
    int servos[] = {a[4],a[6]};
    int angles[] = {180, 0};
    smoothMoveSimultaneously(servos, angles, 2);

     {
      int servos[] = {a[0], a[2], a[4],a[6]};
      int angles[] = {120, 60, 120, 60};
      smoothMoveSimultaneously(servos, angles, 4);

      int servos2[] = {a[1], a[3], a[5], a[7]};
      int angles2[] = {0, 180, 0, 180};
      smoothMoveSimultaneously(servos2, angles2, 4);
    } 
}

else if(command=="sit")
{
    smoothMove(a[0], 170);
    smoothMove(a[2], 10);
    smoothMove(a[5], 40);
    smoothMove(a[7], 140);
    int servos[] = {a[4],a[6]};
    int angles[] = {180, 0};
    smoothMoveSimultaneously(servos, angles, 2);

     {
      int servos[] = {a[0], a[2], a[4],a[6]};
      int angles[] = {120, 60, 120, 60};
      smoothMoveSimultaneously(servos, angles, 4);

      int servos2[] = {a[1], a[3], a[5], a[7]};
      int angles2[] = {0, 180, 0, 180};
      smoothMoveSimultaneously(servos2, angles2, 4);
    } 
}

else if(command=="wb")
{
  STEP_SIZE=2;
  for(int i=0;i<2;i++)
  {
    int servosr[] = {a[4], a[6], a[0], a[2]};
    int anglesr[] = {120, 60, 120, 60};
    smoothMoveSimultaneously(servosr, anglesr, 4);
    // First set of movements
    smoothMove(a[1], 30);
    smoothMove(a[0], 100);
    smoothMove(a[1], 10);
 
    smoothMove(a[5], 30);
    smoothMove(a[4], 140);
    smoothMove(a[5], 10);

    smoothMove(a[3], 150);
    smoothMove(a[2], 80);
    smoothMove(a[3], 170);

    smoothMove(a[7], 150);
    smoothMove(a[6], 40);
    smoothMove(a[7], 170);


    {
      int servos[] = {a[0], a[2], a[4],a[6]};
      int angles[] = {120, 60, 120, 60};
      smoothMoveSimultaneously(servos, angles, 4);

      int servos2[] = {a[1], a[3], a[5], a[7]};
      int angles2[] = {0, 180, 0, 180};
      smoothMoveSimultaneously(servos2, angles2, 4);
    } 
     smoothMoveSimultaneously(servosr, anglesr, 4);

    // First set of movements
    smoothMove(a[3], 150);
    smoothMove(a[2], 100);
    smoothMove(a[3], 170);

    smoothMove(a[7], 150);
    smoothMove(a[6], 40);
    smoothMove(a[7], 170);

     smoothMove(a[5], 30);
    smoothMove(a[4], 140);
    smoothMove(a[5], 10);

    smoothMove(a[1], 30);
    smoothMove(a[0], 80);
    smoothMove(a[1], 10);
    {
      int servos[] = {a[0], a[2], a[4],a[6]};
      int angles[] = {120, 60, 120, 60};
      smoothMoveSimultaneously(servos, angles, 4);

      int servos2[] = {a[1], a[3], a[5], a[7]};
      int angles2[] = {0, 180, 0, 180};
      smoothMoveSimultaneously(servos2, angles2, 4);
    } 
  }

}
else if(command=="dance")
{
  STEP_SIZE=2;
  int servos2[] = {a[1],a[3],a[5],a[7]};
  int angles2[] = {40, 140, 40, 140};
  smoothMoveSimultaneously(servos2, angles2, 4);

  for(int i=0;i<3;i++)
  {
    int servosr[] = {a[0], a[2], a[4], a[6]};
    int anglesr[] = {140, 80, 140, 80};
    smoothMoveSimultaneously(servosr, anglesr, 4);
    int angles[] = {120,60,120,60};
    smoothMoveSimultaneously(servosr, angles, 4);
    int anglesl[] = {100,40,100,40};
    smoothMoveSimultaneously(servosr, anglesl, 4);
    smoothMoveSimultaneously(servosr, angles, 4);
  }
  int angles2r[] = {0, 180, 0, 180};
  smoothMoveSimultaneously(servos2, angles2r, 4);
}
else if(command=="dance1")
{
      STEP_SIZE=2;
      int servos2[] = {a[1],a[3],a[5],a[7]};
      int angles2[] = {80, 100, 40, 140};
      smoothMoveSimultaneously(servos2, angles2, 4);

  for(int i=0;i<3;i++)
  {
    int servosr[] = {a[0], a[2], a[4], a[6]};
    int anglesr[] = {140, 80, 140, 80};
    smoothMoveSimultaneously(servosr, anglesr, 4);
    int angles[] = {120,60,120,60};
    smoothMoveSimultaneously(servosr, angles, 4);
    int anglesl[] = {100,40,100,40};
    smoothMoveSimultaneously(servosr, anglesl, 4);
    smoothMoveSimultaneously(servosr, angles, 4);
    
  }
  int angles2r[] = {0, 180, 0, 180};
      smoothMoveSimultaneously(servos2, angles2r, 4);
}
  if (latestCommand == "on") 
    digitalWrite(4, HIGH);  // Turn on LED (built-in LED is usually on GPIO 4)

  else if (latestCommand == "off") 
    digitalWrite(4, LOW);  // Turn off LED
  }
}

// Move multiple servos in parallel to target angles
void smoothMoveSimultaneously(int* servos, int* targetAngles, int numServos) 
{
  int maxSteps = 0;

  // Find the maximum number of steps needed for any servo
  for (int i = 0; i < numServos; i++) 
  {
    int steps = abs(currentAngles[servos[i]] - targetAngles[i]);
    if (steps > maxSteps) 
    {
      maxSteps = steps;
    }
  }

  // Move all servos step by step
  for (int step = 0; step <= maxSteps; step += STEP_SIZE) 
  {
    for (int i = 0; i < numServos; i++) 
    {
      if (step <= abs(currentAngles[servos[i]] - targetAngles[i])) 
      {
        pwm.setPWM(servos[i], 0, angleToPulse(currentAngles[servos[i]] + signum(targetAngles[i] - currentAngles[servos[i]]) * step, zeroError[servos[i]]));
      }
    }
    delay(dtime); // Adjust delay for speed
  }

  // Update the current angles
  for (int i = 0; i < numServos; i++) 
  {
    currentAngles[servos[i]] = targetAngles[i];
  }
}

// Move servo to the target angle in steps for smooth movement and update current angle
void smoothMove(int servoNum, int targetAngle) 
{
  int currentAngle = currentAngles[servoNum]; // Get current angle from stored array

  if (targetAngle > currentAngle) 
  {
    for (int pos = currentAngle; pos <= targetAngle; pos += STEP_SIZE) 
    {
      pwm.setPWM(servoNum, 0, angleToPulse(pos, zeroError[servoNum]));
      delay(dtime); // Adjust delay for speed
    }
  } else 
  {
    for (int pos = currentAngle; pos >= targetAngle; pos -= STEP_SIZE) 
    {
      pwm.setPWM(servoNum, 0, angleToPulse(pos, zeroError[servoNum]));
      delay(dtime); // Adjust delay for speed
    }
  }

  // Update the current angle in the array
  currentAngles[servoNum] = targetAngle;
}

// Convert angle (0-180 degrees) to pulse length (SERVOMIN to SERVOMAX) with zero error adjustment
int angleToPulse(int angle, int zeroError) 
{
  return map(angle + zeroError, 0, 180, SERVOMIN, SERVOMAX);
}

// Helper function to return the sign of a number
int signum(int value) 
{
  return (value > 0) - (value < 0);
}