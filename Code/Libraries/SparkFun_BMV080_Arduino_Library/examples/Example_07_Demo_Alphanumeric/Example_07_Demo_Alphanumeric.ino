/*
  Using the BMV080 Particulate Matter PM2.5 Sensor

  This example shows how display the Pm2.5 readings on a SparkFun Qwiic 
  Alphanumeric display.
  
  It uses the sensor in "continuous mode" to get
  particulate matter readings once every second.

  It uses polling of the device to check if new data is available.

  By: Pete Lewis
  SparkFun Electronics
  Date: September, 2024
  SparkFun code, firmware, and software is released under the MIT License.
    Please see LICENSE.md for further details.

  Hardware Connections:
  IoT RedBoard --> BMV080
  QWIIC --> QWIIC

  BMV080 "mode" jumper set to I2C (default)

  Serial.print it out at 115200 baud to serial monitor.

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/26554
*/

#include "SparkFun_BMV080_Arduino_Library.h" // CTRL+Click here to get the library: http://librarymanager/All#SparkFun_BMV080
#include <Wire.h>

SparkFunBMV080 bmv080;           // Create an instance of the BMV080 class
#define BMV080_ADDR 0x57 // SparkFun BMV080 Breakout defaults to 0x57

#include <SparkFun_Alphanumeric_Display.h> //Click here to get the library: http://librarymanager/All#SparkFun_Qwiic_Alphanumeric_Display by SparkFun
HT16K33 display;
#define DISPLAY_ADDRESS 0x70 // Default I2C address when A0, A1 are floating

// Some Dev boards have their QWIIC connector on Wire or Wire1
// This #ifdef will help this sketch work across more products

#ifdef ARDUINO_SPARKFUN_THINGPLUS_RP2040
#define wirePort   Wire1
#else
#define wirePort  Wire
#endif

void setup()
{
    Serial.begin(115200);

    while (!Serial)
        delay(10); // Wait for Serial to become available.
    // Necessary for boards with native USB (like the SAMD51 Thing+).
    // For a final version of a project that does not need serial debug (or a USB cable plugged in),
    // Comment out this while loop, or it will prevent the remaining code from running.

    Serial.println();
    Serial.println("BMV080 Example 7 - Alphanumeric Display");

    wirePort.begin();

    if (bmv080.begin(BMV080_ADDR, wirePort) == false)
    {
        Serial.println(
            "BMV080 not detected at default I2C address. Check your jumpers and the hookup guide. Freezing...");
        while (1)
            ;
    }
    Serial.println("BMV080 found!");

    // wirePort.setClock(400000); //Increase I2C data rate to 400kHz

    /* Initialize the Sensor (read driver, open, reset, id etc.)*/
    bmv080.init();

    /* Set the sensor mode to continuous mode */
    if (bmv080.setMode(SF_BMV080_MODE_CONTINUOUS) == true)
    {
        Serial.println("BMV080 set to continuous mode");
    }
    else
    {
        Serial.println("Error setting BMV080 mode");
    }

    if (display.begin(DISPLAY_ADDRESS, DEFAULT_NOTHING_ATTACHED, DEFAULT_NOTHING_ATTACHED, DEFAULT_NOTHING_ATTACHED, wirePort) == false)
    {
        Serial.println("Qwiic Alphanumeric Device did not acknowledge! Freezing.");
        while (1);
    }
    Serial.println("Qwiic Alphanumeric Display acknowledged.");

    display.setBrightness(5); // Set brightness to 5/16 full brightness

    display.print("PM2.5");

}

void loop()
{
    if (bmv080.readSensor())
    {
        float pm25 = bmv080.PM25();

        Serial.print(pm25);
        display.print(int(pm25));

        if (bmv080.isObstructed() == true)
        {
            Serial.print("\tObstructed");
            display.print("Obst");
        }

        Serial.println();
    }
    delay(100);
}
