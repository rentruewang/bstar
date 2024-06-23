#include "boundaries.hpp"

boundary::boundary(int r, int y) : right_(r), top_(y) {}

boundary::boundary(const pin& pin)
    : right_(pin.x() + pin.width()), top_(pin.y() + pin.height()) {}

boundary::boundary(const boundary& bnd) : right_(bnd.right_), top_(bnd.top_) {}

boundary::boundary(boundary&& bnd) : right_(bnd.right_), top_(bnd.top_) {
    bnd.right_ = bnd.top_ = 0;
}

boundary& boundary::operator=(const boundary& bnd) {
    right_ = bnd.right_;
    top_ = bnd.top_;

    return *this;
}

boundary& boundary::operator=(boundary&& bnd) {
    right_ = bnd.right_;
    top_ = bnd.top_;

    bnd.right_ = bnd.top_ = 0;

    return *this;
}

bool boundary::operator==(const boundary& bnd) {
    return right_ == bnd.right_ && top_ == bnd.top_;
}

int boundary::right() const {
    return right_;
}

int boundary::top() const {
    return top_;
}

void boundary::set_right(int r) {
    this->right_ = r;
}

void boundary::set_top(int y) {
    this->top_ = y;
}
