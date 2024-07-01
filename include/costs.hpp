#pragma once

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

    void alpha(bool up, unsigned rounds);
    void width(bool up, unsigned rounds);
    void height(bool up, unsigned rounds);

    double operator()(unsigned width,
                      unsigned width_norm,
                      unsigned height,
                      unsigned height_norm,
                      unsigned hpwl,
                      unsigned net_norm,
                      bool accepted) const;

    double operator()(unsigned width,
                      unsigned width_norm,
                      unsigned height,
                      unsigned height_norm,
                      unsigned hpwl,
                      unsigned net_norm,
                      bool accepted,
                      unsigned rounds);

   private:
    double area_, hpwl_;

    double alpha_, width_, height_;
};
