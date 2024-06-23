#include "costs.hpp"
#include <assert.h>
#include <math.h>
#include <algorithm>

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

void cost::set_area(double a) {
    area_ = a;
}

void cost::set_hpwl(double h) {
    hpwl_ = h;
}

void cost::set_alpha(double a) {
    alpha_ = a;
}

void cost::set_width(double w) {
    width_ = w;
}

void cost::set_height(double h) {
    height_ = h;
}

double& cost::get_alpha_mut() {
    return alpha_;
}

double& cost::get_width_mut() {
    return width_;
}

double& cost::get_height_mut() {
    return height_;
}

void cost::rotate() {
    swap(width_, height_);
}

static double square(double v) {
    return v * v;
}

static void update_variable(double& reference, double target, unsigned rounds) {
    double coeff = 1. / (double)rounds;
    double remaining = 1. - coeff;

    assert(coeff > 0);
    assert(remaining >= 0 && remaining < 1);

    reference = remaining * reference + coeff * target;
}

void cost::update_alpha(bool up, unsigned rounds) {
    update_variable(alpha_, (double)up, rounds);
}

void cost::update_width(bool up, unsigned rounds) {
    update_variable(width_, ((double)up) / 2., rounds);
}

void cost::update_height(bool up, unsigned rounds) {
    update_variable(height_, ((double)up) / 2., rounds);
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
    update_variable(area_, x * y, num);
    update_variable(hpwl_, w, num);
    return operator()(x, xn, y, yn, w, wn, accepted);
}
