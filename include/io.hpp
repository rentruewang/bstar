#pragma once

#include <fstream>
#include <string>
#include <unordered_map>

#include "nets.hpp"
#include "pins.hpp"

std::pair<size_t, size_t> read_pin_file(
    std::ifstream& file,
    std::vector<pin>& pin_list,
    std::unordered_map<std::string, size_t>& pin_map,
    size_t& num_blocks);

void read_net_file(std::ifstream& file,
                   const std::vector<pin>& pin_list,
                   std::vector<net>& net_list,
                   const std::unordered_map<std::string, size_t>& pin_map);

void save_file(std::ofstream& file,
               time_t start_time,
               double alpha,
               std::pair<int, int> dimension,
               const std::vector<pin>& pin_list,
               const std::vector<net>& net_list);
