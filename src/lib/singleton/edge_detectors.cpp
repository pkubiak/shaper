#include<cmath>
#include  "filters.hpp"
#include<functional>
#include "edge_detectors.hpp"
#include "../common/fs.hpp"
#define _DEBUG 1

namespace shaper{
namespace singleton{
namespace edge_detectors{

void CannyEdgeDetector::apply(std::array<float*, 1> INs, std::array<float**, 1> OUTs, int w, int h){
	float *out = NULL;
	
	//Blue image to reduce noises
	shaper::singleton::filters::GaussianBlurFilter blurFilter(5, 1.1);
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
	if(false){
		float *gradAngle = NULL;
		shaper::singleton::filters::utils::applyPerPixelFilter({gradX, gradY}, &gradAngle, w, h, [](float x, float y) -> float{
			return atan2f(x,y);
		});
		
	}
	shaper::singleton::filters::normalize(gradMagnitude, w, h);
	
	//calculate direction
	/*shaper::singleton::filters::utils::applyPerPixelFilter({gradX, gradY}, &gradDirection, w, h, [](float x, float y) -> float{
		if( (x > 0.3742*y) == (x < -0.3742*y)) return 0.0;//kierunek |
		if( (y > 0.3742*x) == (y < -0.3742*x)) return 1.0;//kierunek -
		if( x*y > 0 )return 2.0;//kierunek /
		return 3.0;//kierunek \. 
	});*/
	
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
	shaper::singleton::filters::normalize(gradDirection, w, h);
	shaper::fs::save("canny_direction.pgm", "PGM", {gradDirection}, w, h);
	#endif
	//nonmaximal supression
	if(OUTs[0] != NULL)
		*OUTs[0] = gradDirection;
	
	
	//thresholding
	
	//free memory
	free(out); free(gradX); free(gradY); free(gradMagnitude); 
}


}
}
}
