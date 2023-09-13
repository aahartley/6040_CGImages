//-------------------------------------------------------
//
//  ImgViewer.C
//
//  This viewer runs the program,
//  parses cmd line arguments, and
//  will execute ImgProc methods.
//  
//
//--------------------------------------------------------
#include "ImgViewer.h"

using namespace starter;
using namespace img;

void ImgViewer::run(const int argc, char const* const* argv){
	StarterViewer* viewer = CreateViewer();
    std::vector<std::string> args;
	viewer->Init(args);

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
				int read = imgProc.read_image(args[i+1]);
				if(read==0)
					viewer->SetImage(imgProc,args[i+1]);

			}
		}
	}
    viewer->MainLoop();
}




