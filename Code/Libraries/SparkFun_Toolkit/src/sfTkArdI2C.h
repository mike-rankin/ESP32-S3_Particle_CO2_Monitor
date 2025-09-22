/**
 * @file sfTkArdI2C.cpp
 * @brief header file for the SparkFun Toolkit Arduino I2C driver.
 *
 * This file contains the Arduino I2C header for the SparkFun Toolkit library.
 *
 * @author SparkFun Electronics
 * @date 2024-2025
 * @copyright Copyright (c) 2024-2025, SparkFun Electronics Inc. This project is released under the MIT License.
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <Arduino.h>
#include <Wire.h>

// Include our platform I2C interface definition.
#include "sfTkArduino.h"
#include <sfTk/sfTkII2C.h>

/**
 * @brief The sfTkArdI2C implements an sfTkII2C interface, defining the Arduino implementation for I2C in the Toolkit
 */

class sfTkArdI2C : public sfTkII2C
{
  public:
    /**
    @brief Constructor
    */

    sfTkArdI2C(void) : _i2cPort(nullptr), _bufferChunkSize{kDefaultBufferChunk}
    {
    }
    /**
        @brief Constructor

        @param addr The address of the device
    */
    sfTkArdI2C(uint8_t addr) : sfTkII2C(addr)
    {
    }

    /**
     * @brief copy constructor
     */
    sfTkArdI2C(sfTkArdI2C const &rhs) : sfTkII2C(), _i2cPort{rhs._i2cPort}
    {
    }

    /**
     * @brief Copy assignment
     *
     * @param rhs right hand side of the assignment
     * @return value of the left hand side of the assignment
     */
    sfTkArdI2C &operator=(const sfTkArdI2C &rhs)
    {
        _i2cPort = rhs._i2cPort;
        return *this;
    }

    /**
        @brief Method sets up the required I2C settings.
        @note This function provides a default I2C Port.

        @retval ksfTkErrOk on successful execution.
    */
    sfTkError_t init();

    /**
        @brief - address version of the init method

        @param addr The address of the device
    */
    sfTkError_t init(uint8_t addr);

    /**
        @brief Method sets up the required I2C settings.

        @param wirePort Port for I2C communication.
        @param addr The address of the device
        @param bInit This flag tracks whether the bus has been initialized.

        @retval ksfTkErrOk on successful execution.
    */
    sfTkError_t init(TwoWire &wirePort, uint8_t addr, bool bInit = false);

    /**
        @brief A simple ping of the device at the given address.
        @note sfTkIBus interface method

        @retval ksfTkErrOk on success,
    */
    sfTkError_t ping();

    /**
     * @brief Writes data to a specified register on an I2C device.
     *
     * @param devReg Pointer to the register address on the I2C device.
     * @param regLength Length of the register address in bytes.
     * @param data Pointer to the data to be written to the register.
     * @param length Length of the data to be written in bytes.
     * @return sfTkError_t Error code indicating the success or failure of the operation.
     */
    sfTkError_t writeRegister(uint8_t *devReg, size_t regLength, const uint8_t *data, size_t length);

    /**
     * @brief Reads data from a specified register of an I2C device.
     *
     * @param devReg Pointer to the register address to read from.
     * @param regLength Length of the register address.
     * @param data Pointer to the buffer where the read data will be stored.
     * @param numBytes Number of bytes to read from the register.
     * @param readBytes Reference to a variable where the number of bytes actually read will be stored.
     * @param read_delay After sending the address, delay in milliseconds before reading the data
     * @return sfTkError_t Error code indicating the success or failure of the read operation.
     */
    sfTkError_t readRegister(uint8_t *devReg, size_t regLength, uint8_t *data, size_t numBytes, size_t &readBytes,
                             uint32_t read_delay = 0);
    // Buffer size chunk getter/setter
    /**
        @brief set the buffer chunk size

        @note default size is 32

        @param theChunk the new size  - must be > 0

    */
    void setBufferChunkSize(size_t theChunk)
    {
        if (theChunk > 0)
            _bufferChunkSize = theChunk;
    }

    /**
        @brief set the buffer chunk size

        @retval The current chunk size

    */
    size_t bufferChunkSize(void)
    {
        return _bufferChunkSize;
    }

    // For overloaded virtual methods, make sure our subclass methods appear on this object
    using sfTkIBus::readRegister;
    using sfTkIBus::writeRegister;

  protected:
    // note: The wire port is protected, allowing access if a sub-class is
    //      created to implement a special read/write routine
    //
    /** The actual Arduino i2c port */
    TwoWire *_i2cPort;

  private:
    /** Default buffer chunk size*/
    static constexpr size_t kDefaultBufferChunk = 32;

    /** The I2C buffer chunker - chunk size*/
    size_t _bufferChunkSize;
};
