//-------------------------------------------------------
//
//  ImageProcessing.h
//
//  HJolds Stencil and image algorithmns
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
    Stencil(int halfwidth);
    ~Stencil();

    int halfwidth() const;
    float& operator()(int i, int j);
    const float& operator()(int i, int j) const;

  private:
    int half_width;
    float *stencil_values;
};

//Algorithms
void PeriodicLinearConvolution( const Stencil& stencil, const ImgProc& in, ImgProc& out );
void BoundedLinearConvolution( const Stencil& stencil, const ImgProc& in, ImgProc& out );
void Gamma(const ImgProc& in, ImgProc& out, float s);
void Gamma(ImgProc& out, float s);

}








#endif