//-------------------------------------------------------
//
//  Mandelbrot.h
//
//  Contains the mandelbrot look up table and the JuliaSet calculations
//  
//  
//--------------------------------------------------------
#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <complex>
#include <cmath>

#include "Warp.h"
#include "ImgProc.h"

namespace img
{

class ColorLUT
{
public:
    //don't allow default constructor
    ColorLUT(int chan, const double gam);
    ~ColorLUT(){}
  
    void generate_color(std::vector<float>& color, const uint8_t r, const uint8_t g, const uint8_t b) const;

    // Generate color value from the input value
    void operator()(const double& value, std::vector<float>& C) const;
  
private:  
    double gamma;
    int channels;
    std::vector<float> black;
    std::vector< std::vector<float> > bands;
};  
  
  
class JuliaSet : public Warp
{
public:
    //don't allow default constructor
    JuliaSet(const Point& P0, const int nb, const int cycle);
    ~JuliaSet(){}
    
    //calcute complex number
    Point operator()(const Point& P) const;

private:
    Point c;
    int nb_iterations;
    int cycles;
  
};  
void ApplyFractalWarpLUT(const Point& center, const double range, const Warp& w, const ColorLUT& lut, img::ImgProc& out);

}
  
#endif