/**
   @file main.cpp
   @author Julian Schaa 
   @brief LoRaWan node example with OTAA registration
          RAK4631
   @version 1.1
   @date 2023-06-08
**/

#define EXAMPLE_TEXT "LoRaWan node example with OTAA registration\n"

#include "main.h"

 
// variable to keep a timestamp
time_t timeout;
time_t timeout_lora;
// actual state of one led
uint8_t led_state = HIGH;

// forward declarations (needed for platformIO)
void initLeds();
void initSerial();

/**
 * @brief Application specific setup functions
 *
 */
void setup(void)
{
    initLeds();
    initSerial();
    initLoRaWAN();
    // Set AnalogIn 1 as Input

    // Keep the actual timestamp for the loop
    timeout = millis();
    timeout_lora = millis();
    // Setup finished...
    MYLOG("SETUP", "Starting LOOP...");
}
/**
 * @brief Bodenfeuchtigkeits messung 
 *
 */
    float bodenFeutigikeit() {
        //Read data 
    float data = analogRead(SOIL_PIN);
    //(Maximalwert - Datenwert) / (Maximalwert - Minimalwert)) * 100
    float data_prozentual = (((1023 - data) / 1023 ) *100) ;

    //print data
    Serial.print("Analoger Wert:");
    Serial.println(data);
    Serial.print("Boden Feuchtigkeit:");
    Serial.print(data_prozentual);
    
    Serial.println("%");
    //Serial.print(",A1_Volt:");
    //Serial.println(data * (3.3 / 1023.0));
    //print modify Data between 0 and 255 
    //Serial.println ((data-340)/2);
    timeout = millis();
    //2.50 troken 825
    //1.10 sehr nass 345
    //(data-300)/2 

    //return modify Data 
    return (data_prozentual);
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
        //bodenFeutigikeit();
        digitalWrite(LED_BLUE,  led_state);
        digitalWrite(LED_GREEN,!led_state);
        led_state = !led_state;
        timeout = millis();
    }

    if ((millis() - timeout_lora) > LORA_TIMEOUT)
    {
        if (lorawanJoined)
        {
            send_lora_frame();
        } else {
            Serial.println("LoRaWAN not joined!");
        }
        timeout_lora = millis();
    }
}

/**
 * @brief LED setup function
 *
 */
void initLeds()
{
    // Set GPIOs for LED's as Output
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);

    // Set the GPIOs LOW (disable LEDs)
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BLUE, LOW);
}

/**
 * @brief Serial setup function
 *
 */
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
    for(uint8_t i=0; i < 15;i++)
    {
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


