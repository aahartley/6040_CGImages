//-------------------------------------------------------
//
//  StarterViewer.C
//
//  This viewer is a wrapper of the Glut calls needed to
//  display opengl data.  Options for zooming, 
//  labeling the window frame, etc are available for 
//  derived classes to use.
//
//
//  Copyright (c) 2003,2017,2023 Jerry Tessendorf
//
//--------------------------------------------------------
#include "StarterViewer.h"


using namespace std;
using namespace img;

namespace starter{


// These are the GLUT Callbacks that are implemented in StarterViewer.
void cbDisplayFunc()
{
   glClear( GL_COLOR_BUFFER_BIT );	
   StarterViewer::Instance() -> Display();
   glutSwapBuffers();
   glutPostRedisplay();
}

void cbIdleFunc()
{
   StarterViewer::Instance() -> Idle();
}


void cbKeyboardFunc( unsigned char key, int x, int y )
{
   StarterViewer::Instance() -> Keyboard( key, x, y );
}

void cbMotionFunc( int x, int y )
{
   
   StarterViewer::Instance() -> Motion( x, y );
   glutPostRedisplay();
}

void cbMouseFunc( int button, int state, int x, int y )
{
   StarterViewer::Instance() -> Mouse( button, state, x, y );
}

void cbReshapeFunc( int w, int h )
{
   StarterViewer::Instance() -> Reshape( w, h );
   glutPostRedisplay();
}


StarterViewer* StarterViewer::pStarterViewer = nullptr;
	
StarterViewer::StarterViewer() : 
   initialized    ( false ),
   width          ( 512 ), 
   height         ( 512),
   display_mode   (GLUT_DOUBLE | GLUT_RGBA  ),
   title          ( string("Img Viewer") ),
   mouse_x        ( 0 ),
   mouse_y        ( 0 ),
   fileName       ("")
{
   cout << "ImgViewer Loaded\n";
}

StarterViewer::~StarterViewer(){}

void StarterViewer::Init( const std::vector<std::string>& args )
{
   int argc = (int)args.size();
   char** argv = new char*[argc];
   for( int i=0;i<argc;i++)
   {
      argv[i] = new char[args[i].length() + 1];
      std::strcpy(argv[i], args[i].c_str());
   }
   glutInit( &argc, argv );
   string window_title = title;

   if(imgProc.nx()!=0)width=imgProc.nx();
   if(imgProc.ny()!=0)height=imgProc.ny();
   if(width > 1300) width = 1300;  //should fit most displays
   if(height> 600) height = 600;

   glutInitDisplayMode( display_mode );
   glutInitWindowSize( width, height );
   glutCreateWindow( window_title.c_str() );
   glClearColor(0.5,0.5,0.6,0.0);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //enable transparency
   glEnable( GL_BLEND );
   glutDisplayFunc( &cbDisplayFunc );
   glutIdleFunc( &cbIdleFunc );
   glutKeyboardFunc( &cbKeyboardFunc );
   glutMotionFunc( &cbMotionFunc );
   glutMouseFunc( &cbMouseFunc );
   glutReshapeFunc( &cbReshapeFunc );

   initialized = true;
   cout << "ImgViewer Initialized\n";
   Usage();

}

void StarterViewer::MainLoop()
{
   glutMainLoop();
}


void StarterViewer::Display()
{
   //Draw image based off color channels
   if(imgProc.depth() == 3)
   {
      glDrawPixels( imgProc.nx(), imgProc.ny(), GL_RGB, GL_FLOAT, imgProc.raw() );
   }
   else
   {
      glDrawPixels( imgProc.nx(), imgProc.ny(), GL_RGBA,GL_FLOAT, imgProc.raw() );
   }

}



void StarterViewer::Reshape( int w, int h )
{
   width = w;
   height = h;
   //glViewport( 0, 0, (GLsizei) width, (GLsizei) height );
   //glMatrixMode( GL_PROJECTION );
   //glLoadIdentity();
}

void StarterViewer::Keyboard( unsigned char key, int x, int y )
{
   switch (key)
   {
      case 'j':
      case 'J':
         if(fileName!="") //dont allow writing unless image has been found
            write_image(fileName,'j',imgProc); //write jpg
         break;
      case 'g':{
         ImgProc out;
         Gamma(imgProc, out, 0.9);
         imgProc = out;
         }break;
      case 'G':{
         ImgProc out;
         Gamma(imgProc, out, 1.111111);
         imgProc = out;
         }break;
      case 's':{
         ImgProc out;
         Stencil stencil(5);
         LinearConvolution(stencil, imgProc, out);
         imgProc = out;
         }break;
      case 'r':
	      Reset();
         break;
      case 'h':
	      Home();
         break;
      case 'u':
	      Usage();
         break;
   }
}


void StarterViewer::Motion( int x, int y )
{
   // float dx = x - mouse_x;
   // float dy = y - mouse_y;
   // float pos_x = current_raster_pos[0] + dx;
   // float pos_y = current_raster_pos[1] - dy;
   //glRasterPos2f( pos_x, pos_y ); 

   if(keystate == GLUT_ACTIVE_SHIFT )
   {
   }
   mouse_x = x;
   mouse_y = y;
}


void StarterViewer::Mouse( int b, int state, int x, int y )
{
   mouse_x = x;
   mouse_y = y;
   keystate = glutGetModifiers();
   button = b;
   mouse_state = state;
   glGetFloatv( GL_CURRENT_RASTER_POSITION, current_raster_pos );
}


void StarterViewer::Idle() {}


void StarterViewer::Usage()
{
   cout << "--------------------------------------------------------------\n";
   cout << "ImgViewer usage: -image <image.ext>  loads images\n";
   cout << "--------------------------------------------------------------\n";
   cout << "j/J           write image to JPEG/JPG\n";
   cout << "--------------------------------------------------------------\n";
}

void StarterViewer::Reset()
{
   std::cout << "Reset\n";
}

void StarterViewer::Home()
{
   std::cout << "Home\n";
  
}

StarterViewer* CreateViewer() { return StarterViewer::Instance(); }

}


