#include "libshaper.h"

namespace Shaper{
	namespace EdgeOperator{
			enum EdgeOperatorType{
				SOBEL = 0, SOBEL3x3 = 0,
				SOBEL5x5 = 1,
				PREWITT = 2,
				SCHARR5x5 = 3,
				USER = 127
			}
	}
	
	enum SHAPE_TYPE { SEGMENT, POLYGON, CIRCLE, ELLIPSE };

	struct Shape{
		enum SHAPE_TYPE type;
		
	}

	vector<Shape> detect(char* path);
    void RGBtoL(float R, float G, float B);//L = R*Pr+G*Pg+B*Pb
    void blur();//Gaussian with given radius

    void gradient(enum EdgeOperatorType, ...);
	
    void maximalSupression();

    void hysteresisTresholding();:    
}

/**
Use case:

	Shaper::detect("img.png");

	->
		Cirlce: c=(100,90) r=30
		Polygon: n=5 v0=(10,20) v1=(30, 30) v2=(50,90) v3=(20,40) v4=(20,10)
		...
./shaper --cuda img.png
*/
