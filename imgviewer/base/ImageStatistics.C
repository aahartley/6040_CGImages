#include "ImageStatistics.h"

using namespace img;

float img::calcChannelAvg(int channel, const ImgProc& in)
{
    float avgColorA = 0;
    #pragma omp parallel for reduction (+:avgColorA)
    for(int j = 0; j < in.ny(); j++)
    {
        for(int i = 0; i < in.nx(); i++)
        {
            avgColorA += in.raw()[index(i, j, channel, in.depth(), in.nx())];
        }
    }
    avgColorA /= in.nx() * in.ny();
    return avgColorA;
}

float img::calcCovariance(int channelA, int channelB, std::vector<float>& avgs, const ImgProc& in)
{
    float covariance = 0;
    #pragma omp parallel for reduction (+:covariance)
    for(int j = 0; j < in.ny(); j++)
    {
        for(int i = 0; i < in.nx(); i++)
        {
            float fluctA = in.raw()[index(i, j, channelA, in.depth(), in.nx())] - avgs[channelA];
            float fluctB = in.raw()[index(i, j, channelB, in.depth(), in.nx())] - avgs[channelB];

            covariance += fluctA * fluctB;
        }
    }
    covariance /= in.nx() * in.ny();
    return covariance;
}

void img::ContrastUnits(const ImgProc& in, ImgProc& out)
{
    out.clear(in.nx(), in.ny(), in.depth());
    
    std::vector<float> channelAvgs(in.depth());
    for(int i = 0; i < in.depth(); i++)
    {
        channelAvgs[i] = calcChannelAvg(i, in);
    }
    //seperate loop to allow covariance of all channels
    std::vector<float> covariances(in.depth());
    for( int i = 0; i < in.depth(); i++)
    {
        covariances[i] = calcCovariance(i, i, channelAvgs, in);
    }
    //convert to contrast units delta C_aij-avg/rms
    #pragma omp parallel for collapse(2)
    for(int j = 0; j < in.ny(); j++)
    {
        for(int i = 0; i < in.nx(); i++)
        {
            for(int c = 0; c < in.depth(); c++)
            {
                if(c == 3) out.raw()[index(i, j, c, in.depth(), in.nx())] = 1; //so OpenGL alpha channel doesnt stop img from being seen
                else
                    out.raw()[index(i, j, c, in.depth(), in.nx())] = (in.raw()[index(i, j, c, in.depth(), in.nx())] - channelAvgs[c]) / std::sqrt(covariances[c]);
            }
        }
    }

}




void img::calcMinMaxWidth(int channel, int num_bins, std::vector<float>& e_mmw, const ImgProc& in)
{
    //min
    e_mmw[0] = in.raw()[index(0, 0, channel, in.depth(), in.nx())];
    //max
    e_mmw[1] = in.raw()[index(0, 0, channel, in.depth(), in.nx())];
    for(int j = 0; j < in.ny(); j++)
    {
        for(int i = 0; i < in.nx(); i++)
        {
            if(in.raw()[index(i, j, channel, in.depth(), in.nx())] < e_mmw[0]) 
                e_mmw[0] = in.raw()[index(i, j, channel, in.depth(), in.nx())];

            if(in.raw()[index(i, j, channel, in.depth(), in.nx())] > e_mmw[1])
                 e_mmw[1] = in.raw()[index(i, j, channel, in.depth(), in.nx())];

        }
    }
    //width
    e_mmw[2] = (e_mmw[1] - e_mmw[0]) / num_bins;

}

void img::calcHistogram(int channel, int num_bins, std::vector<float>& histogram, const std::vector<float>& e_mmw, const ImgProc& in)
{    
    for(int j = 0; j < in.ny(); j++)
    {
        for(int i = 0; i < in.nx(); i++)
        {
            //[0 - num_bins-1], bins are [0, 1), [1, 2), ...., [num_bins-1, num_bins)
            //i.e, max value does not go into last bin for this implementation, bin_i with max value = num_bins
            // bin_i is susceptible to floating point error, max value can round down to be num_bins-1
            if(in.raw()[index(i, j, channel, in.depth(), in.nx())] != e_mmw[1])//avoid floating point error & / 0
            {
                int bin_i = int((in.raw()[index(i, j, channel, in.depth(), in.nx())] - e_mmw[0]) / e_mmw[2]);
                if(bin_i >= 0 && bin_i < num_bins){ histogram[bin_i] += 1;}
            }
        }
    }
  
}

void img::calcPDF(int channel, int num_bins, std::vector<float>& pdf, const std::vector<float>& histogram)
{
    float hist_total = 0;
    for(int i = 0; i < num_bins; i++)
    {
        hist_total += histogram[i];
    }
    if(hist_total == 0) hist_total = 1; //avoids / 0, in case data is min==max
    //normalize
    for(int i = 0; i < num_bins; i++)
    {
        pdf[i] = histogram[i] / hist_total;
    }

}

void img::calcCDF(int channel, int num_bins, std::vector<float>& cdf, const std::vector<float>& pdf)
{
    for(int j = 0; j <= num_bins; j++)
    {
        for(int i = 0; i < j; i++)
        {
            cdf[j] += pdf[i];
        }
    }
    std::cout << cdf[num_bins] << '\n';
}

void img::HistogramEqualization(const ImgProc& in, ImgProc& out, int num_bins)
{
    if(num_bins == 0) return;

    out.clear(in.nx(), in.ny(), in.depth());

    std::vector< std::vector<float> > e_mmws(in.depth());
    std::vector< std::vector<float> > histograms(in.depth());
    std::vector< std::vector<float> > pdfs(in.depth());
    std::vector< std::vector<float> > cdfs(in.depth());

    std::vector<float> e_mmw(3);
    std::vector<float> histogram(num_bins, 0);
    std::vector<float> pdf(num_bins, 0);
    std::vector<float> cdf(num_bins+1, 0);
    for(int k = 0; k < in.depth(); k++)
    {
        calcMinMaxWidth(k, num_bins, e_mmw, in); 
        e_mmws[k] = e_mmw;
        //no need to clear e_mmw

        calcHistogram(k, num_bins, histogram, e_mmws[k], in);
        histograms[k] = histogram;
        std::fill(histogram.begin(), histogram.end(), 0);

        calcPDF(k, num_bins, pdf, histograms[k]);
        pdfs[k] = pdf;
        std::fill(pdf.begin(), pdf.end(), 0);

        calcCDF(k, num_bins, cdf, pdfs[k]);
        cdfs[k] = cdf;
        std::fill(cdf.begin(), cdf.end(), 0);

    }

   //replace C_kij with cdf_kbin_i
    #pragma omp parallel for collapse(2)
    for(int j = 0; j < in.ny(); j++)
    {
        for(int i = 0; i < in.nx(); i++)
        {
            for(int c = 0; c < in.depth(); c++)
            {
                if(c == 3) out.raw()[index(i, j, c, in.depth(), in.nx())] = 1; // so OpenGL doesnt erase image b/c of alpha
                else
                {
                    //max value maps to cdf_(num_bins), which always equals 1
                    //this ensures max value is displayed in image, max values < 1 will be slightly rounded up b/c of this
                    if(in.raw()[index(i, j, c, in.depth(), in.nx())] == e_mmws[c][1])
                    {
                        out.raw()[index(i, j, c, in.depth(), in.nx())] = cdfs[c][num_bins];
                    }
                    //non-max values map to cdf_(bin_i)
                    else
                    {
                        int bin_i = int((in.raw()[index(i, j, c, in.depth(), in.nx())] - e_mmws[c][0]) / e_mmws[c][2]);
                        if(bin_i >= 0 && bin_i < num_bins) 
                            out.raw()[index(i, j, c, in.depth(), in.nx())] = cdfs[c][bin_i];
                    }
                }
            }
        }
    }

}


