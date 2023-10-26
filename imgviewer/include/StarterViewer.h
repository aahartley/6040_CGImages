//-------------------------------------------------------
//
//  StarterViewer.h
//
//  This viewer is a wrapper of the Glut calls needed to
//  display opengl data.  Options for zooming, 
//  labeling the window frame, etc are available for 
//  derived classes to use.
//
//  
//
//  Copyright (c) 2003,2017,2023 Jerry Tessendorf
//
//
//--------------------------------------------------------


#ifndef STARTER_VIEWER_H
#define STARTER_VIEWER_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cmath>
#include <cstring>

#include <GL/gl.h>   // OpenGL itself.
#include <GL/glu.h>  // GLU support library.
#include <GL/glut.h> // GLUT support library.

#include "ImgProc.h"
#include "ImageProcessing.h"
#include "Mandelbrot.h"

namespace starter{


class StarterViewer
{
  public:

    //! The viewer is a singleton
    static  StarterViewer* Instance()
    {
       if(pStarterViewer==nullptr)
       {
          pStarterViewer = new StarterViewer();
       }
       return pStarterViewer;
    }

    ~StarterViewer();

    //! Initialization, including GLUT initialization.
    void Init( const std::vector<std::string>& args );
    //! Invokes the GLUT main loop.
    void MainLoop();
    
    //! Set the window width
    void SetWidth( const int w ) { width = w; }
    //! Set the window height 
    void SetHeight( const int h ) { height = h; }

    //! Get the window width
    const int& GetWidth() { return width;  }
    //! Get the window height 
    const int& GetHeight() { return height; }

    //! Set the window title
    void SetTitle( const std::string& t ){ title = t; }
    //! Set the window title
    void SetTitle( const char * t ) { title = t; }
    //! Get the window title
    const std::string& GetTitle() { return title; }

    //! Set the Image
    void SetImage(img::ImgProc* img, std::string fileN) {imgProc = img, fileName=fileN;}

    // Callback functions
    //! Cascading callback for initiating a display event
    void Display();
    //! Cascading callback for a keyboard event 
    void Keyboard( unsigned char key, int x, int y );
    //! Cascading callback for a mouse event 
    void Mouse( int button, int state, int x, int y );
    //! Cascading callback for a mouse motion event 
    void Motion( int x, int y );
    //! Cascading callback for a GLUT Special event 
    void Special( int key, int x, int y ){}
    //! Cascading callback for an idle  event 
    void Idle();
    //! Cascading callback for a window reshape 
    void Reshape( int w, int h );
    //! Cascading callback for reseting parameters
    void Reset();
    //! Cascading callback to home parameters
    void Home();

    //! Cascading callback for usage information
    void Usage();

   

  private:

    bool initialized;
    int width, height;
    unsigned int display_mode;

    std::string title;
    int mouse_x, mouse_y;
    int keystate, button;
    int mouse_state;
    float current_raster_pos[4];
    
    img::ImgProc* imgProc;
    std::string fileName;


    static StarterViewer* pStarterViewer;

    // dont allow any of these
    StarterViewer();
    StarterViewer( const StarterViewer& );
    StarterViewer& operator= (const StarterViewer&);

};


StarterViewer* CreateViewer();



}





#endif
