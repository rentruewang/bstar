#include "mutations.hpp"
#include <cstddef>
#include <cstdlib>

mutation::mutation(b_star& tree) noexcept : tree_(tree) {}

void mutation::random() {
    setup_random();
    mutate();
}

void mutation::revert() {
    mutate();
}

permuter::permuter(b_star& tree) noexcept : mutation(tree), idx_(0) {}

void permuter::setup_random() {
    idx_ = rand() % tree_.nodes().size();
}

void permuter::mutate() {
    tree_.pin_list()[tree_.nodes()[idx_]].rotate();
}

swapper::swapper(b_star& tree) noexcept : mutation(tree), i_(0), j_(0) {}

void swapper::setup_random() {
    i_ = (rand() % (tree_.nodes().size() - 1)) + 1;
    j_ = (rand() % (tree_.nodes().size() - 2)) + 1;

    if (i_ <= j_) {
        ++j_;
    }
}

void swapper::mutate() {
    auto& plist = tree_.pin_list();
    pin &pin_i = plist[tree_.nodes()[i_]], &pin_j = plist[tree_.nodes()[j_]];

    std::swap(pin_i.width(), pin_j.width());
    std::swap(pin_i.height(), pin_j.height());
    std::swap(pin_i.name(), pin_j.name());
}

delete_inserter::delete_inserter(b_star& tree) noexcept
    : mutation(tree), from_(0), to_(0), from_side_(false), to_side_(false) {}

void delete_inserter::setup_random() {
    int root;
    bool side;

    auto& plist = tree_.pin_list();

    if (plist[tree_.root()].leaf()) {
        from_ = to_ = 0;
        from_side_ = to_side_ = false;
        return;
    }

    do {
        root = rand() % tree_.nodes().size();
    } while (plist[root].leaf());

    while (true) {
        const pin& root_pin = plist[root];
        int left = root_pin.left(), right = root_pin.right();

        bool both_fine = (left >= 0) && (right >= 0);

        if (left < 0 || (both_fine && (rand() % 2))) {
            const pin& right_node = plist[right];

            if (right_node.leaf()) {
                side = true;
                break;
            }

            root = right;
        } else {
            const pin& left_node = plist[left];

            if (left_node.leaf()) {
                side = false;
                break;
            }

            root = left;
        }
    }

    int from = root;
    bool from_side = side;

    int next;

    do {
        root = rand() % tree_.nodes().size();
    } while (plist[root].leaf());

    while (true) {
        const pin& p = plist[root];

        if (root == from && from_side) {
            if (next = p.left(); next < 0) {
                side = false;
                break;
            }

        } else if (root == from && !from_side) {
            if (next = p.right(); next < 0) {
                side = true;
                break;
            }
        } else if (root != from && rand() % 2) {
            if (next = p.right(); next < 0) {
                side = true;
                break;
            }
        } else {
            if (next = p.left(); next < 0) {
                side = false;
                break;
            }
        }

        root = next;
    }

    from_ = from;
    from_side_ = from_side;
    to_ = root;
    to_side_ = side;
}

void delete_inserter::mutate() {
    auto& plist = tree_.pin_list();
    pin& from_pin = plist[from_];
    pin& to_pin = plist[to_];

    int target;

    if (from_side_) {
        target = from_pin.right();
        from_pin.rm_right();
    } else {
        target = from_pin.left();
        from_pin.rm_left();
    }

    if (to_side_) {
        to_pin.right(target);
    } else {
        to_pin.left(target);
    }
}

mirrorer::mirrorer(b_star& tree) noexcept : mutation(tree), i_(0) {}

void mirrorer::setup_random() {
    i_ = rand() % tree_.pin_list().size();
}

void mirrorer::mutate() {
    pin& pin = tree_.pin_list()[i_];
    std::swap(pin.left(), pin.right());
}
