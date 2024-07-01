#include "costs.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>

using namespace std;

cost::cost() : area_(1.), hpwl_(1.), alpha_(.5) {}

cost::cost(double init_area,
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

double cost::area() const {
    return area_;
}

double cost::hpwl() const {
    return hpwl_;
}

double cost::alpha() const {
    return alpha_;
}

double cost::width() const {
    return width_;
}

double cost::height() const {
    return height_;
}

void cost::area(double a) {
    area_ = a;
}

void cost::hpwl(double h) {
    hpwl_ = h;
}

void cost::alpha(double a) {
    alpha_ = a;
}

void cost::width(double w) {
    width_ = w;
}

void cost::height(double h) {
    height_ = h;
}

double& cost::alpha() {
    return alpha_;
}

double& cost::width() {
    return width_;
}

double& cost::height() {
    return height_;
}

void cost::rotate() {
    swap(width_, height_);
}

static double square(double v) {
    return v * v;
}

static void update(double& reference, double target, unsigned rounds) {
    double coeff = 1. / (double)rounds;
    double remaining = 1. - coeff;

    assert(coeff > 0);
    assert(remaining >= 0 && remaining < 1);

    reference = remaining * reference + coeff * target;
}

void cost::alpha(bool up, unsigned rounds) {
    update(alpha_, (double)up, rounds);
}

void cost::width(bool up, unsigned rounds) {
    update(width_, ((double)up) / 2., rounds);
}

void cost::height(bool up, unsigned rounds) {
    update(height_, ((double)up) / 2., rounds);
}

double cost::operator()(unsigned x,
                        unsigned xn,
                        unsigned y,
                        unsigned yn,
                        unsigned w,
                        unsigned wn,
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

double cost::operator()(unsigned x,
                        unsigned xn,
                        unsigned y,
                        unsigned yn,
                        unsigned w,
                        unsigned wn,
                        bool accepted,
                        unsigned num) {
    update(area_, x * y, num);
    update(hpwl_, w, num);
    return operator()(x, xn, y, yn, w, wn, accepted);
}
