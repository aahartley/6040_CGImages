//-------------------------------------------------------
//
//  ImgProc.C
//
//  Stores the image pixels and the methods to manipulate
//  them. Handles the IO of images.
//  
//--------------------------------------------------------
#include "ImgProc.h"

using namespace img;
using namespace OIIO;

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
//clears data, sets size for img
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
//updates pixel color from img_data
void ImgProc::value( int i, int j, std::vector<float>& pixel) const
{
	pixel.clear();
	if( img_data == nullptr ){ return; }
	if( i<0 || i>=Nx ){ return; }
	if( j<0 || j>=Ny ){ return; }
	pixel.resize(Nc);
	for( int c=0;c<Nc;c++ )
	{
		pixel[c] = img_data[index(i,j,c,Nc,Nx)];
	}
	return;
}
//updates img_data from pixel colors
void ImgProc::set_value( int i, int j, const std::vector<float>& pixel)
{
	if( img_data == nullptr ){ return; }
	if( i<0 || i>=Nx ){ return; }
	if( j<0 || j>=Ny ){ return; }
	if( Nc > (int)pixel.size() ){ return; }
	//#pragma omp parallel for  // kills performance, false sharing?
	for( int c=0;c<Nc;c++ )
	{
		img_data[index(i,j,c,Nc,Nx)] = pixel[c];

	}
	return;
}

int ImgProc::read_image(const std::string& s)
{
	std::string filename = s;
	std::cout << "\nAttempting to find: " << filename << std::endl;
	auto inp = ImageInput::open(filename);
	if (! inp)
	{
		std::cout << "Couldn't find " << filename << std::endl;
    	return 1;
	}
	const ImageSpec &spec = inp->spec();
	int xres = spec.width;
	int yres = spec.height;
	int nchannels = spec.nchannels;
	auto pixels = std::unique_ptr<unsigned char[]>(new unsigned char[xres * yres * nchannels]);
	inp->read_image(0, 0, 0, nchannels, TypeDesc::UINT8, &pixels[0]);
	inp->close();

	clear(xres,yres,nchannels);

	std::vector<uint8_t> pixel(xres*yres*nchannels);
	//flip image horizontally
	for(int i=xres*yres*nchannels-1; i>=0;i--)
	{
		pixel[i]=pixels[(xres*yres*nchannels-1)-i];
	}
	//std::reverse(pixel.begin(),pixel.end()); alternative

	//TypeDesc::FLOAT causing segmentation fault
	//flip image vertically and convert uint8 to float (0-1) manually
	//row to width
	#pragma omp parallel for collapse(2)
	for(int i=0; i<xres;i++)
	{
		//col to height
		for(int j=0; j<yres;j++)
		{
			std::vector<float> p(nchannels);

			if(nchannels==3)
			{
				int index = ((xres-1)-i + j*xres)*nchannels; //starts at B
				p[2]=(float)pixel[index]*(1.0f/(float)0xFF); //optimize by shifting, pre-compiled lookuptable?
				p[1]=(float)pixel[index+1]*(1.0f/(float)0xFF);
				p[0]=(float)pixel[index+2]*(1.0f/(float)0xFF);
			}
			else if(nchannels==4)
			{
				int index = ((xres-1)-i + j*xres)*nchannels; //starts at A
				p[3]=float(pixel[index])*(1.0f/(float)0xFF);
				p[2]=float(pixel[index+1])*(1.0f/(float)0xFF);
				p[1]=float(pixel[index+2])*(1.0f/(float)0xFF);
				p[0]=float(pixel[index+3])*(1.0f/(float)0xFF);
			}
			set_value(i,j,p);

		}
	}
	std::cout <<"Image loaded\n";
	return 0;
}


void ImgProc::write_image(std::string fileName)
{
	//reload image to automatically adjust pixel data to fit jpg format
	auto inp = ImageInput::open(fileName);
	if (! inp)
	{
		std::cout << "Couldn't find " << fileName << std::endl;
    	return;
	}
	const ImageSpec &spec = inp->spec();
	int xres = spec.width;
	int yres = spec.height;
	int channels = 3;  // RGB
	auto pixels = std::unique_ptr<unsigned char[]>(new unsigned char[xres * yres * channels]);
	inp->read_image(0, 0, 0, channels, TypeDesc::UINT8, &pixels[0]);
	inp->close();

    std::size_t pos = fileName.find(".");
    std::string fn = fileName.substr(0,pos);
	//save new file to the original filepath
	std::string filename = fn+"jpgdemo.jpeg";
	std::cout << "writing: " << filename <<'\n';
	unsigned char* pixel= new unsigned char[xres * yres * channels];
	#pragma omp parallel for
	for(int i=0; i<xres*yres*channels; i++)
	{
		pixel[i]= pixels[i];
	}

	std::unique_ptr<ImageOutput> out = ImageOutput::create (filename);
	if (! out)
	{
		std::cout<< "write error\n";
    	return;
	}
	//OIIO should delte this pointer for me?
	ImageSpec* spec1 = new ImageSpec(xres, yres, channels, TypeDesc::UINT8); //no pointer causing linking error
	out->open (filename, *spec1);
	out->write_image (TypeDesc::UINT8, &pixels[0]);
	out->close ();
	std::cout << "write successful\n";
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
