/**
 * @file sfTkISPI.h
 * @brief Header file for the SparkFun Toolkit SPI Interface Definition.
 *
 * This file contains the interface declaration for the SPI communication bus
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
 * @brief Interface that defines the SPI communication bus for the SparkFun Electronics Toolkit.
 *
 * The SPI bus interface extends the IBus interface and adds the ability to set and get the CS pin.
 */

const uint8_t ksfTkBusTypeSPI = 0x02;
class sfTkISPI : public sfTkIBus
{
  public:
    /**--------------------------------------------------------------------------
        @brief Constructor for the SPI bus

    */
    sfTkISPI() : _cs{kNoCSPin}
    {
    }

    /**--------------------------------------------------------------------------
        @brief Constructor for the SPI bus

        @param csPin The CS Pin for the device

    */
    sfTkISPI(uint8_t csPin) : _cs{csPin}
    {
    }

    /**--------------------------------------------------------------------------
        @brief setter for the CS Pin

        @param devCS The device's CS Pin

    */
    virtual void setCS(uint8_t devCS)
    {
        _cs = devCS;
    }

    /**--------------------------------------------------------------------------
        @brief getter for the cs pin

        @retval uint8_t returns the CS pin for the device

    */
    virtual uint8_t cs(void)
    {
        return _cs;
    }

    /**--------------------------------------------------------------------------
        @brief A constant for no CS pin
    */
    static constexpr uint8_t kNoCSPin = 0;

    virtual uint8_t type(void)
    {
        return ksfTkBusTypeSPI;
    }

  private:
    /** The internal storage of the _cs value*/
    uint8_t _cs;
};

//};
