/*
 * @file Antenna.cpp
 * @brief Component emulation: RF antenna
 */

#include "Antenna.hpp"

#include <Library/utils/Macros.hpp>
#include <cmath>

Antenna::Antenna(const int id, const libra::Quaternion& q_b2c, const bool is_transmitter, const bool is_receiver, const double frequency,
                 const Vector<4> tx_params, const Vector<4> rx_params)
    : id_(id), is_transmitter_(is_transmitter), is_receiver_(is_receiver), frequency_(frequency) {
  q_b2c_ = q_b2c;
  tx_output_power_W_ = tx_params[0];
  tx_params_.gain_dBi_ = tx_params[1];
  tx_params_.loss_feeder_dB_ = tx_params[2];
  tx_params_.loss_pointing_dB_ = tx_params[3];

  rx_params_.gain_dBi_ = rx_params[0];
  rx_params_.loss_feeder_dB_ = rx_params[1];
  rx_params_.loss_pointing_dB_ = rx_params[2];
  rx_system_noise_temperature_K_ = rx_params[3];

  // Calculate the EIRP or GT for the maximum gain
  if (is_transmitter_) {
    tx_eirp_ = 10 * log10(tx_output_power_W_) + tx_params_.gain_dBi_ + tx_params_.loss_feeder_dB_ + tx_params_.loss_pointing_dB_;
  } else {
    tx_eirp_ = 0.0;
  }
  if (is_receiver_) {
    rx_gt_ = rx_params_.gain_dBi_ + rx_params_.loss_feeder_dB_ + rx_params_.loss_pointing_dB_ - 10 * std::log10(rx_system_noise_temperature_K_);
  } else {
    rx_gt_ = 0.0;
  }
}

Antenna::~Antenna() {}

double Antenna::CalcAntennaGain(double theta, bool is_tx) const {
  UNUSED(theta);
  // TODO: implement gain calculation considering the angle theta

  if (is_tx) {
    return tx_params_.gain_dBi_;
  } else {
    return rx_params_.gain_dBi_;
  }
}

double Antenna::CalcTxEIRP(double theta) const { return tx_eirp_ - tx_params_.gain_dBi_ + CalcAntennaGain(theta, true); }
double Antenna::CalcRxGT(double theta) const { return rx_gt_ - rx_params_.gain_dBi_ + CalcAntennaGain(theta, false); }
