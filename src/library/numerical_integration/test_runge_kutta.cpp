/**
 * @file test_runge_kutta.cpp
 * @brief Test codes for RungeKutta class with GoogleTest
 */
#include <gtest/gtest.h>

#include "../orbit/kepler_orbit.hpp"
#include "numerical_integrator_manager.hpp"
#include "ode_examples.hpp"
#include "runge_kutta_4.hpp"
#include "runge_kutta_fehlberg.hpp"

/**
 * @brief Test for constructor
 */
TEST(NUMERICAL_INTEGRATION, Constructor) {
  libra::numerical_integration::ExampleLinearOde ode;
  libra::numerical_integration::RungeKutta4<1> linear_ode(0.1, ode);

  libra::Vector<1> state = linear_ode.GetState();
  EXPECT_DOUBLE_EQ(0.0, state[0]);
}

/**
 * @brief Test for integration with nominal linear function with RK4
 */
TEST(NUMERICAL_INTEGRATION, IntegrateLinearRk4) {
  double step_width_s = 0.1;
  libra::numerical_integration::ExampleLinearOde ode;
  libra::numerical_integration::RungeKutta4<1> rk4_ode(step_width_s, ode);

  libra::Vector<1> state = rk4_ode.GetState();
  EXPECT_DOUBLE_EQ(0.0, state[0]);

  size_t step_num = 10000;
  for (size_t i = 0; i < step_num; i++) {
    rk4_ode.Integrate();
  }
  state = rk4_ode.GetState();
  double estimated_result = step_width_s * step_num;

  EXPECT_NEAR(estimated_result, state[0], 1e-6);
}

/**
 * @brief Test for integration with nominal linear function with RKF
 */
TEST(NUMERICAL_INTEGRATION, IntegrateLinearRkf) {
  double step_width_s = 0.1;
  libra::numerical_integration::ExampleLinearOde ode;
  libra::numerical_integration::RungeKuttaFehlberg<1> rkf_ode(step_width_s, ode);

  libra::Vector<1> state = rkf_ode.GetState();
  EXPECT_DOUBLE_EQ(0.0, state[0]);

  size_t step_num = 10000;
  for (size_t i = 0; i < step_num; i++) {
    rkf_ode.Integrate();
  }
  state = rkf_ode.GetState();
  double estimated_result = step_width_s * step_num;

  EXPECT_NEAR(estimated_result, state[0], 1e-6);
}

/**
 * @brief Test for integration with nominal linear function with NumericalIntegratorManager/RK4
 */
TEST(NUMERICAL_INTEGRATION, IntegrateLinearNumericalIntegratorManagerRk4) {
  double step_width_s = 0.1;
  libra::numerical_integration::ExampleLinearOde ode;
  libra::numerical_integration::NumericalIntegratorManager<1> numerical_integrator(step_width_s, ode);

  libra::Vector<1> state = numerical_integrator.GetIntegrator()->GetState();
  EXPECT_DOUBLE_EQ(0.0, state[0]);

  size_t step_num = 10000;
  for (size_t i = 0; i < step_num; i++) {
    numerical_integrator.GetIntegrator()->Integrate();
  }
  state = numerical_integrator.GetIntegrator()->GetState();
  double estimated_result = step_width_s * step_num;

  EXPECT_NEAR(estimated_result, state[0], 1e-6);
}

/**
 * @brief Test for integration with nominal linear function with NumericalIntegratorManager/RKF
 */
TEST(NUMERICAL_INTEGRATION, IntegrateLinearNumericalIntegratorManagerRkf) {
  double step_width_s = 0.1;
  libra::numerical_integration::ExampleLinearOde ode;
  libra::numerical_integration::NumericalIntegratorManager<1> numerical_integrator(step_width_s, ode,
                                                                                   libra::numerical_integration::NumericalIntegrationMethod::kRkf);

  libra::Vector<1> state = numerical_integrator.GetIntegrator()->GetState();
  EXPECT_DOUBLE_EQ(0.0, state[0]);

  size_t step_num = 10000;
  for (size_t i = 0; i < step_num; i++) {
    numerical_integrator.GetIntegrator()->Integrate();
  }
  state = numerical_integrator.GetIntegrator()->GetState();
  double estimated_result = step_width_s * step_num;
  EXPECT_NEAR(estimated_result, state[0], 1e-6);

  double sigma = 0.1;
  state = numerical_integrator.GetIntegrator()->CalcInterpolationState(sigma);
  estimated_result = step_width_s * (double(step_num) - 1.0 + sigma);
  EXPECT_NEAR(estimated_result, state[0], 1e-6);
}

/**
 * @brief Test for integration with quadratic function with RK4
 */
TEST(NUMERICAL_INTEGRATION, IntegrateQuadraticRk4) {
  double step_width_s = 0.1;
  libra::numerical_integration::ExampleQuadraticOde ode;
  libra::numerical_integration::RungeKutta4<1> rk4_ode(step_width_s, ode);

  libra::Vector<1> state = rk4_ode.GetState();
  EXPECT_DOUBLE_EQ(0.0, state[0]);

  size_t step_num = 10000;
  for (size_t i = 0; i < step_num; i++) {
    rk4_ode.Integrate();
  }
  state = rk4_ode.GetState();
  double estimated_result = (step_width_s * step_num) * (step_width_s * step_num);

  EXPECT_NEAR(estimated_result, state[0], 1e-6);
}

/**
 * @brief Test for integration with quadratic function with RKF
 */
TEST(NUMERICAL_INTEGRATION, IntegrateQuadraticRkf) {
  double step_width_s = 0.1;
  libra::numerical_integration::ExampleQuadraticOde ode;
  libra::numerical_integration::RungeKuttaFehlberg<1> rkf_ode(step_width_s, ode);

  libra::Vector<1> state = rkf_ode.GetState();
  EXPECT_DOUBLE_EQ(0.0, state[0]);

  size_t step_num = 10000;
  for (size_t i = 0; i < step_num; i++) {
    rkf_ode.Integrate();
  }
  state = rkf_ode.GetState();
  double estimated_result = (step_width_s * step_num) * (step_width_s * step_num);

  EXPECT_NEAR(estimated_result, state[0], 1e-6);
}

/**
 * @brief Test for interpolation with quadratic function with RKF
 */
TEST(NUMERICAL_INTEGRATION, InterpolationQuadraticRkf) {
  double step_width_s = 10.0;
  libra::numerical_integration::ExampleQuadraticOde ode;
  libra::numerical_integration::RungeKuttaFehlberg<1> rkf_ode(step_width_s, ode);

  libra::Vector<1> state = rkf_ode.GetState();
  EXPECT_DOUBLE_EQ(0.0, state[0]);

  rkf_ode.Integrate();

  // Final value
  state = rkf_ode.GetState();
  double estimated_result = step_width_s * step_width_s;
  EXPECT_NEAR(estimated_result, state[0], 1e-6);
  // Interpolation value
  double sigma = 0.1;
  state = rkf_ode.CalcInterpolationState(sigma);
  estimated_result = (step_width_s * sigma) * (step_width_s * sigma);
  EXPECT_NEAR(estimated_result, state[0], 1e-6);

  sigma = 0.515;
  state = rkf_ode.CalcInterpolationState(sigma);
  estimated_result = (step_width_s * sigma) * (step_width_s * sigma);
  EXPECT_NEAR(estimated_result, state[0], 1e-6);
}

/**
 * @brief Test for integration with 1D position and velocity function with RK4
 */
TEST(NUMERICAL_INTEGRATION, Integrate1dPositionVelocityRk4) {
  double step_width_s = 0.1;
  libra::numerical_integration::Example1dPositionVelocityOde ode;
  libra::numerical_integration::RungeKutta4<2> rk4_ode(step_width_s, ode);

  libra::Vector<2> initial_state(0.0);
  initial_state[0] = 0.0;
  initial_state[1] = 0.1;
  rk4_ode.SetState(0.0, initial_state);

  libra::Vector<2> state = rk4_ode.GetState();
  EXPECT_DOUBLE_EQ(initial_state[0], state[0]);
  EXPECT_DOUBLE_EQ(initial_state[1], state[1]);

  size_t step_num = 10000;
  for (size_t i = 0; i < step_num; i++) {
    rk4_ode.Integrate();
  }
  state = rk4_ode.GetState();
  libra::Vector<2> estimated_result(0.0);
  estimated_result[0] = (step_width_s * step_num) * initial_state[1] + initial_state[0];
  estimated_result[1] = initial_state[1];

  EXPECT_NEAR(estimated_result[0], state[0], 1e-6);
  EXPECT_NEAR(estimated_result[1], state[1], 1e-6);
}

/**
 * @brief Test for integration with 1D position and velocity function with RKF
 */
TEST(NUMERICAL_INTEGRATION, Integrate1dPositionVelocityRkf) {
  double step_width_s = 0.1;
  libra::numerical_integration::Example1dPositionVelocityOde ode;
  libra::numerical_integration::RungeKuttaFehlberg<2> rkf_ode(step_width_s, ode);

  libra::Vector<2> initial_state(0.0);
  initial_state[0] = 0.0;
  initial_state[1] = 0.1;
  rkf_ode.SetState(0.0, initial_state);

  libra::Vector<2> state = rkf_ode.GetState();
  EXPECT_DOUBLE_EQ(initial_state[0], state[0]);
  EXPECT_DOUBLE_EQ(initial_state[1], state[1]);

  size_t step_num = 10000;
  for (size_t i = 0; i < step_num; i++) {
    rkf_ode.Integrate();
  }
  state = rkf_ode.GetState();
  libra::Vector<2> estimated_result(0.0);
  estimated_result[0] = (step_width_s * step_num) * initial_state[1] + initial_state[0];
  estimated_result[1] = initial_state[1];

  EXPECT_NEAR(estimated_result[0], state[0], 1e-6);
  EXPECT_NEAR(estimated_result[1], state[1], 1e-6);
}

/**
 * @brief Accuracy comparison between RK4 and RKF for integration with 2D two body orbit with small eccentricity
 */
TEST(NUMERICAL_INTEGRATION, Integrate2dTwoBodyOrbitSmallEccentricity) {
  double step_width_s = 0.1;
  libra::numerical_integration::Example2dTwoBodyOrbitOde ode;
  libra::numerical_integration::RungeKutta4<4> rk4_ode(step_width_s, ode);
  libra::numerical_integration::RungeKuttaFehlberg<4> rkf_ode(step_width_s, ode);

  libra::Vector<4> initial_state(0.0);
  const double eccentricity = 0.1;
  initial_state[0] = 1.0 - eccentricity;
  initial_state[1] = 0.0;
  initial_state[2] = 0.0;
  initial_state[3] = sqrt((1.0 + eccentricity) / (1.0 - eccentricity));
  rk4_ode.SetState(0.0, initial_state);
  rkf_ode.SetState(0.0, initial_state);

  libra::Vector<4> state_rk4 = rk4_ode.GetState();
  libra::Vector<4> state_rkf = rkf_ode.GetState();
  for (size_t i = 0; i < 4; i++) {
    EXPECT_DOUBLE_EQ(initial_state[i], state_rk4[i]);
    EXPECT_DOUBLE_EQ(initial_state[i], state_rkf[i]);
  }

  size_t step_num = 200;
  for (size_t i = 0; i < step_num; i++) {
    rk4_ode.Integrate();
    rkf_ode.Integrate();
  }
  state_rk4 = rk4_ode.GetState();
  state_rkf = rkf_ode.GetState();

  // Estimation by Kepler Orbit calculation
  libra::Vector<3> initial_position(0.0);
  libra::Vector<3> initial_velocity(0.0);

  initial_position[0] = initial_state[0];
  initial_position[1] = initial_state[1];
  initial_velocity[0] = initial_state[2];
  initial_velocity[1] = initial_state[3];
  OrbitalElements oe(1.0, 0.0, initial_position, initial_velocity);
  KeplerOrbit kepler(1.0, oe);
  kepler.CalcOrbit((double)(step_num * step_width_s) / (24.0 * 60.0 * 60.0));

  double error_tolerance = 2e-4;
  EXPECT_NEAR(kepler.GetPosition_i_m()[0], state_rk4[0], error_tolerance);
  EXPECT_NEAR(kepler.GetPosition_i_m()[1], state_rk4[1], error_tolerance);
  EXPECT_NEAR(kepler.GetVelocity_i_m_s()[0], state_rk4[2], error_tolerance);
  EXPECT_NEAR(kepler.GetVelocity_i_m_s()[1], state_rk4[3], error_tolerance);

  error_tolerance = 2e-5;
  EXPECT_NEAR(kepler.GetPosition_i_m()[0], state_rkf[0], error_tolerance);
  EXPECT_NEAR(kepler.GetPosition_i_m()[1], state_rkf[1], error_tolerance);
  EXPECT_NEAR(kepler.GetVelocity_i_m_s()[0], state_rkf[2], error_tolerance);
  EXPECT_NEAR(kepler.GetVelocity_i_m_s()[1], state_rkf[3], error_tolerance);
}

/**
 * @brief Accuracy comparison between RK4 and RKF for integration with 2D two body orbit with high eccentricity
 */
TEST(NUMERICAL_INTEGRATION, Integrate2dTwoBodyOrbitLargeEccentricity) {
  double step_width_s = 0.01;
  libra::numerical_integration::Example2dTwoBodyOrbitOde ode;
  libra::numerical_integration::RungeKutta4<4> rk4_ode(step_width_s, ode);
  libra::numerical_integration::RungeKuttaFehlberg<4> rkf_ode(step_width_s, ode);

  libra::Vector<4> initial_state(0.0);
  const double eccentricity = 0.9;
  initial_state[0] = 1.0 - eccentricity;
  initial_state[1] = 0.0;
  initial_state[2] = 0.0;
  initial_state[3] = sqrt((1.0 + eccentricity) / (1.0 - eccentricity));
  rk4_ode.SetState(0.0, initial_state);
  rkf_ode.SetState(0.0, initial_state);

  libra::Vector<4> state_rk4 = rk4_ode.GetState();
  libra::Vector<4> state_rkf = rkf_ode.GetState();
  for (size_t i = 0; i < 4; i++) {
    EXPECT_DOUBLE_EQ(initial_state[i], state_rk4[i]);
    EXPECT_DOUBLE_EQ(initial_state[i], state_rkf[i]);
  }

  size_t step_num = 2000;
  for (size_t i = 0; i < step_num; i++) {
    rk4_ode.Integrate();
    rkf_ode.Integrate();
  }
  state_rk4 = rk4_ode.GetState();
  state_rkf = rkf_ode.GetState();

  // Estimation by Kepler Orbit calculation
  libra::Vector<3> initial_position(0.0);
  libra::Vector<3> initial_velocity(0.0);

  initial_position[0] = initial_state[0];
  initial_position[1] = initial_state[1];
  initial_velocity[0] = initial_state[2];
  initial_velocity[1] = initial_state[3];
  OrbitalElements oe(1.0, 0.0, initial_position, initial_velocity);
  KeplerOrbit kepler(1.0, oe);
  kepler.CalcOrbit((double)(step_num * step_width_s) / (24.0 * 60.0 * 60.0));

  double error_tolerance = 2e-1;
  EXPECT_NEAR(kepler.GetPosition_i_m()[0], state_rk4[0], error_tolerance);
  EXPECT_NEAR(kepler.GetPosition_i_m()[1], state_rk4[1], error_tolerance);
  EXPECT_NEAR(kepler.GetVelocity_i_m_s()[0], state_rk4[2], error_tolerance);
  EXPECT_NEAR(kepler.GetVelocity_i_m_s()[1], state_rk4[3], error_tolerance);

  error_tolerance = 1e-2;
  EXPECT_NEAR(kepler.GetPosition_i_m()[0], state_rkf[0], error_tolerance);
  EXPECT_NEAR(kepler.GetPosition_i_m()[1], state_rkf[1], error_tolerance);
  EXPECT_NEAR(kepler.GetVelocity_i_m_s()[0], state_rkf[2], error_tolerance);
  EXPECT_NEAR(kepler.GetVelocity_i_m_s()[1], state_rkf[3], error_tolerance);
}

/**
 * @brief Accuracy comparison between RK4 and RKF for integration with 2D two body orbit with small eccentricity
 */
TEST(NUMERICAL_INTEGRATION, Interpolation2dTwoBodyOrbitSmallEccentricity) {
  double step_width_s = 1.0;
  libra::numerical_integration::Example2dTwoBodyOrbitOde ode;
  libra::numerical_integration::RungeKuttaFehlberg<4> rkf_ode(step_width_s, ode);

  libra::Vector<4> initial_state(0.0);
  const double eccentricity = 0.1;
  initial_state[0] = 1.0 - eccentricity;
  initial_state[1] = 0.0;
  initial_state[2] = 0.0;
  initial_state[3] = sqrt((1.0 + eccentricity) / (1.0 - eccentricity));
  rkf_ode.SetState(0.0, initial_state);

  libra::Vector<4> state_rkf = rkf_ode.GetState();
  for (size_t i = 0; i < 4; i++) {
    EXPECT_DOUBLE_EQ(initial_state[i], state_rkf[i]);
  }

  size_t step_num = 1;
  for (size_t i = 0; i < step_num; i++) {
    rkf_ode.Integrate();
  }
  state_rkf = rkf_ode.GetState();

  // Estimation by Kepler Orbit calculation
  libra::Vector<3> initial_position(0.0);
  libra::Vector<3> initial_velocity(0.0);

  // Final value
  initial_position[0] = initial_state[0];
  initial_position[1] = initial_state[1];
  initial_velocity[0] = initial_state[2];
  initial_velocity[1] = initial_state[3];
  OrbitalElements oe(1.0, 0.0, initial_position, initial_velocity);
  KeplerOrbit kepler(1.0, oe);
  kepler.CalcOrbit((double)(step_num * step_width_s) / (24.0 * 60.0 * 60.0));

  double error_tolerance = 1e-1;
  EXPECT_NEAR(kepler.GetPosition_i_m()[0], state_rkf[0], error_tolerance);
  EXPECT_NEAR(kepler.GetPosition_i_m()[1], state_rkf[1], error_tolerance);
  EXPECT_NEAR(kepler.GetVelocity_i_m_s()[0], state_rkf[2], error_tolerance);
  EXPECT_NEAR(kepler.GetVelocity_i_m_s()[1], state_rkf[3], error_tolerance);

  // Interpolation
  double sigma = 0.11;
  state_rkf = rkf_ode.CalcInterpolationState(sigma);
  kepler.CalcOrbit((double)(step_num * step_width_s * sigma) / (24.0 * 60.0 * 60.0));

  error_tolerance = 5e-3;
  EXPECT_NEAR(kepler.GetPosition_i_m()[0], state_rkf[0], error_tolerance);
  EXPECT_NEAR(kepler.GetPosition_i_m()[1], state_rkf[1], error_tolerance);
  EXPECT_NEAR(kepler.GetVelocity_i_m_s()[0], state_rkf[2], error_tolerance);
  EXPECT_NEAR(kepler.GetVelocity_i_m_s()[1], state_rkf[3], error_tolerance);

  // Interpolation
  sigma = 0.79;
  state_rkf = rkf_ode.CalcInterpolationState(sigma);
  kepler.CalcOrbit((double)(step_num * step_width_s * sigma) / (24.0 * 60.0 * 60.0));

  error_tolerance = 5e-2;
  EXPECT_NEAR(kepler.GetPosition_i_m()[0], state_rkf[0], error_tolerance);
  EXPECT_NEAR(kepler.GetPosition_i_m()[1], state_rkf[1], error_tolerance);
  EXPECT_NEAR(kepler.GetVelocity_i_m_s()[0], state_rkf[2], error_tolerance);
  EXPECT_NEAR(kepler.GetVelocity_i_m_s()[1], state_rkf[3], error_tolerance);
}
