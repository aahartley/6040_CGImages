
#include <vector>
#include <string>
#include "StarterViewer.h"
#include <iostream>

#include <OpenImageIO/imageio.h>
using namespace OIIO;

using namespace starter;


void read_image(){
	std::string filename = "dummy.jpg";
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
}

int main( int argc, char** argv )
{
   
    read_image();
    StarterViewer* viewer = CreateViewer();

    std::vector<std::string> args;

    for(int i=0;i<argc;i++)
    {
       std::string s(argv[i]);
       args.push_back(s);
    }
    viewer->Init(args);

    viewer->MainLoop();

}
