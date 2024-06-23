#pragma once

#include <vector>

#include "pins.hpp"

// Read only Net
class net {
   public:
    net();
    net(net&& net);
    net(std::vector<unsigned>&& conn, const std::vector<pin>& all_pins);

    net& operator=(net&& net);

    unsigned at(unsigned index) const;

    unsigned hpwl() const;

   private:
    std::vector<unsigned> connected_pins_;
    const std::vector<pin>* all_pins_;
};
