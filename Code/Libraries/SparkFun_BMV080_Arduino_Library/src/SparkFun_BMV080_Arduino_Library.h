/******************************************************************************
 * @file SparkFun_BMV080_Arduino_Library.h
 * @brief SparkFun BMV080 Library header file
 *
 * This file implements the SparkFunBMV080 and SparkFunBMV080SPI classes,
 * for use with the SparkFun BMV080 sensor qwiic breakout board, HW version v01.
 *
 * @author Pete Lewis
 * @date Sprint 2025
 * @version 1.0
 * @copyright (c) 2024 SparkFun Electronics Inc. This project is released under the MIT License.
 *
 * SPDX-License-Identifier: MIT
 *
 ******************************************************************************/

#pragma once

// helps to keep the Toolkit header before the tk calls
// clang-format off
#include <SparkFun_Toolkit.h>
#include "sfTk/sfDevBMV080.h"
// clang-format on

// The BMV080 Bosch API requires a larger than usual stack size
// In particular, bmv080_serve_interrupt is the culprit.
// If we are an ESP32 architecture, then we need to increase the loop stack size
// to 60KB. This is because the ESP32 has a 32KB stack size by default.
#if defined(ESP32)
SET_LOOP_TASK_STACK_SIZE(60 * 1024); // 60KB
#endif

/**
 * @brief Class for interfacing with the BMV080 sensor using I2C communication
 *
 * This class provides methods to initialize and communicate with the BMV080 sensor
 * over an I2C bus. It inherits from the sfDevBMV080 class and uses the SparkFun
 * Toolkit for I2C communication.
 *
 * @see sfDevBMV080
 */
class SparkFunBMV080 : public sfDevBMV080
{
  public:
    /**
     * @brief Begins the Device with I2C as the communication bus
     *
     * This method initializes the I2C bus and sets up communication with the BMV080 sensor.
     *
     * @param address I2C device address to use for the sensor
     * @param wirePort Wire port to use for I2C communication
     * @return True if successful, false otherwise
     */
    bool begin(const uint8_t address = SF_BMV080_DEFAULT_ADDRESS, TwoWire &wirePort = Wire)
    {
        // Setup Arduino I2C bus
        _theI2CBus.init(wirePort, address);
        _theI2CBus.setByteOrder(SFTK_MSBFIRST);

        // Begin the sensor
        sfTkError_t rc = sfDevBMV080::begin(&_theI2CBus);

        return rc == ksfTkErrOk ? isConnected() : false;
    }

    /// @brief Checks if the Device is connected
    /// @return True if the sensor is connected, false otherwise
    bool isConnected()
    {
        return _theI2CBus.ping() == ksfTkErrOk;
    }

  private:
    sfTkArdI2C _theI2CBus;
};

/**
 * @brief Class for interfacing with the BMV080 sensor using SPI communication
 *
 * This class provides methods to initialize and communicate with the BMV080 sensor
 * over an SPI bus. It inherits from the sfDevBMV080 class and uses the SparkFun
 * Toolkit for SPI communication.
 *
 * @see sfDevBMV080
 */
class SparkFunBMV080SPI : public sfDevBMV080
{
  public:
    /**
     * @brief Begins the Device with SPI as the communication bus
     *
     * This method initializes the SPI bus and sets up communication with the BMV080 sensor.
     *
     * @param csPin The chip select pin for the sensor
     * @param spiPort The SPI port to use for communication
     * @param spiSettings The SPI settings to use for communication
     * @return True if successful, false otherwise
     */
    bool begin(uint8_t csPin, SPIClass &spiPort = SPI,
               SPISettings spiSettings = SPISettings(100000, MSBFIRST, SPI_MODE0))
    {

        // Setup Arduino SPI bus
        _theSPIBus.init(spiPort, spiSettings, csPin, true);

        // Begin the sensor
        sfTkError_t rc = sfDevBMV080::begin(&_theSPIBus);

        return rc == ksfTkErrOk ? true : false;
    }

  private:
    sfTkArdSPI _theSPIBus;
};