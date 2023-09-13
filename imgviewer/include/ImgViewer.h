//-------------------------------------------------------
//
//  ImgViewer.h
//
//  This viewer runs the program,
//  parses cmd line arguments, and
//  will execute ImgProc methods.
//  
//
//--------------------------------------------------------
#ifndef IMGVIEWER_H
#define IMGVIEWER_H

#include <vector>
#include <string>
#include <iostream>

#include "StarterViewer.h"

#include "ImgProc.h"


class ImgViewer{

  public:
    ImgViewer(){};
    ~ImgViewer(){};

    void run(const int argc, char const* const* argv);
  private:
    img::ImgProc imgProc;
};

#endif