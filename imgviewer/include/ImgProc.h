//-------------------------------------------------------
//
//  ImgProc.h
//
//  Stores the image pixels and the methods to manipulate
//  them. Handles the IO of images.
//  
//--------------------------------------------------------
#ifndef IMGPROC_H
#define IMGPROC_H

#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <ctime>

#include <OpenImageIO/imageio.h>

#include "Constants.h"
#include "Stencil.h"

namespace img{

class ImgProc
{
  public:
	
	ImgProc();
	~ImgProc();

	void clear ();
	void clear(int nX, int nY, int nC);

	int nx() const { return Nx; } 
	int ny() const { return Ny; }
	int depth() const { return Nc; }
	int size() const {return Nsize;}
	float* raw() const { return img_data;}

	void set_raw(float* data);
	void set_raw_OI(float* data);

	
	void value(int i, int j, std::vector<float>& pixel) const;
	void set_value(int i, int j, const std::vector<float>& pixel);

    int read_image(const std::string& s);
	void write_image(std::string fileName, char f) const;

	void PeriodicLinearConvolution( const Stencil& stencil, ImgProc& out ) const;
	void BoundedLinearConvolution( const Stencil& stencil, ImgProc& out ) const;
	void Gamma(ImgProc& out, float s) const;
	void GammaInPlace(float s);

	//Affine transformations
	void Flip();
	void Flop();


	ImgProc(const ImgProc& img); //copy constructor
	ImgProc& operator=(const ImgProc& img); //copy assignment

  private:	
  
	int Nx, Ny, Nc;
	long Nsize;
	float* img_data;
};

int read_image(const std::string& s, ImgProc& imgProc);
void write_image(std::string fileName, char f, const ImgProc& imgProc);

//Affine transformations
void Flip(ImgProc& in);
void Flop(ImgProc& in);

void PeriodicLinearConvolution( const Stencil& stencil, const ImgProc& in, ImgProc& out );
void BoundedLinearConvolution( const Stencil& stencil, const ImgProc& in, ImgProc& out );
void Gamma(const ImgProc& in, ImgProc& out, float s);
void GammaInPlace(ImgProc& in, float s);

}
#endif
