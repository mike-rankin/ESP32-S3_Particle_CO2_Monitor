/**
 * @file sfDevBMV080.h
 * @brief Header file for the SparkFun BMV080 Library
 *
 * This file contains the header of the sfDevBMV080 class, which provides
 * an interface to the BMV080 sensor. It includes methods for initializing the sensor,
 * reading sensor data, and configuring sensor settings.
 *
 * @author Pete Lewis
 * @date 2025
 * @version 1.0
 * @copyright (c) 2025 SparkFun Electronics Inc. This project is released under the MIT License.
 *
 * SPDX-License-Identifier: MIT
 *
 * @see sfDevBMV080.cpp
 */

#pragma once

#if __has_include("bmv080.h")
#include "bmv080.h"
#include "bmv080_defs.h"
#else
#error                                                                                                                 \
    "BOSCH BMV080 SDK Must be installed.  See instructions at www.github.com/sparkfun/SparkFun_BMV080_Arduino_Library - ERROR: bmv080.h not found"

#endif

// Include the platform independent layer of the SparkFun Toolkit
#include <sfTk/sfTkIBus.h>
#include <sfTk/sfToolkit.h>
#include <stdint.h>

#define SF_BMV080_DEFAULT_ADDRESS 0x57
#define SF_BMV080_DEFAULT_IRQ_PIN 14

#define SF_BMV080_MODE_CONTINUOUS 0
#define SF_BMV080_MODE_DUTY_CYCLE 1

class sfDevBMV080
{
  public:
    /// @brief Default constructor
    sfDevBMV080() : _theBus{nullptr}
    {
    }

    /**
     * @brief Begins communication with the BMV080 sensor
     *
     * This method initializes the communication interface with the sensor.
     * It must be called before init() and any other operations with the sensor.
     *
     * @param theBus SparkFun Toolkit bus interface to use for communication.
     *
     * @return sfTkError_t Status code:
     *         - 0: Success
     *         - Negative: Error occurred
     *         - Positive: Warning condition
     *
     * @see init()
     * @see open()
     */
    sfTkError_t begin(sfTkIBus *theBus = nullptr);

    /**
     * @brief Initializes the BMV080 sensor
     *
     * This method performs complete initialization of the sensor by:
     * - Opening communication with the sensor
     * - Performing a soft reset
     * - Getting driver version information
     * - Reading the sensor ID
     *
     * It must be called after begin() and before attempting any other operations.
     *
     * @return true if initialization was successful
     * @return false if any initialization step failed
     *
     * @see begin()
     * @see open()
     * @see reset()
     * @see driverVersion()
     * @see ID()
     */
    bool init(void);

    /**
     * @brief Gets the version information of the BMV080 sensor driver
     *
     * This method retrieves the vendor-supplied version information for the
     * sensor driver software. The version follows semantic versioning format
     * with major, minor, and patch numbers.
     *
     * @param[out] major Major version number indicating incompatible API changes
     * @param[out] minor Minor version number indicating backwards-compatible functionality
     * @param[out] patch Patch version number indicating backwards-compatible bug fixes
     *
     * @return true if the version information was successfully retrieved
     * @return false if there was an error getting the version information
     *
     * @see init()
     */
    bool driverVersion(uint16_t &major, uint16_t &minor, uint16_t &patch);

    /**
     * @brief Opens and initializes communication with the BMV080 sensor
     *
     * This method initializes a new handle for communicating with the sensor.
     * It must be called before attempting to configure or read from the sensor.
     *
     * @return true if the sensor was successfully opened and handle created
     * @return false if the sensor could not be opened or handle creation failed
     *
     * @note This method is automatically called by init()
     *
     * @see init()
     * @see begin()
     */
    bool open(void);

    /**
     * @brief Closes communication with the BMV080 sensor
     *
     * @return true if the sensor was successfully closed and handle created
     * @return false if the sensor could not be closed or handle creation failed
     *
     * @see open()
     */
    bool close(void);

    /**
     * @brief Resets the BMV080 sensor to its default state
     *
     * This method performs a soft reset of the sensor, returning all settings
     * to their default values. The sensor will need to be reconfigured after
     * a reset.
     *
     * @return true if the reset was successful
     * @return false if the reset failed
     *
     * @note After reset, you may need to reinitialize settings like operational mode
     *       and duty cycling period
     *
     * @see init()
     * @see setMode()
     */
    bool reset(void);

    /**
     * @brief Length of the BMV080 sensor ID string buffer
     *
     * This constant defines the required buffer size for storing the sensor's
     * unique identification string. The buffer must be at least this size
     * when calling the ID() method.
     *
     * @see ID()
     */
    static const size_t kBMV800IDLength = 13;
    /**
     * @brief Gets the unique identifier of the BMV080 sensor
     *
     * This method retrieves the sensor's unique identification string.
     * The ID can be used to distinguish between different BMV080 sensors
     * or verify the sensor's authenticity.
     *
     * @param[out] idOut Buffer to store the sensor's ID string.
     *                   Must be at least kBMV800IDLength (13) bytes long.
     *
     * @return true if the ID was successfully retrieved
     * @return false if there was an error reading the ID
     *
     * @note The buffer must be pre-allocated with at least kBMV800IDLength bytes
     *
     * @see kBMV800IDLength
     */
    bool ID(char idOut[kBMV800IDLength]);

    /**
     * @brief Sets the operational mode of the BMV080 sensor
     *
     * This method configures how the sensor takes measurements. It supports two modes:
     * continuous measurement or duty-cycled measurement.
     *
     * @param mode The desired operational mode:
     *             - SFE_BMV080_MODE_CONTINUOUS: Sensor takes measurements continuously
     *             - SFE_BMV080_MODE_DUTY_CYCLE: Sensor takes measurements at specified intervals
     *
     * @return true if the mode was set successfully
     * @return false if setting the mode failed
     *
     * @note When using duty cycle mode, the measurement interval can be configured
     *       using setDutyCyclingPeriod()
     *
     * @see setDutyCyclingPeriod()
     * @see readSensor()
     * @see bmv080_output_t
     */
    bool setMode(uint8_t mode);

    /**
     * @brief Gets the PM10 (particulate matter ≤10 µm) concentration
     *
     * This method returns the latest PM10 reading from the sensor's internal cache.
     * The value represents the mass concentration of particles with a diameter
     * of 10 micrometers or less.
     *
     * @return The PM10 concentration in micrograms per cubic meter (µg/m³)
     *
     * @note The PM10 value is updated when readSensor() is called
     *
     * @see readSensor()
     * @see PM1()
     * @see bmv080_output_t
     */
    float PM10(void);

    /**
     * @brief Gets the PM2.5 (particulate matter ≤2.5 µm) concentration
     *
     * This method returns the latest PM2.5 reading from the sensor's internal cache.
     * The value represents the mass concentration of particles with a diameter
     * of 2.5 micrometers or less.
     *
     * @return The PM2.5 concentration in micrograms per cubic meter (µg/m³)
     *
     * @note The PM2.5 value is updated when readSensor() is called
     *
     * @see readSensor()
     * @see PM1()
     * @see bmv080_output_t
     */
    float PM25(void);

    /**
     * @brief Gets the PM1 (particulate matter ≤1.0 µm) concentration
     *
     * This method returns the latest PM1 reading from the sensor's internal cache.
     * The value represents the mass concentration of particles with a diameter
     * of 1.0 micrometers or less.
     *
     * @return The PM1 concentration in micrograms per cubic meter (µg/m³)
     *
     * @note The PM1 value is updated when readSensor() is called
     *
     * @see readSensor()
     * @see PM25()
     * @see bmv080_output_t
     */
    float PM1(void);

    /**
     * @brief Checks if the BMV080 sensor is obstructed
     *
     * This method returns the obstruction status from the latest sensor reading.
     * An obstruction could be caused by dust, debris, or other particles
     * blocking the sensor's optical path.
     *
     * @return true if the sensor is obstructed
     * @return false if the sensor is not obstructed
     *
     * @note The obstruction status is updated when readSensor() is called
     *
     * @see readSensor()
     * @see bmv080_output_t
     */
    bool isObstructed();

    /**
     * @brief Internal method to set sensor values from callback
     *
     * This method is called by the BMV080 driver callback to update internal sensor data.
     * It stores the latest sensor readings and sets the data available flag.
     *
     * @param bmv080_output The sensor output structure containing the latest readings
     *                      including PM2.5, PM1, and obstruction status
     *
     * @note This is primarily an internal method used as part of the callback mechanism
     *       from the Bosch BMV080 driver. It should not typically be called directly
     *       by library users.
     *
     * @see set_sensor_value()
     * @see bmv080_output_t
     */
    void setSensorValue(bmv080_output_t bmv080_output);

    /**
     * @brief Reads the latest sensor values from the BMV080
     *
     * This method triggers a sensor reading and updates the internal data cache.
     * If a pointer to a bmv080_output_t struct is provided, it will be populated
     * with the latest sensor values.
     *
     * @param[out] bmv080_output Optional pointer to a bmv080_output_t struct to store the sensor readings.
     *                          If nullptr (default), the values are only stored internally.
     *
     * @return true if new data was successfully read from the sensor
     * @return false if the sensor read failed or no new data is available
     *
     * @note This method clears the _dataAvailable flag before attempting to read new data
     *
     * @see sensorServiceRoutine()
     * @see bmv080_output_t
     */
    bool readSensor(bmv080_output_t *bmv080_output = nullptr);

    /**
     * @brief Gets the current duty cycling period setting
     *
     * Returns the time interval between measurements when the sensor is in
     * duty cycle mode. This setting has no effect when the sensor is in
     * continuous measurement mode.
     *
     * @return The duty cycling period in seconds
     *
     * @note This setting only affects the sensor when in SFE_BMV080_MODE_DUTY_CYCLE mode
     *
     * @see setDutyCyclingPeriod()
     * @see setMode()
     */
    uint16_t dutyCyclingPeriod();

    /**
     * @brief Sets the time interval between measurements in duty cycle mode
     *
     * This method configures how frequently the sensor takes measurements when
     * operating in duty cycle mode. A longer period reduces power consumption
     * but provides less frequent updates.
     *
     * @param duty_cycling_period The time between measurements in seconds
     *
     * @return true if the period was successfully set
     * @return false if setting the period failed
     *
     * @note This setting only takes effect when the sensor is in SFE_BMV080_MODE_DUTY_CYCLE mode
     *
     * @see dutyCyclingPeriod()
     * @see setMode()
     */
    bool setDutyCyclingPeriod(uint16_t duty_cycling_period);

    /**
     * @brief Gets the volumetric mass density setting
     *
     * This method returns the current volumetric mass density setting used for
     * particle concentration calculations. This value affects how the sensor
     * converts between particle counts and mass concentrations.
     *
     * @return The volumetric mass density in grams per cubic centimeter (g/cm³)
     *
     * @see setVolumetricMassDensity()
     * @see PM25()
     * @see PM1()
     */
    float volumetricMassDensity();

    /**
     * @brief Sets the volumetric mass density for particle concentration calculations
     *
     * This method configures the density value used to convert between particle
     * counts and mass concentrations. This setting affects the accuracy of
     * PM2.5 and PM1 measurements based on the expected particle density.
     *
     * @param volumetric_mass_density The particle density in grams per cubic centimeter (g/cm³)
     *
     * @return true if the density was successfully set
     * @return false if setting the density failed
     *
     * @see volumetricMassDensity()
     * @see PM25()
     * @see PM1()
     */
    bool setVolumetricMassDensity(float volumetric_mass_density);

    /**
     * @brief Gets the sensor's integration time setting
     *
     * This method returns the current integration time setting used for
     * particle measurements. The integration time affects the sensor's
     * measurement accuracy and response time.
     *
     * @return The integration time in milliseconds (ms)
     *
     * @see setIntegrationTime()
     * @see readSensor()
     */
    float integrationTime();

    /**
     * @brief Sets the sensor's integration time for measurements
     *
     * This method configures the integration time used for particle measurements.
     * Longer integration times can improve measurement accuracy but increase
     * response time and power consumption.
     *
     * @param integration_time The measurement integration time in milliseconds (ms)
     *
     * @return true if the integration time was successfully set
     * @return false if setting the integration time failed
     *
     * @see integrationTime()
     * @see readSensor()
     */
    bool setIntegrationTime(float integration_time);

    /**
     * @brief Gets the current distribution ID setting
     *
     * This method returns the distribution ID used by the sensor for particle
     * size classification. The distribution ID affects how particles are
     * categorized into different size bins.
     *
     * @return The current distribution ID value
     *
     * @see setDistributionId()
     * @see PM25()
     * @see PM1()
     */
    uint32_t distributionId();

    /**
     * @brief Sets the distribution ID for particle size classification
     *
     * This method configures which particle size distribution model the sensor
     * uses for classifying particles. Different distribution IDs are optimized
     * for different types of particles and environments.
     *
     * @param distribution_id The distribution ID to use for particle classification
     *
     * @return true if the distribution ID was successfully set
     * @return false if setting the distribution ID failed
     *
     * @see distributionId()
     * @see PM25()
     * @see PM1()
     */
    bool setDistributionId(uint32_t distribution_id);

    /**
     * @brief Gets the obstruction detection setting
     *
     * This method returns whether the sensor's obstruction detection feature
     * is enabled. When enabled, the sensor will monitor for any blockages
     * in its optical path.
     *
     * @return true if obstruction detection is enabled
     * @return false if obstruction detection is disabled
     *
     * @see setDoObstructionDetection()
     * @see isObstructed()
     */
    bool doObstructionDetection();

    /**
     * @brief Enables or disables the sensor's obstruction detection feature
     *
     * This method controls whether the sensor actively monitors for obstructions
     * in its optical path. When enabled, the sensor will report blockages
     * through the isObstructed() method.
     *
     * @param do_obstruction_detection true to enable obstruction detection,
     *                                false to disable it
     *
     * @return true if the setting was successfully changed
     * @return false if changing the setting failed
     *
     * @see doObstructionDetection()
     * @see isObstructed()
     */
    bool setDoObstructionDetection(bool do_obstruction_detection);

    /**
     * @brief Gets the vibration filtering setting
     *
     * This method returns whether the sensor's vibration filtering feature
     * is enabled. When enabled, the sensor applies algorithms to reduce
     * measurement noise caused by mechanical vibrations.
     *
     * @return true if vibration filtering is enabled
     * @return false if vibration filtering is disabled
     *
     * @see setDoVibrationFiltering()
     * @see readSensor()
     */
    bool doVibrationFiltering();

    /**
     * @brief Enables or disables vibration filtering
     *
     * This method controls whether the sensor applies vibration filtering
     * algorithms to reduce measurement noise caused by mechanical vibrations.
     * Enabling this feature can improve measurement accuracy in environments
     * with significant vibration.
     *
     * @param do_vibration_filtering true to enable vibration filtering,
     *                              false to disable it
     *
     * @return true if the setting was successfully changed
     * @return false if changing the setting failed
     *
     * @see doVibrationFiltering()
     * @see readSensor()
     */
    bool setDoVibrationFiltering(bool do_vibration_filtering);

    /**
     * @brief Gets the current measurement algorithm setting
     *
     * This method returns the measurement algorithm used by the sensor for
     * particle analysis. Different algorithms can be optimized for specific
     * measurement conditions or particle types.
     *
     * @return The current measurement algorithm identifier
     *
     * @see setMeasurementAlgorithm()
     * @see readSensor()
     */
    uint8_t measurementAlgorithm();

    /**
     * @brief Sets the measurement algorithm for particle analysis
     *
     * This method configures which algorithm the sensor uses for analyzing
     * particle measurements. Different algorithms can be optimized for
     * specific types of particles or measurement environments.
     *
     * @param measurement_algorithm The algorithm identifier to use for measurements
     *
     * @return true if the algorithm was successfully set
     * @return false if setting the algorithm failed
     *
     * @see measurementAlgorithm()
     * @see readSensor()
     */
    bool setMeasurementAlgorithm(uint8_t measurement_algorithm);

  private:
    // bosch bmv080 library callback functions (static methods to be used as callbacks)
    /**
     * @brief Callback function for reading 16-bit values from the BMV080 sensor
     *
     * This static method serves as a callback for the Bosch BMV080 driver to read
     * 16-bit registers from the sensor over the communication bus.
     *
     * @param handle The SERCOM handle for communication with the sensor
     * @param reg_addr The register address to read from
     * @param[out] reg_data Pointer to store the read data
     * @param len Number of 16-bit words to read
     *
     * @return int8_t Status code:
     *         - 0: Success
     *         - Negative: Error occurred
     *
     * @note This is an internal callback used by the Bosch BMV080 driver
     *
     * @see device_write_16bit_CB()
     */
    static int8_t device_read_16bit_CB(bmv080_sercom_handle_t, uint16_t, uint16_t *, uint16_t);

    /**
     * @brief Callback function for writing 16-bit values to the BMV080 sensor
     *
     * This static method serves as a callback for the Bosch BMV080 driver to write
     * 16-bit registers to the sensor over the communication bus.
     *
     * @param handle The SERCOM handle for communication with the sensor
     * @param reg_addr The register address to write to
     * @param[in] reg_data Pointer to the data to write
     * @param len Number of 16-bit words to write
     *
     * @return int8_t Status code:
     *         - 0: Success
     *         - Negative: Error occurred
     *
     * @note This is an internal callback used by the Bosch BMV080 driver
     *
     * @see device_read_16bit_CB()
     */
    static int8_t device_write_16bit_CB(bmv080_sercom_handle_t, uint16_t, const uint16_t *, uint16_t);

    /**
     * @brief Callback function for implementing delays in the BMV080 sensor driver
     *
     * This static method serves as a callback for the Bosch BMV080 driver to
     * implement timing delays required by the sensor operations.
     *
     * @param delay_ms The delay duration in milliseconds
     *
     * @return int8_t Status code:
     *         - 0: Success
     *         - Negative: Error occurred
     *
     * @note This is an internal callback used by the Bosch BMV080 driver
     *
     * @see device_read_16bit_CB()
     * @see device_write_16bit_CB()
     */
    static int8_t device_delay_CB(uint32_t);

    /**
     * @brief Static callback for updating sensor values from the BMV080 driver
     *
     * This static method serves as a callback function for the Bosch BMV080 driver
     * to update sensor readings. It receives new measurement data and passes it
     * to the appropriate instance through the void pointer to user data.
     *
     * @param bmv080_output The sensor output structure containing the latest readings
     * @param[in] user_data Pointer to the sfDevBMV080 instance (cast from void*)
     *
     * @note This is an internal callback used by the Bosch BMV080 driver
     *
     * @see setSensorValue()
     * @see bmv080_output_t
     */
    static void set_sensor_value(bmv080_output_t, void *);

    /**
     * @brief Services the BMV080 sensor driver
     *
     * This method pumps the service routine of the BMV080 sensor driver,
     * allowing it to process measurements and update sensor values.
     * It is called internally by readSensor() to maintain proper
     * sensor operation.
     *
     * @return true if the service routine completed successfully
     * @return false if an error occurred during servicing
     *
     * @note This is an internal method used by the driver
     *
     * @see readSensor()
     * @see set_sensor_value()
     */
    bool sensorServiceRoutine(void);

    // BMV080 sensor handle - used when referencing the sensor using the Bosch API
    bmv080_handle_t _bmv080_handle_class = NULL;

    // Internal flag to track if new sensor data is available
    bool _dataAvailable = false;

    // Internal cache for the latest sensor values
    bmv080_output_t _sensorValue;

  protected:
    // Pointer to the SparkFun Toolkit bus interface used for communication
    sfTkIBus *_theBus;
};