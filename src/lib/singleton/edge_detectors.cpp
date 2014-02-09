#include<cmath>
#include  "filters.hpp"
#include<functional>
#include "edge_detectors.hpp"
#include "../common/fs.hpp"
#include<queue>
#include<cstring>

#define _DEBUG 1

namespace shaper{
namespace singleton{
namespace edge_detectors{

void CannyEdgeDetector::apply(std::array<float*, 1> INs, std::array<float**, 1> OUTs, int w, int h){
	float *out = NULL;
	
	//Blue image to reduce noises
	shaper::singleton::filters::GaussianBlurFilter blurFilter(10, 2.0);
	blurFilter.apply({INs[0]}, {&out}, w, h);

	 
	//Apply edge operator 
	float *gradX = NULL, *gradY = NULL;
	shaper::singleton::filters::SobelEdgeOperator edgeOperator;
	edgeOperator.apply( {out}, {&gradX, &gradY}, w, h);
	
	
	//Calculate gradient magnitude
	float *gradMagnitude = NULL, *gradDirection = NULL;

	shaper::singleton::filters::utils::applyPerPixelFilter({gradX, gradY}, &gradMagnitude, w, h, [](float x, float y) -> float{
		return sqrtf(x*x+y*y);
	});
	
	//Detect angle
	/*if(false){
		float *gradAngle = NULL;
		shaper::singleton::filters::utils::applyPerPixelFilter({gradX, gradY}, &gradAngle, w, h, [](float x, float y) -> float{
			return atan2f(x,y);
		});
		
	}*/
	//shaper::singleton::filters::normalize(gradMagnitude, w, h);
	
	//nonmaximal supression
	shaper::singleton::filters::utils::applyPerPixelFilter(&gradDirection, w, h, [gradX, gradY, gradMagnitude, w,h](int x, int y) -> float{
		float mag = gradMagnitude[x+y*w], X = gradX[x+y*w], Y = gradY[x+y*w];
		
		if( (X > 0.3742*Y) == (X < -0.3742*Y)){//kierunek |
			if( (y==0 || gradMagnitude[x+(y-1)*w]<=mag) && (y==h-1 || gradMagnitude[x+(y+1)*w]<mag))
				return mag;
			return 0.0f;
		}
		if( (Y > 0.3742*X) == (Y < -0.3742*X)){//kierunek -
			if ( (x==0 || gradMagnitude[(x-1)+y*w]<=mag) && ( x==w-1 || gradMagnitude[(x+1)+y*w]< mag))
				return mag;
			return 0.0f;
		}
		if( X*Y > 0 ){//kierunek /
			if( ((x==0||y==0) || gradMagnitude[(x-1)+(y-1)*w]<=mag) && ((x==w-1 || y == h-1) || gradMagnitude[(x+1)+(y+1)*w] < mag))
				return mag;
			return 0.0f;
		}
		if (((y==0||x==w-1) || gradMagnitude[(x+1)+(y-1)*w] <=mag) && ((y==h-1||x==0) || gradMagnitude[(x-1)+(y+1)*w] < mag))
			return mag;
		return 0.0f;
	});
	
	shaper::singleton::filters::normalize(gradDirection, w, h);
	
	#ifdef _DEBUG
	shaper::singleton::filters::normalize(INs[0], w, h);
	shaper::fs::save("canny_input.pgm", "PGM", {INs[0]}, w, h);
	shaper::singleton::filters::normalize(out, w, h);
	shaper::fs::save("canny_blur.pgm", "PGM", {out}, w, h);
	shaper::singleton::filters::normalize(gradX, w, h);
	shaper::fs::save("canny_gradX.pgm", "PGM", {gradX}, w, h);
	shaper::singleton::filters::normalize(gradY, w, h);
	shaper::fs::save("canny_gradY.pgm", "PGM", {gradY}, w, h);
	shaper::singleton::filters::normalize(gradMagnitude, w, h);
	shaper::fs::save("canny_magnitude.pgm", "PGM", {gradMagnitude}, w, h);
	//shaper::fs::save("canny_angle.pgm", "PGM", {grad}, w, h);
	//shaper::singleton::filters::normalize(gradDirection, w, h);
	shaper::fs::save("canny_direction.pgm", "PGM", {gradDirection}, w, h);
	#endif
	
	//tresholding
	
	float lowThreshold = 0.05;
	float highThreshold = 0.2;
	
	memset(out, 0, sizeof(float)*w*h);
	std::queue<int > Q;
	for(int y=0;y<h;y++)
		for(int x=0;x<w;x++){
			if(gradDirection[x+y*w]>highThreshold){
				int pos = x+y*w, nextpos = -1;
				gradDirection[pos] = 0.0f;	
				while(true){
					nextpos = -1;
					
					out[pos] = 1.0f;
					for(int dy=(pos>w?-1:0);dy<=(pos<w*(h-1)?1:0);dy++)
						for(int dx=(pos%w?-1:0);dx<=((pos%w)==w-1?0:1);dx++)
							if((dx||dy)&&gradDirection[pos+dx+dy*w]>lowThreshold){
								gradDirection[pos+dx+dy*w] = 0.0f;
								if(nextpos == -1)
									nextpos = pos+dx+dy*w;
								else Q.push(pos+dx+dy*w);
							}
					
					if(nextpos==-1){
						if(Q.empty())
							break;
						pos = Q.front();
						Q.pop();
					}else
						pos = nextpos;
				}
			}
		}
	
	#ifdef _DEBUG
	shaper::singleton::filters::normalize(out, w, h);
	shaper::fs::save("canny_threshold.pgm", "PGM", {out}, w, h);
	//shaper::singleton::filters::normalize(gradDirection, w, h);
	shaper::fs::save("canny_direction_remained.pgm", "PGM", {gradDirection}, w, h);
	#endif
	
	
	if(OUTs[0] != NULL)
		*OUTs[0] = out;

	//free memory
	 free(gradX); free(gradY); free(gradMagnitude); 
}


}
}
}
