#include "nets.hpp"

#include <cassert>
#include <utility>

using namespace std;

net::net() : connected_pins_(vector<size_t>()), all_pins_(nullptr) {}

net::net(net&& net)
    : connected_pins_(std::move(net.connected_pins_)),
      all_pins_(net.all_pins_) {
    net.all_pins_ = nullptr;
}

net::net(vector<size_t>&& conn, const vector<pin>& all_pins_)
    : connected_pins_(std::move(conn)), all_pins_(&all_pins_) {}

net& net::operator=(net&& net) {
    connected_pins_ = std::move(net.connected_pins_);
    all_pins_ = net.all_pins_;
    net.all_pins_ = nullptr;
    return *this;
}

size_t net::at(size_t index) const {
    return this->connected_pins_.at(index);
}

static pair<size_t, size_t> get_center(const pin& pin) {
    if (!pin.area()) {
        assert(pin.width() == 0);
        assert(pin.height() == 0);
    }
    return make_pair(pin.x() + (pin.width() >> 1),
                     pin.y() + (pin.height() >> 1));
}

size_t net::hpwl() const {
    const auto& pin_list = *all_pins_;

    size_t max_w, min_w, max_h, min_h;
    auto center = get_center(pin_list[connected_pins_[0]]);

    max_w = min_w = center.first;
    max_h = min_h = center.second;

    size_t i, w, h;
    bool halt;
    for (i = 1, halt = false; i < connected_pins_.size(); ++i) {
        const pin& pin = pin_list[connected_pins_[i]];
        if (!pin.area()) {
            halt = true;
        }

        if (halt) {
            w = pin.x();
            h = pin.y();
            assert(pin.width() == 0);
            assert(pin.height() == 0);
        } else {
            center = get_center(pin_list[connected_pins_[i]]);
            w = center.first;
            h = center.second;
        }

        max_w = max_w >= w ? max_w : w;
        min_w = min_w <= w ? min_w : w;
        max_h = max_h >= h ? max_h : h;
        min_h = min_h <= h ? min_h : h;
    }
    assert(max_w >= min_w);
    assert(max_h >= min_h);
    return max_w - min_w + max_h - min_h;
}
