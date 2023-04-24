/**
   @file main.cpp
   @author Marco Roobi (marco.roobi@h-brs.de)
   @brief Simple Blink Example
          RAK4631
   @version 1.0
   @date 2023-03-01
**/

#include <Arduino.h>
#include <Adafruit_TinyUSB.h>

uint8_t ledState = HIGH;

void setup() {
  // Set LED GPIOs as Output
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  // Set LEDs OFF
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  digitalWrite(LED_GREEN, ledState);
  digitalWrite(LED_BLUE, !ledState);
  ledState = !ledState;
}
