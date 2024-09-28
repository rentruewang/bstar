#pragma once

#include <string>
#include <utility>
#include <vector>

class pin {
   public:
    pin();
    pin(const pin& pin);
    pin(pin&& pin);
    pin(size_t i, size_t j, bool b, const std::string& n);

    pin& operator=(const pin& pin);
    pin& operator=(pin&& pin);

    void reset();

    int& x();
    int& y();

    int x() const;
    int y() const;

    std::pair<int, int> loc() const;

    size_t width() const;
    size_t height() const;

    size_t& width();
    size_t& height();

    std::pair<size_t, size_t> dim() const;

    const std::string& name() const;

    std::string& name();

    void rotate();

    int left() const;
    int right() const;

    int& left();
    int& right();

    bool has_left() const;
    bool has_right() const;

    void left(int l);
    void right(int r);

    void rm_left();
    void rm_right();

    /// Compute the area of the pin.
    int area() const;
    bool leaf() const;

    /// Select the pins that have non zero area.
    static void filter_area_nonzero(std::vector<pin>& pin_list,
                                    std::vector<size_t>& block_list);

   private:
    int x_, y_;
    size_t w_, h_;
    int l_, r_;
    std::string name_;
};
