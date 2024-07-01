#pragma once

#include <utility>

#include "b_star.hpp"
#include "nets.hpp"
#include "pins.hpp"

size_t total_area(const std::pair<int, int> dim);

size_t total_hpwl(const std::vector<net>& net_list);

double total_cost(const double area, const double hpwl, const double alpha);

std::pair<int, int> sim_anneal(std::pair<size_t, size_t> boundary,
                               b_star& tree,
                               std::vector<pin>& pin_list,
                               const std::vector<net>& net_list,
                               std::pair<size_t, size_t> iter_info,
                               size_t num_blocks,
                               size_t episodes,
                               size_t high_temp,
                               double alpha,
                               double ratio,
                               double P,
                               double C);
