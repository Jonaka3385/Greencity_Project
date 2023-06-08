/**
   @file main.h
   @author Julian Schaa
   @brief Header File
          RAK4631
   @version 1.1
   @date 2023-06-09
 **/

#ifndef MAIN_H
#define MAIN_H

#ifndef MY_DEBUG
  #define MY_DEBUG 1 // to disable debug, set this to 0 (in PlatformIO this is done automatically, depending on the build target)
#endif

//***********************************************/
//** Set the application firmware version here  */
//** (in PlatformIO this is done automatically) */
//***********************************************/
// ; major version increase on API change / not backwards compatible
#ifndef SW_VERSION_1
  #define SW_VERSION_1 1
#endif
// ; minor version increase on API change / backward compatible
#ifndef SW_VERSION_2
  #define SW_VERSION_2 0
#endif
// ; patch version increase on bugfix, no affect on API
#ifndef SW_VERSION_3
  #define SW_VERSION_3 0
#endif

/** Include library's */
#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
/** Include TwoWire and SPI Interface */
#include <SPI.h>
#include <Wire.h>
/** Include the SX126x-API */
#include <SX126x-Arduino.h>
#include <LoRaWan-RAK4630.h> //http://librarymanager/All#SX126x
/** Add your required includes after SX126x-Arduino.h */

/** LoRaWAN forward declarations and settings */
extern void initLoRaWAN();
extern void send_lora_frame(void);
extern bool lorawanJoined;
extern uint16_t bodenFeutigikeit();

#if MY_DEBUG > 0
    #define MYLOG(tag, ...)            \
        do                             \
        {                              \
            if (tag)                   \
                PRINTF("[%s] ", tag);  \
            PRINTF(__VA_ARGS__);       \
            PRINTF("\n");              \
        } while (0)
#else
    #define MYLOG(...)
#endif

// ADD YOUR CODE HERE
#define LOOP_TIMEOUT 1000
#define LORA_TIMEOUT 60000 //60 s (sende abstand )
#define SOIL_PIN WB_A1
#define PIR_PIN WB_IO1

#endif  // MAIN_H

