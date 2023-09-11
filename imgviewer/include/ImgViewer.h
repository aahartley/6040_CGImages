#ifndef IMGVIEWER_H
#define IMGVIEWER_H

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

#include <OpenImageIO/imageio.h>

#include "StarterViewer.h"

#include "ImgProc.h"


class ImgViewer{

  public:
    ImgViewer(){};
    ~ImgViewer(){};
    
    void run(const int argc, char const* const* argv);
    void read_image(const std::string& s);
  private:
    img::ImgProc* imgProc = new img::ImgProc;
};

#endif