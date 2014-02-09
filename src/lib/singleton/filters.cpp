#include "filters.hpp"
#include<initializer_list>
#include<array>
#include<functional>
#include<cmath>
#include<exception>
#include<cstring>

namespace shaper{
namespace singleton{
namespace filters{


			
	template<int N, int M>
	void IFilter<N,M>::apply(std::array<float*, N> INs, std::array<float**, M> OUTs, int w, int h){
		throw "Not Implemented";
	}
	
	//std::enable_if<M == 1>
	//template<int U = N, typename std::enable_if< M == 1, int>::type = 0>
	template<int N, int M>
	void IFilter<N,M>::applyInPlace(std::array<float*, N> INs, int w, int h){
		throw "Not Implemented";
	}


	namespace utils{
		void applyMatrix(float *in, float *out, int w, int h, float *kernel, int kw, int kh){
			int yr = -(kh-1)/2, xr = -(kw-1)/2;
			
			for(int y=0;y<h;y++){
				for(int x=0;x<w;x++){
					float res = 0.0f;
					
					for(int ky=0;ky<kh;ky++)
						for(int kx=0;kx<kw;kx++)
							if(0<=x+kx+xr&& x+kx+xr<w && 0<= y+ky+yr && y+ky+yr < h)
								res+=kernel[kx+kw*ky]*in[(y+ky+yr)*w+(x+kx+xr)];
					out[y*w+x] = res;
				}
			}
		}
		
		/** Inplace operations **/
		void applyVectorHorizontalInPlace(float* in, int w, int h, float *kernel, float kw){
			throw std::runtime_error("Not Implemented");
		}
		
		void applyVectorVerticalInPlace(float* in, int w, int h, float *kernel, float kw){
			throw std::runtime_error("Not Implemented");
		}
		
		//FIXME:
		void applyMatrixInPlace(float *in, int w, int h, float *kernel, int kw, int kh){
			
			float *ptr = (float*)malloc(sizeof(float)*w*h);
			memcpy(ptr, in, w*h*sizeof(float));
			
			applyMatrix(ptr, in, w, h, kernel, kw, kh);
			free(ptr);
			/*if(kw == 1)
				applyVectorVerticalInPlace(in,  w, h, kernel, kh);
			else if(kh == 1)
				applyVectorHorizontalInPlace(in, w, h, kernel, kw);
			else{
				throw std::runtime_error("Not Implemented");
			}*/
		}
		
		void applyPerPixelFilter(std::array<float*, 1> INs, float **out, int w, int h, std::function<float(float)> f){
			if(*out == NULL)*out = (float*)malloc(sizeof(float)*w*h);
			for(int y=0;y<h;y++)
				for(int x=0;x<w;x++)
					(*out)[x+y*w] = f(INs[0][x+y*w]);
		};
		
		
		void applyPerPixelFilter(std::array<float*, 2> INs, float **out, int w, int h, std::function<float(float, float)> f){
			if(*out == NULL)*out = (float*)malloc(sizeof(float)*w*h);
			for(int y=0;y<h;y++)
				for(int x=0;x<w;x++)
					(*out)[x+y*w] = f(INs[0][x+y*w], INs[1][x+y*w]);
		};
		

		void applyPerPixelFilter(std::array<float*, 3> INs, float **out, int w, int h, std::function<float(float, float, float)> f){
			if(*out == NULL)*out = (float*)malloc(sizeof(float)*w*h);
			for(int y=0;y<h;y++)
				for(int x=0;x<w;x++)
					(*out)[x+y*w] = f(INs[0][x+y*w], INs[1][x+y*w], INs[2][x+y*w]);
		};
		
		void applyPerPixelFilter(float **out, int w, int h, std::function<float(int, int)> f){
			if(*out == NULL)*out = (float*)malloc(sizeof(float)*w*h);
			for(int y=0;y<h;y++)
				for(int x=0;x<w;x++)
					(*out)[x+y*w] = f(x, y);
		}
		
	}
	
	/** Filters implementations **/
	


	GaussianBlurFilter::GaussianBlurFilter(int n, float sigma): kernel_radius(n){
		kernel = (float*)malloc(sizeof(float)*(2*n+1));

		sigma = fabs(sigma);
		float m = 0.3989422804f/sigma;

		for(int i=0;i<=n;i++)
			kernel[n-i] = kernel[n+i] = m*exp(-(float)(i*i)/(2*sigma*sigma));
	}
	
	void GaussianBlurFilter::apply(std::array<float*, 1> INs, std::array<float**, 1> OUTs, int w, int h){
		float *out = (*OUTs[0] == NULL ? (float*)malloc(w*h*sizeof(float)) : *OUTs[0]);
		*OUTs[0] = out;
		
		utils::applyMatrix(INs[0], out, w, h, kernel, 2*kernel_radius+1, 1);
		utils::applyMatrixInPlace(out, w, h, kernel, 1, 2*kernel_radius+1);
	}
	
	void GaussianBlurFilter::applyInPlace(std::array<float*, 1> INs, int w, int h){
		utils::applyMatrixInPlace(INs[0], w, h, kernel, 2*kernel_radius+1, 1);
		utils::applyMatrixInPlace(INs[0], w, h, kernel, 1, 2*kernel_radius+1);
	}
	
	
	/** Edge Operators **/
	
	
	void SobelEdgeOperator::apply(std::array<float*, 1> INs, std::array<float**, 2> OUTs, int w, int h){
		float *outX = (*OUTs[0] == NULL ? (float*)malloc(w*h*sizeof(float)) : *OUTs[0]);
		float *outY = (*OUTs[1] == NULL ? (float*)malloc(w*h*sizeof(float)) : *OUTs[1]);
		*OUTs[0] = outX;
		*OUTs[1] = outY;
		
		/* Horizontal Gradient */
		utils::applyMatrix(INs[0], outX, w, h, v1, 1, 3);
		utils::applyMatrixInPlace( outX, w, h, v2, 3, 1);
		
		/* Vertical Gradient */
		utils::applyMatrix(INs[0], outY, w, h, v2, 1, 3);
		utils::applyMatrixInPlace( outY, w, h, v1, 3, 1);
	}
	
	/** Components Reduction **/
	

	CustomColorReduction::CustomColorReduction(float R, float G, float B): cR(R), cG(G), cB(B){}
	
	void CustomColorReduction::apply(std::array<float*, 3> INs, std::array<float**, 1> OUTs, int w, int h){
		float *out = (*OUTs[0] == NULL ? (float*)malloc(w*h*sizeof(float)) : *OUTs[0]);
		*OUTs[0] = out;
		
		for(int y=0;y<h;y++)
			for(int x=0;x<w;x++)
				out[x+y*w] = cR*(INs[0][x+y*w])+cG*(INs[1][x+y*w])+cB*(INs[2][x+y*w]);
	}
	
	/** Color reduction using coefficient used in PAL tv format **/

	void PALColorReduction::apply(std::array<float*, 3> INs, std::array<float**, 1> OUTs, int w, int h){
		float *out = (*OUTs[0] == NULL ? (float*)malloc(w*h*sizeof(float)) : *OUTs[0]);
		*OUTs[0] = out;
		
		for(int y=0;y<h;y++)
			for(int x=0;x<w;x++)
				out[x+y*w] = 0.299*(INs[0][x+y*w])+0.587*(INs[1][x+y*w])+0.114*(INs[2][x+y*w]);
	}
	
	void normalize(float* in, int w, int h){
		float minV = 10e1111, maxV = -10e1111;
		for(int i=0;i<w*h;i++){
			if(in[i]>maxV)maxV = in[i];
			if(in[i]<minV)minV = in[i];
		}
		if(maxV!=minV)
			for(int i=0;i<w*h;i++)
				in[i] = (in[i]-minV)/(maxV-minV);
	}
}
}
}
