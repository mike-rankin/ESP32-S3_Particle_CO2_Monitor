
/**
 * @file sfTkArduino.h
 * @brief header file for the SparkFun Toolkit Arduino layer
 *
 * This file contains the header for the SparkFun Toolkit library Arduino layer.
 *
 * @author SparkFun Electronics
 * @date 2024-2025
 * @copyright Copyright (c) 2024-2025, SparkFun Electronics Inc. This project is released under the MIT License.
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

// Purpose:
//
// "Arduino-ized" value of toolkit values constants"

// Just include the toolkit headers

#include <sfTk/sfToolkit.h>

// Arduino-ize our byte order types

const sfTkByteOrder SFTK_MSBFIRST = sfTkByteOrder::BigEndian;
const sfTkByteOrder SFTK_LSBFIRST = sfTkByteOrder::LittleEndian;