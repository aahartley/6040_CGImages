#include "ImageProcessing.h"

using namespace img;

 Stencil::Stencil(int halfwidth)
 {
    half_width = halfwidth;
    stencil_values = new float[(2*half_width+1)*(2*half_width+1)];
    const float range_from  = -0.1f;
    const float range_to    = 0.1f;
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_real_distribution<float> distr(range_from, range_to);
    float sum =0.f;
    for(int j = 0; j <(2*half_width+1); j++)
    {
        for(int i = 0; i <(2*half_width+1); i++)
        {
            if(i != half_width && j != half_width)
            {
                stencil_values[j * (2*half_width+1) + i] = distr(generator);
                sum += stencil_values[j * (2*half_width+1) + i];
            }
        }
    }
    stencil_values[half_width * (2*half_width+1) + half_width] = 1.0f-sum;

 }

 Stencil::~Stencil()
 {
    delete[] stencil_values;
 }

 int Stencil::halfwidth() const
 {
    return half_width;
 }
 float& Stencil::operator()(int i, int j)
 {
    return stencil_values[j * (2*half_width+1) + i];
 }
 const float& Stencil::operator()(int i, int j) const
 {
    return stencil_values[j * (2*half_width+1) + i];
 }



 void img::PeriodicLinearConvolution( const Stencil& stencil, const ImgProc& in, ImgProc& out )
{
	out.clear(in.nx(), in.ny(), in.depth());
	for( int j = 0;j < out.ny(); j++)
	{
		int jmin = j - stencil.halfwidth();
		int jmax = j + stencil.halfwidth();	

		#pragma omp parallel for
		for(int i = 0; i < out.nx(); i++)
		{
			int imin = i - stencil.halfwidth();
			int imax = i + stencil.halfwidth();
			std::vector<float> pixel(out.depth(),0.0);
			std::vector<float> sample(in.depth(),0.0);
			for(int jj = jmin; jj <= jmax; jj++)
			{
				int stencilj = jj-j;
				int jjj = jj;
				if(jjj < 0 ){ jjj += out.ny(); }         
				if(jjj >= out.ny() ){ jjj -= out.ny(); }
				for(int ii = imin; ii <= imax; ii++)
				{
					int stencili = ii-i;
					int iii = ii;

					if(iii < 0 ){ iii += out.nx(); }        
					if(iii >= out.nx() ){ iii -= out.nx(); }
					const float& stencil_value = stencil(stencili+stencil.halfwidth(), stencilj+stencil.halfwidth());
					in.value(iii,jjj,sample);
					for(size_t c = 0; c < sample.size(); c++)
					{
						 pixel[c] += sample[c] * stencil_value;
				    }
				}
 			}
 			out.set_value(i,j,pixel);
 		}
	}
}
void img::BoundedLinearConvolution( const Stencil& stencil, const ImgProc& in, ImgProc& out )
{
	std::cout << "Starting Linear Convolution...\n";
	out.clear(in.nx(), in.ny(), in.depth());
	for(int j = 0; j < out.ny(); j++)
	{
		int jmin = j - stencil.halfwidth();
		int jmax = j + stencil.halfwidth();	

		#pragma omp parallel for
		for(int i = 0; i < out.nx(); i++)
		{
			bool outB =false;
			int imin = i - stencil.halfwidth();
			int imax = i + stencil.halfwidth();
			std::vector<float> pixel(out.depth(),0.0f);
			std::vector<float> sample(in.depth(),0.0f);
			for(int jj = jmin; jj <= jmax; jj++)
			{
				int stencilj = jj-j;
				int jjj = jj;
				if(jjj <0 || jjj>= out.ny()){outB=true;}
	
				for(int ii = imin; ii <= imax; ii++)
				{
					int stencili = ii-i;
					int iii = ii;
					if(iii <0 || iii>= out.nx()){outB=true;}

					const float& stencil_value = stencil(stencili+stencil.halfwidth(), stencilj+stencil.halfwidth());
					if(!outB)
					{
						in.value(iii,jjj,sample);
					}
					else
					{
						for(size_t c = 0; c < sample.size(); c++)
						{
							sample[c] = 0;
				    	}
						outB=false;

					}

					for(size_t c = 0; c < sample.size(); c++)
					{
						 pixel[c] += sample[c] * stencil_value;
				    }
				}
 			}
 			out.set_value(i,j,pixel);
 		}
	}
	std::cout <<"Finished\n";

}

void img::Gamma(const ImgProc& in, ImgProc& out, float s)
{
	out.clear(in.nx(),in.ny(),in.depth());

	#pragma omp parallel for
 	for(int i = 0; i < in.size(); i++)
 	{
 		out.raw()[i] = std::pow(in.raw()[i], s);
 	}
}
//in place
void img::Gamma(ImgProc& out, float s)
{
	#pragma omp parallel for
 	for(int i = 0; i < out.size(); i++)
 	{
 		out.raw()[i] = std::pow(out.raw()[i], s);
 	}
}