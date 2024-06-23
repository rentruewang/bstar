#pragma once

#include <utility>
#include <vector>

#include "pins.hpp"

class b_star {
   public:
    b_star(std::vector<pin>& plist, unsigned width, unsigned height);

    unsigned get_size() const;
    unsigned get_root() const;

    std::vector<pin>& pin_list();
    const std::vector<pin>& pin_list() const;

    void check() const;
    std::pair<int, int> update() const;

    void flip();

    void permute(unsigned idx);
    void swap(unsigned i, unsigned j);
    void delete_insert(unsigned from,
                       unsigned to,
                       bool from_side,
                       bool to_side);
    void mirror(unsigned i);

    unsigned random_permute();
    std::pair<unsigned, unsigned> random_swap();
    std::pair<std::pair<unsigned, bool>, std::pair<unsigned, bool>>
    random_delete_insert();
    unsigned random_mirror();

    void revert_permute(unsigned action);
    void revert_swap(std::pair<unsigned, unsigned> action);
    void revert_delete_insert(
        std::pair<std::pair<unsigned, bool>, std::pair<unsigned, bool>> action);
    void revert_mirror(unsigned action);

   private:
    std::vector<unsigned> nodes_;
    std::vector<pin>& pin_list_;
};
