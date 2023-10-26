#include "Mandelbrot.h"

using namespace img;


ColorLUT::ColorLUT()
{
    gamma = 1.0;
    black = std::vector<float>(3);
    black[0] = black[1] = black[2] = 0;

    //init colors and set uint8 for float value 0-1
    bands = std::vector< std::vector<float> >(5);
    std::vector<float> color {255/(float)0xFF, 230/(float)0xFF, 255/(float)0xFF};
    bands[0] = color;
    color = {5/(float)0xFF, 102/(float)0xFF, 153/(float)0xFF};
    bands[1] = color;
    color = {255/(float)0xFF, 179/(float)0xFF, 255/(float)0xFF};
    bands[2] = color;
    color = {77/(float)0xFF, 38/(float)0xFF, 0/(float)0xFF};
    bands[3] = color;
    color = {255/(float)0xFF, 153/(float)0xFF, 255/(float)0xFF};
    bands[4] = color;

}

void ColorLUT::operator()(const double& value, std::vector<float>& C) const
{
    //value can be inf/nan
    if(value > 1 || !std::isfinite(value))
    {
        C = black;
    }
    else
    {
        float x = value * int(bands.size()-1);
        int m = int(x);
        int mPrime = m + 1;
        float weight = x - m;
        if(mPrime > int(bands.size()-1))
        {
            mPrime = m;
        }

        for(size_t i = 0; i < C.size(); i++)
        {
  
            C[i] = (bands[m][i] * (1-weight)) + (bands[mPrime][i] * weight);
        }
 
    }
}

JuliaSet::JuliaSet(const Point& P0, const int nb, const int cycle)
{
    this->c = P0;
    this->nb_iterations = nb;
    this->cycles = cycle;
}

Point JuliaSet::operator()(const Point& P) const
{
    std::complex<double> Pc(P.x,P.y);
    std::complex<double> cc(c.x,c.y); //Zc

    for(int i = 0 ; i < nb_iterations; i++)
    {
        std::complex<double> temp = Pc;
        for(int c = 1; c < cycles; c++)
        {
            temp = temp * Pc;
        }
        Pc = temp + cc;
    }
    Point Pout;
    Pout.x = Pc.real();
    Pout.y = Pc.imag();
    //can return inf/nan
    return Pout;
}

void img::ApplyFractalWarpLUT( const Point& center, const double range, const Warp& w, const ColorLUT& lut, img::ImgProc& out )
{
    float R = 2.0;
    for(int j = 0; j < out.ny(); j++)
    {
        #pragma omp parallel for
        for(int i = 0; i< out.nx(); i++)
        {
            Point P;
            P.x = 2.0*(double)i/(double)out.nx() - 1.0;
            P.y = 2.0*(double)j/(double)out.ny() - 1.0;
            P.x *= range;
            P.y *= range;
            P.x += center.x;
            P.y += center.y;
            Point PP = w(P);
            double rate = std::sqrt(PP.x*PP.x + PP.y*PP.y)/R;
            std::vector<float> v(3,0.0);
            lut(rate,v);
            out.set_value(i, j, v);
        }
    }
}