/**
   @file main.cpp
   @author Marco Roobi (marco.roobi@h-brs.de) and group 6
   @brief Main
          RAK4631
   @version 1.0
   @date 2023-03-01
**/

#define EXAMPLE_TEXT "Main\n"

#include "main.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_TSL2561_U.h"
#define SERIAL_PC     Serial
#define BAUDRATE_PC   115200

#define SERIAL_GNSS   Serial1
#define BAUDRATE_GNSS 9600


Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(LIGHTSENSOR_INT);

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
void initGNSS();


/**
 * @brief Application specific setup functions
 *
 */
void setup(void) {
    initLeds();
    initSerial();
    initGNSS();

    pinMode(SOIL_PIN, INPUT);
    pinMode(PIR_PIN, INPUT_PULLUP);
    pinMode(LIGHTSENSOR_INT, INPUT);

    // Keep the actual timestamp for the loop
    timeout = millis();
    // Setup finished...
    MYLOG("SETUP", "Starting LOOP...");
}

void bodenFeutigikeit() {
    uint16_t data = analogRead(SOIL_PIN);
    Serial.print("A1_Data:");
    Serial.print(data);
    Serial.print(",A1_Volt:");
    Serial.println(data * ( 3.3 / 1023.0 ));
    timeout = millis();
}

void bewegungsSensor() {
  Serial.print("Bewegung:");
  Serial.println(digitalRead(PIR_PIN));
}


void lichtsensor() {
    uint16_t broadband, ir;
    tsl.getLuminosity(&broadband, &ir);
    Serial.print("Broadband: ");
    Serial.print(broadband);
    Serial.print(" IR: ");
    Serial.print(ir);
    Serial.print(" Lux: ");
    Serial.println(tsl.calculateLux(broadband, ir));
}
void initGNSS()
{
    // Activate 3V power
    pinMode(WB_IO2, OUTPUT);
    digitalWrite(WB_IO2, 0);
    delay(1000);
    digitalWrite(WB_IO2, 1);
    delay(1000);

    SERIAL_PC.println("Configure GPS...");
    // activate the Serial1 interface
    SERIAL_GNSS.begin(BAUDRATE_GNSS);
    digitalWrite(LED_GREEN, HIGH);
    delay(100);
    digitalWrite(LED_GREEN, LOW);
    SERIAL_PC.println("Bridge:");
}
void gNSS(){ 
    if (SERIAL_GNSS.available())
    {
        SERIAL_PC.write(SERIAL_GNSS.read());
    }

    if (SERIAL_PC.available())
    {
        SERIAL_GNSS.write(SERIAL_PC.read());
    }}
/**
 * @brief Application loop
 *
 */
void loop(void) {
    // Simple non-blocking loop
    if ((millis() - timeout) > LOOP_TIMEOUT)
    {
        bodenFeutigikeit();
        bewegungsSensor();
        //lichtsensor();
        gNSS();
        digitalWrite(LED_BLUE,  led_state);
        digitalWrite(LED_GREEN,!led_state);
        led_state = !led_state;
        timeout = millis();
        
    }

    
}

void initLeds() {
    // Set GPIOs for LED's as Output
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);

    // Set the GPIOs LOW (disable LEDs)
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BLUE, LOW);
}

void initSerial() {
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
