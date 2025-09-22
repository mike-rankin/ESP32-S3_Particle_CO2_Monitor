/*
  Using the BMV080 Particulate Matter PM2.5 Sensor

  This Example shows how to read and set the parameters of the BMV080 sensor.

  These include:
    volumetric_mass_density
    integration_time
    distribution_id
    do_obstruction_detection
    do_vibration_filtering
    measurement_algorithm

  After these parameters are read and set, This example shows how to use the 
  sensor in "continuous mode" to get particulate matter readings once every 
  second.

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
    Serial.println("BMV080 Example 5 - Get and Set Parameters");

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

    getSetParameters(); // Get and set parameters

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

void getSetParameters(void)
{
    // Variables to store the parameters
    // Note, for custom settings, you will need to set these variables before
    // calling the set functions (down below).

    float volumetric_mass_density = 0.0f;
    float integration_time = 0.0f;
    uint32_t distribution_id = 0;
    bool do_obstruction_detection = false;
    bool do_vibration_filtering = false;
    uint8_t measurementAlgorithm = E_BMV080_MEASUREMENT_ALGORITHM_BALANCED;

    /***************************************************************************
     * 
     * Reading configuration parameters
     * 
     * *************************************************************************/


    /* Get default parameter "volumetric_mass_density" */
    volumetric_mass_density = bmv080.volumetricMassDensity();
    Serial.print("BMV080 parameter 'volumetric_mass_density' READ: ");
    Serial.println(volumetric_mass_density);

    /* Get default parameter "integration_time" */
    integration_time = bmv080.integrationTime();
    Serial.print("BMV080 parameter 'integration_time' READ: ");
    Serial.println(integration_time);

    /* Get default parameter "distribution_id" */
    distribution_id = bmv080.distributionId();
    Serial.print("BMV080 parameter 'distribution_id' READ: ");
    Serial.println(distribution_id);

    /* Get default parameter "do_obstruction_detection" */
    do_obstruction_detection = bmv080.doObstructionDetection();
    Serial.print("BMV080 parameter 'do_obstruction_detection' READ: ");
    if(do_obstruction_detection == true)
    {
        Serial.println("true");
    }
    else
    {
        Serial.println("false");
    }

    /* Get default parameter "do_vibration_filtering" */
    do_vibration_filtering = bmv080.doVibrationFiltering();
    Serial.print("BMV080 parameter 'do_vibration_filtering' READ: ");
    if(do_vibration_filtering == true)
    {
        Serial.println("true");
    }
    else
    {
        Serial.println("false");
    }

    /* Get default parameter "measurement_algorithm" */
    measurementAlgorithm = bmv080.measurementAlgorithm();
    Serial.print("BMV080 parameter 'measurement_algorithm' READ: ");
    switch (measurementAlgorithm)
    {
        case E_BMV080_MEASUREMENT_ALGORITHM_FAST_RESPONSE:
            Serial.println("Fast Response");
            break;
        case E_BMV080_MEASUREMENT_ALGORITHM_BALANCED:
            Serial.println("Balanced");
            break;
        case E_BMV080_MEASUREMENT_ALGORITHM_HIGH_PRECISION:
            Serial.println("High Precision");
            break;
        default:
            Serial.println("Unknown");
            break;
    }


    /***************************************************************************
     * 
     * Setting (custom) configuration parameters
     * 
     * *************************************************************************/

    // Uncomment the following lines to set the parameters to custom values

    // volumetric_mass_density = 1.6f;
    // integration_time = 10.0f;
    // distribution_id = 3;
    // do_obstruction_detection = true;
    // do_vibration_filtering = false;
    // measurementAlgorithm = E_BMV080_MEASUREMENT_ALGORITHM_BALANCED;

    /* Set custom parameter "volumetric_mass_density" */
    if(bmv080.setVolumetricMassDensity(volumetric_mass_density) == true)
    {
        Serial.print("BMV080 parameter 'volumetric_mass_density' SET TO: ");
        Serial.println(volumetric_mass_density);
    }
    else
    {
        Serial.println("Error setting BMV080 parameter 'volumetric_mass_density'");
    }

    /* Set custom parameter "integration_time" */
    if(bmv080.setIntegrationTime(integration_time) == true)
    {
        Serial.print("BMV080 parameter 'integration_time' SET TO: ");
        Serial.println(integration_time);
    }
    else
    {
        Serial.println("Error setting BMV080 parameter 'integration_time'");
    }

    /* Set custom parameter "distribution_id" */
    if(bmv080.setDistributionId(distribution_id) == true)
    {
        Serial.print("BMV080 parameter 'distribution_id' SET TO: ");
        Serial.println(distribution_id);
    }
    else
    {
        Serial.println("Error setting BMV080 parameter 'distribution_id'");
    }

    /* Set custom parameter "do_obstruction_detection" */
    if(bmv080.setDoObstructionDetection(do_obstruction_detection) == true)
    {
        Serial.print("BMV080 parameter 'do_obstruction_detection' SET TO: ");
        if(do_obstruction_detection == true)
        {
            Serial.println("true");
        }
        else
        {
            Serial.println("false");
        }
    }
    else
    {
        Serial.println("Error setting BMV080 parameter 'do_obstruction_detection'");
    }

    /* Set custom parameter "do_vibration_filtering" */
    if(bmv080.setDoVibrationFiltering(do_vibration_filtering) == true)
    {
        Serial.print("BMV080 parameter 'do_vibration_filtering' SET TO: ");
        if(do_vibration_filtering == true)
        {
            Serial.println("true");
        }
        else
        {
            Serial.println("false");
        }
    }
    else
    {
        Serial.println("Error setting BMV080 parameter 'do_vibration_filtering'");
    }

    /* Set custom parameter "measurement_algorithm" */
    if(bmv080.setMeasurementAlgorithm(measurementAlgorithm) == true)
    {
        Serial.print("BMV080 parameter 'measurement_algorithm' SET TO: ");
        switch (measurementAlgorithm)
        {
            case E_BMV080_MEASUREMENT_ALGORITHM_FAST_RESPONSE:
                Serial.println("Fast Response");
                break;
            case E_BMV080_MEASUREMENT_ALGORITHM_BALANCED:
                Serial.println("Balanced");
                break;
            case E_BMV080_MEASUREMENT_ALGORITHM_HIGH_PRECISION:
                Serial.println("High Precision");
                break;
            default:
                Serial.println("Unknown");
                break;
        }
    }
    else
    {
        Serial.println("Error setting BMV080 parameter 'measurement_algorithm'");
    }

    Serial.println();
}