/**
 * @file sfTkArdI2C.h
 * @brief Implementation file for the SparkFun Toolkit Arduino I2C driver.
 *
 * This file contains the Arduino I2C driver for the SparkFun Toolkit library.
 *
 * @author SparkFun Electronics
 * @date 2024-2025
 * @copyright Copyright (c) 2024-2025, SparkFun Electronics Inc. This project is released under the MIT License.
 *
 * SPDX-License-Identifier: MIT
 */

#include "sfTkArdI2C.h"

//---------------------------------------------------------------------------------

/**
 * @brief Initializes the I2C communication for the sfTkArdI2C object.
 *
 * This function sets up the I2C communication by assigning the provided TwoWire
 * object to the internal _i2cPort pointer. If the I2C port is not already set,
 * it will use the provided wirePort and optionally initialize it by calling
 * its begin() method. It also sets the I2C address for communication.
 *
 * @param wirePort Reference to a TwoWire object representing the I2C port.
 * @param addr The I2C address to be used for communication.
 * @param bInit Boolean flag indicating whether to initialize the I2C port by
 *              calling its begin() method. If true, begin() will be called.
 *
 * @return sfTkError_t Returns ksfTkErrOk on successful initialization.
 */
sfTkError_t sfTkArdI2C::init(TwoWire &wirePort, uint8_t addr, bool bInit)
{
    // if we don't have a wire port already
    if (!_i2cPort)
    {
        // use the pass in port
        _i2cPort = &wirePort;

        if (bInit)
            _i2cPort->begin();
    }

    setAddress(addr);
    return ksfTkErrOk;
}

//---------------------------------------------------------------------------------
/**
 * @brief Initializes the I2C communication with the specified address.
 *
 * This function initializes the I2C communication for the device. If no I2C port
 * is specified, it defaults to using the Wire library. If an I2C port is already
 * specified, it assumes the initialization is already done.
 *
 * @param addr The I2C address of the device.
 * @return sfTkError_t Returns ksfTkErrOk if the initialization is successful,
 *         otherwise returns an error code.
 */
sfTkError_t sfTkArdI2C::init(uint8_t addr)
{
    // no port yet, do the default version of it
    if (!_i2cPort)
        return init(Wire, addr);

    // We have a port, so arcady init'd - right?
    return ksfTkErrOk;
}

//---------------------------------------------------------------------------------

/**
 * @brief Initializes the I2C communication with the currently set address.
 *
 * This function initializes the I2C communication by calling the init function
 * with the address that is currently set for the I2C device.
 *
 * @return sfTkError_t Returns an error code indicating the success or failure of the initialization.
 */
sfTkError_t sfTkArdI2C::init(void)
{
    // call with our currently set address ...
    return init(address());
}
//---------------------------------------------------------------------------------
/**
 * @brief Pings the I2C device to check if it is responsive.
 *
 * This function attempts to initiate a transmission to the I2C device
 * and checks if the device acknowledges the transmission. It can be
 * used to verify that the device is properly connected and responsive.
 *
 * @return sfTkError_t
 *         - ksfTkErrOk: if the device acknowledges the transmission.
 *         - ksfTkErrFail: if the device does not acknowledge the transmission.
 *         - ksfTkErrBusNotInit: if the I2C bus is not initialized.
 */
sfTkError_t sfTkArdI2C::ping()
{
    // no port, no
    if (!_i2cPort)
        return ksfTkErrBusNotInit;

    _i2cPort->beginTransmission(address());
    return _i2cPort->endTransmission() == 0 ? ksfTkErrOk : ksfTkErrFail;
}

/**
 * @brief Writes an array of bytes to a register on the target address. Supports any address size
 *
 * @param devReg The device's register's address - can be any size
 * @param regLength The length of the register address
 * @param data The data to write
 * @param length The length of the data buffer
 * @return sfTkError_t Returns ksfTkErrOk on success, or ksfTkErrFail code
 */
sfTkError_t sfTkArdI2C::writeRegister(uint8_t *devReg, size_t regLength, const uint8_t *data, size_t length)
{
    if (!_i2cPort)
        return ksfTkErrBusNotInit;

    _i2cPort->beginTransmission(address());

    if (devReg != nullptr && regLength > 0)
        _i2cPort->write(devReg, regLength);

    _i2cPort->write(data, (int)length);

    return _i2cPort->endTransmission() ? ksfTkErrFail : ksfTkErrOk;
}

/**
 * @brief Reads an array of bytes to a register on the target address. Supports any address size
 *
 * @param devReg The device's register's address - can be any size
 * @param regLength The length of the register address
 * @param data The data to buffer to read into
 * @param numBytes The length of the data buffer
 * @param readBytes[out] The number of bytes read
 * @param read_delay After sending the address, delay in milliseconds before reading the data
 * @return sfTkError_t Returns ksfTkErrOk on success, or ksfTkErrFail code
 */
sfTkError_t sfTkArdI2C::readRegister(uint8_t *devReg, size_t regLength, uint8_t *data, size_t numBytes,
                                     size_t &readBytes, uint32_t read_delay)
{
    // got port
    if (!_i2cPort)
        return ksfTkErrBusNotInit;

    // Buffer valid?
    if (!data)
        return ksfTkErrBusNullBuffer;

    if (devReg == nullptr || regLength == 0)
        return ksfTkErrInvalidParam;

    readBytes = 0;

    uint16_t nOrig = numBytes; // original number of bytes.
    uint8_t nChunk;
    uint16_t nReturned;
    uint16_t i;              // counter in loop
    bool bFirstInter = true; // Flag for first iteration - used to send devRegister

    while (numBytes > 0)
    {
        if (bFirstInter)
        {
            _i2cPort->beginTransmission(address());

            _i2cPort->write(devReg, regLength);

            if (_i2cPort->endTransmission(stop()) != 0)
                return ksfTkErrFail; // error with the end transmission

            bFirstInter = false;

            // Was a delay requested between write addr and read bytes
            if (read_delay > 0)
                delay(read_delay);
        }

        // We're chunking in data - keeping the max chunk to kMaxI2CBufferLength
        nChunk = numBytes > _bufferChunkSize ? _bufferChunkSize : numBytes;

        // Request the bytes. If this is the last chunk, always send a stop
        nReturned = _i2cPort->requestFrom((int)address(), (int)nChunk, (int)(nChunk == numBytes ? true : stop()));

        // No data returned, no dice
        if (nReturned == 0)
            return ksfTkErrBusUnderRead; // error

        // Copy the retrieved data chunk to the current index in the data segment
        for (i = 0; i < nReturned; i++)
            *data++ = _i2cPort->read();

        // Decrement the amount of data received from the overall data request amount
        numBytes = numBytes - nReturned;

    } // end while

    readBytes = nOrig - numBytes; // Bytes read.

    return (readBytes == nOrig) ? ksfTkErrOk : ksfTkErrBusUnderRead; // Success
}
