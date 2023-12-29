//-------------------------------------------------------
//
//  ImageProcessing.h
//
//  Holds Stencil and image algorithmns
//  
//  
//--------------------------------------------------------
#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H
 
#include <iostream>
#include <random>

#include "ImgProc.h"

namespace img{

class Stencil
{
  public:
    Stencil():half_width(0), stencil_values(0){};
    Stencil(int halfwidth);
    ~Stencil();

    int halfwidth() const;
    float& operator()(int i, int j);
    const float& operator()(int i, int j) const;
    float* values() const { return stencil_values;}
    void setHalfWidth(int halfwidth) { half_width = halfwidth; stencil_values = new float[(2*half_width+1)*(2*half_width+1)];}

  private:
    int half_width;
    float* stencil_values;
};
// test stencils
Stencil makeSharpen3x3Filter();
Stencil makeEdgeDetect3x3Filter();
Stencil makeEmboss3x3Filter();



//Algorithms
void PeriodicLinearConvolution( const Stencil& stencil, const ImgProc& in, ImgProc& out );
void BoundedLinearConvolution( const Stencil& stencil, const ImgProc& in, ImgProc& out );
void Gamma(const ImgProc& in, ImgProc& out, const float s);
void Gamma(ImgProc& out, const float s);
void Brightness(const ImgProc& in, ImgProc& out, const float s);
void Brightness(ImgProc& out, const float s);
void Bias(const ImgProc& in, ImgProc& out, const std::vector<float>& color);
void Bias(ImgProc& out, const std::vector<float>& color);
void Compliment(const ImgProc& in, ImgProc& out);
void Compliment(ImgProc& out);
void Grayscale(const ImgProc& in, ImgProc& out);
void Grayscale(ImgProc& out);
void Quantize(const ImgProc& in, ImgProc& out, int levels);
void Quantize(ImgProc& out, float levels);
}








#endif