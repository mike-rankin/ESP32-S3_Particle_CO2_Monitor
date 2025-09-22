/*
  Using the BMV080 Particulate Matter PM2.5 Sensor with SPI

  This example shows how to use the sensor in "continuous mode" to get
  particulate matter readings once every second.

  It uses polling of the device to check if new data is available.

  By: Pete Lewis
  SparkFun Electronics
  Date: September, 2024
  SparkFun code, firmware, and software is released under the MIT License.
	Please see LICENSE.md for further details.

  Hardware Connections:
  IoT RedBoard  -->     BMV080
  ------------------------------
  GND           -->     GND
  3V3           -->     3V3
  PICO          -->     PICO (SDA)
  POCI          -->     POCI (AB0)
  SCK           -->     SCK (SCL)
  CS            -->     CS (AB1)

  BMV080 jumpers set to SPI:
  MODE Jumper set to SPI
  ABO jumper left OPEN (both sides of the jumper are not connected)

  Serial.print it out at 115200 baud to serial monitor.

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/26554
*/

#include <Wire.h>
#include "SparkFun_BMV080_Arduino_Library.h" // CTRL+Click here to get the library: http://librarymanager/All#SparkFun_BMV080

SparkFunBMV080SPI bmv080; // Create an instance of the BMV080 class
#define CS_PIN 5 // Define the chip select pin

void setup()
{
    Serial.begin(115200);

    while(!Serial) delay(10); // Wait for Serial to become available.
    // Necessary for boards with native USB (like the SAMD51 Thing+).
    // For a final version of a project that does not need serial debug (or a USB cable plugged in),
    // Comment out this while loop, or it will prevent the remaining code from running.

    pinMode(CS_PIN, OUTPUT);
    pinMode(MOSI, OUTPUT);
    pinMode(SCK, OUTPUT);
    pinMode(MISO, INPUT);
    digitalWrite(CS_PIN, HIGH);

    Serial.println();
    Serial.println("BMV080 Example 4 - SPI");

    if (bmv080.begin(CS_PIN, SPI) == false) {
        Serial.println("SPI init failure. Check your jumpers and the hookup guide. Freezing...");
        while (1)
        ;
    }
    Serial.println("BMV080 SPI init successful");


    /* Initialize the Sensor (read driver, open, reset, id etc.)*/
    bmv080.init();

    /* Set the sensor mode to continuous mode */
    if(bmv080.setMode(SF_BMV080_MODE_CONTINUOUS) == true)
    {
        Serial.println("BMV080 set to continuous mode");
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
    delay(100);
}