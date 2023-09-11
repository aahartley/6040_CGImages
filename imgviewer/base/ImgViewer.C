#include "ImgViewer.h"

using namespace starter;
using namespace OIIO;
using namespace img;

void ImgViewer::run(const int argc, char const* const* argv){
	StarterViewer* viewer = CreateViewer();
    std::vector<std::string> args;
	
    for(int i=0;i<argc;i++)
    {
       std::string s(argv[i]);
       args.push_back(s);
    }
	if(args.size()>1){
		for(unsigned int i=1; i<args.size();i++)
		{
			std::string s = args[i];
			if(s=="-image")
			{
				read_image(args[i+1]);
				viewer->SetImage(imgProc);

			}
		}
	}
    viewer->Init(args);
    viewer->MainLoop();
}


void ImgViewer::read_image(const std::string& s){
	std::string filename = s;
	std::cout << "\n\n\n" << filename << std::endl;
	auto inp = ImageInput::open(filename);
	if (! inp)
    	return;
	const ImageSpec &spec = inp->spec();
	int xres = spec.width;
	int yres = spec.height;
	int nchannels = spec.nchannels;
	auto pixels = std::unique_ptr<unsigned char[]>(new unsigned char[xres * yres * nchannels]);
	inp->read_image(0, 0, 0, nchannels, TypeDesc::UINT8, &pixels[0]);
	inp->close();
	imgProc.clear(xres,yres,nchannels);
	std::vector<uint8_t> pixel(xres*yres*nchannels);
	for(int i=xres*yres*nchannels-1; i>=0;i--){
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
			imgProc.set_value(i,j,p);

		}
	}
	

}

