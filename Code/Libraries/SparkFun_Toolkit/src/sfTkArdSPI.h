/**
 * @file sfTkArdSPI.cpp
 * @brief header file for the SparkFun Toolkit Arduino SPI driver.
 *
 * This file contains the Arduino SPI header for the SparkFun Toolkit library.
 *
 * @author SparkFun Electronics
 * @date 2024-2025
 * @copyright Copyright (c) 2024-2025, SparkFun Electronics Inc. This project is released under the MIT License.
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "sfTkArduino.h"
#include <SPI.h>
#include <sfTk/sfTkISPI.h>

/**
  @brief This class implements the IBus interface for an SPI Implementation on Arduino
 */
class sfTkArdSPI : public sfTkISPI
{
  public:
    /**
        @brief Constructor for Arduino SPI bus object of the toolkit
    */
    sfTkArdSPI(void) : _spiPort(nullptr)
    {
    }

    /**
        @brief Constructor for Arduino SPI bus object of the toolkit

        @param csPin The CS Pin for the device
    */
    sfTkArdSPI(uint8_t csPin) : sfTkISPI(csPin)
    {
    }
    /**
        @brief Copy constructor for Arduino SPI bus object of the toolkit

        @param rhs source of the copy operation
    */
    sfTkArdSPI(sfTkArdSPI const &rhs) : sfTkISPI(), _spiPort{rhs._spiPort}, _sfeSPISettings{rhs._sfeSPISettings}
    {
    }

    /**
        @brief Assignment copy operator for Arduino SPI bus object of the toolkit

        @param rhs The right hand side of the assignment.
        @return sfTkArdSPI& - The left hand side of the assignment.
    */
    sfTkArdSPI &operator=(const sfTkArdSPI &rhs)
    {
        _spiPort = rhs._spiPort;
        _sfeSPISettings = rhs._sfeSPISettings;
        return *this;
    }

    /**
        @brief Method sets up the required SPI settings.
        @note This function provides a default SPI Port.

        @param bInit Init the device - default is false.

        @retval sfTkError_t -  ksfTkErrOk on success
    */
    sfTkError_t init(bool bInit = false);

    /**
        @brief Method sets up the required SPI settings.
        @note This function provides a default SPI Port.

        @param csPin The CS Pin for the device
        @param bInit Init the device - default is false.

        @retval sfTkError_t -  ksfTkErrOk on success
    */
    sfTkError_t init(uint8_t csPin, bool bInit = false);

    /**
        @brief Method sets up the required SPI settings.

        @param spiPort Port for SPI communication.
        @param busSPISettings Settings for speed, endianness, and spi mode of the SPI bus.
        @param csPin The CS Pin for the device
        @param bInit This flag tracks whether the bus has been initialized.

        @retval sfTkError_t - ksfTkErrOk on success
    */
    sfTkError_t init(SPIClass &spiPort, SPISettings &busSPISettings, uint8_t csPin, bool bInit = false);

    /**
        @brief Write an array of data to the device without indexing to a register.

        @param data Data to write
        @param length Length of Data

        @retval sfTkError_t - ksfTkErrOk on success
    */
    sfTkError_t writeRegion(const uint8_t *data, size_t length);

    /**
        @brief Writes a number of bytes starting at the given register's address.
        @note This method is virtual to allow it to be overridden to support a device that requires a unique impl

        @param devReg The device's register's address.
        @param regLength Size of the address - value of 1 or 2 are only accepted
        @param data Data to write.
        @param length - length of data

        @retval sfTkError_t - ksfTkErrOk on success
    */
    sfTkError_t writeRegister(uint8_t *devReg, size_t regLength, const uint8_t *data, size_t length);

    /**
        @brief Writes a number of bytes starting at the given register's address.
        @note This method is virtual to allow it to be overridden to support a device that requires a unique impl

        @param devReg The device's register's address.
        @param data Data to write.
        @param length - length of data

        @retval sfTkError_t - ksfTkErrOk on success
    */
    sfTkError_t writeRegister(uint16_t devReg, const uint8_t *data, size_t length);

    /**
        @brief Writes a number of uint16s starting at the given register's address.
        @note This method is virtual to allow it to be overridden to support a device that requires a unique impl

        @param devReg The device's register's address.
        @param data Data to write.
        @param length - length of data

        @retval sfTkError_t - ksfTkErrOk on success
        */
    sfTkError_t writeRegister(uint16_t devReg, const uint16_t *data, size_t length);

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

    /**
        @brief Reads a block of data from the given register.
        @note This method is virtual to allow it to be overridden to support a device that requires a unique impl

        @param reg The device's register's 16 bit address.
        @param[out] data Data buffer to read into
        @param numBytes - Length of data to read/size of data buffer
        @param[out] readBytes - Number of bytes read

        @retval sfTkError_t - true on success
    */
    virtual sfTkError_t readRegister(uint16_t reg, uint8_t *data, size_t numBytes, size_t &readBytes);

    /**
            @brief Reads a block of data from the given register.
            @note This method is virtual to allow it to be overridden to support a device that requires a unique impl

            @param reg The device's register's 16 bit address.
            @param[out] data Data buffer to read into
            @param length - Length of data to read/size of data buffer
            @param[out] readWords - Number of words read

            @retval sfTkError_t - true on success
        */
    virtual sfTkError_t readRegister(uint16_t reg, uint16_t *data, size_t length, size_t &readWords);

    // For overloaded virtual methods, make sure to elevate our subclass methods.
    using sfTkIBus::readRegister;
    using sfTkIBus::writeRegister;

  protected:
    // note: The instance data is protected, allowing access if a sub-class is
    //      created to implement a special read/write routine
    //
    /** Pointer to the spi port being used */
    SPIClass *_spiPort;

    /** This object's spi settings are used for every transaction. */
    SPISettings _sfeSPISettings;
};
