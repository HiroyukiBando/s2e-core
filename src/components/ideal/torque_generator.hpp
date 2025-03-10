/*
 * @file torque_generator.hpp
 * @brief Ideal component which can generate torque for control algorithm test
 */

#ifndef S2E_COMPONENTS_IDEAL_TORQUE_GENERATOR_HPP_
#define S2E_COMPONENTS_IDEAL_TORQUE_GENERATOR_HPP_

#include <components/base/component.hpp>
#include <dynamics/dynamics.hpp>
#include <library/logger/logger.hpp>
#include <library/math/vector.hpp>
#include <library/randomization/normal_randomization.hpp>

/*
 * @class TorqueGenerator
 * @brief Ideal component which can generate for control algorithm test
 */
class TorqueGenerator : public Component, public ILoggable {
 public:
  /**
   * @fn TorqueGenerator
   * @brief Constructor
   * @param [in] prescaler: Frequency scale factor for update
   * @param [in] clock_generator: Clock generator
   * @param [in] magnitude_error_standard_deviation_Nm: Standard deviation of magnitude error [Nm]
   * @param [in] direction_error_standard_deviation_rad: Standard deviation of direction error [rad]
   * @param [in] dynamics: Dynamics information
   */
  TorqueGenerator(const int prescaler, ClockGenerator* clock_generator, const double magnitude_error_standard_deviation_Nm,
                  const double direction_error_standard_deviation_rad, const Dynamics* dynamics);
  /**
   * @fn ~TorqueGenerator
   * @brief Destructor
   */
  ~TorqueGenerator();

  // Override functions for Component
  /**
   * @fn MainRoutine
   * @brief Main routine to calculate torque generation
   */
  void MainRoutine(const int time_count);
  /**
   * @fn PowerOffRoutine
   * @brief Power off routine to stop torque generation
   */
  void PowerOffRoutine();

  // Override ILoggable
  /**
   * @fn GetLogHeader
   * @brief Override GetLogHeader function of ILoggable
   */
  virtual std::string GetLogHeader() const;
  /**
   * @fn GetLogValue
   * @brief Override GetLogValue function of ILoggable
   */
  virtual std::string GetLogValue() const;

  // Getter
  /**
   * @fn GetGeneratedTorque_b_Nm
   * @brief Return generated torque in the body fixed frame [Nm]
   */
  inline const Vector<3> GetGeneratedTorque_b_Nm() const { return generated_torque_b_Nm_; };

  // Setter
  /**
   * @fn SetTorque_b_Nm
   * @brief Set ordered torque in the body fixed frame [Nm]
   */
  inline void SetTorque_b_Nm(const libra::Vector<3> torque_b_Nm) { ordered_torque_b_Nm_ = torque_b_Nm; };

 protected:
  libra::Vector<3> ordered_torque_b_Nm_{0.0};    //!< Ordered torque in the body fixed frame [Nm]
  libra::Vector<3> generated_torque_b_Nm_{0.0};  //!< Generated torque in the body fixed frame [Nm]

  // Noise
  libra::NormalRand magnitude_noise_;              //!< Normal random for magnitude noise
  libra::NormalRand direction_noise_;              //!< Normal random for direction noise
  double direction_error_standard_deviation_rad_;  //!< Standard deviation of direction error [rad]

  /**
   * @fn GenerateDirectionNoiseQuaternion
   * @brief Generate direction noise quaternion
   * @param [in] true_direction: True direction
   * @param [in] error_standard_deviation_rad: Standard deviation of direction error [rad]
   */
  libra::Quaternion GenerateDirectionNoiseQuaternion(libra::Vector<3> true_direction, const double error_standard_deviation_rad);

  const Dynamics* dynamics_;  //!< Spacecraft dynamics information
};

#endif  // S2E_COMPONENTS_IDEAL_TORQUE_GENERATOR_HPP_
