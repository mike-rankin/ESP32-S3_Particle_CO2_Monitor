/**
 * @file sfTkArduino.cpp
 * @brief Implementation file for the SparkFun Toolkit Arduino layer
 *
 * This file contains the implementation for the SparkFun Toolkit library Arduino layer.
 *
 * @author SparkFun Electronics
 * @date 2024-2025
 * @copyright Copyright (c) 2024-2025, SparkFun Electronics Inc. This project is released under the MIT License.
 *
 * SPDX-License-Identifier: MIT
 */

#include <Arduino.h>

// Implements the sfToolkit functions for the Arduino platform

void sftk_delay_ms(uint32_t ms)
{
    delay(ms);
}

uint32_t sftk_ticks_ms(void)
{
    return millis();
}