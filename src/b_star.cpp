#include "b_star.hpp"
#include <assert.h>
#include <stdlib.h>
#include <algorithm>
#include <deque>
#include "boundaries.hpp"

using namespace std;

static unsigned left_child(unsigned index) {
    return (index << 1) + 1;
}

static unsigned right_child(unsigned index) {
    return (index << 1) + 2;
}

b_star::b_star(vector<pin>& pin_list, unsigned width, unsigned height)
    : pin_list_(pin_list) {
    auto index_list = vector<unsigned>();

    pin::filter_area_nonzero(pin_list, index_list);

    for (unsigned idx = 0; idx < index_list.size(); ++idx) {
        pin& p = pin_list[index_list[idx]];
        if (p.height() > p.width()) {
            p.rotate();
        }
        assert(p.width() >= p.height());
    }

    sort(index_list.rbegin(), index_list.rend(),
         [&](unsigned a, unsigned b) -> bool {
             const auto &pin_a = pin_list[a], &pin_b = pin_list[b];
             if (pin_a.width() < pin_b.width()) {
                 return true;
             }
             if (pin_a.width() > pin_b.width()) {
                 return false;
             }
             return pin_a.height() < pin_b.height();
         });

    // * heap update
    for (unsigned index = 0; index < index_list.size(); ++index) {
        const unsigned l = left_child(index), r = right_child(index);
        unsigned self = index_list[index];
        pin& pin = pin_list[self];
        assert(pin.area_nonzero());

        assert(pin.left() < 0);
        assert(pin.right() < 0);
        if (r == index_list.size()) {
            pin.left(index_list[l]);
        } else if (r < index_list.size()) {
            pin.left(index_list[l]);
            pin.right(index_list[r]);
        }
    }

    this->nodes_ = std::move(index_list);
}

unsigned b_star::get_size() const {
    return nodes_.size();
}

unsigned b_star::get_root() const {
    return nodes_[0];
}

vector<pin>& b_star::pin_list() {
    return pin_list_;
}

const vector<pin>& b_star::pin_list() const {
    return pin_list_;
}

static unsigned size_recursive(unsigned root, const vector<pin>& pin_list) {
    unsigned ans = 0;
    int id;
    const pin& root_node = pin_list[root];
    if ((id = root_node.left()) >= 0) {
        ans += size_recursive(id, pin_list);
    }
    if ((id = root_node.right()) >= 0) {
        ans += size_recursive(id, pin_list);
    }
    return ans + 1;
}

static void overlap_recursive(unsigned root, const vector<pin>& pin_list) {
    const pin& root_node = pin_list[root];
    int id;

    if ((id = root_node.left()) >= 0) {
        const pin& left_node = pin_list[id];
        assert(root_node.x() + (int)root_node.width() == left_node.x());
    }

    if ((id = root_node.right()) >= 0) {
        const pin& right_node = pin_list[id];
        assert(root_node.x() == right_node.x());
        assert(root_node.y() + (int)root_node.height() <= right_node.y());
    }

    if (root_node.left() >= 0) {
        overlap_recursive(root_node.left(), pin_list);
    }

    if (root_node.right() >= 0) {
        overlap_recursive(root_node.right(), pin_list);
    }
}

void b_star::check() const {
#ifdef NDEBUG
    return;
#endif
    assert(size_recursive(get_root(), pin_list()) == get_size());
    overlap_recursive(get_root(), pin_list());
}

static void remove_overlap(pin& pin,
                           deque<boundary>& contour,
                           const unsigned index) {
    assert(index <= contour.size());
    assert(contour.size());

    if (pin.x() >= contour[contour.size() - 1].right()) {
        assert(index == contour.size());
        pin.y(0);
        return;
    }

    const int total_width = pin.x() + pin.width();

    unsigned until;
    int max;

    for (until = index, max = -1;
         until < contour.size() && contour[until].right() < total_width;
         ++until) {
        if (contour[until].top() > max) {
            max = contour[until].top();
        }
    }

    assert(until <= contour.size());

    if (until == contour.size()) {
        contour.erase(contour.begin() + index, contour.end());
    } else {
        assert(until < contour.size());
        assert(contour[until].right() >= total_width);

        if (contour[until].top() > max) {
            max = contour[until].top();
        }

        for (; until < contour.size() && contour[until].right() == total_width;
             ++until)
            ;

        contour.erase(contour.begin() + index, contour.begin() + until);
    }
    pin.y(max);
}

static void update_left(vector<pin>& pin_list,
                        unsigned root,
                        unsigned left,
                        deque<boundary>& contour,
                        unsigned affected) {
    const pin& root_node = pin_list[root];
    pin& left_node = pin_list[left];

    assert(affected < contour.size());

    left_node.x(root_node.x() + root_node.width());

    const unsigned index = affected + 1;

    remove_overlap(left_node, contour, index);

    contour.insert(contour.begin() + index, std::move(boundary(left_node)));

    assert(index < contour.size());
}

static void update_right(vector<pin>& pin_list,
                         unsigned root,
                         unsigned right,
                         deque<boundary>& contour,
                         unsigned affected) {
    const pin& root_node = pin_list[root];
    pin& right_node = pin_list[right];

    assert(affected < contour.size());

    right_node.x(root_node.x());

    const unsigned index = affected;

    remove_overlap(right_node, contour, index);

    contour.insert(contour.begin() + index, std::move(boundary(right_node)));
    assert(index < contour.size());
}

static void update_recursive(vector<pin>& pin_list,
                             unsigned root,
                             deque<boundary>& contour,
                             unsigned affected) {
    const pin& root_node = pin_list[root];

    auto& bnd = contour[affected];
    assert(bnd == boundary(root_node));

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

pair<int, int> b_star::update() const {
    auto& pin_list = pin_list_;
    auto contour = deque<boundary>();
    int root = get_root();
    pin& root_node = pin_list[root];
    root_node.loc(0, 0);
    contour.emplace_back(root_node);
    update_recursive(pin_list, root, contour, 0);

    int X, Y, i;
    for (i = X = Y = 0; (unsigned)i < contour.size(); ++i) {
        const boundary& bnd = contour[i];
        if (Y < bnd.top())
            Y = bnd.top();

        if (X < bnd.right())
            X = bnd.right();
    }
    return make_pair(X, Y);
}

static void flip_recursive(unsigned root, vector<pin>& pin_list) {
    pin& pin = pin_list[root];
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

void b_star::flip() {
    flip_recursive(get_root(), pin_list());
}

void b_star::permute(const unsigned idx) {
    pin_list()[nodes_[idx]].rotate();
}

void b_star::swap(const unsigned i, const unsigned j) {
    auto& plist = pin_list();
    pin &pin_i = plist[nodes_[i]], &pin_j = plist[nodes_[j]];

    assert(i != j);
    assert(pin_i.name() != pin_j.name());

    std::swap(pin_i.width(), pin_j.width());
    std::swap(pin_i.height(), pin_j.height());
    std::swap(pin_i.name(), pin_j.name());
}

unsigned b_star::random_permute() {
    unsigned r = rand() % nodes_.size();
    permute(r);
    return r;
}

void b_star::delete_insert(unsigned from,
                           unsigned to,
                           bool from_side,
                           bool to_side) {
    auto& plist = pin_list();
    pin& from_pin = plist[from];
    pin& to_pin = plist[to];

    int target;

    if (from_side) {
        assert(from_pin.has_right());
        target = from_pin.right();
        from_pin.rm_right();
        assert(!from_pin.has_right());
    } else {
        assert(from_pin.has_left());
        target = from_pin.left();
        from_pin.rm_left();
        assert(!from_pin.has_left());
    }

    assert(target >= 0);

    const pin& target_pin = plist[target];

    assert(target_pin.leaf());

    if (to_side) {
        assert(!to_pin.has_right());
        to_pin.right(target);
        assert(to_pin.has_right());
    } else {
        assert(!to_pin.has_left());
        to_pin.left(target);
        assert(to_pin.has_left());
    }
}

void b_star::mirror(const unsigned i) {
    pin& pin = pin_list()[i];
    std::swap(pin.left(), pin.right());
}

constexpr bool fix_root = false;

pair<unsigned, unsigned> b_star::random_swap() {
    unsigned a, b;
    if (fix_root) {
        a = (rand() % (nodes_.size() - 1)) + 1;
        b = (rand() % (nodes_.size() - 2)) + 1;
        assert(a != 0);
        assert(b != 0);
    } else {
        a = rand() % nodes_.size();
        b = rand() % (nodes_.size() - 1);
    }
    if (b >= a)
        ++b;
    assert(a != b);
    this->swap(a, b);
    return make_pair(a, b);
}

pair<pair<unsigned, bool>, pair<unsigned, bool>> b_star::random_delete_insert(
    void) {
    int root;
    bool side;

    auto& plist = pin_list();

    if (plist[get_root()].leaf()) {
        return make_pair(make_pair(0U, false), make_pair(0U, false));
    }

    do {
        root = rand() % get_size();
    } while (plist[root].leaf());

    while (true) {
        const pin& root_pin = plist[root];
        int left = root_pin.left(), right = root_pin.right();

        assert(!root_pin.leaf());

        bool both_fine = (left >= 0) && (right >= 0);

        if (left < 0 || (both_fine && (rand() % 2))) {
            assert(right >= 0 && right < (int)nodes_.size());
            const pin& right_node = plist[right];

            if (right_node.leaf()) {
                side = true;
                break;
            }

            root = right;
        } else {
            if (right < 0) {
                assert(left >= 0);
            } else {
                assert(both_fine);
            }

            const pin& left_node = plist[left];

            if (left_node.leaf()) {
                side = false;
                break;
            }

            root = left;
        }
    }

    assert(root >= 0);

    const int from = root;
    const bool from_side = side;

    int next;

    do {
        root = rand() % get_size();
    } while (plist[root].leaf());

    while (true) {
        const pin& pin = plist[root];
        if (root == from) {
            if (from_side) {
                next = pin.left();
                if (next < 0) {
                    side = false;
                    break;
                }

            } else {
                next = pin.right();
                if (next < 0) {
                    side = true;
                    break;
                }
            }
        } else {
            if (rand() % 2) {
                next = pin.right();
                if (next < 0) {
                    side = true;
                    break;
                }
            } else {
                next = pin.left();
                if (next < 0) {
                    side = false;
                    break;
                }
            }
        }
        root = next;
    }

    assert(root >= 0);

    const int to = root;
    const bool to_side = side;

    this->delete_insert(from, to, from_side, to_side);

    return make_pair(make_pair(from, from_side), make_pair(to, to_side));
}

unsigned b_star::random_mirror() {
    int r = rand() % nodes_.size();
    this->mirror(r);
    return r;
}

void b_star::revert_permute(unsigned action) {
    this->permute(action);
}

void b_star::revert_swap(pair<unsigned, unsigned> action) {
    this->swap(action.first, action.second);
}

void b_star::revert_delete_insert(
    const pair<pair<unsigned, bool>, pair<unsigned, bool>> action) {
    const auto from_pair = action.first, to_pair = action.second;
    const auto from = from_pair.first, to = to_pair.first;
    const auto from_side = from_pair.second, to_side = to_pair.second;
    this->delete_insert(to, from, to_side, from_side);
}

void b_star::revert_mirror(unsigned i) {
    this->mirror(i);
}
