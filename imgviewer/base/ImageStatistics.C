#include "ImageStatistics.h"

using namespace img;

float img::calcAvgColorChannel(int channel, const ImgProc& in)
{
    float avgColorA = 0;
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

float img::calcCovarianceColorChannel(int channelA, int channelB, std::vector<float>& avgs, const ImgProc& in)
{
    float covariance = 0;
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
    
    std::vector<float> avgColorChannels(in.depth());
    for(int i = 0; i < in.depth(); i++)
    {
        avgColorChannels[i] = calcAvgColorChannel(i, in);
    }

    std::vector<float> covariances(in.depth());
    for( int i = 0; i < in.depth(); i++)
    {
        covariances[i] = calcCovarianceColorChannel(i, i, avgColorChannels, in);
    }
    //convert to contrast units delta C_aij-avg/rms
    #pragma omp parallel for collapse(2)
    for(int j = 0; j < in.ny(); j++)
    {
        for(int i = 0; i < in.nx(); i++)
        {
            for(int c = 0; c < in.depth(); c++)
            {
                if(c == 3) out.raw()[index(i, j, c, in.depth(), in.nx())] = 1; //so alpha channel doesnt stop img from being seen
                else
                    out.raw()[index(i, j, c, in.depth(), in.nx())] = (in.raw()[index(i, j, c, in.depth(), in.nx())] - avgColorChannels[c]) / std::sqrt(covariances[c]);
            }
        }
    }

}




void img::calcMinMaxWidthColorChannel(int channel, int num_bins, std::vector<float>& e_mmw, const ImgProc& in)
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

            if(e_mmw[0] == 0 && e_mmw[1] == 1) break;
        }
        if(e_mmw[0] == 0 && e_mmw[1] == 1) break;
    }
    //width
    e_mmw[2] = (e_mmw[1] - e_mmw[0]) / num_bins;

}

void img::calcHistogramColorChannel(int channel, int num_bins, std::vector<float>& histogram, const std::vector<float>& e_mmw, const ImgProc& in)
{    
    for(int j = 0; j < in.ny(); j++)
    {
        for(int i = 0; i < in.nx(); i++)
        {
            //[0 - num_bins], bins are [0, 1), [1, 2), ...., [num_bins-1, num_bins)
            int bin_i = int((in.raw()[index(i, j, channel, in.depth(), in.nx())] - e_mmw[0]) / e_mmw[2]);
            //exclude the right bin edge for bin_numbins-1, i.e there is no overflow bin for values => max
            if(bin_i >= 0 && bin_i < num_bins) histogram[bin_i] += 1;
        }
    }
}

void img::calcPDFColorChannel(int channel, int num_bins, std::vector<float>& pdf, const std::vector<float>& histogram)
{
    float hist_total = 0;
    for(int i = 0; i < num_bins; i++)
    {
        hist_total += histogram[i];
    }
    //normalize
    for(int i = 0; i < num_bins; i++)
    {
        pdf[i] = histogram[i] / hist_total;
    }
    //summation should equal 1
    // float test = 0;

    // for(int i = 0; i < num_bins; i++)
    // {
    //     test += pdf[i]; 
    // }
    // std::cout << "test" << k << " : " << test << '\n';

}

void img::calcCDFColorChannel(int channel, int num_bins, std::vector<float>& cdf, const std::vector<float>& pdf)
{
    for(int j = 0; j <= num_bins; j++)
    {
        for(int i = 0; i < j; i++)
        {
            cdf[j] += pdf[i];
        }
    }
}

void img::HistogramEqualization(const ImgProc& in, ImgProc& out, int num_bins)
{
    out.clear(in.nx(), in.ny(), in.depth());

    std::vector< std::vector<float> > e_mmws(in.depth());
    std::vector< std::vector<float> > histograms(in.depth());
    std::vector< std::vector<float> > pdfs(in.depth());
    std::vector< std::vector<float> > cdfs(in.depth());

    std::vector<float> e_mmw(3);
    std::vector<float> histogram(num_bins, 0);
    std::vector<float> pdf(num_bins,0);
    std::vector<float> cdf(num_bins+1, 0);
    for(int k = 0; k < in.depth(); k++)
    {
        calcMinMaxWidthColorChannel(k, num_bins, e_mmw, in); 
        e_mmws[k] = e_mmw;
        //no need to clear e_mmw
        //std::cout << "min max" << k << ": " << e_mmws[k][0] << ' ' << e_mmws[k][1] << '\n';

        calcHistogramColorChannel(k, num_bins, histogram, e_mmws[k], in);
        histograms[k] = histogram;
        std::fill(histogram.begin(), histogram.end(), 0);

        calcPDFColorChannel(k, num_bins, pdf, histograms[k]);
        pdfs[k] = pdf;
        std::fill(pdf.begin(), pdf.end(), 0);

        calcCDFColorChannel(k, num_bins, cdf, pdfs[k]);
        cdfs[k] = cdf;
        std::fill(cdf.begin(), cdf.end(), 0);
        //std::cout << "pdf[" << k << "][0]: " << pdfs[k][0] << '\n';
        //std::cout << "cdf["<< k << "] "  << cdfs[k][0]<< ' ' << cdfs[k][1] << ' ' << cdfs[k][num_bins] << '\n';
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
                    int bin_i = int((in.raw()[index(i, j, c, in.depth(), in.nx())] - e_mmws[c][0]) / e_mmws[c][2]);
                    //allow bin_numBins-1 right edge, since cdf[numBins] = 1 so the image shows the max value: 1
                    if(bin_i >= 0 && bin_i <= num_bins) 
                        out.raw()[index(i, j, c, in.depth(), in.nx())] = cdfs[c][bin_i];
                }
            }
        }
    }

}


