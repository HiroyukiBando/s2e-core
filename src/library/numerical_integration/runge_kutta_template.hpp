/**
 * @file runge_kutta.cpp
 * @brief Class for General Runge-Kutta method
 */
#ifndef S2E_LIBRARY_NUMERICAL_INTEGRATION_RUNGE_KUTTA_TEMPLATE_HPP_
#define S2E_LIBRARY_NUMERICAL_INTEGRATION_RUNGE_KUTTA_TEMPLATE_HPP_

#include "./runge_kutta.hpp"

namespace libra {

template <size_t N>
void RungeKutta<N>::Integrate() {
  std::vector<Vector<N>> slope = CalcSlope();  //!< k vector in the equation

  for (size_t i = 0; i < number_of_stages_; i++) {
    current_state_ = current_state_ + weights_[i] * step_width_s_ * slope[i];
  }
  current_time_s_ += step_width_s_;
}

template <size_t N>
std::vector<Vector<N>> RungeKutta<N>::CalcSlope() {
  std::vector<Vector<N>> slope;
  slope.assign(number_of_stages_, Vector<N>(0.0));

  for (size_t i = 0; i < number_of_stages_; i++) {
    Vector<N> state = current_state_;
    for (size_t j = 0; j < i; j++) {
      state = state + rk_matrix_[i][j] * step_width_s_ * slope[j];
    }
    double time_s = current_time_s_ + nodes_[i] * step_width_s_;
    slope[i] = DerivativeFunction(time_s, state);
  }

  return slope;
}

}  // namespace libra

#endif  // S2E_LIBRARY_NUMERICAL_INTEGRATION_RUNGE_KUTTA_TEMPLATE_HPP_
