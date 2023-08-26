/**
 * @file mtq_magnetometer_interference.hpp
 * @brief Class for MTQ Magnetometer interference
 */

#ifndef S2E_COMPONENTS_REAL_AOCS_MTQ_MAGNETORQUER_INTERFERENCE_HPP_
#define S2E_COMPONENTS_REAL_AOCS_MTQ_MAGNETORQUER_INTERFERENCE_HPP_

#include "magnetometer.hpp"
#include "magnetorquer.hpp"

/**
 * @enum
 * @brief
 */
enum class MtqPreviousState {
  kPlus,  //!< Plus output
  kZero,  //!< Zero output
  kMinus  //!< Minus output
};

/**
 * @class MtqMagnetorquerInterference
 * @brief Class for MTQ Magnetometer interference
 */
class MtqMagnetorquerInterference {
 public:
  /**
   * @fn MtqMagnetorquerInterference
   * @brief Constructor
   * @param[in] ini_file: initialize file
   */
  MtqMagnetorquerInterference(const std::string file_name, Magnetometer& magnetometer, const Magnetorquer& magnetorquer);

  /**
   * @fn UpdateInterference
   * @brief Update MTQ-Magnetometer interference
   */
  void UpdateInterference(void);

 protected:
  size_t polynomial_degree_; //!< Polynomial degree
  std::vector<libra::Matrix<3, 3>> additional_bias_by_mtq_coefficients_;  //!< Polynomial coefficients of additional bias noise
  libra::Vector<3> previous_added_bias_c_nT_{0.0};

  Magnetometer& magnetometer_;        //!< Magnetometer
  const Magnetorquer& magnetorquer_;  //!< Magnetorquer
};

#endif  // S2E_COMPONENTS_REAL_AOCS_MTQ_MAGNETORQUER_INTERFERENCE_HPP_
