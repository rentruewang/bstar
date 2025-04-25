#pragma once

#include <vector>

#include "pins.hpp"

// Read only Net
class Net {
   public:
    Net();
    Net(Net&& net);
    Net(std::vector<size_t>&& conn, const std::vector<Pin>& all_pins);

    Net& operator=(Net&& net);

    size_t at(size_t index) const;

    size_t hpwl() const;

   private:
    std::vector<size_t> connected_pins_;
    const std::vector<Pin>* all_pins_;
};
