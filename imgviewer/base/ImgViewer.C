#include "ImgViewer.h"

using namespace starter;
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
				imgProc.read_image(args[i+1]);
				viewer->SetImage(imgProc);

			}
		}
	}
    viewer->Init(args);
    viewer->MainLoop();
}



