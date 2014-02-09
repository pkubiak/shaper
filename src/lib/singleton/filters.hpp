#pragma once

#include<initializer_list>
#include<array>
#include<functional>

namespace shaper{
	namespace singleton{
		namespace filters{

			/**
			 * Generic class for filters working on arrays of floats and returning other arrays of float
			 **/
			template<int N, int M>
			class IFilter{
				public:
					/**
					 * @param INs array of input data, of size according to filter specification
					 * @param OUTs list of pointers to output, if not null then willbe used as output
					 * @param w width of input
					 * @param h height of input
					 */
					virtual void apply(std::array<float*, N> INs, std::array<float**, M> OUTs, int w, int h);
					/**
					 * Implementation of filter that doesn't use additional memory, result are stored in input data
					 */
					virtual void applyInPlace(std::array<float*, N> INs, int w, int h);
			};
			
			namespace utils{
				void applyMatrix(float *in, float *out, int w, int h, float *kernel, int kw, int kh);
				
				/** Inplace operations **/

				void applyMatrixInPlace(float *in, int w, int h, float *kernel, int kw, int kh);
				
				void applyPerPixelFilter(std::array<float*, 1> INs, float **out, int w, int h, std::function<float(float)> f);

				
				void applyPerPixelFilter(std::array<float*, 2> INs, float **out, int w, int h, std::function<float(float, float)> f);
				
				void applyPerPixelFilter(std::array<float*, 3> INs, float **out, int w, int h, std::function<float(float, float, float)> f);
				
				void applyPerPixelFilter(float **out, int w, int h, std::function<float(int, int)> f);
			}
			
			/** Filters implementations **/
			
			/** Noise Reduction Filters **/
			class GaussianBlurFilter: public IFilter<1, 1>{
				private:
					float *kernel;
					int kernel_radius;
				public:
					GaussianBlurFilter() = delete;
					GaussianBlurFilter(int n, float sigma);
					virtual void apply(std::array<float*, 1> INs, std::array<float**, 1> OUTs, int w, int h);
					virtual void applyInPlace(std::array<float*, 1> INs, int w, int h);
			};
			
			/** Edge Operators **/
			class IEdgeOperator: public IFilter<1, 2>{};
			
			class SobelEdgeOperator: public IEdgeOperator{
				private:
					float v1[3] = {1.0f, 2.0f, 1.0f};
					float v2[3] = {1.0f, 0.0f, -1.0f};
				public:
					virtual void apply(std::array<float*, 1> INs, std::array<float**, 2> OUTs, int w, int h);
					//virtual void applyInPlace(std::array<float*, 1> INs, int w, int h);
			};
			
			/** Components Reduction **/
			
			class IColorReduction: public IFilter<3, 1>{};
			
			class CustomColorReduction: IColorReduction{
				private:
					float cR, cG, cB;
				public:
					CustomColorReduction(float R, float G, float B);
					virtual void apply(std::array<float*, 3> INs, std::array<float**, 1> OUTs, int w, int h);
					//virtual void applyInPlace(std::array<float*, 3> INs, int w, int h);
			};
			
			/** Color reduction using coefficient used in PAL tv format **/
			class PALColorReduction: IColorReduction{
				public:
					virtual void apply(std::array<float*, 3> INs, std::array<float**, 1> OUTs, int w, int h);
					//virtual void applyInPlace(std::array<float*, 3> INs, int w, int h);
			};
			
			void normalize(float* in, int w, int h);
		}
	}
}
