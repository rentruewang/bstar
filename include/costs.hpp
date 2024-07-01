#pragma once

#include <cstddef>

class cost {
   public:
    cost();
    cost(double init_area,
         double init_hpwl,
         double init_alpha,
         double init_ratio);

    double area() const;
    double hpwl() const;

    double alpha() const;
    double width() const;
    double height() const;

    void area(double area);
    void hpwl(double hpwl);

    void alpha(double alpha);
    void width(double width);
    void height(double height);

    double& alpha();
    double& width();
    double& height();

    void rotate();

    void alpha(bool up, size_t rounds);
    void width(bool up, size_t rounds);
    void height(bool up, size_t rounds);

    double operator()(size_t width,
                      size_t width_norm,
                      size_t height,
                      size_t height_norm,
                      size_t hpwl,
                      size_t net_norm,
                      bool accepted) const;

    double operator()(size_t width,
                      size_t width_norm,
                      size_t height,
                      size_t height_norm,
                      size_t hpwl,
                      size_t net_norm,
                      bool accepted,
                      size_t rounds);

   private:
    double area_, hpwl_;
    double alpha_, width_, height_;
};
