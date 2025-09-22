/**
 * @file sfTkII2C.h
 * @brief Header file for the SparkFun Toolkit I2C Interface Definition.
 *
 * This file contains the interface declaration for the I2C communication bus
 *
 * @author SparkFun Electronics
 * @date 2024-2025
 * @copyright Copyright (c) 2024-2025, SparkFun Electronics Inc. This project is released under the MIT License.
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "sfTkIBus.h"

/**
 * @brief Interface that defines the I2C communication bus for the SparkFun Electronics Toolkit.
 *
 * The I2C bus interface extends the IBus interface and adds the ability to set and get the I2C
 * address and stop flag.
 */
const uint8_t ksfTkBusTypeI2C = 0x01;

class sfTkII2C : public sfTkIBus
{
  public:
    // set the address to No address and stop flag to true
    /**
     * @brief Constructor for the I2C bus
     */
    sfTkII2C() : _address{kNoAddress}, _stop{true}
    {
    }
    /**
     * @brief Constructor for the I2C bus with an address passed in
     *
     * @param addr
     */
    sfTkII2C(uint8_t addr) : _address{addr}
    {
    }

    /**--------------------------------------------------------------------------
        @brief A simple ping of the device at the set address

        @retval sfTkError_t - ok on success

    */
    virtual sfTkError_t ping() = 0;

    /**--------------------------------------------------------------------------
        @brief setter for the I2C address

        @param devAddr The device's address

    */
    virtual void setAddress(uint8_t devAddr)
    {
        _address = devAddr;
    }

    /**--------------------------------------------------------------------------
        @brief getter for the I2C address

        @retval uint8_t returns the address for the device

    */
    virtual uint8_t address(void)
    {
        return _address;
    }

    /**--------------------------------------------------------------------------
        @brief setter for I2C stop message (vs restarts)

        @param stop The value to set for "send stop"
    */
    virtual void setStop(bool stop)
    {
        _stop = stop;
    }

    /**--------------------------------------------------------------------------
        @brief getter for I2C stops message (vs restarts)

        @retval bool returns the value of "send stop"

    */
    virtual bool stop(void)
    {
        return _stop;
    }

    /**
     * @brief kNoAddress is a constant to indicate no address has been set
     */
    static constexpr uint8_t kNoAddress = 0;

    virtual uint8_t type(void)
    {
        return ksfTkBusTypeI2C;
    }

  private:
    uint8_t _address;
    bool _stop;
};

//};
