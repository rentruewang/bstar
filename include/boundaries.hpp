#pragma once

#include "pins.hpp"

class boundary {
   public:
    boundary(int r, int y);
    boundary(const pin& pin);
    boundary(const boundary& bnd);
    boundary(boundary&& bnd);

    boundary& operator=(const boundary& bnd);
    boundary& operator=(boundary&& bnd);

    bool operator==(const boundary& bnd);

    int right() const;
    int top() const;

    void set_right(int r);
    void set_top(int y);

   private:
    int right_, top_;
};
