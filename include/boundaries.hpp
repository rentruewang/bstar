#pragma once

#include "pins.hpp"

class Boundary {
   public:
    Boundary(int r, int y);
    Boundary(const Pin& pin);
    Boundary(const Boundary& bnd);

    Boundary& operator=(const Boundary& bnd);
    Boundary& operator=(Boundary&& bnd);

    bool operator==(const Boundary& bnd) const;

    int right() const;
    int top() const;

    void right(int r);
    void top(int y);

   private:
    int right_, top_;
};
