#pragma once

#include "pins.hpp"

class boundary {
   public:
    boundary(int r, int y);
    boundary(const pin& pin);
    boundary(const boundary& bnd);

    boundary& operator=(const boundary& bnd);
    boundary& operator=(boundary&& bnd);

    bool operator==(const boundary& bnd) const;

    int right() const;
    int top() const;

    void right(int r);
    void top(int y);

   private:
    int right_, top_;
};
