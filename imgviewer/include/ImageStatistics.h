//-------------------------------------------------------
//
//  ImageStatistics.h
//
//  Contains functions to calculate statistical functions
//  Converts to Contrast Units and Histogram equalizes
//  
//--------------------------------------------------------
#ifndef IMAGESTATISTICS_H
#define IMAGESTATISTICS_H

#include <vector>
#include <string>
#include <algorithm>
#include "ImgProc.h"

namespace img
{

float calcChannelAvg(int channel, const ImgProc& in);
float calcCovariance(int channelA, int channelB, std::vector<float>& avgs, const ImgProc& in);

void ContrastUnits(const ImgProc& in, ImgProc& out);

//per channel
void calcCDF(int channel, int num_bins, std::vector<float>& cdf, const std::vector<float>& pdf);
void calcPDF(int channel, int num_bins, std::vector<float>& pdf, const std::vector<float>& histogram);
void calcHistogram(int channel, int num_bins, std::vector<float>& histogram, const std::vector<float>& e_mmw, const ImgProc& in);
void calcMinMaxWidth(int channel, int num_bins, std::vector<float>& e_mmw, const ImgProc& in);

void HistogramEqualization(const ImgProc& in, ImgProc& out, int num_bins);

}



#endif