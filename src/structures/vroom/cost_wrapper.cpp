/*

This file is part of VROOM.

Copyright (c) 2015-2022, Julien Coupey.
All rights reserved (see LICENSE).

*/

#include "structures/vroom/cost_wrapper.h"
#include "utils/exception.h"
#include <cmath>

namespace vroom {

CostWrapper::CostWrapper(double speed_factor, Cost per_hour, Cost per_km)
  : _per_hour(per_hour),
    _per_km(per_km),
    discrete_duration_factor(std::round(1 / speed_factor * DURATION_FACTOR)),
    discrete_duration_cost_factor(
      std::round(1 / speed_factor * DURATION_FACTOR * per_hour)),
    discrete_distance_cost_factor(DISTANCE_FACTOR * per_km) {
  if (speed_factor <= 0 || speed_factor > MAX_SPEED_FACTOR) {
    throw InputException("Invalid speed factor: " +
                         std::to_string(speed_factor));
  }
}

void CostWrapper::set_durations_matrix(const Matrix<UserDuration>* matrix) {
  duration_matrix_size = matrix->size();
  duration_data = (*matrix)[0];
}

void CostWrapper::set_distances_matrix(const Matrix<UserDistance>* matrix) {
  distance_matrix_size = matrix->size();
  distance_data = (*matrix)[0];
}

void CostWrapper::set_costs_matrix(const Matrix<UserCost>* matrix,
                                   bool reset_cost_factor) {
  cost_matrix_size = matrix->size();
  cost_data = (*matrix)[0];

  if (reset_cost_factor) {
    discrete_duration_cost_factor = DURATION_FACTOR * COST_FACTOR;
    discrete_distance_cost_factor = 0;
    _cost_based_on_duration = false;
  }
}

UserCost CostWrapper::user_cost_from_user_duration(UserDuration d) const {
  assert(_cost_based_on_duration);
  return (d * _per_hour) / COST_FACTOR;
}

} // namespace vroom
