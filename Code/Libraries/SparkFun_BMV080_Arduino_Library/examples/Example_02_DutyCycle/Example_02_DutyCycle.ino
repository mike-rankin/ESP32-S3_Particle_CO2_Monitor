/*
  Using the BMV080 Particulate Matter PM2.5 Sensor in Duty Cycle Mode

  This example shows how to use the sensor in "duty cycle mode" to get
  particulate matter readings once every 20 seconds.

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

#include <Wire.h>
#include "SparkFun_BMV080_Arduino_Library.h" // CTRL+Click here to get the library: http://librarymanager/All#SparkFun_BMV080

SparkFunBMV080 bmv080; // Create an instance of the BMV080 class
#define BMV080_ADDR 0x57  // SparkFun BMV080 Breakout defaults to 0x57

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

    while(!Serial) delay(10); // Wait for Serial to become available.
    // Necessary for boards with native USB (like the SAMD51 Thing+).
    // For a final version of a project that does not need serial debug (or a USB cable plugged in),
    // Comment out this while loop, or it will prevent the remaining code from running.

    Serial.println();
    Serial.println("BMV080 Example 2 - Duty Cycle");

    wirePort.begin();

    if (bmv080.begin(BMV080_ADDR, wirePort) == false) {
        Serial.println("BMV080 not detected at default I2C address. Check your jumpers and the hookup guide. Freezing...");
        while (1)
        ;
    }
    Serial.println("BMV080 found!");

    // wirePort.setClock(400000); //Increase I2C data rate to 400kHz

    /* Initialize the Sensor (read driver, open, reset, id etc.)*/
    bmv080.init();

    /* Set the sensor Duty Cycling Period (seconds)*/
    uint16_t duty_cycling_period = 20;
    if(bmv080.setDutyCyclingPeriod(duty_cycling_period) == true)
    {
        Serial.println("BMV080 set to 20 second duty cycle period");
    }
    else
    {
        Serial.println("Error setting BMV080 duty cycle period");
    }

    /* Set the sensor mode to Duty Cycle mode */
    if(bmv080.setMode(SF_BMV080_MODE_DUTY_CYCLE) == true)
    {
        Serial.println("BMV080 set to Duty Cycle mode");
    }
    else
    {
        Serial.println("Error setting BMV080 mode");
    }
}

void loop()
{
    if(bmv080.readSensor())
    {
        float pm25 = bmv080.PM25();

        Serial.print(pm25);

        if(bmv080.isObstructed() == true)
        {
            Serial.print("\tObstructed");
        }

        Serial.println();
    }
    delay(1000);
}
