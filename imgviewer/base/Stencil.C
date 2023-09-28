#include "Stencil.h"

 Stencil::Stencil(int halfwidth)
 {
    half_width= halfwidth;
    stencil_values = new float[(2*half_width+1)*(2*half_width+1)];
    const float range_from  = -1.f;
    const float range_to    = 1.f;
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_real_distribution<float>  distr(range_from, range_to);
    float sum =0.f;
    for(int j=0; j<(2*half_width+1);j++)
    {
        for(int i=0; i<(2*half_width+1);i++)
        {
            if(i!=half_width && j!=half_width)
            {
                stencil_values[j*(2*half_width+1) + i] = distr(generator);
                sum += stencil_values[i*(2*half_width+1) + j];
                //std::cout << stencil_values[i*(2*half_width+1) + j] << '\n';
            }
        }
    }
    stencil_values[half_width*(2*half_width+1) + half_width] = 1-sum;
    //std::cout << sum+stencil_values[half_width*(2*half_width+1) + half_width] << '\n';

 }

 Stencil::~Stencil()
 {
    delete[] stencil_values;
 }

 int Stencil::halfwidth() const
 {
    return half_width;
 }
 float& Stencil::operator()(int i, int j)
 {
    return stencil_values[j*(2*half_width+1) + i];
 }
 const float& Stencil::operator()(int i, int j) const
 {
    return stencil_values[j*(2*half_width+1) + i];
 }