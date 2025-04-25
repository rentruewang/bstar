#pragma once

#include <utility>
#include <vector>

#include "pins.hpp"

class BStar {
   public:
    BStar(std::vector<Pin>& plist);

    size_t root() const;

    std::vector<Pin>& pin_list();
    const std::vector<Pin>& pin_list() const;

    std::vector<size_t>& nodes();
    const std::vector<size_t>& nodes() const;

    std::pair<int, int> update() const;

    void flip();

   private:
    std::vector<size_t> nodes_;
    std::vector<Pin>& pin_list_;
};
