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
	for(unsigned int i=1; i<args.size();i++)
	{
		std::string s = args[i];
		if(s=="-image")
		{
			read_image(args[i+1]);
		}
	}
	//std::cout << index(2,2,2,3,3) <<'\n';
	viewer->SetImage(imgProc);
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
	// std::vector<float> p(xres*yres*nchannels);
	// for(int i=0; i<xres*yres*nchannels;i++)
	// {
	// 	p[i]= (float)pixels[i];
	// }
	//col to width
	for(int j=0; j<xres;j++)
	{
		//rows to height
		for(int i=0; i<yres;i++)
		{
			std::vector<float> p(4);
			p[0]=(float)pixels[j*xres+i];
			p[1]=(float)pixels[j*xres+i+1];
			p[2]=(float)pixels[j*xres+i+2];
			p[3]=(float)pixels[j*xres+i+3];


			imgProc.set_value(i,j,p);
			//std::cout<< j*yres+i << '\n';

		}
	}
	// int index=0;
	// int count=1;
	// for(int i=0; i<xres;i++)
	// {
		
	// 	int cols = yres*nchannels-1;
	// 	//std::reverse( p.begin() +index, p.end() - ((p.size()-1) +cols*count+i) );
	// 	count++;
	// 	index +=cols+1;
	// }
	//std::cout << xres*yres*nchannels << '\n';
	//for(int i=0; i<xres*yres*nchannels;i++)
	// {
		//imgProc.raw()[i] = p[i];
	//}
	//imgProc.setRaw(&p[0]);

}

