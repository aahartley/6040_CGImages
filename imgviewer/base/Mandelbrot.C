#include "Mandelbrot.h"

using namespace img;


ColorLUT::ColorLUT(int chan, const double gam)
{
    //minimum channels allowed is 3
    if(chan < 3)
    { 
        std::cout << "Look-up table requires at least 3 channels. It has been set to 3 by default\n";
        chan = 3;
    }
    channels = chan;
    gamma = gam;
    black = std::vector<float>(channels);
    generate_color(black, 0, 0, 0);

    
    bands = std::vector< std::vector<float> >(5);
    std::vector<float> color(channels);
    generate_color(color, 255, 230, 255);
    bands[0] = color;
    generate_color(color, 5, 102, 153); 
    bands[1] = color;
    generate_color(color, 255, 179, 255);
    bands[2] = color;
    generate_color(color, 77, 38, 0); 
    bands[3] = color;
    generate_color(color, 255, 153, 255); 
    bands[4] = color;

}

void ColorLUT::generate_color(std::vector<float>& color, const uint8_t r, const uint8_t g, const uint8_t b) const
{
    //init colors and convert uint8 t0 float value 0-1
    color[0] = std::pow(r/(float)0xFF, gamma);
    color[1] = std::pow(g/(float)0xFF, gamma);
    color[2] = std::pow(b/(float)0xFF, gamma);
    // set alpha and other channels to 1
    for(int i = 3; i < channels; i++)
    {
        color[i] = 1;
    }
  
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
        //doesnt alter alpha/4+ channels
        for(int i = 0; i < 3; i++)
        {
            //linear interpolation
            C[i] = (bands[m][i] * (1-weight)) + (bands[mPrime][i] * weight);
        }
        //this is needed for depth check in imgProc.set_Value() 
        for(int i = 3; i < channels; i++)
        {
            C[i] = bands[m][i]; //sets to 1, future calculations can also be done
        }
    }
}

JuliaSet::JuliaSet(const Point& P0, const int nb, const int cycle)
{
    c = P0;
    nb_iterations = nb;
    cycles = cycle;
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
            std::vector<float> v(out.depth(),0.0);
            lut(rate,v);
            out.set_value(i, j, v);
        }
    }
}