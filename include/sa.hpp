#pragma once

#include <utility>

#include "b_star.hpp"
#include "nets.hpp"
#include "pins.hpp"

unsigned total_area(const std::pair<int, int> dim);

unsigned total_hpwl(const std::vector<pin>& pin_list,
                    const std::vector<net>& net_list);

double total_cost(const double area, const double hpwl, const double alpha);

std::pair<int, int> sim_anneal(std::pair<unsigned, unsigned> boundary,
                               b_star& tree,
                               std::vector<pin>& pin_list,
                               const std::vector<net>& net_list,
                               std::pair<unsigned, unsigned> iter_info,
                               unsigned num_blocks,
                               unsigned episodes,
                               unsigned high_temp,
                               double alpha,
                               double ratio,
                               double P,
                               double C);
