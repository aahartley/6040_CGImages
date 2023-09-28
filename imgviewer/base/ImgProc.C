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

	std::vector<float> pixel(xres*yres*nchannels);
	//flip image horizontally
	#pragma omp parallel for
	for(int i=xres*yres*nchannels-1; i>=0;i--)
	{
		pixel[i]=pixels[(xres*yres*nchannels-1)-i];
	}
	//std::reverse(pixel.begin(),pixel.end()); alternative

	//flip image vertically
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
				p[2] = pixel[index];    
				p[1] = pixel[index+1];  
				p[0] = pixel[index+2];  

				// convert uint8 to float (0-1) manually
				// pixel[index]*(1.0f/(float)0xFF); //optimize by shifting, pre-compiled lookuptable?

			}
			else if(nchannels==4)
			{
				int index = ((xres-1)-i + j*xres)*nchannels; //starts at A
				p[3] = pixel[index];   
				p[2] = pixel[index+1];  
				p[1] = pixel[index+2]; 
				p[0] = pixel[index+3];  
			}
			set_value(i,j,p);

		}
	}
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
		//save new file to the original filepath
		nfileName = fn+"jpgdemo.jpeg";
		std::cout << "Writing: " << nfileName <<'\n';
	
		pixels= new float[xres * yres * channels];
		//flip vertically to match OIIO
		//row to width
		
		#pragma omp parallel for collapse(2)
		for(int i=0; i<xres;i++)
		{
			//col to height
			for(int j=0; j<yres;j++)
			{
				if(Nc==3)
				{
					int ind = ((xres-1)-i + j*xres)*channels; //starts at B
					pixels[ind]= img_data[index(i,j,2,Nc,Nx)];    
					pixels[ind+1]= img_data[index(i,j,1,Nc,Nx)];
					pixels[ind+2]= img_data[index(i,j,0,Nc,Nx)];
				}
				else if(Nc==4)
				{
					int ind = ((xres-1)-i + j*xres)*channels; //starts at B
					pixels[ind]= img_data[index(i,j,2,Nc,Nx)];    
					pixels[ind+1]= img_data[index(i,j,1,Nc,Nx)];
					pixels[ind+2]= img_data[index(i,j,0,Nc,Nx)];
				}
			}
		}
	}
	//flip horizontally to match OIIO
	//reversing array
	float temp;
	int length = xres*yres*channels;
	#pragma omp parallel for
	for (int i = 0; i < length/2; i++)
    {
        temp = pixels[i];
        pixels[i] = pixels[(length - 1) - i];
        pixels[(length - 1) - i] = temp;

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

void ImgProc::LinearConvolution( const Stencil& stencil, ImgProc& out ) const
{
	out.clear( Nx, Ny, Nc );
	for( int j=0;j<out.ny();j++)
	{
		int jmin = j - stencil.halfwidth();
		int jmax = j + stencil.halfwidth();	

		#pragma omp parallel for
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
					const float& stencil_value = stencil(stencili, stencilj);
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
void ImgProc::Gamma(ImgProc& out) const
{

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

	std::vector<float> pixel(xres*yres*nchannels);
	//flip image horizontally
	#pragma omp parallel for
	for(int i=xres*yres*nchannels-1; i>=0;i--)
	{
		pixel[i]=pixels[(xres*yres*nchannels-1)-i];
	}
	//std::reverse(pixel.begin(),pixel.end()); alternative

	//flip image vertically
	//row to height
	#pragma omp parallel for collapse(2)
	for(int j=0; j<yres;j++)
	{
		//col to width
		for(int i=0; i<xres;i++)
		{
			std::vector<float> p(nchannels);

			if(nchannels==3)
			{
				int index = ((xres-1)-i + j*xres)*nchannels; //starts at B
				p[2] = pixel[index];    
				p[1] = pixel[index+1];  
				p[0] = pixel[index+2];  

				// convert uint8 to float (0-1) manually
				// pixel[index]*(1.0f/(float)0xFF); //optimize by shifting, pre-compiled lookuptable?

			}
			else if(nchannels==4)
			{
				int index = ((xres-1)-i + j*xres)*nchannels; //starts at A
				p[3] = pixel[index];   
				p[2] = pixel[index+1];  
				p[1] = pixel[index+2]; 
				p[0] = pixel[index+3];  
			}
			imgProc.set_value(i,j,p);

		}
	}
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
		std::cout << "Writing: " << nfileName <<'\n';
	
		pixels= new float[xres * yres * channels];

		//flip vertically to match OIIO
		//row to height
		#pragma omp parallel for collapse(2)
		for(int j=0; j<yres;j++)
		{
			//col to width
			for(int i=0; i<xres;i++)
			{
				if(imgProc.depth()==3)
				{
					int ind = ((xres-1)-i + j*xres)*channels; //starts at B
					pixels[ind]= imgProc.raw()[index(i,j,2,imgProc.depth(),imgProc.nx())];    
					pixels[ind+1]= imgProc.raw()[index(i,j,1,imgProc.depth(),imgProc.nx())];
					pixels[ind+2]= imgProc.raw()[index(i,j,0,imgProc.depth(),imgProc.nx())];
				}
				else if(imgProc.depth()==4)
				{
					int ind = ((xres-1)-i + j*xres)*channels; //starts at B
					pixels[ind]= imgProc.raw()[index(i,j,2,imgProc.depth(),imgProc.nx())];    
					pixels[ind+1]= imgProc.raw()[index(i,j,1,imgProc.depth(),imgProc.nx())];
					pixels[ind+2]= imgProc.raw()[index(i,j,0,imgProc.depth(),imgProc.nx())];
				}
			}
		}
	}
	//flip horizontally to match OIIO
	//reversing array
	float temp;
	int length = xres*yres*channels;
	#pragma omp parallel for
	for (int i = 0; i < length/2; i++)
    {
        temp = pixels[i];
        pixels[i] = pixels[(length - 1) - i];
        pixels[(length - 1) - i] = temp;

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
//bounded
void img::LinearConvolution( const Stencil& stencil, const ImgProc& in, ImgProc& out )
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
			std::vector<float> pixel(out.depth(),0.0);
			std::vector<float> sample(in.depth(),0.0);
			for(int jj=jmin;jj<=jmax;jj++)
			{
				int stencilj = jj-j;
				//std::cout << stencilj << '\n';
				int jjj = jj;
				if(jjj <0 || jjj>= out.ny()){outB=true;}
				//if(jjj < 0 ){ jjj += out.ny(); }         periodic
				//if(jjj >= out.ny() ){ jjj -= out.ny(); }
				for(int ii=imin;ii<=imax;ii++)
				{
					int stencili = ii-i;
					int iii = ii;
					if(iii <0 || iii>= out.nx()){outB=true;}

					//if(iii < 0 ){ iii += out.nx(); }        periodic
					//if(iii >= out.nx() ){ iii -= out.nx(); }
					const float& stencil_value = stencil(stencili+stencil.halfwidth(), stencilj+stencil.halfwidth());
					if(!outB)
					{
						in.value(iii,jjj,sample);
					}
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
