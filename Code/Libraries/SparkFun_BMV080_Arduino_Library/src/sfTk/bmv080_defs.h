/**
 * Copyright (C) Bosch Sensortec GmbH. All Rights Reserved. Confidential.
 *
 * Disclaimer
 *
 * Common:
 * Bosch Sensortec products are developed for the consumer goods industry. They may only be used
 * within the parameters of the respective valid product data sheet. Bosch Sensortec products are
 * provided with the express understanding that there is no warranty of fitness for a particular purpose.
 * They are not fit for use in life-sustaining, safety or security sensitive systems or any system or device
 * that may lead to bodily harm or property damage if the system or device malfunctions. In addition,
 * Bosch Sensortec products are not fit for use in products which interact with motor vehicle systems.
 * The resale and/or use of products are at the purchaser's own risk and his own responsibility. The
 * examination of fitness for the intended use is the sole responsibility of the Purchaser.
 *
 * The purchaser shall indemnify Bosch Sensortec from all third party claims, including any claims for
 * incidental, or consequential damages, arising from any product use not covered by the parameters of
 * the respective valid product data sheet or not approved by Bosch Sensortec and reimburse Bosch
 * Sensortec for all costs in connection with such claims.
 *
 * The purchaser must monitor the market for the purchased products, particularly with regard to
 * product safety and inform Bosch Sensortec without delay of all security relevant incidents.
 *
 * Engineering Samples are marked with an asterisk (*) or (e). Samples may vary from the valid
 * technical specifications of the product series. They are therefore not intended or fit for resale to third
 * parties or for use in end products. Their sole purpose is internal client testing. The testing of an
 * engineering sample may in no way replace the testing of a product series. Bosch Sensortec
 * assumes no liability for the use of engineering samples. By accepting the engineering samples, the
 * Purchaser agrees to indemnify Bosch Sensortec from all claims arising from the use of engineering
 * samples.
 *
 * Special:
 * This software module (hereinafter called "Software") and any information on application-sheets
 * (hereinafter called "Information") is provided free of charge for the sole purpose to support your
 * application work. The Software and Information is subject to the following terms and conditions:
 *
 * The Software is specifically designed for the exclusive use for Bosch Sensortec products by
 * personnel who have special experience and training. Do not use this Software if you do not have the
 * proper experience or training.
 *
 * This Software package is provided `` as is `` and without any expressed or implied warranties,
 * including without limitation, the implied warranties of merchantability and fitness for a particular
 * purpose.
 *
 * Bosch Sensortec and their representatives and agents deny any liability for the functional impairment
 * of this Software in terms of fitness, performance and safety. Bosch Sensortec and their
 * representatives and agents shall not be liable for any direct or indirect damages or injury, except as
 * otherwise stipulated in mandatory applicable law.
 *
 * The Information provided is believed to be accurate and reliable. Bosch Sensortec assumes no
 * responsibility for the consequences of use of such Information nor for any infringement of patents or
 * other rights of third parties which may result from its use. No license is granted by implication or
 * otherwise under any patent or patent rights of Bosch. Specifications mentioned in the Information are
 * subject to change without notice.
 *
 * It is not allowed to deliver the source code of the Software to any third party without permission of
 * Bosch Sensortec.
 *
 * @file bmv080_defs.h
 *
 * @brief BMV080 sensor driver definitions
 *
 *
 */

#ifndef BMV080_DEFS_H_
#define BMV080_DEFS_H_

#include <stdint.h>
#include <stdbool.h>

/*********************************************************************************************************************
* Type definitions
*********************************************************************************************************************/

/*!
* @brief Status codes returned by the BMV080 sensor driver.
*/
typedef enum 
{
    /* Ok ***********************************************************************************************************/

    /*! 0: Default result on success or if there is no warning / error. */
    E_BMV080_OK = 0,

    /* Warnings *****************************************************************************************************/

    /*! 1: Misuse detected in BMV080 API integration. */
    E_BMV080_WARNING_INVALID_REG_READ = 1,
    /*! 2: Misuse detected in BMV080 API integration. */
    E_BMV080_WARNING_INVALID_REG_WRITE = 2,

    /*! 3: Misuse detected in BMV080 API integration.
     *  Hint: Please review SPI / I2C implementation, specifically multi-word reads.
     */
    E_BMV080_WARNING_FIFO_READ = 3,

    /*! 4: Too many particle / obstruction events detected than can be processed by bmv080_serve_interrupt API.
     *  Hint: Call bmv080_serve_interrupt more frequently, or switch from polling to interrupt based use,
     *  or remove potential obstructions from field of view.
     */
    E_BMV080_WARNING_FIFO_EVENTS_OVERFLOW = 4,

    /*! 208: Misuse detected in BMV080 API integration. */
    E_BMV080_WARNING_FIFO_SW_BUFFER_SIZE = 208,
    /*! 209: Too many particle / obstruction events detected than can be processed by bmv080_serve_interrupt API.
     *  Hint: Call bmv080_serve_interrupt more frequently, or switch from polling to interrupt based use,
     *  or remove potential obstructions from field of view.
     */
    E_BMV080_WARNING_FIFO_HW_BUFFER_SIZE = 209,

    /* Errors *******************************************************************************************************/

    /*! 100: Misuse detected in BMV080 API integration. A reference points to an invalid / null address.
     *  Hint: Please check the pointer arguments of the API e.g. device handle.
     */
    E_BMV080_ERROR_NULLPTR = 100,

    /*! 101: Misuse detected in BMV080 API integration. */
    E_BMV080_ERROR_REG_ADDR = 101,

    /*! 179: The sensor parameter is locked during measurement.
     *  Hint: Please configure BMV080 file logging (e.g. path) before starting a measurement.
     *  This is valid for general purpose platforms.
     */
    E_BMV080_ERROR_PARAM_LOCKED = 179,
    /*! 115: The parameter key is invalid.
     *  Hint: Invalid key provided to bmv080_set_parameters or bmv080_get_parameters APIs.
     */
    E_BMV080_ERROR_PARAM_INVALID = 115,
    /*! 102: Misuse detected in BMV080 API integration */
    E_BMV080_ERROR_PARAM_INVALID_CHANNEL = 102,
    /*! 123: The parameter's value is invalid.
     *  Hint: Please provide correct value to bmv080_set_parameters API (e.g. duty_cycling_period)
     *  as specified in the API documentation.
     */
    E_BMV080_ERROR_PARAM_INVALID_VALUE = 123,
    /*! 104: Misuse detected in BMV080 API integration. */
    E_BMV080_ERROR_PARAM_INVALID_INTERNAL_CONFIG = 104,

    /*! 180: An API call is invalid because a precondition is still unsatisfied.
     * Hint: Please ensure correct 'API calling sequence' or handle previously occurred errors.
     */
    E_BMV080_ERROR_PRECONDITION_UNSATISFIED = 180,

    /*! 105: Reading via the provided hardware communication interface failed.
     *  Hint: Please review communication or implementation of SPI / I2C read function.
     */
    E_BMV080_ERROR_HW_READ = 105,
    /*! 106: Writing via the provided hardware communication interface failed.
     *  Hint: Please review communication or implementation of SPI / I2C write function.
     */
    E_BMV080_ERROR_HW_WRITE = 106,

    /*! 107: The immutable "chip id" changed.
     *  Hint: Please review communication or implementation of SPI / I2C interface.
     */
    E_BMV080_ERROR_MISMATCH_CHIP_ID = 107,
    /*! 160: Misuse detected in BMV080 API integration. */
    E_BMV080_ERROR_MISMATCH_REG_VALUE = 160,

    /*! 116: The requested operation mode is invalid.
     *  Hint: Please check that power provided to BMV080 is sufficient & stable.
     */
    E_BMV080_ERROR_OPERATION_MODE_INVALID = 116,
    /*! 113: Changing to the requested operation mode failed.
     *  Hint: Please check that power provided to BMV080 is sufficient & stable.
     */
    E_BMV080_ERROR_OPERATION_MODE_CHANGE = 113,
    /*! 114: The operation modes of different channels are out of sync.
     *  Hint: Please check that power provided to BMV080 is sufficient & stable.
     */
    E_BMV080_ERROR_OPERATION_MODE_CHANNELS_OUT_OF_SYNC = 114,
    /*! 157: Misuse detected in BMV080 API integration.
     *  Hint: Please contact BST customer support team.
     */
    E_BMV080_ERROR_ASIC_NOT_CONFIGURED = 157,

    /*! 133: Memory read failed.
     *  Hint: Please check that power provided to BMV080 is sufficient & stable.
     */
    E_BMV080_ERROR_MEM_READ = 133,
    /*! 135: Misuse detected in BMV080 API integration.
     *  Hint: Please contact BST customer support team.
     */
    E_BMV080_ERROR_MEM_ADDRESS = 135,
    /*! 136: Memory command failed.
     *  Hint: Please check that power provided to BMV080 is sufficient & stable.
     */
    E_BMV080_ERROR_MEM_CMD = 136,
    /*! 137: Memory access timeout.
     *  Hint: Please check that power provided to BMV080 is sufficient & stable.
     */
    E_BMV080_ERROR_MEM_TIMEOUT = 137,
    /*! 138: Misuse detected in BMV080 API integration. */
    E_BMV080_ERROR_MEM_INVALID = 138,
    /*! 139: Misuse detected in BMV080 API integration. */
    E_BMV080_ERROR_MEM_OBSOLETE = 139,
    /*! 140: Changing to the requested operation mode failed.
     *  Hint: Please check that power provided to BMV080 is sufficient & stable.
     */
    E_BMV080_ERROR_MEM_OPERATION_MODE = 140,
    /*! 153: Memory data integrity check failed.
     *  Hint: Please contact BST customer support team.
     */
    E_BMV080_ERROR_MEM_DATA_INTEGRITY = 153,
    /*! 154: Memory data integrity check for internal test 1 region failed.
     *  Hint: Please contact BST customer support team.
     */
    E_BMV080_ERROR_MEM_DATA_INTEGRITY_INTERNAL_TEST_1 = 154,
    /*! 156: Memory check for internal test 1 region failed.
     *  Hint: Please contact BST customer support team.
     */
    E_BMV080_ERROR_MEM_INTERNAL_TEST_1 = 156,
    /*! 159: Memory data integrity check for internal test 2 region failed.
     *  Hint: Please contact BST customer support team.
     */
    E_BMV080_ERROR_MEM_DATA_INTEGRITY_INTERNAL_TEST_2 = 159,
    /*! 181: Memory check for internal test 2 region failed.
     *  Hint: Please contact BST customer support team.
     */
    E_BMV080_ERROR_MEM_INTERNAL_TEST_2 = 181,

    /*! 210: Misuse detected in BMV080 API integration.
     *  Hint: Please review SPI / I2C implementation.
     */
    E_BMV080_ERROR_FIFO_FORMAT = 210,
    /*! 213: Too many particle / obstruction events detected than can be processed by bmv080_serve_interrupt API.
     *  Hint: Call bmv080_serve_interrupt more frequently, or switch from polling to interrupt based use,
     *  or remove potential obstructions from field of view.
     */
    E_BMV080_ERROR_FIFO_EVENTS_COUNT_SATURATED = 213,
    /*! 174: Misuse detected in BMV080 API integration.
     *  Hint: Please review SPI / I2C implementation and check that power provided to BMV080 is sufficient & stable.
     */
    E_BMV080_ERROR_FIFO_UNAVAILABLE = 174,
    /*! 214: Misuse detected in BMV080 API integration.
     *  Hint: Please contact BST customer support team.
     */
    E_BMV080_ERROR_FIFO_EVENTS_COUNT_DIFF = 214,

    /*! 161: An error occurred regarding the communication synchronization.
     *  Hint: Please check that power provided to BMV080 is sufficient & stable.
     *  If the problem is not resolved, please contact BST customer support team.
     */
    E_BMV080_ERROR_SYNC_COMM = 161,
    /*! 162: An error occurred regarding the control synchronization.
     *  Hint: Please check that power provided to BMV080 is sufficient & stable.
     *  If the problem is not resolved, please contact BST customer support team.
     */
    E_BMV080_ERROR_SYNC_CTRL = 162,
    /*! 163: An error occurred regarding the measurement synchronization.
     *  Hint: Please check that power provided to BMV080 is sufficient & stable.
     *  If the problem is not resolved, please contact BST customer support team.
     */
    E_BMV080_ERROR_SYNC_MEAS = 163,
    /*! 164: An error occurred regarding a locked synchronization.
     *  Hint: Please check that power provided to BMV080 is sufficient & stable.
     *  If the problem is not resolved, please contact BST customer support team.
     */
    E_BMV080_ERROR_SYNC_LOCKED = 164,
    /*! 165: An error occurred regarding the DC cancellation range.
     *  Hint: Please check thermal integration and/or remove potential obstructions from field of view.
     *  If the problem is not resolved, please contact BST customer support team.
     */
    E_BMV080_ERROR_DC_CANCEL_RANGE = 165,
    /*! 166: An error occurred regarding the DC estimation range.
     *  Hint: Please check thermal integration and/or remove potential obstructions from field of view.
     *  If the problem is not resolved, please contact BST customer support team.
     */
    E_BMV080_ERROR_DC_ESTIM_RANGE = 166,
    /*! 167: Misuse detected in BMV080 API integration.
     *  Hint: Please contact BST customer support team.
     */
    E_BMV080_ERROR_LPWR_T = 167,
    /*! 168: An error occurred regarding LPWR range.
     *  Hint: Please check thermal integration sensor wear-out, humidity,
     *  and/or remove potential obstructions from field of view.
     *  If the problem is not resolved, please contact BST customer support team.
     */
    E_BMV080_ERROR_LPWR_RANGE = 168,
    /*! 169: An error occurred regarding the power domain.
     *  Hint: Please check that power provided to BMV080 is sufficient & stable (VDDL, VDDA or VSS).
     *  If the problem is not resolved, please contact BST customer support team.
     */
    E_BMV080_ERROR_POWER_DOMAIN = 169,
    /*! 170: An error occurred regarding the headroom detection.
     *  Hint: Please check that power provided to BMV080 is sufficient & stable (VDDL).
     *  If the problem is not resolved, please contact BST customer support team.
     */
    E_BMV080_ERROR_HEADROOM_VDDL = 170,
    /*! 171: An error occurred regarding the headroom LDV output.
     *  Hint: Please check that power provided to BMV080 is sufficient & stable (VDDL).
     *  If the problem is not resolved, please contact BST customer support team.
     */
    E_BMV080_ERROR_HEADROOM_LDV_OUTPUT = 171,
    /*! 172: An error occurred regarding the headroom LDV reference.
     *  Hint: Please check that power provided to BMV080 is sufficient & stable (VDDL).
     *  If the problem is not resolved, please contact BST customer support team.
     */
    E_BMV080_ERROR_HEADROOM_LDV_REF = 172,
    /*! 173: An error occurred regarding the user headroom detection.
     *  Hint: Please contact BST customer support team.
     */
    E_BMV080_ERROR_HEADROOM_INTERNAL = 173,
    /*! 120: An error occurred due to a safety precaution mechanism.
     *  Hint: Please contact BST customer support team.
     */
    E_BMV080_ERROR_SAFETY_PRECAUTION = 120,
    /*! 211: Possible communication issue on SPI/I2C 
     *  or too many particle / obstruction events detected than can be processed by bmv080_serve_interrupt API.
     *  Hint: Please review communication or implementation of SPI / I2C interface.
     */
    E_BMV080_ERROR_TIMESTAMP_DIFFERENCE = 211,
    /*! 212: The timestamp is smaller than the previous timestamp.
     *  Hint: Please review communication or implementation of SPI / I2C interface.
     */
    E_BMV080_ERROR_TIMESTAMP_OVERFLOW = 212,

    /*! 300: BMV080 libraries are incompatible with each other.
     *  Hint: Please use compatible API libraries from the same SW package.
     */
    E_BMV080_ERROR_LIB_VERSION_INCOMPATIBLE = 300,
    /*! 301: Hint: Please contact BST customer support team. */
    E_BMV080_ERROR_INTERNAL_PARAMETER_VERSION_INVALID = 301,
    /*! 302: Hint: Please contact BST customer support team. */
    E_BMV080_ERROR_INTERNAL_PARAMETER_INDEX_INVALID = 302,

    /*! 403: FIFO level invalid leading to memory allocation issue.
     *  Hint: Please review communication or implementation of SPI / I2C read function.
     *  If the problem is not resolved, please contact BST customer support team.
     */
    E_BMV080_ERROR_MEMORY_ALLOCATION = 403,

    /*! 303: A linked delay callback function produced an error.
     *  Hint: callback function of type bmv080_callback_delay_t does not return 0,
     *  when it is called internally to perform a delay operation. 
     *  Please check implementation of the delay function.
     */
    E_BMV080_ERROR_CALLBACK_DELAY = 303,

    /*! 418: Sensor driver is not compatible with the used sensor HW sample.
     *  Hint: Please contact BST customer support team.
     */
    E_BMV080_ERROR_INCOMPATIBLE_SENSOR_HW = 418
}bmv080_status_code_t;

/*!
* @brief Unique handle for a sensor unit.
*/
typedef void* bmv080_handle_t;

/*!
* @brief Unique handle for a serial communication, i.e. the hardware connection to the sensor unit.
*/
typedef void* bmv080_sercom_handle_t;

/*!
* @brief Modes of performing a duty cycling measurement.
*/
typedef enum
{
    /*! 0: Mode with fixed duty cycle, ON time = integration_time and OFF time = sleep time */
    E_BMV080_DUTY_CYCLING_MODE_0 = 0
} bmv080_duty_cycling_mode_t;

/*!
* @brief Measurement algorithm choices.
*/
typedef enum
{
    E_BMV080_MEASUREMENT_ALGORITHM_FAST_RESPONSE = 1,
    E_BMV080_MEASUREMENT_ALGORITHM_BALANCED = 2,
    E_BMV080_MEASUREMENT_ALGORITHM_HIGH_PRECISION = 3
} bmv080_measurement_algorithm_t;

/*!
* @brief Placeholder structure for extended sensor output information.
*/
struct bmv080_extended_info_s;

/*!
* @brief Output structure which is updated by bmv080_serve_interrupt when sensor output is available.
*/
typedef struct
{
    /*! runtime_in_sec: estimate of the time passed since the start of the measurement, in seconds */
    float runtime_in_sec;
    /*! pm2_5_mass_concentration: PM2.5 value in ug/m3 */
    float pm2_5_mass_concentration;
    /*! pm1_mass_concentration: PM1 value in ug/m3 */
    float pm1_mass_concentration;
    /*! pm10_mass_concentration: PM10 value in ug/m3 */
    float pm10_mass_concentration;

    /*! pm2_5_number_concentration: PM2.5 value in particles/cm3 */
    float pm2_5_number_concentration;
    /*! pm1_number_concentration: PM1 value in particles/cm3 */
    float pm1_number_concentration;
    /*! pm10_number_concentration: PM10 value in particles/cm3 */
    float pm10_number_concentration;
    
    /*! is_obstructed: flag to indicate whether the sensor is obstructed and cannot perform a valid measurement */
    bool is_obstructed;
    /*! is_outside_measurement_range: flag to indicate whether the PM2.5 concentration is 
     * outside the specified measurement range (0..1000 ug/m3) 
     */
    bool is_outside_measurement_range;
    /*! reserved_0: for internal use only */
    float reserved_0;
    /*! reserved_1: for internal use only */
    float reserved_1;
    /*! reserved_2: for internal use only */
    float reserved_2;
    /*! extended_info: for internal use only */
    struct bmv080_extended_info_s *extended_info;
}bmv080_output_t;

/*********************************************************************************************************************
* Callback definitions
*********************************************************************************************************************/

/*!
* @brief Function pointer for reading an array of _payload_length_ words of 16 bit _payload_.
*
* @details All data, _header_ and _payload_, is transferred as MSB first.
*
* @pre Both _header_ and _payload_ words are 16 bit and combined.
*      A _payload_ is only transferred on a complete transmission of 16 bits.
* @pre Burst transfers, i.e. reading a _header_ followed by several _payload_ elements, must be supported.
*
* @param[in] sercom_handle : Handle for a serial communication interface to a specific sensor unit.
* @param[in] header : Header information for the following _payload_.
* @param[out] payload : Payload to be read consisting of 16 bit words.
* @param[in] payload_length : Number of _payload_ elements to be read.
*
* @return Zero if successful, otherwise the return value is an externally defined error code.
*/
typedef int8_t(*bmv080_callback_read_t)(bmv080_sercom_handle_t sercom_handle, uint16_t header, uint16_t* payload,
    uint16_t payload_length);

/*!
* @brief Function pointer for writing an array of _payload_length_ words of 16 bit _payload_.
*
* @details All data, _header_ and _payload_, is transferred as MSB first.
* 
* @pre Both _header_ and _payload_ words are 16 bit.
*      A _payload_ is only transferred on a complete transmission of 16 bits.
* @pre Burst transfers, i.e. writing a _header_ followed by several _payload_ elements, must be supported.
*
* @param[in] sercom_handle : Handle for a serial communication interface to a specific sensor unit.
* @param[in] header : Header information for the following _payload_.
* @param[in] payload : Payload to be written consisting of 16 bit words.
* @param[in] payload_length : Number of _payload_ elements to be written.
*
* @return Zero if successful, otherwise the return value is an externally defined error code.
*/
typedef int8_t(*bmv080_callback_write_t)(bmv080_sercom_handle_t sercom_handle, uint16_t header, const uint16_t* payload,
    uint16_t payload_length);

/*!
* @brief Function pointer for executing a software delay operation.
*
* @param[in] duration_in_ms : Duration of the delay in milliseconds.
*
* @return Zero if successful, otherwise the return value is an externally defined error code.
*/
typedef int8_t(*bmv080_callback_delay_t)(uint32_t duration_in_ms);

/*!
* @brief Function pointer for getting a tick value in milliseconds (based on the host system clock). 
*
* @details This serves as a measure of the elapsed time since start up,
*          .e.g uint32_t HAL_GetTick(void) of the STM32 HAL Framework or uint32_t GetTickCount(void) of the Windows API.
*
* @return Tick value in milliseconds
*/
typedef uint32_t(*bmv080_callback_tick_t)(void);

/*!
* @brief Function pointer for handling the sensor's output information.
*
* @param[in] bmv080_output : Structure containing sensor output.
* @param[inout] callback_parameters : user defined parameters to be passed to the callback function.
*/
typedef void(*bmv080_callback_data_ready_t)(bmv080_output_t bmv080_output, void* callback_parameters);

#endif /* BMV080_DEFS_H_ */
