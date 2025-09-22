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
 * @file bmv080.h
 *
 * @brief BMV080 sensor driver functions
 *
 *
 */

#ifndef BMV080_H_
#define BMV080_H_

#include <stdint.h>

#include "bmv080_defs.h"

#ifdef __cplusplus  
extern "C" {
#endif /* __cplusplus */

/*********************************************************************************************************************
* Sensor driver functions
*********************************************************************************************************************/

/*!
* @brief Open a sensor unit by initializing a new handle.
*
* @pre Must be called first in order to create the _handle_ required by other functions. 
* @post The _handle_ must be destroyed via _bmv080_close_.
*
* @param[out] handle : Unique handle for a sensor unit.
* @param[in] sercom_handle : Unique handle for a serial communication interface.
* @param[in] read : Function pointer for reading from an endpoint.
* @param[in] write : Function pointer for writing to an endpoint.
* @param[in] delay_ms : Function pointer for a delay in milliseconds.
*
* @return E_BMV080_OK if successful, otherwise the return value is a BMV080 status code.
*/
bmv080_status_code_t bmv080_open(bmv080_handle_t* handle, const bmv080_sercom_handle_t sercom_handle,
    const bmv080_callback_read_t read, const bmv080_callback_write_t write, const bmv080_callback_delay_t delay_ms);

/*!
* @brief Get the version information of this sensor driver.
*
* @details The version numbers follow <a href = "https://semver.org/#semantic-versioning-200">semantic versioning</a>.
*
* @pre No preconditions apply, i.e. no connected sensor unit or sensor driver _handle_ is required. 
*
* @param[out] major : Major version number.
* @param[out] minor : Minor version number.
* @param[out] patch : Patch version number.
* @param[out] git_hash :  Character array of 13 elements, for internal use only.
* @param[out] num_commits_ahead : For internal use only.
*
* @return E_BMV080_OK if successful, otherwise the return value is a BMV080 status code.
*/
bmv080_status_code_t bmv080_get_driver_version(uint16_t* major, uint16_t* minor, uint16_t* patch, char git_hash[12], int32_t* num_commits_ahead);

/*!
* @brief Reset a sensor unit including both hardware and software.
*
* @pre A valid _handle_ generated by _bmv080_open_ is required. 
* @post Any parameter changed through _bmv080_set_parameter_ is reverted back to its default.
*
* @param[in] handle : Unique handle for a sensor unit.
*
* @return E_BMV080_OK if successful, otherwise the return value is a BMV080 status code.
*/
bmv080_status_code_t bmv080_reset(const bmv080_handle_t handle);

/*!
* @brief Get a parameter.
*
* @details Table \ref parameter lists the available parameters with their keys and the expected types.
* This function can be called multiple times and is optional.
*
* <table>
* <caption id = "parameter">Table parameter : Available parameters </caption>
* <tr> <th> Key                             <th> Type                 <th> Unit      <th> Default                    <th> Details
* <tr> <td> "error_logging"                 <td> bool                 <td>           <td> false                      <td> Set if error log files are created.
*                                                                                                                    <br> Disabled if _false_, enabled if _true_.
* <tr> <td> "meta_data_logging"             <td> bool                 <td>           <td> false                      <td> Set if meta data log files are created.
*                                                                                                                    <br> Disabled if _false_, enabled if _true_.
* <tr> <td> "path"                          <td> char*                <td>           <td> "" (empty)                 <td> Path to directory where log files are written.
*                                                                                                                    <br> The maximum allowed length is 256 characters, which must be pre-allocated.
*                                                                                                                    <br> File logging is not available on embedded platforms (e.g. ARM Cortex-M).
*                                                                                                                    <br> This is applicable to all parameters with the suffix "_logging" in its name.
* <tr> <td> "pm_logging"                    <td> bool                 <td>           <td> false                      <td> Set if particulate matter output log files are created.
*                                                                                                                    <br> Disabled if _false_, enabled if _true_.
* <tr> <td> "integration_time"              <td> float                <td> s         <td> 10                         <td> Measurement window.
*                                                                                                                    <br> In duty cycling mode, this measurement window is also the sensor ON time.
* <tr> <td> "duty_cycling_period"           <td> uint16_t             <td> s         <td> 30                         <td> Duty cycling period (sum of integration time and sensor OFF / sleep time).
*                                                                                                                    <br> This must be greater than integration time by at least 2 seconds.
* <tr> <td> "do_obstruction_detection"      <td> bool                 <td>           <td> true                       <td> Set if obstruction detection feature is enabled.
* <tr> <td> "do_vibration_filtering"        <td> bool                 <td>           <td> false                      <td> Set if vibration filter is enabled.
* <tr> <td> "measurement_algorithm"         <td> bmv080_measurement_algorithm_t <td> <td> E_BMV080_MEASUREMENT_ALGORITHM_HIGH_PRECISION (3) <td> Selection of measurement algorithm based on the
*                                                                                                                    <br> use case, as defined by the type bmv080_measurement_algorithm_t,
*                                                                                                                    <br> in bmv080_defs.h. For a duty cycling measurement, this parameter is
*                                                                                                                    <br> fixed to E_BMV080_MEASUREMENT_ALGORITHM_FAST_RESPONSE.
* </table>
*
* @pre A valid _handle_ generated by _bmv080_open_ is required.
*
* @param[in] handle : Unique handle for a sensor unit.
* @param[in] key : Key of the parameter to get. Valid keys are listed in table \ref parameter.
* @param[out] value : Value of the parameter to get of the in table \ref parameter listed type casted as void-pointer.
*
* @return E_BMV080_OK if successful, otherwise the return value is a BMV080 status code.
*/
bmv080_status_code_t bmv080_get_parameter(const bmv080_handle_t handle, const char* key, void* value);

/*!
* @brief Set a parameter. 
*
* @details Table \ref parameter lists the available parameters with their keys and the expected types. 
* This function can be called multiple times and is optional.
*
* <table>
* <caption id = "parameter">Table parameter : Available parameters </caption>
* <tr> <th> Key                             <th> Type                 <th> Unit      <th> Default                    <th> Details
* <tr> <td> "error_logging"                 <td> bool                 <td>           <td> false                      <td> Set if error log files are created.
*                                                                                                                    <br> Disabled if _false_, enabled if _true_.
* <tr> <td> "meta_data_logging"             <td> bool                 <td>           <td> false                      <td> Set if meta data log files are created.
*                                                                                                                    <br> Disabled if _false_, enabled if _true_.
* <tr> <td> "path"                          <td> char*                <td>           <td> "" (empty)                 <td> Path to directory where log files are written.
*                                                                                                                    <br> The path length must be less than 256 characters.
*                                                                                                                    <br> Relative and absolute paths with separators ( / ) or ( \\ ) are supported.
*                                                                                                                    <br> ( . ) indicates the working directory and ( .. ) the parent directory.
*                                                                                                                    <br> File logging is not available on embedded platforms (e.g. ARM Cortex-M).
*                                                                                                                    <br> This is applicable to all parameters with the suffix "_logging" in its name.
* <tr> <td> "pm_logging"                    <td> bool                 <td>           <td> false                      <td> Set if particulate matter output log files are created.
*                                                                                                                    <br> Disabled if _false_, enabled if _true_.
* <tr> <td> "integration_time"              <td> float                <td> s         <td> 10                         <td> Measurement window. 
*                                                                                                                    <br> In duty cycling mode, this measurement window is also the sensor ON time.
* <tr> <td> "duty_cycling_period"           <td> uint16_t             <td> s         <td> 30                         <td> Duty cycling period (sum of integration time and sensor OFF / sleep time).
*                                                                                                                    <br> This must be greater than integration time by at least 2 seconds.
* <tr> <td> "do_obstruction_detection"      <td> bool                 <td>           <td> true                       <td> Set if obstruction detection feature is enabled.
* <tr> <td> "do_vibration_filtering"        <td> bool                 <td>           <td> false                      <td> Set if vibration filter is enabled.
* <tr> <td> "measurement_algorithm"         <td> bmv080_measurement_algorithm_t <td> <td> E_BMV080_MEASUREMENT_ALGORITHM_HIGH_PRECISION (3) <td> Selection of measurement algorithm based on the 
*                                                                                                                    <br> use case, as defined by the type bmv080_measurement_algorithm_t,
*                                                                                                                    <br> in bmv080_defs.h. For a duty cycling measurement, this parameter is
*                                                                                                                    <br> fixed to E_BMV080_MEASUREMENT_ALGORITHM_FAST_RESPONSE.
* </table>
*
* @pre A valid _handle_ generated by _bmv080_open_ is required.
* @pre This function must be called before _bmv080_start_continuous_measurement_ or _bmv080_start_duty_cycling_measurement_
*      in order to apply the parameter in the configuration of particle measurement.
*
* @param[in] handle : Unique handle for a sensor unit.
* @param[in] key : Key of the parameter to set. Valid keys are listed in table \ref parameter.
* @param[in] value : Value of the parameter to set of the in table \ref parameter listed type casted as void-pointer.
*
* @return E_BMV080_OK if successful, otherwise the return value is a BMV080 status code.
*/
bmv080_status_code_t bmv080_set_parameter(const bmv080_handle_t handle, const char* key, const void* value);

/*!
* @brief Get the sensor ID of a sensor unit.
*
* @pre A valid _handle_ generated by _bmv080_open_ is required.
* @pre The char array _id_ must have been allocated by the application with a size of 13 elements.
*
* @param[in] handle : Unique handle for a sensor unit.
* @param[out] id : Character array of 13 elements.
*
* @return E_BMV080_OK if successful, otherwise the return value is a BMV080 status code.
*/
bmv080_status_code_t bmv080_get_sensor_id(const bmv080_handle_t handle, char id[13]);

/*!
* @brief Start particle measurement in continuous mode.
*
* @pre A valid _handle_ generated by _bmv080_open_ is required.
* @pre Optionally, parameters can be set by preceding _bmv080_set_parameter_ calls.
* @post The measurement mode increases energy consumption.
* @post The sensor unit stays in measurement mode until _bmv080_stop_measurement_ is called.
* @post In measurement mode, _bmv080_serve_interrupt_ should be called regularly.
*
* @param[in] handle : Unique handle for a sensor unit.
*
* @return E_BMV080_OK if successful, otherwise the return value is a BMV080 status code.
*/
bmv080_status_code_t bmv080_start_continuous_measurement(const bmv080_handle_t handle);

/*!
* @brief Start particle measurement in duty cycling mode
*
* @pre A valid _handle_ generated by _bmv080_open_ is required.
* @pre Optionally, duty cycling parameters (integration_time and duty_cycling_period) can be set by preceding _bmv080_set_parameter_ calls.
* @post The sensor unit stays in duty cycling mode until _bmv080_stop_measurement_ is called.
* @post In measurement mode, _bmv080_serve_interrupt_ should be called regularly.
*
* @param[in] handle : Unique handle for a sensor unit.
* @param[in] get_tick_ms : Function pointer that provides a tick value in milliseconds (based on the host system clock).
* @param[in] duty_cycling_mode : Mode of performing the duty cycling measurement.
*
* @return E_BMV080_OK if successful, otherwise the return value is a BMV080 status code.
*/
bmv080_status_code_t bmv080_start_duty_cycling_measurement(const bmv080_handle_t handle, const bmv080_callback_tick_t get_tick_ms, 
    bmv080_duty_cycling_mode_t duty_cycling_mode);

/*!
* @brief Serve an interrupt using a callback function.
*
* @pre A valid _handle_ generated by _bmv080_open_ is required and 
*      the sensor unit entered measurement mode via _bmv080_start_continuous_measurement_ or _bmv080_start_duty_cycling_measurement_.
* @pre The application can call this function every time the sensor or the application triggers an interrupt.
*      This interrupt may be a type of a software timeout (e.g. at least once per second) or a hardware interrupt (e.g. FIFO watermark exceeded).
* @pre This function tolerates very frequent or random calls to a certain extent. 
*      However, not calling _bmv080_serve_interrupt_ over longer periods might impair the measurement mode since events might be missed.
* @pre In continuous mode, new sensor output is available every second
*      Hence, _data_ready_ is called once every second of the sensor unit's uptime.
*      For example, if _bmv080_serve_interrupt_ is called 5 seconds after _bmv080_start_continuous_measurement_, 
*      the callback function _data_ready_ would subsequently be called 5 times to report the collected sensor output of each period.
* @pre In duty cycling mode, new sensor output is available every duty cycling period.
*      Hence, _data_ready_ is called at the end of the integration time, once every duty cycling period.
*      Note, _bmv080_serve_interrupt_ must be called at least once per second. 
* @pre The recommendation is to call this function based on hardware interrupts.
* @pre The callback function _bmv080_callback_data_ready_t_ and the according _callback_parameter_ are provided by the caller application.
* @post Interrupt condition is served, e.g. FIFO is fetched or ASIC condition is solved
*
* @param[in] handle : Unique handle for a sensor unit.
* @param[in] data_ready : User defined callback function which is called when sensor output is available.
* @param[in] callback_parameters : User defined parameters to be passed to the callback function.
*
* @return E_BMV080_OK if successful, otherwise the return value is a BMV080 status code.
*/
bmv080_status_code_t bmv080_serve_interrupt(const bmv080_handle_t handle, bmv080_callback_data_ready_t data_ready, void* callback_parameters);

/*!
* @brief Stop particle measurement.
*
* @pre A valid _handle_ generated by _bmv080_open_ is required and
*      the sensor unit entered measurement mode via _bmv080_start_continuous_measurement_ or _bmv080_start_duty_cycling_measurement_.
* @pre Must be called at the end of a data acquisition cycle to ensure that the sensor unit is ready for the next measurement cycle.
*
* @param[in] handle : Unique handle for a sensor unit.
*
* @return E_BMV080_OK if successful, otherwise the return value is a BMV080 status code.
*/
bmv080_status_code_t bmv080_stop_measurement(const bmv080_handle_t handle);

/*!
* @brief Close the sensor unit.
*
* @pre Must be called last in order to destroy the _handle_ created by _bmv080_open_.
*
* @param[in] handle : Unique handle for a sensor unit.
*
* @return E_BMV080_OK if successful, otherwise the return value is a BMV080 status code.
*/
bmv080_status_code_t bmv080_close(bmv080_handle_t* handle);

#ifdef __cplusplus  
}
#endif /* __cplusplus */

#endif /* BMV080_H_ */
