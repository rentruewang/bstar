#pragma once

#include "b_star.hpp"

class Mutator {
   public:
    Mutator(BStar& tree) noexcept;
    virtual ~Mutator() {}

    void random();
    void revert();

    virtual void setup_random() = 0;
    virtual void mutate() = 0;

   protected:
    BStar& tree_;
};

class Permuter final : public Mutator {
   public:
    Permuter(BStar& tree) noexcept;

    void setup_random();
    void mutate();

   private:
    size_t idx_;
};

class Swapper final : public Mutator {
   public:
    Swapper(BStar& tree) noexcept;

    void setup_random();
    void mutate();

   private:
    size_t i_, j_;
};

class DeleteInserter final : public Mutator {
   public:
    DeleteInserter(BStar& tree) noexcept;

    void setup_random();
    void mutate();

   private:
    int from_, to_;
    bool from_side_, to_side_;
};

class Mirrorer final : public Mutator {
   public:
    Mirrorer(BStar& tree) noexcept;

    void setup_random();
    void mutate();

   private:
    size_t i_;
};
