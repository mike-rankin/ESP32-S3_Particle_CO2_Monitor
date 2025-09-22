/**
 * @file sfToolkit.cpp
 * @brief Implementation file for the SparkFun Toolkit.
 *
 * This file contains the implementation of various utility functions
 * for the SparkFun Toolkit library.
 *
 * @author SparkFun Electronics
 * @date 2024-2025
 * @copyright Copyright (c) 2024-2025, SparkFun Electronics Inc. This project is released under the MIT License.
 *
 * SPDX-License-Identifier: MIT
 */

#include "sfToolkit.h"

#ifdef ARDUINO
#include <Arduino.h>
#endif
//---------------------------------------------------------------------------------
/**
 * @brief C function - Runtime check for system byte order
 */
sfTkByteOrder sftk_system_byteorder(void)
{
    uint16_t i = 1;
    return *((uint8_t *)&i) == 0 ? sfTkByteOrder::BigEndian : sfTkByteOrder::LittleEndian;
}

//---------------------------------------------------------------------------------
/**
 * @brief to catch 8 bit calls for byte swap
 *
 */
uint8_t sftk_byte_swap(uint8_t i)
{
    return i;
}
//---------------------------------------------------------------------------------
/**
 * @brief function - Byte swap a 16 bit value
 */
uint16_t sftk_byte_swap(uint16_t i)
{
    // Use the fast intrinsic if available
#if defined(__clang__) || defined(__GNUC__)
    return __builtin_bswap16(i);
#else
    return (i << 8) | (i >> 8);
#endif
}

//---------------------------------------------------------------------------------
/**
 * @brief function - Byte swap a 32 bit value
 */
uint32_t sftk_byte_swap(uint32_t i)
{
#if defined(__clang__) || defined(__GNUC__)
    return __builtin_bswap32(i);
#else
    return ((i << 24) & 0xff000000) | ((i << 8) & 0x00ff0000) | ((i >> 8) & 0x0000ff00) | ((i >> 24) & 0x000000ff);
#endif
}

//---------------------------------------------------------------------------------
/**
 * @brief function - Byte swap an int 16
 */
int16_t sftk_byte_swap(int16_t i)
{
    uint16_t tmp = sftk_byte_swap(*(uint16_t *)&i);
    return *(int16_t *)&tmp;
}

//---------------------------------------------------------------------------------
/**
 * @brief function - Byte swap an int 32
 */
int32_t sftk_byte_swap(int32_t i)
{
    uint32_t tmp = sftk_byte_swap(*(uint32_t *)&i);
    return *(int32_t *)&tmp;
}