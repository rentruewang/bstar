#pragma once

#include <utility>
#include <vector>

#include "pins.hpp"

class b_star {
   public:
    b_star(std::vector<pin>& plist);

    size_t root() const;

    std::vector<pin>& pin_list();
    const std::vector<pin>& pin_list() const;

    std::vector<size_t>& nodes();
    const std::vector<size_t>& nodes() const;

    std::pair<int, int> update() const;

    void flip();

   private:
    std::vector<size_t> nodes_;
    std::vector<pin>& pin_list_;
};
