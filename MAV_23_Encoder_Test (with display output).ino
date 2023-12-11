/*
Test Script for MAV_23 Rotary Encoder
Programmed by Mohammad Mohammad
Started on December 6, 2023
*/

// Libraries for operating display
#include <SPI.h>
#include <Wire.h> 
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Height and width of the display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define encoderPin 2 // Assigns IR sensor pin to pin 2

const int slotsPerRotation = 50; // Amount of open slots that are on the encoder disk. With the sensor, after it detects 50 high signals, it can be known that one full revolution has been completed. 

int wheelRPM = 0; // Variable for wheel RPM

float speed = 0; // Variable for speed

const float wheelCircumference = 0.00153; // This is the circumference of the current drive wheel in which this encoder attaches to, in kilometres

volatile int rotationCounter = 0; // Used whenever a change in rotation is detected

int timeDifference = 0; // Integer used to calculate the time difference, allowing for the calculation of RPM

void setup() {
  Serial.begin(9600); // Opens up the serial monitor for output
  
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) // Checks if the display has booted up correctly
  { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  // Tests display
  display.display();
  delay(2000);
  
  display.clearDisplay();
  
  display.setTextSize(1); 
  display.setCursor(10, 0);
  display.setTextColor(SSD1306_WHITE);
  display.println(F("Tachometer Test"));
  display.display();
  delay(2000);

  pinMode(encoderPin, INPUT); // Sets up the sensor pin as an input device 
  
  attachInterrupt(digitalPinToInterrupt(encoderPin), countRotations, RISING ); // This program runs using ISR, hence this function. The reasoning behind this is that after research, using an ISR is quicker and more efficient in terms of processing resources, and is good to use when projects such as this rotary encoder, which rely heavily on gaps between timings. 
  // Note that the RISING parameter in this interrupt above is used for when the interrupt pin goes from low to high, which is what this encoder disk is doing, as it constantly switches logic states. 
}

void loop() {
  unsigned int initialTime = millis(); // The millis() function assigned to this variable enables this program to calculate the difference in time to update the program every half second
  
  if ((initialTime - timeDifference) >= 1000) // Polls every second
  {
    speed = (wheelRPM * wheelCircumference) * 60; // Updates speed by multiplying the circumference by the RPM, then multiplying by 60 minutes to get units of km/h
    
    // Outputs speed and RPM on serial monitor
    Serial.print("Speed: ");
    Serial.print(speed);
    Serial.println("km/h");
    
    wheelRPM = (rotationCounter * 60) / slotsPerRotation; // Calculates wheel RPM
    
    // Outputs the wheel RPM on display and serial monitor
    Serial.print("Wheel RPM: ");
    Serial.println(wheelRPM);
    
    display.clearDisplay(); // Clears the display so text can be updated
    
    // Outputs speed and RPM onto a physical display
    display.setTextSize(1);
    display.setCursor(5, 0);
    display.print("Speed: ");
    display.print(speed);
    display.println("km/h");
    
    display.setCursor(5, 20);
    display.print("Wheel RPM: ");
    display.print(wheelRPM);
    display.println("RPM");
    display.display(); // Outputs onto the display
    
    rotationCounter = 0;
    
    timeDifference = initialTime; // Updates the time differences 
  }
}

void countRotations() // This function is used in the interrupt routine whenever it detects a change in the high-low logic state of the IR sensor
{
  rotationCounter++;
}