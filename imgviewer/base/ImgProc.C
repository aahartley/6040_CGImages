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

void ImgProc::set_value( int i, int j, const std::vector<float>& pixel)
{
	if( img_data == nullptr ){ return; }
	if( i<0 || i>=Nx ){ return; }
	if( j<0 || j>=Ny ){ return; }
	if( Nc > (int)pixel.size() ){ return; }
	//#pragma omp parallel for
	for( int c=0;c<Nc;c++ )
	{
		img_data[index(i,j,c,Nc,Nx)] = pixel[c];

	}
	return;
}

void ImgProc::read_image(const std::string& s)
{
	std::string filename = s;
	fn=filename;
	std::cout << "\nAttempting to find: " << filename << std::endl;
	auto inp = ImageInput::open(filename);
	if (! inp)
	{
		std::cout << "Couldn't find " << filename << std::endl;
    	return;
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
	for(int i=xres*yres*nchannels-1; i>=0;i--)
	{
		//flip image horizontally
		pixel[i]=pixels[(xres*yres*nchannels-1)-i];
	}
	//std::reverse(pixel.begin(),pixel.end());
	//row to width
	for(int i=0; i<xres;i++)
	{
		//col to height
		for(int j=0; j<yres;j++)
		{
			std::vector<float> p(nchannels);

			if(nchannels==3)
			{
				//flip image vertically
				int index = ((xres-1)-i + j*xres)*nchannels; //starts at B
				p[2]=(float)pixel[index]*(1.0f/(float)0xFF); //optimize by shifting, pre-compiled lookuptable?
				p[1]=(float)pixel[index+1]*(1.0f/(float)0xFF);
				p[0]=(float)pixel[index+2]*(1.0f/(float)0xFF);
			}
			else if(nchannels==4)
			{
				//flip image vertically
				int index = ((xres-1)-i + j*xres)*nchannels; //starts at B
				p[3]=float(pixel[index])*(1.0f/(float)0xFF);
				p[2]=float(pixel[index+1])*(1.0f/(float)0xFF);
				p[1]=float(pixel[index+2])*(1.0f/(float)0xFF);
				p[0]=float(pixel[index+3])*(1.0f/(float)0xFF);
			}
			set_value(i,j,p);

		}
	}
	write_image();
}


void ImgProc::write_image()
{
	auto inp = ImageInput::open(fn);
	if (! inp)
	{
		//std::cout << "Couldn't find " << filename << std::endl;
    	return;
	}
	const ImageSpec &spec = inp->spec();
	int xres = spec.width;
	int yres = spec.height;
	int nchannels = spec.nchannels;
	auto pixels = std::unique_ptr<unsigned char[]>(new unsigned char[xres * yres * nchannels]);
	inp->read_image(0, 0, 0, nchannels, TypeDesc::UINT8, &pixels[0]);
	inp->close();


	const char *filename = "/home/aahartl/Pictures/skydemo.jpeg";
	//const int xres = Nx, yres = Ny;
	std::cout << Nx <<'\n';
	const int channels = 3;  // RGB
	unsigned char* pixel= new unsigned char[xres * yres * channels];
	for(int i=0; i<xres*yres*channels; i++)
	{
		pixel[i]= pixels[i];
	}

	std::unique_ptr<ImageOutput> out = ImageOutput::create (filename);
	if (! out)
	{
		std::cout<< "error\n";
    	return;
	}
	///ImageSpec spec1(xres, yres, channels, TypeDesc::UINT8);
	out->open (filename, spec);
	out->write_image (TypeDesc::UINT8, &pixels[0]);
	out->close ();
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
