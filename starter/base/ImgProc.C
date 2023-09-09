#include "ImgProc.h"

using namespace img;

ImgProc::ImgProc() : Nx(0), Ny(0), Nc(0), Nsize(0), img_data(0) {}

ImgProc::~ImgProc()
{
	clear();
}

void ImgProc::clear()
{
	if(img_data !=0) { delete[] img_data; img_data=0; }
	Nx=0;
	Ny=0;
	Nc=0;
	Nsize=0;
}

void ImgProc::clear(int nX, int nY, int nC)
{
	clear();
	Nx = nX;
	Ny = nY;
	Nc = nC;
	Nsize = (long)Nx * (long)Ny * (long)Nc;
	img_data = new float[Nsize];

	#pragma omp parallel for
	for(long i=0; i<Nsize; i++) { img_data[i] = 0.0; }
}

ImgProc::ImgProc(const ImgProc& v) :
  Nx (v.Nx), 
  Ny (v.Ny),
  Nc (v.Nc), 
  Nsize (v.Nsize)
{
	img_data = new float[Nsize];
	#pragma omp parallel for
 	for( long i=0;i<Nsize;i++){ img_data[i] = v.img_data[i]; }
}

ImgProc& ImgProc::operator=(const ImgProc& v)
{
	if( this == &v ){ return *this; }
	if( Nx != v.Nx || Ny != v.Ny || Nc != v.Nc )
	{
	    clear();
		Nx = v.Nx;
		Ny = v.Ny;
		Nc = v.Nc;
		Nsize = v.Nsize;
	}
	img_data = new float[Nsize];
	#pragma omp parallel for
	for( long i=0;i<Nsize;i++){ img_data[i] = v.img_data[i]; }
	return *this;
}
