/**
 * @file sfTkIBus.h
 * @brief Header file for the SparkFun Toolkit Device Bus Interface Definition.
 *
 * This file contains the interface declaration for the Communication Bus Interface
 *
 * @author SparkFun Electronics
 * @date 2024-2025
 * @copyright Copyright (c) 2024-2025, SparkFun Electronics Inc. This project is released under the MIT License.
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "sfToolkit.h"
#include <stddef.h>

/**
 * @brief Define our error codes for the bus. Note Errors are negative, warnings/info positive,
 * but keep the same increment on the base.
 *
 */

/**
 * @brief Error code for when a bus system is not initalized.
 */
const sfTkError_t ksfTkErrBusNotInit = ksfTkErrFail * (ksfTkErrBaseBus + 1);

/**
 * @brief Returned when a bus system times out.
 */
const sfTkError_t ksfTkErrBusTimeout = ksfTkErrFail * (ksfTkErrBaseBus + 2);

/**
 * @brief Returned when a bus system does not respond.
 */
const sfTkError_t ksfTkErrBusNoResponse = ksfTkErrFail * (ksfTkErrBaseBus + 3);

/**
 * @brief Returned when the data to be sent is too long or received is too short.
 */
const sfTkError_t ksfTkErrBusDataTooLong = ksfTkErrFail * (ksfTkErrBaseBus + 4);

/**
 * @brief Returned when the bus settings are null, invalid or on set/initialised
 */
const sfTkError_t ksfTkErrBusNullSettings = ksfTkErrFail * (ksfTkErrBaseBus + 5);

/**
 * @brief Returned when the buffer is null or invalid.
 */
const sfTkError_t ksfTkErrBusNullBuffer = ksfTkErrFail * (ksfTkErrBaseBus + 6);

/**
 * @brief Returned when the bus is under read. Warning
 */
const sfTkError_t ksfTkErrBusUnderRead = ksfTkErrBaseBus + 7;

/**
 * @brief Returned when the bus is not enabled. Warning
 */
const sfTkError_t ksfTkErrBusNotEnabled = ksfTkErrBaseBus + 8;

/**
 * @brief Interface that defines the communication bus for the SparkFun Electronics Toolkit.
 *
 * The bus interface defines the basic methods for reading and writing data to a device. Specific
 * bus implementations will extend this interface to provide the necessary functionality for the
 * desired bus type.
 */
class sfTkIBus
{
  public:
    /**
     * @brief Constructor
     */
    sfTkIBus()
    {
        _byteOrder = sftk_system_byteorder();
    }

    /**
     * @brief Writes an array of bytes to a register on the target address. Supports any address size
     * @note This is the minimum required write method to implement for a driver
     *
     * @param devReg The device's register's address - can be any size, If nullptr, address is not sent
     * @param regLength The length of the register address. If 0, address is not sent
     * @param data The data to write
     * @param length The length of the data buffer
     * @return sfTkError_t Returns ksfTkErrOk on success, or ksfTkErrFail code
     */
    virtual sfTkError_t writeRegister(uint8_t *devReg, size_t regLength, const uint8_t *data, size_t length) = 0;

    /**--------------------------------------------------------------------------
     * @brief Writes a 8-bit data value.
     *
     * This function writes a 8-bit data value
     *
     * @param data The 8-bit data value to be written.
     * @return sfTkError_t The status of the write operation.
     */
    virtual sfTkError_t writeData(uint8_t data)
    {
        return writeRegister(nullptr, 0, &data, sizeof(uint8_t));
    }

    /**--------------------------------------------------------------------------
     * @brief Writes a 8-bit data value.
     *
     * This function writes a 8-bit data value
     *
     * @param data The 8-bit data value to be written.
     * @return sfTkError_t The status of the write operation.
     */
    sfTkError_t writeUInt8(uint8_t data)
    {
        return writeData(data);
    }

    /**--------------------------------------------------------------------------
     *  @brief Send a 16 bit value to the device.
     *  @param data Data to write. If the byte order is different than system byteorder, data is byteswapped
     *
     *  @retval sfTkError_t -  ksfTkErrOk on successful execution.
     *
     */
    virtual sfTkError_t writeData(uint16_t data)
    {
        // byte swap?
        if (sftk_system_byteorder() != _byteOrder)
            data = sftk_byte_swap(data);

        return writeRegister(nullptr, 0, (uint8_t *)&data, sizeof(uint16_t));
    }

    /**--------------------------------------------------------------------------
     *  @brief Send a 16 bit value to the device.
     *  @param data Data to write. If the byte order is different than system byteorder, data is byteswapped
     *
     *  @retval sfTkError_t -  ksfTkErrOk on successful execution.
     *
     */
    sfTkError_t writeUInt16(uint16_t data)
    {
        return writeData(data);
    }

    /**--------------------------------------------------------------------------
     * @brief Writes a 32-bit data value.
     *
     * This function writes a 32-bit data value
     *
     * @param data The 32-bit data value to be written. The data is byteswapped if needed.
     * @return sfTkError_t The status of the write operation.
     */
    virtual sfTkError_t writeData(uint32_t data)
    {
        // byte swap?
        if (sftk_system_byteorder() != _byteOrder)
            data = sftk_byte_swap(data);

        return writeRegister(nullptr, 0, (uint8_t *)&data, sizeof(uint32_t));
    }

    /**--------------------------------------------------------------------------
     * @brief Writes a 32-bit data value.
     *
     * This function writes a 32-bit data value
     *
     * @param data The 32-bit data value to be written. The data is byteswapped if needed.
     * @return sfTkError_t The status of the write operation.
     */
    sfTkError_t writeUInt32(uint32_t data)
    {
        return writeData(data);
    }
    /**--------------------------------------------------------------------------
     * @brief Writes an array
     *
     * This function writes a 8-bit data array
     *
     * @param data The 8-bit data array to be written. The data is byteswapped if needed.
     * @param length The length of the data array to be written.
     * @return sfTkError_t The status of the write operation.
     */
    virtual sfTkError_t writeData(const uint8_t *data, size_t length)
    {
        return writeRegister(nullptr, 0, data, length);
    }

    /**--------------------------------------------------------------------------
     * @brief Write a single byte (8 bit) to the given register
     *
     *   @param devReg The device's register's address.
     *   @param data Data to write. Note - if byte order differs, swap the data before writing.
     *
     *   @retval sfTkError_t -  ksfTkErrOk on successful execution.
     *
     */
    sfTkError_t writeRegister(uint8_t devReg, uint8_t data)
    {
        return writeRegister(&devReg, sizeof(devReg), &data, sizeof(uint8_t));
    }

    /**--------------------------------------------------------------------------
     * @brief Write a single byte (8 bit) to the given register
     *
     *   @param devReg The device's register's address.
     *   @param data Data to write. Note - if byte order differs, swap the data before writing.
     *
     *   @retval sfTkError_t -  ksfTkErrOk on successful execution.
     *
     */
    sfTkError_t writeRegisterUInt8(uint8_t reg, uint8_t data)
    {
        return writeRegister(reg, data);
    }

    /**--------------------------------------------------------------------------
     * @brief Write a single word (16 bit) to the given register
     *
     *   @param devReg The device's register's address.
     *   @param data Data to write. Note - if byte order differs, swap the data before writing.
     *
     *   @retval sfTkError_t -  ksfTkErrOk on successful execution.
     *
     */
    virtual sfTkError_t writeRegister(uint8_t devReg, const uint16_t data)
    {
        // swap the data?
        uint16_t value = data;
        if (sftk_system_byteorder() != _byteOrder)
            value = sftk_byte_swap(value);

        return writeRegister(&devReg, sizeof(devReg), (uint8_t *)&value, sizeof(value));
    }

    /**--------------------------------------------------------------------------
     * @brief Write a single word (16 bit) to the given register
     *
     *   @param devReg The device's register's address.
     *   @param data Data to write. Note - if byte order differs, swap the data before writing.
     *
     *   @retval sfTkError_t -  ksfTkErrOk on successful execution.
     *
     */
    sfTkError_t writeRegisterUInt16(uint8_t reg, uint16_t data)
    {
        return writeRegister(reg, data);
    }

    /**
     * @brief Writes a 32-bit data value to a specified device register.
     *
     * This function writes a 32-bit data value to a specified device register.
     * If the system byte order differs from the byte order expected by the device,
     * the data value is byte-swapped before being written.
     *
     * @param devReg The register address to write to.
     * @param data The 32-bit data value to write.
     * @return sfTkError_t Error code indicating the success or failure of the operation.
     */
    virtual sfTkError_t writeRegister(uint8_t devReg, const uint32_t data)
    {
        uint32_t value = data;
        // swap the data?
        if (sftk_system_byteorder() != _byteOrder)
            value = sftk_byte_swap(value);

        return writeRegister(&devReg, sizeof(devReg), (uint8_t *)&value, sizeof(data));
    }

    /**
     * @brief Writes a 32-bit data value to a specified device register.
     *
     * This function writes a 32-bit data value to a specified device register.
     * If the system byte order differs from the byte order expected by the device,
     * the data value is byte-swapped before being written.
     *
     * @param devReg The register address to write to.
     * @param data The 32-bit data value to write.
     * @return sfTkError_t Error code indicating the success or failure of the operation.
     */
    sfTkError_t writeRegisterUInt32(uint8_t reg, uint32_t data)
    {
        return writeRegister(reg, data);
    }

    /**--------------------------------------------------------------------------
     *  @brief Writes a number of bytes starting at the given register's address.
     *
     *  @param devReg The device's register's address.
     *  @param data Data to write.
     *  @param length - length of data
     *
     *   @retval sfTkError_t ksfTkErrOk on successful execution
     *
     */
    virtual sfTkError_t writeRegister(uint8_t devReg, const uint8_t *data, size_t length)
    {
        return writeRegister(&devReg, sizeof(devReg), data, length);
    }

    /**--------------------------------------------------------------------------
     *  @brief Writes a number of bytes starting at the given register's 16-bit address.
     *
     *  @param devReg The device's register's 16 bit address. If the byteorder is different than the systems, the
     * address is byteswapped
     *  @param data Data to write.
     *  @param length - length of data
     *
     *   @retval sfTkError_t ksfTkErrOk on successful execution
     *
     */
    virtual sfTkError_t writeRegister(uint16_t devReg, const uint8_t *data, size_t length)
    {
        if (sftk_system_byteorder() != _byteOrder)
            devReg = sftk_byte_swap(devReg);

        return writeRegister((uint8_t *)&devReg, sizeof(devReg), data, length);
    }

    /**
     * @brief Writes a 8-bit data value to a specified device register.
     *
     * This function writes a 16-bit data value to a specified device register.
     * If the system byte order differs from the byte order expected by the device,
     * the data value is byte-swapped before being written.
     *
     * @param devReg The device's register's 16 bit address. If the byteorder is different than the systems, the address
     * is byteswapped
     * @param data The 8-bit data value to write.
     * @return sfTkError_t Error code indicating the success or failure of the operation.
     */
    virtual sfTkError_t writeRegister(uint16_t devReg, const uint8_t data)
    {
        return writeRegister(devReg, &data, 1);
    }

    /**
     * @brief Writes a 8-bit data value to a specified device register.
     *
     * This function writes a 16-bit data value to a specified device register.
     * If the system byte order differs from the byte order expected by the device,
     * the data value is byte-swapped before being written.
     *
     * @param devReg The device's register's 16 bit address. If the byteorder is different than the systems, the address
     * is byteswapped
     * @param data The 8-bit data value to write.
     * @return sfTkError_t Error code indicating the success or failure of the operation.
     */
    sfTkError_t writeRegisterUInt8(uint16_t reg, const uint8_t data)
    {
        return writeRegister(reg, data);
    }
    /**
     * @brief Writes a 16-bit data value to a specified device 16bit register.
     *
     * This function writes a 16-bit data value to a specified device register.
     * If the system byte order differs from the byte order expected by the device,
     * the data value is byte-swapped before being written.
     *
     * @param devReg The 16 bit  register address to write to.
     * @param data The 16-bit data value to write.
     * @return sfTkError_t Error code indicating the success or failure of the operation.
     */
    virtual sfTkError_t writeRegister(uint16_t devReg, const uint16_t data)
    {
        return writeRegister(devReg, &data, sizeof(data));
    }

    /**
     * @brief Writes a 16-bit data value to a specified device 16bit register.
     *
     * This function writes a 16-bit data value to a specified device register.
     * If the system byte order differs from the byte order expected by the device,
     * the data value is byte-swapped before being written.
     *
     * @param devReg The 16 bit  register address to write to.
     * @param data The 16-bit data value to write.
     * @return sfTkError_t Error code indicating the success or failure of the operation.
     */
    sfTkError_t writeRegisterUInt16(uint16_t reg, const uint16_t data)
    {
        return writeRegister(reg, data);
    }

    /**
     * @brief Writes a 32-bit data value to a specified device register.
     *
     * This function writes a 32-bit data value to a specified device register.
     * If the system byte order differs from the byte order expected by the device,
     * the data value is byte-swapped before being written.
     *
     * @param devReg The register address to write to.
     * @param data The 32-bit data value to write. This value is byteswapped if the byteorder set is different that the
     * systems byteorder
     * @return sfTkError_t Error code indicating the success or failure of the operation.
     */
    virtual sfTkError_t writeRegister(uint16_t devReg, const uint32_t data)
    {
        uint32_t value = data;
        // do we need to swap the data value? Note the address is swapped in the called writeReg16 method
        if (sftk_system_byteorder() != _byteOrder)
            value = sftk_byte_swap(data);

        return writeRegister(devReg, (const uint8_t *)&value, sizeof(uint32_t));
    }

    /**
     * @brief Writes a 32-bit data value to a specified device register.
     *
     * This function writes a 32-bit data value to a specified device register.
     * If the system byte order differs from the byte order expected by the device,
     * the data value is byte-swapped before being written.
     *
     * @param devReg The register address to write to.
     * @param data The 32-bit data value to write. This value is byteswapped if the byteorder set is different that the
     * systems byteorder
     * @return sfTkError_t Error code indicating the success or failure of the operation.
     */
    sfTkError_t writeRegisterUInt32(uint16_t reg, const uint32_t data)
    {
        return writeRegister(reg, data);
    }
    /**--------------------------------------------------------------------------
     *  @brief Writes a number of uint16's starting at the given register's 16-bit address.
     *
     *  @param devReg The device's register's address.
     *  @param data 16 bit Data to write. This value is byteswapped if the byteorder set is different than the
     *  systems byteorder
     *  @param length - length of data
     *
     *   @retval sfTkError_t ksfTkErrOk on successful execution
     *
     */
    virtual sfTkError_t writeRegister(uint16_t devReg, const uint16_t *data, size_t length)
    {
        // if the system byte order is the same as the desired order, just send the buffer
        if (sftk_system_byteorder() == _byteOrder)
            return writeRegister(devReg, (const uint8_t *)data, length * sizeof(uint16_t));

        // swap data
        uint16_t data16[length];

        for (size_t i = 0; i < length; i++)
            data16[i] = sftk_byte_swap(data[i]);
        return writeRegister(devReg, (const uint8_t *)data16, length * sizeof(uint16_t));
    }

    //---------------------------------------------------------------------------
    // Read Methods
    //---------------------------------------------------------------------------

    // The method that must be implemented - at a minimum.
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
    virtual sfTkError_t readRegister(uint8_t *devReg, size_t regLength, uint8_t *data, size_t numBytes,
                                     size_t &readBytes, uint32_t read_delay = 0) = 0;
    /**--------------------------------------------------------------------------
     *  @brief Read a single byte from the given register
     *
     *  @param devReg The device's register's address.
     *  @param data Data to read.
     *
     *   @retval sfTkError_t -  ksfTkErrOk on successful execution.
     *
     */
    virtual sfTkError_t readRegister(uint8_t devReg, uint8_t &data)
    {
        size_t nRead;
        sfTkError_t retVal = readRegister(&devReg, sizeof(devReg), &data, sizeof(data), nRead);

        return (retVal == ksfTkErrOk && nRead == sizeof(uint8_t) ? ksfTkErrOk : retVal);
    }

    /**--------------------------------------------------------------------------
     *  @brief Read a single byte from the given register
     *
     *  @param devReg The device's register's address.
     *  @param data Data to read.
     *
     *   @retval sfTkError_t -  ksfTkErrOk on successful execution.
     *
     */
    sfTkError_t readRegisterUInt8(uint8_t devReg, uint8_t &data)
    {
        return readRegister(devReg, data);
    }

    /**--------------------------------------------------------------------------
     *  @brief Read a single word (16 bit) from the given register
     *
     *   @param devReg The device's register's address.
     *   @param data Data to read.-- the data value is byteswapped if needed
     *
     *   @retval sfTkError_t -  ksfTkErrOk on successful execution.
     */
    virtual sfTkError_t readRegister(uint8_t devReg, uint16_t &data)
    {
        size_t nRead;
        sfTkError_t retVal = readRegister(&devReg, sizeof(devReg), (uint8_t *)&data, sizeof(data), nRead);

        // The data is a uint16 - byte swap the result?
        if (retVal == ksfTkErrOk && sftk_system_byteorder() != _byteOrder)
            data = sftk_byte_swap(data);

        return (retVal == ksfTkErrOk && nRead == sizeof(uint16_t) ? ksfTkErrOk : retVal);
    }

    /**--------------------------------------------------------------------------
     *  @brief Read a single word (16 bit) from the given register
     *
     *   @param devReg The device's register's address.
     *   @param data Data to read.-- the data value is byteswapped if needed
     *
     *   @retval sfTkError_t -  ksfTkErrOk on successful execution.
     */
    sfTkError_t readRegisterUInt16(uint8_t devReg, uint16_t &data)
    {
        return readRegister(devReg, data);
    }

    /**--------------------------------------------------------------------------
     *  @brief Read a 32 bit value from the given register
     *
     *   @param devReg The device's register's address.
     *   @param data Data to read. The data value is byteswapped if needed
     *
     *   @retval sfTkError_t -  ksfTkErrOk on successful execution.
     */
    virtual sfTkError_t readRegister(uint8_t devReg, uint32_t &data)
    {
        size_t nRead;
        sfTkError_t retVal = readRegister(&devReg, sizeof(devReg), (uint8_t *)&data, sizeof(data), nRead);

        // The data is a uint32 - byte swap the result?
        if (retVal == ksfTkErrOk && sftk_system_byteorder() != _byteOrder)
            data = sftk_byte_swap(data);

        return (retVal == ksfTkErrOk && nRead == sizeof(uint32_t) ? ksfTkErrOk : retVal);
    }

    /**--------------------------------------------------------------------------
     *  @brief Read a 32 bit value from the given register
     *
     *   @param devReg The device's register's address.
     *   @param data Data to read. The data value is byteswapped if needed
     *
     *   @retval sfTkError_t -  ksfTkErrOk on successful execution.
     */
    sfTkError_t readRegisterUInt32(uint8_t devReg, uint32_t &data)
    {
        return readRegister(devReg, data);
    }

    /**
     * @brief Reads data from a specified register.
     *
     * This function reads a specified number of bytes from a given register and stores the data in the provided buffer.
     *
     * @param devReg The register address to read from.
     * @param data Pointer to the buffer where the read data will be stored.
     * @param numBytes The number of bytes to read from the register.
     * @param readBytes Reference to a variable where the number of bytes actually read will be stored.
     * @return sfTkError_t Error code indicating the success or failure of the read operation.
     */
    virtual sfTkError_t readRegister(uint8_t devReg, uint8_t *data, size_t numBytes, size_t &readBytes)
    {
        return readRegister(&devReg, sizeof(devReg), data, numBytes, readBytes);
    }

    /**
     * @brief Overloaded function to read a 8-bit value from the given 16-bit register address.
     *
     * This function reads a 8-bit value from the specified 16-bit register address.
     *
     * @param reg The device's 16-bit register's address. The address is byteswapped if needed
     * @param[out] value The 8-bit value read from the register.
     *
     * @return sfTkError_t Returns ksfTkErrOk on success, or an error code on failure.
     */
    virtual sfTkError_t readRegister(uint16_t reg, uint8_t &value)
    {
        size_t readBytes = 0;

        // this is a reg 16 address - so call the method to manage this. Note - it manages the
        // byte swapping of the address.
        sfTkError_t retValue = readRegister(reg, (uint8_t *)&value, sizeof(value), readBytes);

        return retValue;
    }

    /**
     * @brief Overloaded function to read a 8-bit value from the given 16-bit register address.
     *
     * This function reads a 8-bit value from the specified 16-bit register address.
     *
     * @param reg The device's 16-bit register's address. The address is byteswapped if needed
     * @param[out] value The 8-bit value read from the register.
     *
     * @return sfTkError_t Returns ksfTkErrOk on success, or an error code on failure.
     */
    sfTkError_t readRegisterUInt8(uint16_t devReg, uint8_t &data)
    {
        return readRegister(devReg, data);
    }

    /**
     * @brief Overloaded function to read a 16-bit value from the given 16-bit register address.
     *
     * This function reads a 16-bit value from the specified 16-bit register address.
     *
     * @param reg The device's 16-bit register's address. The address is byteswapped if needed.
     * @param[out] value The 16-bit value read from the register. The value is byte swapped if needed.
     *
     * @return sfTkError_t Returns ksfTkErrOk on success, or an error code on failure.
     */
    virtual sfTkError_t readRegister(uint16_t reg, uint16_t &value)
    {
        size_t readBytes = 0;

        // this is a reg 16 address - so call the method to manage this. Note - it manages the
        // byte swapping of the address.
        sfTkError_t retValue = readRegister(reg, (uint8_t *)&value, sizeof(value), readBytes);

        // The data is a uint16 - byte swap the result?
        if (retValue == ksfTkErrOk && sftk_system_byteorder() != _byteOrder)
            value = sftk_byte_swap(value);

        return retValue;
    }

    /**
     * @brief Overloaded function to read a 16-bit value from the given 16-bit register address.
     *
     * This function reads a 16-bit value from the specified 16-bit register address.
     *
     * @param reg The device's 16-bit register's address. The address is byteswapped if needed.
     * @param[out] value The 16-bit value read from the register. The value is byte swapped if needed.
     *
     * @return sfTkError_t Returns ksfTkErrOk on success, or an error code on failure.
     */
    sfTkError_t readRegisterUInt16(uint16_t devReg, uint16_t &data)
    {
        return readRegister(devReg, data);
    }

    /**
     * @brief Overloaded function to read a 32-bit value from the given 16-bit register address.
     *
     * This function reads a 32-bit value from the specified 16-bit register address.
     *
     * @param reg The device's 16-bit register's address. The address is byteswapped if needed.
     * @param[out] value The 32-bit value read from the register. The value is byte swapped if needed.
     *
     * @return sfTkError_t Returns ksfTkErrOk on success, or an error code on failure.
     */
    virtual sfTkError_t readRegister(uint16_t reg, uint32_t &value)
    {
        size_t readBytes = 0;

        // this is a reg 16 address - so call the method to manage this. Note - it manages the
        // byte swapping of the address.
        sfTkError_t retValue = readRegister(reg, (uint8_t *)&value, sizeof(uint32_t), readBytes);

        // The data is a uint32 - byte swap the result?
        if (retValue == ksfTkErrOk && sftk_system_byteorder() != _byteOrder)
            value = sftk_byte_swap(value);

        return retValue;
    }

    /**
     * @brief Overloaded function to read a 32-bit value from the given 16-bit register address.
     *
     * This function reads a 32-bit value from the specified 16-bit register address.
     *
     * @param reg The device's 16-bit register's address. The address is byteswapped if needed.
     * @param[out] value The 32-bit value read from the register. The value is byte swapped if needed.
     *
     * @return sfTkError_t Returns ksfTkErrOk on success, or an error code on failure.
     */
    sfTkError_t readRegisterUInt32(uint16_t devReg, uint32_t &data)
    {
        return readRegister(devReg, data);
    }

    // Overload version
    /**
     * @brief Reads a specified number of bytes from a given register with a 16bit address
     *
     * This function reads data from a 16-bit register and stores it in the provided buffer.
     *
     * @param devReg The 16-bit register address to read from. Byteswapped if needed.
     * @param data Pointer to the buffer where the read data will be stored.
     * @param numBytes The number of bytes to read from the register.
     * @param readBytes Reference to a variable where the number of bytes actually read will be stored.
     * @return sfTkError_t Error code indicating the success or failure of the read operation.
     */
    sfTkError_t readRegister(uint16_t devReg, uint8_t *data, size_t numBytes, size_t &readBytes)
    {
        // The address is a uint16 - byte swap?
        if (sftk_system_byteorder() != _byteOrder)
            devReg = sftk_byte_swap(devReg);

        return readRegister((uint8_t *)&devReg, sizeof(devReg), data, numBytes, readBytes);
    }

    /**--------------------------------------------------------------------------
     *  @brief Reads a 16-bit block of data from the given 16-bit register address.
     *
     *   @param reg The device's 16 bit register's address. Byteswapped if needed
     *   @param data Data area to read into - byteswapped if needed
     *   @param length - length of data (16 bit element count)
     *   @param[out] read16 - number of 16 bit words read
     *
     *   @retval int returns ksfTkErrOk on success, or ksfTkErrFail code
     *
     */
    virtual sfTkError_t readRegister(uint16_t reg, uint16_t *data, size_t length, size_t &read16)
    {
        sfTkError_t retValue = readRegister(reg, (uint8_t *)data, length * 2, read16);

        // Do we need to flip the byte order?
        if (retValue == ksfTkErrOk && sftk_system_byteorder() != _byteOrder)
        {
            for (size_t i = 0; i < length; i++)
                data[i] = sftk_byte_swap(data[i]);
        }
        read16 = read16 / 2; // convert to words
        return retValue;     // added return statement to return status
    }

    /**
     * @brief Get the type of the object.
     *
     * This virtual function returns the type of the object as an 8-bit unsigned integer.
     * The default implementation returns 0. It is expected that the subclass will define and
     * implement a type methodology
     *
     * @return uint8_t The type of the object.
     */
    virtual uint8_t type(void)
    {
        return 0;
    }
    /**
     * @brief Set the byte order for multi-byte data transfers
     *
     * @param order The byte order to set - set to either sfTkByteOrder::BigEndian or
     * sfTkByteOrder::LittleEndian
     */
    void setByteOrder(sfTkByteOrder order)
    {
        _byteOrder = order;
    }

    /**
     * @brief Get the current byte order
     *
     * @retval The current byte order
     */
    sfTkByteOrder byteOrder(void)
    {
        return _byteOrder;
    }

  protected:
    /**
     * Flag to manage byte swapping
     */
    sfTkByteOrder _byteOrder;
};
