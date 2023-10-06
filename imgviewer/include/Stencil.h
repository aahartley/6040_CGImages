#ifndef STENCIL_H
#define STENCIL_H

#include <iostream>
#include <random>

class Stencil
{
  public:
    Stencil(int halfwidth);
    ~Stencil();

    int halfwidth() const;
    float& operator()(int i, int j);
    const float& operator()(int i, int j) const;

  private:
    int half_width;
    float *stencil_values;
};

#endif