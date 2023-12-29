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
//test stencils
Stencil img::makeSharpen3x3Filter()
{
	Stencil s;
	s.setHalfWidth(1);
	s.values()[0] = 0;  s.values()[1] = -1; s.values()[2] = 0;
	s.values()[3] = -1; s.values()[4] = 5;  s.values()[5] = -1;
	s.values()[6] = 0;  s.values()[7] = -1; s.values()[8] = 0;
	return s;
}
Stencil img::makeEdgeDetect3x3Filter()
{
	Stencil s;
	s.setHalfWidth(1);
	s.values()[0] = -1; s.values()[1] = -1; s.values()[2] = -1;
	s.values()[3] = -1; s.values()[4] = 8;  s.values()[5] = -1;
	s.values()[6] = -1; s.values()[7] = -1; s.values()[8] = -1;
	return s;
}
Stencil img::makeEmboss3x3Filter()
{
	Stencil s;
	s.setHalfWidth(1);
	s.values()[0] = -2; s.values()[1] = -1; s.values()[2] = 0;
	s.values()[3] = -1; s.values()[4] = 1;  s.values()[5] = 1;
	s.values()[6] = 0;  s.values()[7] = 1;  s.values()[8] = 2;
	return s;
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

void img::Brightness(const ImgProc& in, ImgProc& out, const float s)
{
	
	out.clear(in.nx(),in.ny(),in.depth());

	#pragma omp parallel for
 	for(int i = 0; i < in.size(); i++)
 	{
 		out.raw()[i] = in.raw()[i] * s;
 	}
}
//in place
void img::Brightness(ImgProc& out, const float s)
{
	#pragma omp parallel for
 	for(int i = 0; i < out.size(); i++)
 	{
 		out.raw()[i] *= s;
 	}
}

void img::Bias(const ImgProc& in, ImgProc& out, const std::vector<float>& color)
{
	out.clear(in.nx(),in.ny(),in.depth());
	if((int)color.size() > in.depth()) return;

	for(int j = 0; j < in.ny(); j++)
 	{
		#pragma omp parallel for
 		for(int i = 0; i < in.nx(); i++)
 		{
 			long index = (long)i + (long)in.nx() * (long)j;
 			for(int c = 0; c < in.depth(); c++)
 			{
 				out.raw()[ c + (long)in.depth() * index ] = in.raw()[ c + (long)in.depth() * index ] + color[c];
 			}
 		}
 	}

}
void img::Bias(ImgProc& out, const std::vector<float>& color)
{
	if((int)color.size() > out.depth()) return;

	for(int j = 0; j < out.ny(); j++)
 	{
		#pragma omp parallel for
 		for(int i = 0; i < out.nx(); i++)
 		{
 			long index = (long)i + (long)out.nx() * (long)j;
 			for(int c = 0; c < out.depth(); c++)
 			{
 				out.raw()[ c + (long)out.depth() * index ] +=  color[c];
 			}
 		}
 	}
}

void img::Compliment(const ImgProc& in, ImgProc& out)
{
	out.clear(in.nx(),in.ny(),in.depth());

	#pragma omp parallel for
 	for(long i=0; i < out.size(); i++)
 	{
 		out.raw()[i] = 1.0 - in.raw()[i];
 	}
}
void img::Compliment(ImgProc& out)
{
	#pragma omp parallel for
 	for(long i=0; i < out.size(); i++)
 	{
 		out.raw()[i] = 1.0 - out.raw()[i];
 	}
}

void img::Grayscale(const ImgProc& in, ImgProc& out)
{
	out.clear(in.nx(),in.ny(),in.depth());

	for(int j = 0; j < in.ny(); j++)
 	{
		#pragma omp parallel for
 		for(int i = 0; i < in.nx(); i++)
 		{
 			long index = (long)in.depth() * ((long)i + (long)in.nx() * (long)j);
 			float grayvalue = in.raw()[index]*0.2126 + in.raw()[1+index]*0.7152 + in.raw()[2+index]*0.0722;
 			for(int c = 0; c < in.depth(); c++)
 			{
 				out.raw()[ c + index ] = grayvalue;
 			}
 		}
 	}
}
void img::Grayscale(ImgProc& out)
{
	for(int j = 0; j < out.ny(); j++)
 	{
		#pragma omp parallel for
 		for(int i = 0; i < out.nx(); i++)
 		{
 			long index = (long)out.depth() * ((long)i + (long)out.nx() * (long)j);
 			float grayvalue = out.raw()[index]*0.2126 + out.raw()[1+index]*0.7152 + out.raw()[2+index]*0.0722;
 			for(int c = 0; c < out.depth(); c++)
 			{
 				out.raw()[ c + index ] = grayvalue;
 			}
 		}
 	}
}

void img::Quantize(const ImgProc& in, ImgProc& out, int levels)
{
	out.clear(in.nx(),in.ny(),in.depth());

	for(int j = 0; j < in.ny(); j++)
 	{
		#pragma omp parallel for
 		for(int i = 0; i < in.nx(); i++)
 		{
 			long index = (long)in.depth() * ((long)i + (long)in.nx() * (long)j);
 			for(int c = 0; c < in.depth(); c++)
 			{
 				out.raw()[ c + index ] = (float)((int)(in.raw()[ c+index ]*levels)) / (float)levels;
 			}
 		}
 	}
}
void img::Quantize(ImgProc& out, float levels)
{
	for(int j = 0; j < out.ny(); j++)
 	{
		#pragma omp parallel for
 		for(int i = 0; i < out.nx(); i++)
 		{
 			long index = (long)out.depth() * ((long)i + (long)out.nx() * (long)j);
 			for(int c = 0; c < out.depth(); c++)
 			{
 				out.raw()[ c + index ] = (float)((int)(out.raw()[ c+index ]*levels)) / (float)levels;
 			}
 		}
 	}
}