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
   imgProc        (nullptr),
   fileName       (""),
   zoom_x         (1),
   zoom_y         (1)
{
   cout << "ImgViewer Loaded\n";
}

StarterViewer::~StarterViewer(){}

void StarterViewer::Init( const std::vector<std::string>& args )
{
   int argc = (int)args.size();
   char** argv = new char*[argc];
   for(int i = 0; i < argc; i++)
   {
      argv[i] = new char[args[i].length() + 1];
      std::strcpy(argv[i], args[i].c_str());
   }
   glutInit( &argc, argv );
   string window_title = title;

   if(imgProc != nullptr)
   {
      if(imgProc->nx() != 0) width=imgProc->nx();
      if(imgProc->ny() != 0) height=imgProc->ny();
   }

   if(width > 1300) width = 1300;  //should fit most displays
   if(height> 600) height = 600;

   glutInitDisplayMode( display_mode );
   glutInitWindowSize( width, height );
   glutCreateWindow( window_title.c_str() );
   glClearColor(0.5,0.5,0.6,0.0);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //enable transparency
   glEnable( GL_BLEND );

   // glEnable(GL_TEXTURE_2D);

   // // Generate a texture ID
   // glGenTextures(1, &textureID);

   // // Bind the texture ID
   // glBindTexture(GL_TEXTURE_2D, textureID);

   // // Set texture parameters
   // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   // glViewport( 0, 0, (GLsizei) width, (GLsizei) height );
   // glMatrixMode(GL_PROJECTION); 
   // glLoadIdentity();
   // glOrtho(0, width, 0, height, -1, 1);
   // glMatrixMode(GL_MODELVIEW); 

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
   if(imgProc != nullptr)
   {
      //Draw image based off color channels
      if(imgProc->depth() == 3)
      {
         glPixelZoom(zoom_x, zoom_y);
         glDrawPixels( imgProc->nx(), imgProc->ny(), GL_RGB, GL_FLOAT, imgProc->raw() );
         // using textures makes my gpu go crazy

         // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgProc->nx(), imgProc->ny(),0, GL_RGB, GL_FLOAT, imgProc->raw());
         // glBindTexture(GL_TEXTURE_2D, textureID);
         // glBegin(GL_QUADS);
         // glTexCoord2i(0, 0); glVertex2i(0, 0);
         // glTexCoord2i(0, 1); glVertex2i(0, height);
         // glTexCoord2i(1, 1); glVertex2i(width, height);
         // glTexCoord2i(1, 0); glVertex2i(width, 0);
         // glEnd();

      }
      else
      {
         glPixelZoom(zoom_x, zoom_y);
         glDrawPixels( imgProc->nx(), imgProc->ny(), GL_RGBA,GL_FLOAT, imgProc->raw() );
         // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgProc->nx(), imgProc->ny(),0, GL_RGBA, GL_FLOAT, imgProc->raw());
         // glBindTexture(GL_TEXTURE_2D, textureID);
         // glBegin(GL_QUADS);
         // glTexCoord2i(0, 0); glVertex2i(0, 0);
         // glTexCoord2i(0, 1); glVertex2i(0, height);
         // glTexCoord2i(1, 1); glVertex2i(width, height);
         // glTexCoord2i(1, 0); glVertex2i(width, 0);
         // glEnd();


      }
   }

}



void StarterViewer::Reshape( int w, int h )
{
   width = w;
   height = h;
   // glViewport( 0, 0, (GLsizei) width, (GLsizei) height );
   // glMatrixMode(GL_PROJECTION); 
   // glLoadIdentity();
   // glOrtho(0, width, 0, height, -1, 1);
   // glMatrixMode(GL_MODELVIEW); 
}

void StarterViewer::Keyboard( unsigned char key, int x, int y )
{
   switch (key)
   {
      case 'o':
         if(fileName != "" && imgProc != nullptr) //dont allow writing unless image has been found
            write_image(fileName,'o',*imgProc); //write exr
         break;
      case 'j':
         if(fileName != "" && imgProc != nullptr) //dont allow writing unless image has been found
            write_image(fileName,'j',*imgProc); //write jpg
         break;
      case 'J':
         if(imgProc != nullptr)
         {
            old_imgProc = *imgProc;
            Point center(0.03811, 0.01329);
            double range = 1.0e-6;
            Point zc(0.8*cos(254.3 * 3.14159265/180.0), 0.8*sin(254.3 * 3.14159265/180.0));
            JuliaSet julia(zc, 500, 2); //zc, iterations, cycles
            //pass depth to allow 3+ channels imgProc to get correct values for lut
            ColorLUT lut(imgProc->depth(), 1.0); // depth, gamma
            ApplyFractalWarpLUT(center, range, julia, lut, *imgProc);     
         }
         break;
      case 'B':
         if(imgProc != nullptr)
         {
            old_imgProc = *imgProc;
            Brightness(*imgProc, 1.1);
         }
         break;
      case 'b':
         if(imgProc != nullptr)
         {
            old_imgProc = *imgProc;
            Bias(*imgProc, std::vector<float>{-0.1,-0.1,-0.1});
         }
         break;
      case 'c':
         if(imgProc != nullptr)
         {
            old_imgProc = *imgProc;
            Compliment(*imgProc);
         }
         break;
      case 'e':
         if(imgProc != nullptr)
         {
            old_imgProc = *imgProc;
            Grayscale(*imgProc);
         }
         break;
      case 'q':
         if(imgProc != nullptr)
         {
            old_imgProc = *imgProc;
            Quantize(*imgProc, 3.4);
         }
         break;
      case 'g':
         if(imgProc != nullptr)
         {
            old_imgProc = *imgProc;
            Gamma(*imgProc, 0.9);
         }
         break;
      case 'G':
         if(imgProc != nullptr)
         {
            old_imgProc = *imgProc;
            Gamma(*imgProc, 1.111111);
         }
         break;
      case 's':
         if(imgProc != nullptr)
         {
            old_imgProc = *imgProc;
            ImgProc out;
            Stencil stencil(5);
            //Stencil stencil = makeSharpen3x3Filter();
            BoundedLinearConvolution(stencil, *imgProc, out);
            *imgProc = out;
         }
         break;
      case 'C':
         if(imgProc != nullptr)
         {
            old_imgProc = *imgProc;
            ImgProc out;
            ContrastUnits(*imgProc, out);
            *imgProc = out;
         }
         break;
      case 'H':
         if(imgProc != nullptr)
         {
            old_imgProc = *imgProc;
            ImgProc out;
            HistogramEqualization(*imgProc, out, 500);
            *imgProc = out;
         }
         break;
      case 'z':
         if(imgProc != nullptr)
         {
            *imgProc = old_imgProc;
         }
         break;
      case 'r':
	      Reset();
         break;
      case 'h':
	      Home();
         break;
      case 'u':
	      Usage();
         break;
      case '+':
         {
         zoom_x += 0.1f;
         zoom_y += 0.1f;
         std::string nt = title + " zoom: "+ std::to_string(zoom_x);
         glutSetWindowTitle(nt.c_str());
         break;
         }
      case '-':
         {
         zoom_x -= 0.1f;
         zoom_y -= 0.1f;
         std::string nt = title + " zoom: "+ std::to_string(zoom_x);
         glutSetWindowTitle(nt.c_str());
         break;
         }
   }
}


void StarterViewer::Motion( int x, int y )
{
   // float dx = x - mouse_x;
   // float dy = y - mouse_y;
   // float pos_x = current_raster_pos[0] + dx;
   // float pos_y = current_raster_pos[1] - dy;
   // glRasterPos2f( -pos_x, -pos_y ); 

   if(keystate == GLUT_ACTIVE_SHIFT )
   {
   }


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
   cout << "+           Zoom in image\n";
   cout << "-           Zoom out image\n";
   cout << "j           Write image to JPEG/JPG\n";
   cout << "o           Write image to EXR\n";
   cout << "z           Undo last change\n\n";


   cout << "g           Apply gamma (lightens)\n";
   cout << "G           Apply gamma (darkens)\n";
   cout << "B           Apply Brightness\n";
   cout << "b           Apply Bias\n";
   cout << "c           Apply compliment\n";
   cout << "e           Apply Grayscale\n";
   cout << "q           Apply Quantize\n\n";

   cout << "s           Apply Bounded Linear Convolution\n";
   
   cout << "J           Draws the Mandelbrot Julia Set\n\n";

   cout << "C           Convert to Contrast Units\n";
   cout << "H           Histogram Equalize\n";


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


