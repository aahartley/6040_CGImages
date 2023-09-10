#ifndef IMGPROC_H
#define IMGPROC_H

#include <vector>

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
	
	void value(int i, int j, std::vector<float>& pixel) const;
	void set_value(int i, int j, const std::vector<float>& pixel);

	ImgProc(const ImgProc& img); //copy constructor
	ImgProc& operator=(const ImgProc& img); //copy assignment

  private:

	int Nx, Ny, Nc;
	long Nsize;
	float* img_data;
};
}
#endif
