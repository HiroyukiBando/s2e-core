/**
 * @file relative_orbit.cpp
 * @brief Class to propagate relative orbit
 */
#include "relative_orbit.hpp"

#include <library/utilities/macros.hpp>

#include "rk4_orbit_propagation.hpp"

RelativeOrbit::RelativeOrbit(const CelestialInformation* celestial_information, double mu_m3_s2, double timestep, int reference_spacecraft_id,
                             Vector<3> initial_relative_position_lvlh, Vector<3> initial_relative_velocity_lvlh,
                             RelativeOrbitUpdateMethod update_method, RelativeOrbitModel relative_dynamics_model_type, STMModel stm_model_type,
                             RelativeInformation* rel_info)
    : Orbit(celestial_information),
      libra::ODE<6>(timestep),
      mu_m3_s2_(mu_m3_s2),
      reference_spacecraft_id_(reference_spacecraft_id),
      update_method_(update_method),
      relative_dynamics_model_type_(relative_dynamics_model_type),
      stm_model_type_(stm_model_type),
      relative_information_(rel_info) {
  propagate_mode_ = OrbitPropagateMode::kRelativeOrbit;

  propagation_time_s_ = 0.0;
  propagation_step_s_ = timestep;

  InitializeState(initial_relative_position_lvlh, initial_relative_velocity_lvlh, mu_m3_s2);
}

RelativeOrbit::~RelativeOrbit() {}

void RelativeOrbit::InitializeState(Vector<3> initial_relative_position_lvlh, Vector<3> initial_relative_velocity_lvlh, double mu_m3_s2,
                                    double init_time) {
  relative_position_lvlh_m_ = initial_relative_position_lvlh;
  relative_velocity_lvlh_m_s_ = initial_relative_velocity_lvlh;

  // Disturbance acceleration are not considered in relative orbit propagation
  spacecraft_acceleration_i_m_s2_ *= 0;

  Vector<3> reference_sat_position_i = relative_information_->GetReferenceSatDynamics(reference_spacecraft_id_)->GetOrbit().GetPosition_i_m();
  Vector<3> reference_sat_velocity_i = relative_information_->GetReferenceSatDynamics(reference_spacecraft_id_)->GetOrbit().GetVelocity_i_m_s();
  Quaternion q_i2lvlh = relative_information_->GetReferenceSatDynamics(reference_spacecraft_id_)->GetOrbit().CalcQuaternion_i2lvlh();
  Quaternion q_lvlh2i = q_i2lvlh.conjugate();
  spacecraft_position_i_m_ = q_lvlh2i.frame_conv(relative_position_lvlh_m_) + reference_sat_position_i;
  spacecraft_velocity_i_m_s_ = q_lvlh2i.frame_conv(relative_velocity_lvlh_m_s_) + reference_sat_velocity_i;

  initial_state_[0] = initial_relative_position_lvlh[0];
  initial_state_[1] = initial_relative_position_lvlh[1];
  initial_state_[2] = initial_relative_position_lvlh[2];
  initial_state_[3] = initial_relative_velocity_lvlh[0];
  initial_state_[4] = initial_relative_velocity_lvlh[1];
  initial_state_[5] = initial_relative_velocity_lvlh[2];

  if (update_method_ == RK4) {
    setup(init_time, initial_state_);
    CalculateSystemMatrix(relative_dynamics_model_type_, &(relative_information_->GetReferenceSatDynamics(reference_spacecraft_id_)->GetOrbit()),
                          mu_m3_s2);
  } else  // update_method_ == STM
  {
    CalculateSTM(stm_model_type_, &(relative_information_->GetReferenceSatDynamics(reference_spacecraft_id_)->GetOrbit()), mu_m3_s2, 0.0);
  }

  TransformEciToEcef();
  TransformEcefToGeodetic();
}

void RelativeOrbit::CalculateSystemMatrix(RelativeOrbitModel relative_dynamics_model_type, const Orbit* reference_sat_orbit, double mu_m3_s2) {
  switch (relative_dynamics_model_type) {
    case RelativeOrbitModel::Hill: {
      double reference_sat_orbit_radius = libra::norm(reference_sat_orbit->GetPosition_i_m());
      system_matrix_ = CalculateHillSystemMatrix(reference_sat_orbit_radius, mu_m3_s2);
    }
    default: {
      // NOT REACHED
      break;
    }
  }
}

void RelativeOrbit::CalculateSTM(STMModel stm_model_type, const Orbit* reference_sat_orbit, double mu_m3_s2, double elapsed_sec) {
  switch (stm_model_type) {
    case STMModel::HCW: {
      double reference_sat_orbit_radius = libra::norm(reference_sat_orbit->GetPosition_i_m());
      stm_ = CalculateHCWSTM(reference_sat_orbit_radius, mu_m3_s2, elapsed_sec);
    }
    default: {
      // NOT REACHED
      break;
    }
  }
}

void RelativeOrbit::Propagate(double end_time_s, double current_time_jd) {
  UNUSED(current_time_jd);

  if (!is_calc_enabled_) return;

  spacecraft_acceleration_i_m_s2_ *= 0;  // Disturbance acceleration are not considered in relative orbit propagation

  if (update_method_ == RK4) {
    PropagateRK4(end_time_s);
  } else  // update_method_ == STM
  {
    PropagateSTM(end_time_s);
  }

  Vector<3> reference_sat_position_i = relative_information_->GetReferenceSatDynamics(reference_spacecraft_id_)->GetOrbit().GetPosition_i_m();
  Vector<3> reference_sat_velocity_i = relative_information_->GetReferenceSatDynamics(reference_spacecraft_id_)->GetOrbit().GetVelocity_i_m_s();
  Quaternion q_i2lvlh = relative_information_->GetReferenceSatDynamics(reference_spacecraft_id_)->GetOrbit().CalcQuaternion_i2lvlh();
  Quaternion q_lvlh2i = q_i2lvlh.conjugate();

  spacecraft_position_i_m_ = q_lvlh2i.frame_conv(relative_position_lvlh_m_) + reference_sat_position_i;
  spacecraft_velocity_i_m_s_ = q_lvlh2i.frame_conv(relative_velocity_lvlh_m_s_) + reference_sat_velocity_i;
  TransformEciToEcef();
  TransformEcefToGeodetic();
}

void RelativeOrbit::PropagateRK4(double elapsed_sec) {
  setStepWidth(propagation_step_s_);  // Re-set propagation dt
  while (elapsed_sec - propagation_time_s_ - propagation_step_s_ > 1.0e-6) {
    Update();  // Propagation methods of the ODE class
    propagation_time_s_ += propagation_step_s_;
  }
  setStepWidth(elapsed_sec - propagation_time_s_);  // Adjust the last propagation dt
  Update();
  propagation_time_s_ = elapsed_sec;

  relative_position_lvlh_m_[0] = state()[0];
  relative_position_lvlh_m_[1] = state()[1];
  relative_position_lvlh_m_[2] = state()[2];
  relative_velocity_lvlh_m_s_[0] = state()[3];
  relative_velocity_lvlh_m_s_[1] = state()[4];
  relative_velocity_lvlh_m_s_[2] = state()[5];
}

void RelativeOrbit::PropagateSTM(double elapsed_sec) {
  Vector<6> current_state;
  CalculateSTM(stm_model_type_, &(relative_information_->GetReferenceSatDynamics(reference_spacecraft_id_)->GetOrbit()), mu_m3_s2_, elapsed_sec);
  current_state = stm_ * initial_state_;

  relative_position_lvlh_m_[0] = current_state[0];
  relative_position_lvlh_m_[1] = current_state[1];
  relative_position_lvlh_m_[2] = current_state[2];
  relative_velocity_lvlh_m_s_[0] = current_state[3];
  relative_velocity_lvlh_m_s_[1] = current_state[4];
  relative_velocity_lvlh_m_s_[2] = current_state[5];
}

void RelativeOrbit::RHS(double t, const Vector<6>& state, Vector<6>& rhs)  // only for RK4 relative dynamics propagation
{
  rhs = system_matrix_ * state;
  (void)t;
}
