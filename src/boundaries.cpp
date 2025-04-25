#include "boundaries.hpp"

Boundary::Boundary(int r, int y) : right_(r), top_(y) {}

Boundary::Boundary(const Pin& pin)
    : right_(pin.x() + pin.width()), top_(pin.y() + pin.height()) {}

Boundary::Boundary(const Boundary& bnd) : right_(bnd.right_), top_(bnd.top_) {}

Boundary& Boundary::operator=(const Boundary& bnd) {
    right_ = bnd.right_;
    top_ = bnd.top_;

    return *this;
}

Boundary& Boundary::operator=(Boundary&& bnd) {
    right_ = bnd.right_;
    top_ = bnd.top_;

    bnd.right_ = bnd.top_ = 0;

    return *this;
}

bool Boundary::operator==(const Boundary& bnd) const {
    return right_ == bnd.right_ && top_ == bnd.top_;
}

int Boundary::right() const {
    return right_;
}

int Boundary::top() const {
    return top_;
}

void Boundary::right(int r) {
    this->right_ = r;
}

void Boundary::top(int y) {
    this->top_ = y;
}
