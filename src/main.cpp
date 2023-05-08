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
#include "SparkFun_u-blox_GNSS_Arduino_Library.h"
SFE_UBLOX_GNSS g_myGNSS;
#define SERIAL_PC     Serial
#define BAUDRATE_PC   115200

#define SERIAL_GNSS   Serial1
#define BAUDRATE_GNSS 9600




// variable to keep a timestamp
time_t timeout;
// actual state of one led
uint8_t led_state = HIGH;

volatile uint8_t pir_fired = false;

time_t pir_timeout;
long g_lastTime = 0; //Simple local timer. Limits amount if I2C traffic to u-blox module.
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
    Serial.println("blubl00");
    initGNSS();

    pinMode(SOIL_PIN, INPUT);
    pinMode(PIR_PIN, INPUT_PULLUP);
    pinMode(LIGHTSENSOR_INT, INPUT);
     Wire.begin();
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
   
}

void initGNSS()
{
    Serial.println("blubl01");
    // Activate 3V power
    pinMode(WB_IO2, OUTPUT);
    digitalWrite(WB_IO2, 0);
    Serial.println("WB_IO01");
    delay(1000);
    digitalWrite(WB_IO2, 1);
    Serial.println("WB_IO02");
    delay(1000);
    Serial.println("WB_IO00");


    Serial.println("Configure GPS...");
    // activate the Serial1 interface
   // SERIAL_GNSS.begin(BAUDRATE_GNSS);
    //digitalWrite(LED_GREEN, HIGH);
    //delay(100);
    //digitalWrite(LED_GREEN, LOW);
    //SERIAL_PC.println("Bridge:");
    if (g_myGNSS.begin() == false) //Connect to the u-blox module using Wire port
  {
    Serial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
    while (1);
  }

  g_myGNSS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
  g_myGNSS.saveConfigSelective(VAL_CFG_SUBSEC_IOPORT); //Save (only) the communications port settings to flash and BBR
}
void gNSS(){ 
   
    g_lastTime = millis(); //Update the timer
    long latitude = g_myGNSS.getLatitude();
    Serial.print(F("Lat: "));
    Serial.print(latitude);

    long longitude = g_myGNSS.getLongitude();
    Serial.print(F(" Long: "));
    Serial.print(longitude);
    Serial.print(F(" (degrees * 10^-7)"));

    long altitude = g_myGNSS.getAltitude();
    Serial.print(F(" Alt: "));
    Serial.print(altitude);
    Serial.print(F(" (mm)"));
    
    long speed = g_myGNSS.getGroundSpeed();
    Serial.print(F(" Speed: "));
    Serial.print(speed);
    Serial.print(F(" (mm/s)"));

    long heading = g_myGNSS.getHeading();
    Serial.print(F(" Heading: "));
    Serial.print(heading);
    Serial.print(F(" (degrees * 10^-5)"));

    byte SIV = g_myGNSS.getSIV();
    Serial.print(F(" SIV: "));
    Serial.println(SIV);
   
    }
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
        lichtsensor();
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
