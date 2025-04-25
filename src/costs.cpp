#include "costs.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>

using namespace std;

Cost::Cost() : area_(1.), hpwl_(1.), alpha_(.5) {}

Cost::Cost(double init_area,
           double init_hpwl,
           double init_alpha,
           double aspect_ratio)
    : area_(init_area),
      hpwl_(init_hpwl),
      alpha_(init_alpha),
      width_(aspect_ratio / 2.),
      height_(aspect_ratio / 2.) {
    assert(init_area >= 0.);
    assert(init_hpwl >= 0.);
    assert(init_alpha >= 0. && init_alpha <= 1.);
    assert(aspect_ratio >= 0. && aspect_ratio <= 1.);
}

double Cost::area() const {
    return area_;
}

double Cost::hpwl() const {
    return hpwl_;
}

double Cost::alpha() const {
    return alpha_;
}

double Cost::width() const {
    return width_;
}

double Cost::height() const {
    return height_;
}

void Cost::area(double a) {
    area_ = a;
}

void Cost::hpwl(double h) {
    hpwl_ = h;
}

void Cost::alpha(double a) {
    alpha_ = a;
}

void Cost::width(double w) {
    width_ = w;
}

void Cost::height(double h) {
    height_ = h;
}

double& Cost::alpha() {
    return alpha_;
}

double& Cost::width() {
    return width_;
}

double& Cost::height() {
    return height_;
}

void Cost::rotate() {
    swap(width_, height_);
}

static double square(double v) {
    return v * v;
}

static void update(double& reference, double target, size_t rounds) {
    double coeff = 1. / (double)rounds;
    double remaining = 1. - coeff;

    assert(coeff > 0);
    assert(remaining >= 0 && remaining < 1);

    reference = remaining * reference + coeff * target;
}

void Cost::alpha(bool up, size_t rounds) {
    update(alpha_, (double)up, rounds);
}

void Cost::width(bool up, size_t rounds) {
    update(width_, ((double)up) / 2., rounds);
}

void Cost::height(bool up, size_t rounds) {
    update(height_, ((double)up) / 2., rounds);
}

double Cost::operator()(size_t x,
                        size_t xn,
                        size_t y,
                        size_t yn,
                        size_t w,
                        size_t wn,
                        bool accepted) const {
    double value = (alpha_ * (double)(x * y) / (double)(xn * yn) +
                    (1. - alpha_) * ((double)w / (double)wn));

    if (!accepted) {
        value = (1. - width_ - height_) * value +
                width_ * square((double)x / (double)xn) +
                height_ * square((double)y / (double)yn);
    }

    return value;
}

double Cost::operator()(size_t x,
                        size_t xn,
                        size_t y,
                        size_t yn,
                        size_t w,
                        size_t wn,
                        bool accepted,
                        size_t num) {
    update(area_, x * y, num);
    update(hpwl_, w, num);
    return operator()(x, xn, y, yn, w, wn, accepted);
}
