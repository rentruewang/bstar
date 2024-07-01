#pragma once

#include "b_star.hpp"

class mutation {
   public:
    mutation(b_star& tree) noexcept;
    virtual ~mutation() {}

    void random();
    void revert();

    virtual void setup_random() = 0;
    virtual void mutate() = 0;

   protected:
    b_star& tree_;
};

class permuter final : public mutation {
   public:
    permuter(b_star& tree) noexcept;

    void setup_random();
    void mutate();

   private:
    unsigned idx_;
};

class swapper final : public mutation {
   public:
    swapper(b_star& tree) noexcept;

    void setup_random();
    void mutate();

   private:
    unsigned i_, j_;
};

class delete_inserter final : public mutation {
   public:
    delete_inserter(b_star& tree) noexcept;

    void setup_random();
    void mutate();

   private:
    int from_, to_;
    bool from_side_, to_side_;
};

class mirrorer final : public mutation {
   public:
    mirrorer(b_star& tree) noexcept;

    void setup_random();
    void mutate();

   private:
    unsigned i_;
};
