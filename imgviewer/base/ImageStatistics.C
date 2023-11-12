#include "ImageStatistics.h"

using namespace img;

void img::ContrastUnits(const ImgProc& in, ImgProc& out)
{
    out.clear(in.nx(), in.ny(), in.depth());
    
    std::vector<double> avgColorChannel(in.depth());
    for(int i = 0; i < in.depth(); i++)
    {
        avgColorChannel[i] = CalcAvgColorChannel(i, in);
    }

    std::vector<double> covariance(in.depth());
    for( int i = 0; i < in.depth(); i++)
    {
        covariance[i] = CalcCovarianceColorChannel(i, i, avgColorChannel, in);
    }
    //convert to contrast units delta C_aij/rms
    for(int j = 0; j < in.ny(); j++)
    {
        for(int i = 0; i < in.nx(); i++)
        {
            for(int c = 0; c < in.depth(); c++)
            {
                if(c == 3) out.raw()[index(i, j, c, in.depth(), in.nx())] = 1; //so alpha channel doesnt stop img from being seen
                else
                    out.raw()[index(i, j, c, in.depth(), in.nx())] = (in.raw()[index(i, j, c, in.depth(), in.nx())] - avgColorChannel[c]) / std::sqrt(covariance[c]);
            }
        }
    }

}

double img::CalcAvgColorChannel(int channel, const ImgProc& in)
{
    double avgColorA = 0;
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

double img::CalcCovarianceColorChannel(int channelA, int channelB, std::vector<double>& avgs, const ImgProc& in)
{
    double covariance = 0;
    for(int j = 0; j < in.ny(); j++)
    {
        for(int i = 0; i < in.nx(); i++)
        {
            double fluctA = in.raw()[index(i, j, channelA, in.depth(), in.nx())] - avgs[channelA];
            double fluctB = in.raw()[index(i, j, channelB, in.depth(), in.nx())] - avgs[channelB];

            covariance += fluctA * fluctB;
        }
    }
    covariance /= in.nx() * in.ny();
    return covariance;
}

void img::Histogram(const ImgProc& in, ImgProc& out, int num_bins)
{
    out.clear(in.nx(), in.ny(), in.depth());

    std::vector< std::vector<double> > e_min_max(in.depth());
    std::vector<double> e_widths(in.depth());
    for(int i = 0; i < in.depth(); i++)
    {
        e_min_max[i] = CalcMinMaxColorChannel(i, in);
        // std::cout << e_min_max[i][0] << ' ' << e_min_max[i][1] << '\n';

        e_widths[i] = (e_min_max[i][1] - e_min_max[i][0]) / num_bins;
    }
    std::vector< std::vector<double> > histograms(in.depth());
    for(int k = 0; k < in.depth(); k++)
    {
        std::vector<double> histogram(num_bins, 0);

        for(int j = 0; j < in.ny(); j++)
        {
            for(int i = 0; i < in.nx(); i++)
            {
                int bin_i = int((in.raw()[index(i, j, k, in.nx(), in.ny())] - e_min_max[k][0]) / e_widths[k]);
                if(bin_i == 500) bin_i = 499;
                if(bin_i >= 0 && bin_i < num_bins) histogram[bin_i] += 1;
                else std::cout << bin_i << '\n';
            }
        }
        histograms[k] = histogram;
    }
    std::vector<double> hist_totals(in.depth(), 0);
    std::vector< std::vector<double> > pdf(in.depth());
    for(int j = 0; j < in.depth(); j++)
    {
        for(int i = 0; i < num_bins; i++)
        {
            hist_totals[j] += histograms[j][i];
        }
        //normalize
        pdf[j] = std::vector<double>(num_bins,0);

        for(int i = 0; i < num_bins; i++)
        {
            pdf[j][i] = histograms[j][i] / hist_totals[j];
           // std::cout << pdf[j][i] << '\n';
        }
    
    }

    std::vector< std::vector<double> > cdf(in.depth());
    for(int k = 0; k < in.depth(); k++)
    {
        cdf[k] = std::vector<double>(num_bins,0);

        for(int j = 0; j < num_bins; j++)
        {
            for(int i = 0; i < j; i++)
            {
                cdf[k][j] += pdf[k][i];
            }
        }
        // std::cout << "pdf: " << pdf[0][0] << '\n';
        // std::cout << cdf[0][0] << ' ' << cdf[0][1] << ' ' << cdf[0][num_bins-1] << '\n';
    }

    for(int k = 0; k < in.depth(); k++)
    {
        for(int j = 0; j < in.ny(); j++)
        {
            for(int i = 0; i < in.nx(); i++)
            {
                int bin_i = int((in.raw()[index(i, j, k, in.depth(), in.nx())] - e_min_max[k][0]) / e_widths[k]);
                if(bin_i == 500) bin_i = 499;

                if(bin_i >= 0 && bin_i < num_bins) 
                    out.raw()[index(i, j, k, in.depth(), in.nx())] = cdf[k][bin_i];
                else{
                    out.raw()[index(i, j, k, in.depth(), in.nx())] = in.raw()[index(i, j, k, in.depth(), in.nx())];
                    std::cout << "what to do \n";
                }
            }
        }
    }

}
// pass vector instead?
std::vector<double> img::CalcMinMaxColorChannel(int channel, const ImgProc& in)
{
    std::vector<double> e_min_max(2);
    e_min_max[0] = in.raw()[index(0, 0, channel, in.depth(), in.nx())];
    e_min_max[1] = in.raw()[index(0, 0, channel, in.depth(), in.nx())];
    for(int j = 0; j < in.ny(); j++)
    {
        for(int i = 0; i < in.nx(); i++)
        {
            if(in.raw()[index(i, j, channel, in.depth(), in.nx())] < e_min_max[0]) e_min_max[0] = in.raw()[index(i, j, channel, in.depth(), in.nx())];
            if(in.raw()[index(i, j, channel, in.depth(), in.nx())] > e_min_max[1]) e_min_max[1] = in.raw()[index(i, j, channel, in.depth(), in.nx())];
            if(e_min_max[0] == 0 && e_min_max[1] == 1) return e_min_max;
        }
    }
    return e_min_max;
}