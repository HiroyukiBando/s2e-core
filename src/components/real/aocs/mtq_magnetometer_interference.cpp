/**
 * @file mtq_magnetometer_interference.cpp
 * @brief Class for MTQ Magnetometer interference
 */

#include "mtq_magnetometer_interference.hpp"

#include "library/initialize/initialize_file_access.hpp"

MtqMagnetometerInterference::MtqMagnetometerInterference(const std::string file_name, Magnetometer& magnetometer, const Magnetorquer& magnetorquer)
    : magnetometer_(magnetometer), magnetorquer_(magnetorquer) {
  // Read ini file
  IniAccess ini_file(file_name);
  const char* section = "MTQ_MAGNETOMETER_INTERFERENCE";

  polynomial_degree_ = (size_t)ini_file.ReadInt(section, "polynomial_degree");

  for (size_t degree = 1; degree <= polynomial_degree_; degree++) {
    const std::string key_name = "additional_bias_by_mtq_coefficients_" + std::to_string(static_cast<long long>(degree));
    libra::Vector<9> additional_bias_by_mtq_coefficients_vec;
    libra::Matrix<3, 3> additional_bias_by_mtq_coefficients;
    ini_file.ReadVector(section, key_name.c_str(), additional_bias_by_mtq_coefficients_vec);
    for (size_t i = 0; i < 3; i++) {
      for (size_t j = 0; j < 3; j++) {
        additional_bias_by_mtq_coefficients[i][j] = additional_bias_by_mtq_coefficients_vec[i * 3 + j];
      }
    }
    additional_bias_by_mtq_coefficients_.push_back(additional_bias_by_mtq_coefficients);
  }
}

void MtqMagnetometerInterference::UpdateInterference(void) {
  // Subtract previous added bias to avoid double addition
  magnetometer_.AddConstantBiasNoise_c_nT(-1.0 * previous_added_bias_c_nT_);

  // Calculate bias
  libra::Vector<3> additional_bias_c_nT{0.0};
  libra::Vector<3> mtq_output_c_Am2 = magnetorquer_.GetOutputMagneticMoment_c_Am2();
  for (size_t degree = 1; degree <= polynomial_degree_; degree++) {
    libra::Vector<3> hadamard_mtq;
    for (size_t axis = 0; axis < 3; axis++) {
      hadamard_mtq[axis] = pow(mtq_output_c_Am2[axis], degree);
    }
    additional_bias_c_nT = additional_bias_c_nT + additional_bias_by_mtq_coefficients_[degree - 1] * hadamard_mtq;
  }

  // Add bias
  magnetometer_.AddConstantBiasNoise_c_nT(additional_bias_c_nT);
  previous_added_bias_c_nT_ = additional_bias_c_nT;
}
