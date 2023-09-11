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
	if(imgProc == viewer->imgProc){
		std::cout <<"we good\n";
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
	imgProc->clear(xres,yres,nchannels);
	std::vector<char> pixel(xres*yres*nchannels);
	for(int i=xres*yres*nchannels-1; i>=0;i--){
		pixel[i]=pixels[(xres*yres*nchannels-1)-i];
	}

	//std::reverse(pixel.begin(),pixel.end());
	//col to width
	for(int i=0; i<xres;i++)
	{
		//rows to height
		for(int j=0; j<yres;j++)
		{
			std::vector<float> p(nchannels);

			if(nchannels==3)
			{
				int index = ((xres-1)-i + j*xres)*nchannels;
				p[0]=(float)pixel[index-2];
				p[1]=(float)pixel[index-1];
				p[2]=(float)pixel[index];
			}
			else if(nchannels==4)
			{
				int index = ((xres-1)-i + j*xres)*nchannels;
				p[0]=(float)pixel[index+1];
				p[1]=(float)pixel[index+2];
				p[2]=(float)pixel[index+3];
				p[3]=(float)pixel[index];
			}


			imgProc->set_value(i,j,p);
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

