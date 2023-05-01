/**
   @file main.cpp
   @author Marco Roobi (marco.roobi@h-brs.de)
   @brief Blink Example
          RAK4631
   @version 1.0
   @date 2023-03-01
**/

#define EXAMPLE_TEXT "Blink Example\n"

#include "main.h"

// variable to keep a timestamp
time_t timeout;
// actual state of one led
uint8_t led_state = HIGH;

volatile uint8_t pir_fired = false;

time_t pir_timeout;

// forward declarations (needed for platformIO)
void initLeds();
void initSerial();
void initPIR();
void intPIRCallback();

/**
 * @brief Application specific setup functions
 *
 */
void setup(void)
{
    initLeds();
    initSerial();
    pinMode(SOIL_PIN, INPUT);
    pinMode(PIR_PIN, INPUT_PULLUP);

    // ADD YOUR CODE HERE

    // Keep the actual timestamp for the loop
    timeout = millis();
    // Setup finished...
    MYLOG("SETUP", "Starting LOOP...");
}
void bodenFeutigikeit(){
uint16_t data = analogRead(SOIL_PIN);
    Serial.print("A1_Data:");
    Serial.print(data);
    Serial.print(",A1_Volt:");
    Serial.println(data * ( 3.3 / 1023.0 ));
    timeout = millis();

}
void bewegungsSensor(){
  Serial.print("Bewegung:");
  Serial.println(digitalRead(PIR_PIN));
}

void intPIRCallback()
{
  pir_fired = true;
}




/**
 * @brief Application loop
 *
 */
void loop(void)
{
    // Simple non-blocking loop
    if ((millis() - timeout) > LOOP_TIMEOUT)
    {
        bodenFeutigikeit();
        bewegungsSensor();
        digitalWrite(LED_BLUE,  led_state);
        digitalWrite(LED_GREEN,!led_state);
        led_state = !led_state;
        timeout = millis();
    }
}

void initLeds()
{
    // Set GPIOs for LED's as Output
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);

    // Set the GPIOs LOW (disable LEDs)
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BLUE, LOW);
}

void initSerial()
{
    // Initialize Serial for debug output
    digitalWrite(LED_GREEN, HIGH);
    Serial.begin(115200);

    // Initialize Serial for debug output
    timeout = millis();
    Serial.begin(115200);
    while (!Serial)
    {
        if ((millis() - timeout) < 5000)
        {
            delay(100);
        }
        else
        {
            break;
        }
    }
    // We should now have a serial interface...
    // Add a short delay 15s before executing code
    for(uint8_t i=0; i < 15;i++){
        delay(1000);
        Serial.print(".");
    }
    Serial.print("\n");
    // disable the green LED
    digitalWrite(LED_GREEN, LOW);
    MYLOG("SETUP", "Starting...");

    Serial.print("============================\n");
    Serial.print("Hochschule Bonn-Rhein-Sieg\n");
    Serial.print("Smart City: Sensorik und Netze\n");
    Serial.print(EXAMPLE_TEXT);
    Serial.printf("SW Version %d.%d.%d\n", SW_VERSION_1, SW_VERSION_2, SW_VERSION_3);
    Serial.print("============================\n");

    
}