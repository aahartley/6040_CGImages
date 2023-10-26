//-------------------------------------------------------
//
//  Warp.h
//
//  Holds struct for Point and abstract class Warp 
//  
//  
//--------------------------------------------------------
#ifndef WARP_H
#define WARP_H

#include "ImgProc.h"

namespace img
{

struct Point{
    Point(){x = 0; y = 0;}
    Point(double xx, double yy): x(xx), y(yy){}
    double x, y;
};

//abstract class
class Warp 
{
public:
    Warp(){}
    virtual ~Warp(){}
    virtual Point operator()(const Point& P) const = 0;
};




}

#endif