#include "pins.hpp"

#include <assert.h>

using namespace std;

pin::pin() : x_(0), y_(0), w_(0), h_(0), l_(-1), r_(-1), name_("") {}

pin::pin(const pin& pin)
    : x_(pin.x_),
      y_(pin.y_),
      w_(pin.w_),
      h_(pin.h_),
      l_(pin.l_),
      r_(pin.r_),
      name_(pin.name_) {}

pin::pin(pin&& pin)
    : x_(pin.x_),
      y_(pin.y_),
      w_(pin.w_),
      h_(pin.h_),
      l_(pin.l_),
      r_(pin.r_),
      name_(std::move(pin.name_)) {}

pin::pin(unsigned i, unsigned j, bool b, const string& n)
    : l_(-1), r_(-1), name_(n) {
    if (b) {
        loc(0, 0);
        set_dim(i, j);
    } else {
        loc(i, j);
        set_dim(0, 0);
    }
}

pin& pin::operator=(const pin& pin) {
    x_ = pin.x_;
    y_ = pin.y_;
    w_ = pin.w_;
    h_ = pin.h_;
    l_ = pin.l_;
    r_ = pin.r_;
    name_ = pin.name_;
    return *this;
}

pin& pin::operator=(pin&& pin) {
    x_ = pin.x_;
    y_ = pin.y_;
    w_ = pin.w_;
    h_ = pin.h_;
    l_ = pin.l_;
    r_ = pin.r_;
    name_ = std::move(pin.name_);
    return *this;
}

int pin::x() const {
    return x_;
}

int pin::y() const {
    return y_;
}

int& pin::x() {
    return x_;
}

int& pin::y() {
    return y_;
}

void pin::x(int x) {
    x_ = x;
}

void pin::y(int y) {
    y_ = y;
}

pair<int, int> pin::loc() const {
    return make_pair(x_, y_);
}

void pin::loc(int a, int b) {
    x(a);
    y(b);
}

unsigned pin::width() const {
    return w_;
}

unsigned pin::height() const {
    return h_;
}

unsigned& pin::width() {
    return w_;
}

unsigned& pin::height() {
    return h_;
}

void pin::width(unsigned w) {
    w_ = w;
}

void pin::height(unsigned h) {
    h_ = h;
}

pair<unsigned, unsigned> pin::dim() const {
    return make_pair(w_, h_);
}

void pin::set_dim(unsigned w, unsigned h) {
    width(w);
    height(h);
}

const string& pin::name() const {
    return name_;
}

void pin::set_name(string&& n) {
    name_ = std::move(n);
}

string& pin::name() {
    return name_;
}

void pin::rotate() {
    assert(area_nonzero());
    unsigned w = width(), h = height();
    set_dim(h, w);
}

int pin::left() const {
    return l_;
}

int pin::right() const {
    return r_;
}

int& pin::left() {
    return l_;
}

int& pin::right() {
    return r_;
}

bool pin::has_left() const {
    return left() >= 0;
}

bool pin::has_right() const {
    return right() >= 0;
}

void pin::left(int l) {
    l_ = l;
}

void pin::right(int r) {
    r_ = r;
}

void pin::rm_left() {
    left(-1);
}

void pin::rm_right() {
    right(-1);
}

bool pin::area_nonzero() const {
    return width() && height();
}

bool pin::leaf() const {
    return (l_ < 0) && (r_ < 0);
}

void pin::filter_area_nonzero(vector<pin>& pin_list,
                              vector<unsigned>& block_list) {
    assert(block_list.size() == 0);
    for (unsigned i = 0; i < pin_list.size(); ++i) {
        if (pin_list[i].area_nonzero()) {
            block_list.push_back(i);
        }
    }
}
