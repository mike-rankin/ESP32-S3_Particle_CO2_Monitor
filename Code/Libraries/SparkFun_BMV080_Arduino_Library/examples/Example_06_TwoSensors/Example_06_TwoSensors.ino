/*
  Using the BMV080 Particulate Matter PM2.5 Sensor

  This example shows how to use two BMV080 sensors on the same I2C bus.
  
  One sensor must have its AB0 Jumper changed to "0".
  
  The sensors will be in "continuous mode" to get
  particulate matter readings once every second.

  It uses polling of the devices to check if new data is available.

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

  Open a plotter to see the PM2.5 values from both sensors.

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/26554
*/

#include "SparkFun_BMV080_Arduino_Library.h" // CTRL+Click here to get the library: http://librarymanager/All#SparkFun_BMV080
#include <Wire.h>

SparkFunBMV080 bmv080;           // Create an instance of the BMV080 class
#define BMV080_ADDR 0x57 // SparkFun BMV080 Breakout defaults to 0x57

SparkFunBMV080 bmv080_2;           // Create an instance of the BMV080 class
#define BMV080_ADDR2 0x56 // AB0 Jumper set to 0

bool newDataAvailable = false; // Flag to indicate new data is available
bool newDataAvailable2 = false; // Flag to indicate new data is available

float pm25 = 0.0; // Variable to store PM2.5 value
float pm25_2 = 0.0; // Variable to store PM2.5 value

bool isObstructed = false; // Flag to indicate sensor is obstructed
bool isObstructed2 = false; // Flag to indicate sensor is obstructed

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
    Serial.println("BMV080 Example 6 - Two Sensors");

    wirePort.begin();

    if (bmv080.begin(BMV080_ADDR, wirePort) == false)
    {
        Serial.println(
            "BMV080 not detected at default I2C address. Check your jumpers and the hookup guide. Freezing...");
        while (1)
            ;
    }
    Serial.println("BMV080 at 0x57 found!");

    if (bmv080_2.begin(BMV080_ADDR2, wirePort) == false)
    {
        Serial.println(
            "BMV080 not detected at 0x56 I2C address. Check your jumpers and the hookup guide. Freezing...");
        while (1)
            ;
    }
    Serial.println("BMV080 at 0x56 found!");    

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

    delay(500);

    bmv080_2.init();

    if (bmv080_2.setMode(SF_BMV080_MODE_CONTINUOUS) == true)
    {
        Serial.println("BMV080_2 set to continuous mode");
    }
    else
    {
        Serial.println("Error setting BMV080_2 mode");
    }
}

void loop()
{
    if (bmv080.readSensor())
    {
        pm25 = bmv080.PM25();
        isObstructed = bmv080.isObstructed();
        newDataAvailable = true;
        //Serial.println("Sensor 1 data available");
    }
    delay(200); // needs a ~200ms delay in between talking to each sensor

    if (bmv080_2.readSensor())
    {
        pm25_2 = bmv080_2.PM25();
        isObstructed2 = bmv080_2.isObstructed();
        newDataAvailable2 = true;
        //Serial.println("Sensor 2 data available");
    }

    delay(200); // needs a ~200ms delay in between talking to each sensor

    if (newDataAvailable && newDataAvailable2)
    {
        //Serial.print("Sensor 1: ");

        Serial.print(pm25);

        if (isObstructed == true)
        {
            Serial.print(-1);
        }

        //Serial.print("\tSensor 2: ");

        Serial.print(",");

        Serial.print(pm25_2);

        if (isObstructed2 == true)
        {
            Serial.print("-1");
        }

        Serial.println();

        // reset variables
        newDataAvailable = false;
        newDataAvailable2 = false;
        isObstructed = false;
        isObstructed2 = false;
    }
}
