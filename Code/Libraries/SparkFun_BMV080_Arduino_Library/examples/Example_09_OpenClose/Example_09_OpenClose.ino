/*
  Using the BMV080 Particulate Matter PM2.5 Sensor

  This example shows how to open an instance of the object, take 5  readings, 
  then close the object.

  It then repeats this process every 5 seconds.

  This example shows how to use the sensor in "continuous mode" to get
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


#define BMV080_ADDR 0x57  // SparkFun BMV080 Breakout defaults to 0x57

// Some Dev boards have their QWIIC connector on Wire or Wire1
// This #ifdef will help this sketch work across more products

#ifdef ARDUINO_SPARKFUN_THINGPLUS_RP2040
#define wirePort Wire1
#else
#define wirePort Wire
#endif

int testTimes = 0; // Test times

void setup()
{
    Serial.begin(115200);

    while (!Serial)
        delay(10); // Wait for Serial to become available.
    // Necessary for boards with native USB (like the SAMD51 Thing+).
    // For a final version of a project that does not need serial debug (or a USB cable plugged in),
    // Comment out this while loop, or it will prevent the remaining code from running.

    Serial.println();
    Serial.println("BMV080 Example 09 - Open and Close");
}

void loop()
{
    Serial.print("Test times: ");
    Serial.println(testTimes); 
    testOnce();
    testTimes++;
    delay(5000);
}


void testOnce()
{
    Serial.println();
    Serial.println("BMV080 Begin testing readings");

    SparkFunBMV080 bmv080; // Create an instance of the BMV080 class

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
    // Do what init() does, but check each call to each function individually
    // to see if there are any issues with the sensor

    uint16_t major, minor, patch;
    if (bmv080.driverVersion(major, minor, patch) == true)
    {
        Serial.print("BMV080 Driver Version: ");
        Serial.print(major);
        Serial.print(".");
        Serial.print(minor);
        Serial.print(".");
        Serial.println(patch);
    }
    else
    {
        Serial.println("Error getting BMV080 driver version");
    }

    if (bmv080.open() == true)
    {
        Serial.println("BMV080 Opened");
    }
    else
    {
        Serial.println("Error opening BMV080");
    }

    if (bmv080.reset() == true)
    {
        Serial.println("BMV080 Reset");
    }
    else
    {
        Serial.println("Error resetting BMV080");
    }

    char idOut[13];
    if (bmv080.ID(idOut) == true)
    {
        Serial.print("BMV080 ID: ");
        Serial.println(idOut);
    }
    else
    {
        Serial.println("Error getting BMV080 ID");
    }

    /* Set the sensor mode to continuous mode */
    if (bmv080.setMode(SF_BMV080_MODE_CONTINUOUS) == true)
    {
        Serial.println("BMV080 set to continuous mode");
    }
    else
    {
        Serial.println("Error setting BMV080 mode");
    }

    // Take 5 readings

    // Poll the sensor 50 times, once every 100ms
    // The sensor is setup to report readings once per second
    // So this will result in 5 readings.
    for(int i = 0 ; i < 50 ; i++) 
    {
        if (bmv080.readSensor())
        {
            float pm10 = bmv080.PM10();
            float pm25 = bmv080.PM25();
            float pm1 = bmv080.PM1();

            Serial.print("PM10: ");
            Serial.print(pm10);
            Serial.print("\t");
            Serial.print("PM2.5: ");
            Serial.print(pm25);
            Serial.print("\t");
            Serial.print("PM1: ");
            Serial.print(pm1);

            if (bmv080.isObstructed() == true)
            {
                Serial.print("\tObstructed");
            }

            Serial.println();
        }
        delay(100);
    }

    Serial.println("BMV080 End testing readings");

    bmv080.close();

    wirePort.end();
}