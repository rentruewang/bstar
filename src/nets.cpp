#include "nets.hpp"

#include <cassert>
#include <utility>

using namespace std;

net::net() : connected_pins_(vector<unsigned>()), all_pins_(nullptr) {}

net::net(net&& net)
    : connected_pins_(std::move(net.connected_pins_)),
      all_pins_(net.all_pins_) {
    net.all_pins_ = nullptr;
}

net::net(vector<unsigned>&& conn, const vector<pin>& all_pins_)
    : connected_pins_(std::move(conn)), all_pins_(&all_pins_) {}

net& net::operator=(net&& net) {
    connected_pins_ = std::move(net.connected_pins_);
    all_pins_ = net.all_pins_;
    net.all_pins_ = nullptr;
    return *this;
}

unsigned net::at(unsigned index) const {
    return this->connected_pins_.at(index);
}

static pair<unsigned, unsigned> get_center(const pin& pin) {
    if (!pin.area_nonzero()) {
        assert(pin.width() == 0);
        assert(pin.height() == 0);
    }
    return make_pair(pin.x() + (pin.width() >> 1),
                     pin.y() + (pin.height() >> 1));
}

unsigned net::hpwl() const {
    const auto& pin_list = *all_pins_;

    unsigned max_w, min_w, max_h, min_h;
    auto center = get_center(pin_list[connected_pins_[0]]);

    max_w = min_w = center.first;
    max_h = min_h = center.second;

    unsigned i, x, y;
    bool halt;
    for (i = 1, halt = false; i < connected_pins_.size(); ++i) {
        const pin& pin = pin_list[connected_pins_[i]];
        if (!pin.area_nonzero()) {
            halt = true;
        }
        if (halt) {
            x = pin.x();
            y = pin.y();
            assert(pin.width() == 0);
            assert(pin.height() == 0);
        } else {
            center = get_center(pin_list[connected_pins_[i]]);
            x = center.first;
            y = center.second;
        }
        if (max_w < x) {
            max_w = x;
        }
        if (min_w > x) {
            min_w = x;
        }
        if (max_h < y) {
            max_h = y;
        }
        if (min_h > y) {
            min_h = y;
        }
    }
    assert(max_w >= min_w);
    assert(max_h >= min_h);
    return max_w - min_w + max_h - min_h;
}
