#ifndef CONSTANTS_H
#define CONSTANTS_H
//interleaved access all channls for one pixel
inline int index(int i, int j, int c,int Nc, int Nx) { return c+(Nc*(i+Nx*j));}

//non-interleaved    access all pixels for one channel
inline int indexNon(int i, int j, int c,int Nx, int Ny) { return i+(Nx*(j+(Ny*c)));}

#endif