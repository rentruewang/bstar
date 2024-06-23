#pragma once

#include <string>
#include <utility>
#include <vector>

class pin {
   public:
    pin();
    pin(const pin& pin);
    pin(pin&& pin);
    pin(unsigned i, unsigned j, bool b, const std::string& n);

    pin& operator=(const pin& pin);
    pin& operator=(pin&& pin);

    int x() const;
    int y() const;

    int& x();
    int& y();

    void x(int x);
    void y(int y);

    std::pair<int, int> loc() const;
    void loc(int x, int y);

    unsigned width() const;
    unsigned height() const;

    unsigned& width();
    unsigned& height();

    void width(unsigned w);
    void height(unsigned h);

    std::pair<unsigned, unsigned> get_dim() const;
    void set_dim(unsigned w, unsigned h);

    const std::string& name() const;
    void set_name(std::string&& n);

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

    bool area_nonzero() const;
    bool leaf() const;

    static void filter_area_nonzero(std::vector<pin>& pin_list,
                                    std::vector<unsigned>& block_list);

   private:
    int x_, y_;
    unsigned w_, h_;
    int l_, r_;
    std::string name_;
};
