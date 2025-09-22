/**
 * @file sfTkError.h
 * @brief Header file for the SparkFun Toolkit - Base Error Code defines.
 *
 * This file contains the base error code definitions for the SparkFun Toolkit library.
 *
 * @author SparkFun Electronics
 * @date 2024-2025
 * @copyright Copyright (c) 2024-2025, SparkFun Electronics Inc. This project is released under the MIT License.
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <stdint.h>

/**
 * General Concept
 *    A SparkFun Toolkit error system. The goal is to keep this simple.
 *
 *    This mimics a variety of systems, using an int type for error codes,
 *    where:
 *   		0   = okay
 *         -1   = general failure
 *         >0   = an informative error
 *
 *    Since *subsystems* in the toolkit can have their own errors,
 *    A start range for these errors are defined. Values > than this value
 *    define the errors for the set subsystem. These start ranges are set
 *    in this file, with actual error values defined in the the respective
 *    subsystem header files.
 *
 */
typedef int32_t sfTkError_t;

// General errors

/**
 * @brief General error code for a failure. Note all errors are negative.
 */
const sfTkError_t ksfTkErrFail = -1; // general fail
/**
 * @brief The error code value for success. This is always 0.
 */
const sfTkError_t ksfTkErrOk = 0; // success

/**
 * @brief Invalid parameter error
 */
const sfTkError_t ksfTkErrInvalidParam = -2; // invalid parameter error

/**
 * @brief A base value for bus errors. All bus errors are greater than this value, in the 1000 range
 */
const sfTkError_t ksfTkErrBaseBus = 0x1000;
