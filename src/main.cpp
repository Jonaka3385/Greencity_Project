/**
   @file main.cpp
   @author Marco Roobi (marco.roobi@h-brs.de) and group 6
   @brief Main
          RAK4631
   @version 1.0
   @date 2023-22-05
**/

#define EXAMPLE_TEXT "Main\n"

#include "main.h"
#include "SparkFun_u-blox_GNSS_Arduino_Library.h" //GPS
#include <Adafruit_BME680.h> //Environmentsensor
#include <ClosedCube_OPT3001.h> //Lichtsensor
#include <U8g2lib.h> //Bildschirm

#define SEALEVELPRESSURE_HPA (1013.25)

#define OPT3001_ADDRESS 0x44


ClosedCube_OPT3001 g_opt3001;

Adafruit_BME680 bme; // I2C
SFE_UBLOX_GNSS g_myGNSS;
// variable to keep a timestamp
time_t timeout;
// actual state of one led
uint8_t led_state = HIGH;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);

long g_lastTime = 0; //Simple local timer. Limits amount if I2C traffic to u-blox module.
// forward declarations (needed for platformIO)

void initLeds();

void initSerial();

void initPIR();

void initGNSS();

void initRAK1906();

void initLichtsensor();

void initBildschirm();

static const unsigned char PROGMEM
bitmap[] =
{
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0xC0, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xFF, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x7F,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0xFC, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0xFE, 0x00, 0x00, 0xFE, 0xFF,
0x03, 0x80, 0x1F, 0x80, 0x0F, 0xE0, 0x07, 0x00, 0x00, 0x00, 0x3E, 0xFF,
0x00, 0x00, 0xFC, 0xFF, 0x07, 0xC0, 0x1F, 0x80, 0x0F, 0xF0, 0x03, 0x00,
0x00, 0x00, 0xDF, 0xFF, 0x06, 0x00, 0xFC, 0xFF, 0x0F, 0xC0, 0x3F, 0x80,
0x0F, 0xF8, 0x01, 0x00, 0x00, 0x00, 0xCE, 0xC7, 0x0E, 0x00, 0x7C, 0xE0,
0x0F, 0xC0, 0x3F, 0x80, 0x0F, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x3E, 0x00, 0x3C, 0x80, 0x1F, 0xE0, 0x3F, 0x80, 0x0F, 0x7E, 0x00, 0x00,
0x00, 0xCE, 0x7B, 0xFF, 0x7E, 0x00, 0x3C, 0x00, 0x1F, 0xE0, 0x7F, 0x80,
0x0F, 0x3F, 0x00, 0x00, 0x00, 0xDE, 0x7B, 0xFF, 0xFC, 0x00, 0x3C, 0x00,
0x1F, 0xE0, 0x7D, 0x80, 0x8F, 0x1F, 0x00, 0x00, 0x00, 0xDE, 0x7D, 0xFF,
0xF8, 0x00, 0x3C, 0x00, 0x1F, 0xF0, 0x79, 0x80, 0xCF, 0x0F, 0x00, 0x00,
0x00, 0xDE, 0x7D, 0xFF, 0xF6, 0x01, 0x3C, 0x00, 0x1F, 0xF0, 0xF8, 0x80,
0xEF, 0x07, 0x00, 0x00, 0x00, 0xDE, 0x7D, 0x00, 0xE6, 0x01, 0x3C, 0x80,
0x0F, 0xF0, 0xF8, 0x80, 0xFF, 0x07, 0x00, 0x00, 0x00, 0xDE, 0x7B, 0xF0,
0xEE, 0x03, 0x7C, 0xE0, 0x0F, 0xF8, 0xF0, 0x80, 0xFF, 0x07, 0x00, 0x00,
0x00, 0xDE, 0x7B, 0xF0, 0xCE, 0x03, 0xFC, 0xFF, 0x03, 0x78, 0xF0, 0x81,
0xFF, 0x0F, 0x00, 0x00, 0x00, 0xBE, 0x7B, 0xF0, 0xDE, 0x03, 0xFC, 0xFF,
0x03, 0x7C, 0xF0, 0x81, 0xFF, 0x1F, 0x00, 0x00, 0x00, 0xBE, 0x03, 0xF0,
0xDE, 0x03, 0xFC, 0xFF, 0x07, 0x7C, 0xE0, 0x83, 0x3F, 0x1F, 0x00, 0x00,
0x00, 0x7C, 0xFB, 0xF7, 0xDE, 0x03, 0x3C, 0xC0, 0x0F, 0xFC, 0xFF, 0x83,
0x1F, 0x3F, 0x00, 0x00, 0x00, 0x78, 0xFA, 0xF7, 0xDE, 0x03, 0x3C, 0x80,
0x0F, 0xFE, 0xFF, 0x83, 0x0F, 0x7E, 0x00, 0x00, 0x00, 0xF8, 0xFC, 0xF7,
0xDE, 0x03, 0x3C, 0x00, 0x1F, 0xFE, 0xFF, 0x87, 0x0F, 0xFC, 0x00, 0x00,
0x00, 0xF0, 0xF9, 0xF7, 0xDE, 0x03, 0x3C, 0x00, 0x1F, 0xFF, 0xFF, 0x87,
0x0F, 0xFC, 0x00, 0x00, 0x00, 0xE0, 0x03, 0x00, 0x0C, 0x00, 0x3C, 0x00,
0x1F, 0x1F, 0xC0, 0x87, 0x0F, 0xF8, 0x01, 0x00, 0x00, 0xC0, 0x0B, 0x1E,
0x03, 0x00, 0x3C, 0x00, 0x1F, 0x1F, 0x80, 0x8F, 0x0F, 0xF0, 0x03, 0x00,
0x00, 0x00, 0xFD, 0xDF, 0x07, 0x00, 0x3C, 0x00, 0x9F, 0x0F, 0x80, 0x8F,
0x0F, 0xF0, 0x03, 0x00, 0x00, 0x00, 0xFC, 0xEF, 0x03, 0x00, 0x3C, 0x00,
0x9F, 0x0F, 0x80, 0x9F, 0x0F, 0xE0, 0x07, 0x00, 0x00, 0x00, 0xF8, 0xF3,
0x03, 0x00, 0x3C, 0x00, 0x9F, 0x07, 0x00, 0x9F, 0x0F, 0xC0, 0x0F, 0x00,
0x00, 0x00, 0x00, 0xFC, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xFF, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x7F,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0xF8, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x07, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00,
};

/**
 * @brief Application specific setup functions
 *
 */
void setup(void) {
    initLeds();
    initSerial();
    Wire.begin();
    initGNSS();
    initRAK1906();
    initLichtsensor();

    u8g2.begin();
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.drawXBM(0, 0, 128, 64, bitmap);
    u8g2.sendBuffer();
    delay(1000);
    pinMode(SOIL_PIN, INPUT);
    pinMode(PIR_PIN, INPUT_PULLUP);
    pinMode(LIGHTSENSOR_INT, INPUT);

    // Keep the actual timestamp for the loop
    timeout = millis();
    // Setup finished...
    MYLOG("SETUP", "Starting LOOP...");
}

void initBildschirm() {
    u8g2.begin();
}

void bildschirm() {
    //u8g2.begin();
    char data[32] = {0};
    // display OPT3001 sensor data on OLED
    u8g2.clearBuffer();         // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB10_tr); // choose a suitable font

    // memset(data, 0, sizeof(data));
    sprintf(data, "Hallo Welt");
    u8g2.drawStr(15, 15, data);    // choose a suitable font
    // u8g2.drawStr(0,10,"Hello World!");	// write something to the internal memory
    u8g2.sendBuffer();// transfer internal memory to the display

    delay(1000);
    u8g2.clearBuffer();
    u8g2.drawStr(0, 15, "Hello World 123!");
    u8g2.drawStr(0, 30, "Hello World 123!");
    u8g2.drawStr(0, 45, "Hello World 123!");
    u8g2.drawStr(0, 60, "Hello World 123!");    // write something to the internal memory
    u8g2.sendBuffer();                    // transfer internal memory to the display

    // Create a sceenshot. The picture (XBM or PBM format) is sent to the serial output.
    // Copy and paste the output from the Arduino serial monitor into a file.
    // Depending on the display controller use u8g2.writeBufferXBM() or u8g2.writeBufferXBM2()
    // For SH1122, LD7032, ST7920, ST7986, LC7981, T6963, SED1330, RA8835, MAX7219, LS0?
    // use u8g2.writeBufferXBM2(), for all others use u8g2.writeBufferXBM()
    u8g2.writeBufferXBM(Serial);            // Write XBM image to serial out

    delay(1000);
}

void initRAK1906() {
    while (!Serial);
    Serial.println(F("BME680 async test"));

    if (!bme.begin(0x76)) {
        Serial.println(F("Could not find a valid BME680 sensor, check wiring!"));
    }
    bme.setTemperatureOversampling(BME680_OS_8X);
    bme.setHumidityOversampling(BME680_OS_2X);
    bme.setPressureOversampling(BME680_OS_4X);
    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme.setGasHeater(320, 150); // 320*C for 150 ms
}

void RAK1906() {
    unsigned long endTime = bme.beginReading();
    if (endTime == 0) {
        Serial.println(F("Failed to begin reading :("));
        return;
    }

    Serial.print(F("Reading started at "));
    Serial.print(millis());
    Serial.print(F(" and will finish at "));
    Serial.println(endTime);

    if (!bme.endReading()) {
        Serial.println(F("Failed to complete reading :("));
        return;
    }

    Serial.print(F("Reading completed at "));
    Serial.println(millis());

    Serial.print(F("Temperature = "));
    Serial.print(bme.temperature);
    Serial.println(F(" *C"));

    Serial.print(F("Pressure = "));
    Serial.print(bme.pressure / 100.0);
    Serial.println(F(" hPa"));

    Serial.print(F("Humidity = "));
    Serial.print(bme.humidity);
    Serial.println(F(" %"));

    Serial.print(F("Gas = "));
    Serial.print(bme.gas_resistance / 1000.0);
    Serial.println(F(" KOhms"));

    Serial.print(F("Approx. Altitude = "));
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(F(" m"));
    Serial.println();
    delay(2000);
}

void bodenFeutigikeit() {
    uint16_t data = analogRead(SOIL_PIN);
    Serial.print("A1_Data:");
    Serial.print(data);
    Serial.print(",A1_Volt:");
    Serial.println(data * (3.3 / 1023.0));
    timeout = millis();
}

void bewegungsSensor() {
    Serial.print("Bewegung:");
    Serial.println(digitalRead(PIR_PIN));
}

void initLichtsensor() {
    g_opt3001.begin(OPT3001_ADDRESS);

    OPT3001_Config newConfig;

    newConfig.RangeNumber = B1100;
    newConfig.ConvertionTime = B0;
    newConfig.Latch = B1;
    newConfig.ModeOfConversionOperation = B11;

    OPT3001_ErrorCode errorConfig = g_opt3001.writeConfig(newConfig);
}

void lichtsensor() {
    OPT3001 result = g_opt3001.readResult();
    Serial.print("OPT3001");
    Serial.print(": ");
    Serial.print(result.lux);
    Serial.println(" lux");
    // display OPT3001 sensor data on OLED
    /** u8g2.clearBuffer();         // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB10_tr); // choose a suitable font

    memset(data, 0, sizeof(data));
    sprintf(data, "LUX METER");
    u8g2.drawStr(15, 15, data);

    memset(data, 0, sizeof(data));
    sprintf(data, "LI = %.2f lux", result.lux);
    u8g2.drawStr(12, 45, data);
    u8g2.sendBuffer(); // transfer internal memory to the display
    **/

    Serial.print(result.lux);
    Serial.println(" lux");
}

void initGNSS() {
    // Activate 3V power
    pinMode(WB_IO2, OUTPUT);
    digitalWrite(WB_IO2, 0);
    delay(1000);
    digitalWrite(WB_IO2, 1);
    delay(1000);

    Serial.println("Configure GPS...");
    if (g_myGNSS.begin() == false) { //Connect to the u-blox module using Wire port
        Serial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
        while (1);
    }
    g_myGNSS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
    g_myGNSS.saveConfigSelective(VAL_CFG_SUBSEC_IOPORT); //Save (only) the communications port settings to flash and BBR
}

void gNSS() {
    Serial.print("GNSS:");
    Serial.print(g_myGNSS.getFixType());
    g_lastTime = millis(); //Update the timer
    Serial.print(F("Lat: "));
    Serial.print(g_myGNSS.getLatitude());

    Serial.print(F(" Long: "));
    Serial.print(g_myGNSS.getLongitude());
    Serial.print(F(" (degrees * 10^-7)"));

    Serial.print(F(" Alt: "));
    Serial.print(g_myGNSS.getAltitude());
    Serial.print(F(" (mm)"));

    Serial.print(F(" Speed: "));
    Serial.print(g_myGNSS.getGroundSpeed());

    Serial.print(F(" (mm/s)"));
    Serial.print(F(" Heading: "));
    Serial.print(g_myGNSS.getHeading());
    Serial.print(F(" (degrees * 10^-5)"));

    Serial.print(F(" SIV: "));
    Serial.println(g_myGNSS.getSIV());
}

/**
 * @brief Application loop
 *
 */
void loop(void) {
    // Simple non-blocking loop
    if ((millis() - timeout) > LOOP_TIMEOUT) {
        bodenFeutigikeit();
        bewegungsSensor();
        RAK1906();
        bildschirm();
        lichtsensor();
        //gNSS();
        digitalWrite(LED_BLUE, led_state);
        digitalWrite(LED_GREEN, !led_state);
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
    while (!Serial) {
        if ((millis() - timeout) < 5000) {
            delay(100);
        } else {
            break;
        }
    }
    // We should now have a serial interface...
    // Add a short delay 15s before executing code
    for (uint8_t i = 0; i < 15; i++) {
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
