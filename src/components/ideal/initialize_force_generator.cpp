/*
 * @file initialize_force_generator.cpp
 * @brief Initialize function for ForceGenerator
 */
#include "initialize_force_generator.hpp"

#include <library/initialize/initialize_file_access.hpp>

ForceGenerator InitializeForceGenerator(ClockGenerator* clock_generator, const std::string file_name, const Dynamics* dynamics) {
  // General
  IniAccess ini_file(file_name);

  // CompoBase
  int prescaler = ini_file.ReadInt("COMPONENT_BASE", "prescaler");
  if (prescaler <= 1) prescaler = 1;

  // ForceGenerator
  char section[30] = "FORCE_GENERATOR";
  double force_magnitude_standard_deviation_N = ini_file.ReadDouble(section, "force_magnitude_standard_deviation_N");
  double force_direction_standard_deviation_deg = ini_file.ReadDouble(section, "force_direction_standard_deviation_deg");
  double force_direction_standard_deviation_rad = libra::deg_to_rad * force_direction_standard_deviation_deg;
  ForceGenerator force_generator(prescaler, clock_generator, force_magnitude_standard_deviation_N, force_direction_standard_deviation_rad, dynamics);

  return force_generator;
}
