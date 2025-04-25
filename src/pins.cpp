#include "pins.hpp"

#include <assert.h>

#include <utility>

using namespace std;

Pin::Pin() : x_(0), y_(0), w_(0), h_(0), l_(-1), r_(-1), name_("") {}

Pin::Pin(const Pin& pin)
    : x_(pin.x_),
      y_(pin.y_),
      w_(pin.w_),
      h_(pin.h_),
      l_(pin.l_),
      r_(pin.r_),
      name_(pin.name_) {}

Pin::Pin(Pin&& pin)
    : x_(pin.x_),
      y_(pin.y_),
      w_(pin.w_),
      h_(pin.h_),
      l_(pin.l_),
      r_(pin.r_),
      name_(std::move(pin.name_)) {}

Pin::Pin(size_t i, size_t j, bool b, const string& n)
    : l_(-1), r_(-1), name_(n) {
    if (b) {
        x_ = y_ = 0;
        w_ = i;
        h_ = j;
    } else {
        x_ = i;
        y_ = j;
        w_ = h_ = 0;
    }
}

Pin& Pin::operator=(const Pin& pin) {
    x_ = pin.x_;
    y_ = pin.y_;
    w_ = pin.w_;
    h_ = pin.h_;
    l_ = pin.l_;
    r_ = pin.r_;
    name_ = pin.name_;
    return *this;
}

Pin& Pin::operator=(Pin&& pin) {
    x_ = pin.x_;
    y_ = pin.y_;
    w_ = pin.w_;
    h_ = pin.h_;
    l_ = pin.l_;
    r_ = pin.r_;
    name_ = std::move(pin.name_);
    return *this;
}

int Pin::x() const {
    return x_;
}

int Pin::y() const {
    return y_;
}

int& Pin::x() {
    return x_;
}

int& Pin::y() {
    return y_;
}

pair<int, int> Pin::loc() const {
    return {x_, y_};
}

size_t Pin::width() const {
    return w_;
}

size_t Pin::height() const {
    return h_;
}

size_t& Pin::width() {
    return w_;
}

size_t& Pin::height() {
    return h_;
}

pair<size_t, size_t> Pin::dim() const {
    return {w_, h_};
}

const string& Pin::name() const {
    return name_;
}

string& Pin::name() {
    return name_;
}

void Pin::rotate() {
    assert(area());
    std::swap(w_, h_);
}

int Pin::left() const {
    return l_;
}

int Pin::right() const {
    return r_;
}

int& Pin::left() {
    return l_;
}

int& Pin::right() {
    return r_;
}

bool Pin::has_left() const {
    return left() >= 0;
}

bool Pin::has_right() const {
    return right() >= 0;
}

void Pin::left(int l) {
    l_ = l;
}

void Pin::right(int r) {
    r_ = r;
}

void Pin::rm_left() {
    left(-1);
}

void Pin::rm_right() {
    right(-1);
}

int Pin::area() const {
    return width() * height();
}

bool Pin::leaf() const {
    return (l_ < 0) && (r_ < 0);
}

void Pin::filter_area_nonzero(vector<Pin>& pin_list,
                              vector<size_t>& block_list) {
    assert(block_list.size() == 0);
    for (size_t i = 0; i < pin_list.size(); ++i) {
        if (pin_list[i].area()) {
            block_list.push_back(i);
        }
    }
}
