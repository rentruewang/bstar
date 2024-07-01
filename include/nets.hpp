#pragma once

#include <vector>

#include "pins.hpp"

// Read only Net
class net {
   public:
    net();
    net(net&& net);
    net(std::vector<size_t>&& conn, const std::vector<pin>& all_pins);

    net& operator=(net&& net);

    size_t at(size_t index) const;

    size_t hpwl() const;

   private:
    std::vector<size_t> connected_pins_;
    const std::vector<pin>* all_pins_;
};
