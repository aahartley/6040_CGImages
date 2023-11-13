#ifndef IMAGESTATISTICS_H
#define IMAGESTATISTICS_H

#include <vector>
#include <string>
#include <algorithm>
#include "ImgProc.h"

namespace img
{

float calcAvgColorChannel(int channel, const ImgProc& in);
float calcCovarianceColorChannel(int channelA, int channelB, std::vector<float>& avgs, const ImgProc& in);
void ContrastUnits(const ImgProc& in, ImgProc& out);

void calcCDFColorChannel(int channel, int num_bins, std::vector<float>& cdf, const std::vector<float>& pdf);
void calcPDFColorChannel(int channel, int num_bins, std::vector<float>& pdf, const std::vector<float>& histogram);
void calcHistogramColorChannel(int channel, int num_bins, std::vector<float>& histogram, const std::vector<float>& e_mmw, const ImgProc& in);
void calcMinMaxWidthColorChannel(int channel, int num_bins, std::vector<float>& e_mmw, const ImgProc& in);
void HistogramEqualization(const ImgProc& in, ImgProc& out, int num_bins);

}



#endif