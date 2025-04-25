#include "b_star.hpp"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <deque>
#include <utility>

#include "boundaries.hpp"

using namespace std;

static size_t left_child(size_t index) {
    return (index << 1) + 1;
}

static size_t right_child(size_t index) {
    return (index << 1) + 2;
}

BStar::BStar(vector<Pin>& pin_list) : pin_list_(pin_list) {
    auto index_list = vector<size_t>();

    Pin::filter_area_nonzero(pin_list, index_list);

    for (size_t idx = 0; idx < index_list.size(); ++idx) {
        Pin& p = pin_list[index_list[idx]];
        if (p.height() > p.width()) {
            p.rotate();
        }
        assert(p.width() >= p.height());
    }

    sort(index_list.rbegin(), index_list.rend(),
         [&](size_t a, size_t b) -> bool {
             const auto &pin_a = pin_list[a], &pin_b = pin_list[b];
             return make_pair(pin_a.width(), pin_a.height()) <
                    make_pair(pin_b.width(), pin_b.height());
         });

    // * heap update
    for (size_t index = 0; index < index_list.size(); ++index) {
        const size_t l = left_child(index), r = right_child(index);
        size_t self = index_list[index];
        Pin& pin = pin_list[self];

        assert(pin.area());
        assert(pin.left() < 0);
        assert(pin.right() < 0);

        if (r == index_list.size()) {
            pin.left(index_list[l]);
        }

        if (r < index_list.size()) {
            pin.left(index_list[l]);
            pin.right(index_list[r]);
        }
    }

    nodes_ = std::move(index_list);
}

size_t BStar::root() const {
    return nodes_[0];
}

vector<Pin>& BStar::pin_list() {
    return pin_list_;
}

const vector<Pin>& BStar::pin_list() const {
    return pin_list_;
}

vector<size_t>& BStar::nodes() {
    return nodes_;
}

const vector<size_t>& BStar::nodes() const {
    return nodes_;
}

static void remove_overlap(Pin& pin,
                           deque<Boundary>& contour,
                           const size_t index) {
    if (pin.x() >= contour[contour.size() - 1].right()) {
        pin.y() = 0;
        return;
    }

    const int total_width = pin.x() + pin.width();

    size_t until;
    int max;

    for (until = index, max = -1;
         until < contour.size() && contour[until].right() < total_width;
         ++until) {
        if (contour[until].top() > max) {
            max = contour[until].top();
        }
    }

    if (until == contour.size()) {
        contour.erase(contour.begin() + index, contour.end());
    } else {
        if (contour[until].top() > max) {
            max = contour[until].top();
        }

        for (; until < contour.size() && contour[until].right() == total_width;
             ++until)
            ;

        contour.erase(contour.begin() + index, contour.begin() + until);
    }
    pin.y() = max;
}

static void update_left(vector<Pin>& pin_list,
                        size_t root,
                        size_t left,
                        deque<Boundary>& contour,
                        size_t affected) {
    const Pin& root_node = pin_list[root];
    Pin& left_node = pin_list[left];

    assert(affected < contour.size());

    left_node.x() = root_node.x() + root_node.width();

    const size_t index = affected + 1;

    remove_overlap(left_node, contour, index);

    contour.insert(contour.begin() + index, Boundary(left_node));

    assert(index < contour.size());
}

static void update_right(vector<Pin>& pin_list,
                         size_t root,
                         size_t right,
                         deque<Boundary>& contour,
                         size_t affected) {
    const Pin& root_node = pin_list[root];
    Pin& right_node = pin_list[right];

    assert(affected < contour.size());

    right_node.x() = root_node.x();

    const size_t index = affected;

    remove_overlap(right_node, contour, index);

    contour.insert(contour.begin() + index, Boundary(right_node));
    assert(index < contour.size());
}

static void update_recursive(vector<Pin>& pin_list,
                             size_t root,
                             deque<Boundary>& contour,
                             size_t affected) {
    const Pin& root_node = pin_list[root];

    auto& bnd = contour[affected];
    assert(bnd == Boundary(root_node));

    int left = root_node.left(), right = root_node.right();

    if (left >= 0) {
        update_left(pin_list, root, left, contour, affected);
        update_recursive(pin_list, left, contour, affected + 1);
    }

    if (right >= 0) {
        update_right(pin_list, root, right, contour, affected);
        update_recursive(pin_list, right, contour, affected);
    }
}

pair<int, int> BStar::update() const {
    auto& pin_list = pin_list_;
    auto contour = deque<Boundary>();
    int r = root();
    Pin& root_node = pin_list[r];
    root_node.x() = root_node.y() = 0;
    contour.emplace_back(root_node);
    update_recursive(pin_list, r, contour, 0);

    int X, Y, i;
    for (i = X = Y = 0; (size_t)i < contour.size(); ++i) {
        const Boundary& bnd = contour[i];
        if (Y < bnd.top())
            Y = bnd.top();

        if (X < bnd.right())
            X = bnd.right();
    }
    return make_pair(X, Y);
}

static void flip_recursive(size_t root, vector<Pin>& pin_list) {
    Pin& pin = pin_list[root];
    swap(pin.x(), pin.y());
    swap(pin.width(), pin.height());
    const int left = pin.left(), right = pin.right();
    if (left >= 0) {
        flip_recursive(left, pin_list);
    }
    if (right >= 0) {
        flip_recursive(right, pin_list);
    }
}

void BStar::flip() {
    flip_recursive(root(), pin_list());
}
