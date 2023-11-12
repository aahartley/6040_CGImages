#ifndef IMAGESTATISTICS_H
#define IMAGESTATISTICS_H

#include <vector>
#include <string>
#include <algorithm>
#include "ImgProc.h"

namespace img
{

void ContrastUnits(const ImgProc& in, ImgProc& out);

double CalcAvgColorChannel(int channel, const ImgProc& in);
double CalcCovarianceColorChannel(int channelA, int channelB, std::vector<double>& avgs, const ImgProc& in);

void Histogram(const ImgProc& in, ImgProc& out, int bins);
std::vector<double> CalcMinMaxColorChannel(int channel, const ImgProc& in);

}



#endif