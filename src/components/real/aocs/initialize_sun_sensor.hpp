/**
 * @file initialize_sun_sensor.hpp
 * @brief Initialize functions for sun sensor
 */

#ifndef S2E_COMPONENTS_REAL_AOCS_INITIALIZE_SUN_SENSOR_HPP_
#define S2E_COMPONENTS_REAL_AOCS_INITIALIZE_SUN_SENSOR_HPP_

#include <components/real/aocs/sun_sensor.hpp>

/**
 * @fn InitSunSensor
 * @brief Initialize functions for sun sensor without power port
 * @param [in] clock_generator: Clock generator
 * @param [in] sensor_id: Sensor ID
 * @param [in] file_name: Path to the initialize file
 * @param [in] srp_environment: Solar radiation pressure environment
 * @param [in] local_environment: Local environment information
 */
SunSensor InitSunSensor(ClockGenerator* clock_generator, int sensor_id, const std::string file_name,
                        const SolarRadiationPressureEnvironment* srp_environment, const LocalCelestialInformation* local_celestial_information);
/**
 * @fn InitSunSensor
 * @brief Initialize functions for sun sensor with power port
 * @param [in] clock_generator: Clock generator
 * @param [in] power_port: Power port
 * @param [in] sensor_id: Sensor ID
 * @param [in] file_name: Path to the initialize file
 * @param [in] srp_environment: Solar radiation pressure environment
 * @param [in] local_environment: Local environment information
 */
SunSensor InitSunSensor(ClockGenerator* clock_generator, PowerPort* power_port, int sensor_id, const std::string file_name,
                        const SolarRadiationPressureEnvironment* srp_environment, const LocalCelestialInformation* local_celestial_information);

#endif  // S2E_COMPONENTS_REAL_AOCS_INITIALIZE_SUN_SENSOR_HPP_
