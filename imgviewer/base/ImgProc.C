//-------------------------------------------------------
//
//  ImgProc.C
//
//  Stores the image pixels and the methods to manipulate
//  them. Handles the IO of images.
//  
//--------------------------------------------------------
#include "ImgProc.h"

using namespace OIIO;
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
	//#pragma omp parallel for  //outer loops already in parallel
	for( int c=0;c<Nc;c++ )
	{
		img_data[index(i,j,c,Nc,Nx)] = pixel[c];

	}
	return;
}

void ImgProc::set_raw(float* data)
{
	if( img_data == nullptr ){ return; }

	#pragma omp parallel for  
	for(int i=0; i<Nx*Ny*Nc; i++)
	{
		img_data[i] = data[i];

	}
	return;
}
void ImgProc::set_raw_OI(float* data)
{
	if( img_data == nullptr ){ return; }

	#pragma omp parallel for collapse(2)
	for(int j=0; j<Ny; j++)
	{
		for(int i=0; i<Nx; i++)
		{
			for(int k=0; k<Nc; k++)
			{
				//flip in place
				img_data[index(i,j,k,Nc,Nx)] = data[index(i,Ny-1-j,k,Nc,Nx)];
			}
		}

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
	auto pixels = std::unique_ptr<float[]>(new float[xres * yres * nchannels]);
	inp->read_image(0, 0, 0, nchannels, TypeDesc::FLOAT, &pixels[0]);
	inp->close();

	clear(xres,yres,nchannels);
	set_raw_OI(&pixels[0]);

	
	std::cout <<"Image loaded\n\n";
	return 0;
}

//function will not be called unless image has been read
void ImgProc::write_image(std::string fileName, char f) const
{
	int xres = Nx;
	int yres = Ny;
	int channels = 0;
	float* pixels = nullptr;
	std::string nfileName;
	if(f=='j') ///jpg
	{
		channels=3;
		std::size_t pos = fileName.find(".");
    	std::string fn = fileName.substr(0,pos);
		time_t now = time(0);
  		std::string date = ctime(&now);
		std::replace(date.begin(), date.end(), ' ', '_');
		//save new file to the original filepath
		nfileName = fn+"jpgdemo_"+date+".jpeg";
	
		pixels= new float[xres * yres * channels];
	}
	std::cout << "Writing: " << nfileName <<'\n';

	for(int j=0; j<yres; j++)
	{
		for(int i=0; i<xres; i++)
		{
			for(int k=0; k<channels;k++)
			{
				//flip in place
				pixels[index(i,j,k,channels,xres)] = img_data[index(i,Ny-1-j,k,Nc,Nx)];
			}
		}
	}
	std::unique_ptr<ImageOutput> out = ImageOutput::create (nfileName);
	if (! out)
	{
		std::cout<< "Write error\n";
    	return;
	}
	ImageSpec spec(xres, yres, channels, TypeDesc::FLOAT); 
	out->open (nfileName, spec);
	out->write_image (TypeDesc::FLOAT, pixels);
	out->close ();
	std::cout << "Write successful\n";
	delete [] pixels;
}

void ImgProc::PeriodicLinearConvolution( const Stencil& stencil, ImgProc& out ) const
{
	out.clear( Nx, Ny, Nc );
	for( int j=0;j<out.ny();j++)
	{
		int jmin = j - stencil.halfwidth();
		int jmax = j + stencil.halfwidth();	

		//#pragma omp parallel for
		for(int i=0;i<out.nx();i++)
		{
			int imin = i - stencil.halfwidth();
			int imax = i + stencil.halfwidth();
			std::vector<float> pixel(out.depth(),0.0);
			std::vector<float> sample(Nc,0.0);
			for(int jj=jmin;jj<=jmax;jj++)
			{
				int stencilj = jj-j;
				int jjj = jj;
				if(jjj < 0 ){ jjj += out.ny(); }         
				if(jjj >= out.ny() ){ jjj -= out.ny(); }
				for(int ii=imin;ii<=imax;ii++)
				{
					int stencili = ii-i;
					int iii = ii;

					if(iii < 0 ){ iii += out.nx(); }        
					if(iii >= out.nx() ){ iii -= out.nx(); }
					const float& stencil_value = stencil(stencili+stencil.halfwidth(), stencilj+stencil.halfwidth());
					value(iii,jjj,sample);
					for(size_t c=0;c<sample.size();c++)
					{
						 pixel[c] += sample[c] * stencil_value;
				    }
				}
 			}
 			out.set_value(i,j,pixel);
 		}
	}
}

void ImgProc::BoundedLinearConvolution( const Stencil& stencil, ImgProc& out ) const
{
	out.clear( Nx, Ny, Nc );
	bool outB =false;
	for( int j=0;j<out.ny();j++)
	{
		int jmin = j - stencil.halfwidth();
		int jmax = j + stencil.halfwidth();	

		//#pragma omp parallel for
		for(int i=0;i<out.nx();i++)
		{
			int imin = i - stencil.halfwidth();
			int imax = i + stencil.halfwidth();
			std::vector<float> pixel(out.depth(),0.0f);
			std::vector<float> sample(Nc,0.0f);
			for(int jj=jmin;jj<=jmax;jj++)
			{
				int stencilj = jj-j;
				int jjj = jj;
				if(jjj <0 || jjj>= out.ny()){outB=true;}
	
				for(int ii=imin;ii<=imax;ii++)
				{
					int stencili = ii-i;
					int iii = ii;
					if(iii <0 || iii>= out.nx()){outB=true;}

					const float& stencil_value = stencil(stencili+stencil.halfwidth(), stencilj+stencil.halfwidth());
					if(!outB)
					{
						value(iii,jjj,sample);
					}
					//otherwise sample is 0.0 from initialization
					outB=false;
					for(size_t c=0;c<sample.size();c++)
					{
						 pixel[c] += sample[c] * stencil_value;
				    }
				}
 			}
 			out.set_value(i,j,pixel);
 		}
	}
}

void ImgProc::Flip()
{
	//row to height
	for(int j=0; j<Ny/2;j++)
	{
		//col to width
		for(int i=0; i<Nx;i++)
		{				
			for(int k=0; k<Nc;k++)
			{
				float temp = img_data[index(i,j,k,Nc,Nx)];

				img_data[index(i,j,k,Nc,Nx)] = img_data[index(i,Ny-1-j,k,Nc,Nx)];
				img_data[index(i,Ny-1-j,k,Nc,Nx)]= temp;

			}
		}
	}

}

void ImgProc::Flop()
{
	//row to height
	for(int j=0; j<Ny;j++)
	{
		//col to width
		for(int i=0; i<Nx/2;i++)
		{				
			for(int k=0; k<Nc;k++)
			{
				float temp = img_data[index(i,j,k,Nc,Nx)];

				img_data[index(i,j,k,Nc,Nx)] = img_data[index(Nx-1-i,j,k,Nc,Nx)];
				img_data[index(Nx-1-i,j,k,Nc,Nx)]= temp;

			}
		}
	}
}

void ImgProc::Gamma(ImgProc& out, float s) const
{
	out.clear(Nx,Ny,Nc);

	#pragma omp parallel for
 	for(int i=0; i<out.size(); i++)
 	{
 		out.raw()[i] = std::pow(img_data[i], s);
 	}
}
void ImgProc::GammaInPlace(float s)
{
	#pragma omp parallel for
 	for(int i=0; i<Nsize; i++)
 	{
 		img_data[i] = std::pow(img_data[i], s);
 	}
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





//***************************************************************************************//
//img namespace function definitions

int img::read_image(const std::string& s, ImgProc& imgProc)
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
	auto pixels = std::unique_ptr<float[]>(new float[xres * yres * nchannels]);
	inp->read_image(0, 0, 0, nchannels, TypeDesc::FLOAT, &pixels[0]);
	inp->close();

	imgProc.clear(xres,yres,nchannels);
	imgProc.set_raw_OI(&pixels[0]);

	std::cout <<"Image loaded\n\n";
	return 0;
}

//function will not be called unless image has been read
void img::write_image(std::string fileName, char f, const ImgProc& imgProc)
{
	int xres = imgProc.nx();
	int yres = imgProc.ny();
	int channels = 0;
	float* pixels = nullptr;
	std::string nfileName;
	if(f=='j') ///jpg
	{
		channels=3;
		std::size_t pos = fileName.find(".");
    	std::string fn = fileName.substr(0,pos);
		time_t now = time(0);
  		std::string date = ctime(&now);
		std::replace(date.begin(), date.end(), ' ', '_');
		//save new file to the original filepath
		nfileName = fn+"jpgdemo_"+date+".jpeg";
	
		pixels= new float[xres * yres * channels];
	}
	std::cout << "Writing: " << nfileName <<'\n';

	for(int j=0; j<yres; j++)
	{
		for(int i=0; i<xres; i++)
		{
			for(int k=0; k<channels;k++)
			{
				//flip in place
				pixels[index(i,j,k,channels,xres)] = imgProc.raw()[index(i,imgProc.ny()-1-j,k,imgProc.depth(),imgProc.nx())];
			}
		}
	}
	std::unique_ptr<ImageOutput> out = ImageOutput::create (nfileName);
	if (! out)
	{
		std::cout<< "Write error\n";
    	return;
	}
	ImageSpec spec(xres, yres, channels, TypeDesc::FLOAT); 
	out->open (nfileName, spec);
	out->write_image (TypeDesc::FLOAT, pixels);
	out->close ();
	std::cout << "Write successful\n";
	delete [] pixels;
}
void img::PeriodicLinearConvolution( const Stencil& stencil, const ImgProc& in, ImgProc& out )
{
	out.clear( in.nx(), in.ny(), in.depth() );
	for( int j=0;j<out.ny();j++)
	{
		int jmin = j - stencil.halfwidth();
		int jmax = j + stencil.halfwidth();	

		//#pragma omp parallel for
		for(int i=0;i<out.nx();i++)
		{
			int imin = i - stencil.halfwidth();
			int imax = i + stencil.halfwidth();
			std::vector<float> pixel(out.depth(),0.0);
			std::vector<float> sample(in.depth(),0.0);
			for(int jj=jmin;jj<=jmax;jj++)
			{
				int stencilj = jj-j;
				int jjj = jj;
				if(jjj < 0 ){ jjj += out.ny(); }         
				if(jjj >= out.ny() ){ jjj -= out.ny(); }
				for(int ii=imin;ii<=imax;ii++)
				{
					int stencili = ii-i;
					int iii = ii;

					if(iii < 0 ){ iii += out.nx(); }        
					if(iii >= out.nx() ){ iii -= out.nx(); }
					const float& stencil_value = stencil(stencili+stencil.halfwidth(), stencilj+stencil.halfwidth());
					in.value(iii,jjj,sample);
					for(size_t c=0;c<sample.size();c++)
					{
						 pixel[c] += sample[c] * stencil_value;
				    }
				}
 			}
 			out.set_value(i,j,pixel);
 		}
	}
}
void img::BoundedLinearConvolution( const Stencil& stencil, const ImgProc& in, ImgProc& out )
{
	out.clear( in.nx(), in.ny(), in.depth() );
	bool outB =false;
	for( int j=0;j<out.ny();j++)
	{
		int jmin = j - stencil.halfwidth();
		int jmax = j + stencil.halfwidth();	

		//#pragma omp parallel for
		for(int i=0;i<out.nx();i++)
		{
			int imin = i - stencil.halfwidth();
			int imax = i + stencil.halfwidth();
			std::vector<float> pixel(out.depth(),0.0f);
			std::vector<float> sample(in.depth(),0.0f);
			for(int jj=jmin;jj<=jmax;jj++)
			{
				int stencilj = jj-j;
				//std::cout << stencilj << '\n';
				int jjj = jj;
				if(jjj <0 || jjj>= out.ny()){outB=true;}
	
				for(int ii=imin;ii<=imax;ii++)
				{
					int stencili = ii-i;
					int iii = ii;
					if(iii <0 || iii>= out.nx()){outB=true;}

					const float& stencil_value = stencil(stencili+stencil.halfwidth(), stencilj+stencil.halfwidth());
					if(!outB)
					{
						in.value(iii,jjj,sample);
					}
					//otherwise sample is 0.0 from initialization
					outB=false;
					for(size_t c=0;c<sample.size();c++)
					{
						 pixel[c] += sample[c] * stencil_value;
				    }
				}
 			}
 			out.set_value(i,j,pixel);
 		}
	}
}

void img::Gamma(const ImgProc& in, ImgProc& out, float s)
{
	out.clear(in.nx(),in.ny(),in.depth());

	#pragma omp parallel for
 	for( long i=0;i<out.size();i++ )
 	{
 		out.raw()[i] = std::pow(in.raw()[i], s);
 	}
}
void img::GammaInPlace(ImgProc& in, float s)
{
	#pragma omp parallel for
 	for( int i=0;i<in.size();i++ )
 	{
 		in.raw()[i] = std::pow(in.raw()[i], s);
 	}
}
//in place
void img::Flip(ImgProc& in)
{
	//row to height
	for(int j=0; j<in.ny()/2;j++)
	{
		//col to width
		for(int i=0; i<in.nx();i++)
		{				

			for(int k=0; k<in.depth();k++)
			{
				float temp = in.raw()[index(i,j,k,in.depth(),in.nx())];

				in.raw()[index(i,j,k,in.depth(),in.nx())] = in.raw()[index(i,in.ny()-1-j,k,in.depth(),in.nx())];
				in.raw()[index(i,in.ny()-1-j,k,in.depth(),in.nx())]= temp;

			}
		}
	}

}
//in place
void img::Flop(ImgProc& in)
{
	//row to height
	for(int j=0; j<in.ny();j++)
	{
		//col to width
		for(int i=0; i<in.nx()/2;i++)
		{				

			for(int k=0; k<in.depth();k++)
			{
				float temp = in.raw()[index(i,j,k,in.depth(),in.nx())];

				in.raw()[index(i,j,k,in.depth(),in.nx())] = in.raw()[index(in.nx()-1-i,j,k,in.depth(),in.nx())];
				in.raw()[index(in.nx()-1-i,j,k,in.depth(),in.nx())]= temp;

			}
		}
	}

}
