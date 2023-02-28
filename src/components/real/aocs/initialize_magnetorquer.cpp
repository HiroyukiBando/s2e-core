/**
 * @file initialize_magnetorquer.cpp
 * @brief Initialize functions for magnetorquer
 */
#include "initialize_magnetorquer.hpp"

#include "library/initialize/initialize_file_access.hpp"

Magnetorquer InitMagTorquer(ClockGenerator* clock_generator, int actuator_id, const std::string fname, double compo_step_time,
                            const GeomagneticField* geomagnetic_field) {
  IniAccess magtorquer_conf(fname);
  const char* sensor_name = "MAGNETORQUER_";
  const std::string section_name = sensor_name + std::to_string(static_cast<long long>(actuator_id));
  const char* MTSection = section_name.c_str();

  int prescaler = magtorquer_conf.ReadInt(MTSection, "prescaler");
  if (prescaler <= 1) prescaler = 1;

  Vector<kMtqDimension * kMtqDimension> sf_vec;
  magtorquer_conf.ReadVector(MTSection, "scale_factor_c", sf_vec);
  Matrix<kMtqDimension, kMtqDimension> scale_factor;
  for (size_t i = 0; i < kMtqDimension; i++) {
    for (size_t j = 0; j < kMtqDimension; j++) {
      scale_factor[i][j] = sf_vec[i * kMtqDimension + j];
    }
  }

  Quaternion quaternion_b2c;
  magtorquer_conf.ReadQuaternion(MTSection, "quaternion_b2c", quaternion_b2c);

  Vector<kMtqDimension> max_magnetic_moment_c_Am2;
  magtorquer_conf.ReadVector(MTSection, "max_output_magnetic_moment_c_Am2", max_magnetic_moment_c_Am2);

  Vector<kMtqDimension> min_magnetic_moment_c_Am2;
  magtorquer_conf.ReadVector(MTSection, "min_output_magnetic_moment_c_Am2", min_magnetic_moment_c_Am2);

  Vector<kMtqDimension> bias_noise_c_Am2_;
  magtorquer_conf.ReadVector(MTSection, "constant_bias_noise_c_Am2", bias_noise_c_Am2_);

  double random_walk_step_width_s = compo_step_time * (double)prescaler;
  Vector<kMtqDimension> random_walk_standard_deviation_c_Am2;
  magtorquer_conf.ReadVector(MTSection, "random_walk_standard_deviation_c_Am2", random_walk_standard_deviation_c_Am2);
  Vector<kMtqDimension> random_walk_limit_c_Am2;
  magtorquer_conf.ReadVector(MTSection, "random_walk_limit_c_Am2", random_walk_limit_c_Am2);
  Vector<kMtqDimension> normal_random_standard_deviation_c_Am2;
  magtorquer_conf.ReadVector(MTSection, "white_noise_standard_deviation_c_Am2", normal_random_standard_deviation_c_Am2);

  Magnetorquer magtorquer(prescaler, clock_generator, actuator_id, quaternion_b2c, scale_factor, max_magnetic_moment_c_Am2, min_magnetic_moment_c_Am2,
                          bias_noise_c_Am2_, random_walk_step_width_s, random_walk_standard_deviation_c_Am2, random_walk_limit_c_Am2,
                          normal_random_standard_deviation_c_Am2, geomagnetic_field);
  return magtorquer;
}

Magnetorquer InitMagTorquer(ClockGenerator* clock_generator, PowerPort* power_port, int actuator_id, const std::string fname, double compo_step_time,
                            const GeomagneticField* geomagnetic_field) {
  IniAccess magtorquer_conf(fname);
  const char* sensor_name = "MAGNETORQUER_";
  const std::string section_name = sensor_name + std::to_string(static_cast<long long>(actuator_id));
  const char* MTSection = section_name.c_str();

  int prescaler = magtorquer_conf.ReadInt(MTSection, "prescaler");
  if (prescaler <= 1) prescaler = 1;

  Vector<kMtqDimension * kMtqDimension> sf_vec;
  magtorquer_conf.ReadVector(MTSection, "scale_factor_c", sf_vec);
  Matrix<kMtqDimension, kMtqDimension> scale_factor;
  for (size_t i = 0; i < kMtqDimension; i++) {
    for (size_t j = 0; j < kMtqDimension; j++) {
      scale_factor[i][j] = sf_vec[i * kMtqDimension + j];
    }
  }

  Quaternion quaternion_b2c;
  magtorquer_conf.ReadQuaternion(MTSection, "quaternion_b2c", quaternion_b2c);

  Vector<kMtqDimension> max_magnetic_moment_c_Am2;
  magtorquer_conf.ReadVector(MTSection, "max_output_magnetic_moment_c_Am2", max_magnetic_moment_c_Am2);

  Vector<kMtqDimension> min_magnetic_moment_c_Am2;
  magtorquer_conf.ReadVector(MTSection, "min_output_magnetic_moment_c_Am2", min_magnetic_moment_c_Am2);

  Vector<kMtqDimension> bias_noise_c_Am2_;
  magtorquer_conf.ReadVector(MTSection, "constant_bias_noise_c_Am2", bias_noise_c_Am2_);

  double random_walk_step_width_s = compo_step_time * (double)prescaler;
  Vector<kMtqDimension> random_walk_standard_deviation_c_Am2;
  magtorquer_conf.ReadVector(MTSection, "random_walk_standard_deviation_c_Am2", random_walk_standard_deviation_c_Am2);
  Vector<kMtqDimension> random_walk_limit_c_Am2;
  magtorquer_conf.ReadVector(MTSection, "random_walk_limit_c_Am2", random_walk_limit_c_Am2);
  Vector<kMtqDimension> normal_random_standard_deviation_c_Am2;
  magtorquer_conf.ReadVector(MTSection, "white_noise_standard_deviation_c_Am2", normal_random_standard_deviation_c_Am2);

  // PowerPort
  power_port->InitializeWithInitializeFile(fname);

  Magnetorquer magtorquer(prescaler, clock_generator, power_port, actuator_id, quaternion_b2c, scale_factor, max_magnetic_moment_c_Am2,
                          min_magnetic_moment_c_Am2, bias_noise_c_Am2_, random_walk_step_width_s, random_walk_standard_deviation_c_Am2,
                          random_walk_limit_c_Am2, normal_random_standard_deviation_c_Am2, geomagnetic_field);
  return magtorquer;
}
