#ifndef CONSTANTS_H
#define CONSTANTS_H
//interleaved access all channls for one pixel
inline int Index(int i, int j, int c,int Nc, int Nx) { return c+(Nc*(i+Nx*j));}

//non-interleaved    access all pixels for one channel
inline int IndexNon(int i, int j, int c,int Nx, int Ny) { return i+(Nx*(j+(Ny*c)));}

#endif