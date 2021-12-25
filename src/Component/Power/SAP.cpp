﻿#include "SAP.h"
#include "../../Dynamics/ClockGenerator.h"
#include "../../Interface/InitInput/InitPower/CsvScenarioInterface.h"

SAP::SAP(int id,
  int number_of_series,
  int number_of_parallel,
  double cell_area,
  libra::Vector<3> normal_vector,
  double cell_efficiency,
  double transmission_efficiency,
  const SRPEnvironment* srp,
  const CelestialInformation* celestial)
  :ComponentBase(1000), id_(id), number_of_series_(number_of_series), number_of_parallel_(number_of_parallel),
  cell_area_(cell_area), normal_vector_(libra::normalize(normal_vector)), cell_efficiency_(cell_efficiency),
  transmission_efficiency_(transmission_efficiency),srp_(srp), celestial_(celestial)
{
}

SAP::SAP(const SAP &obj)
  :ComponentBase(obj), id_(obj.id_), number_of_series_(obj.number_of_series_), number_of_parallel_(obj.number_of_parallel_),
  cell_area_(obj.cell_area_), normal_vector_(obj.normal_vector_), cell_efficiency_(obj.cell_efficiency_),
  transmission_efficiency_(obj.transmission_efficiency_),srp_(obj.srp_), celestial_(obj.celestial_)
{
  voltage_ = 0.0;
  power_generation_ = 0.0;
}

SAP::~SAP()
{
}

double SAP::GetPowerGeneration() const
{
  return power_generation_;
}

void SAP::SetVoltage(const double voltage)
{
  voltage_ = voltage;
}

string SAP::GetLogHeader() const
{
  string str_tmp = "";
  str_tmp += WriteScalar("power_generation"+to_string(id_), "W");
  return str_tmp;
}

string SAP::GetLogValue() const
{
  string str_tmp = "";
  str_tmp += WriteScalar(power_generation_);
  return str_tmp;
}

void SAP::MainRoutine(int time_count)
{
  if (CsvScenarioInterface::IsCsvScenarioEnabled())
  {
    double time_in_sec = time_count * ClockGenerator::IntervalMillisecond / 1000;
    const auto solar_constant = srp_->GetSolarConstant();
    libra::Vector<3> sun_direction_body = CsvScenarioInterface::GetSunDirectionBody(time_in_sec);
    libra::Vector<3> normalized_sun_direction_body = libra::normalize(sun_direction_body);
    power_generation_ = cell_efficiency_ * transmission_efficiency_ * solar_constant * (int)CsvScenarioInterface::GetSunFlag(time_in_sec) * cell_area_ * number_of_parallel_ * number_of_series_ *
      inner_product(normal_vector_, normalized_sun_direction_body);
}
  else
  {
    const auto power_density = srp_->CalcPowerDensity();
    libra::Vector<3> sun_direction = celestial_->GetPosFromSC_b("SUN");
    libra::Vector<3> normalized_sun_direction = libra::normalize(sun_direction);
    power_generation_ = cell_efficiency_ * transmission_efficiency_ * power_density * cell_area_ * number_of_parallel_ * number_of_series_ *
      inner_product(normal_vector_, normalized_sun_direction); //仮の実装．実際は太陽方向などからIVカーブを更新．動作電圧に応じた発電電力を求める
  }
  if (power_generation_ < 0) power_generation_ = 0.0;
}