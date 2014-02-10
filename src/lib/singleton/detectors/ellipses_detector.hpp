#pragma once
#include "detectors.hpp"
#include<vector>
#include<memory>

namespace shaper{
	namespace singleton{
		namespace detectors{
			/**
			 * Ellipse detector based on article "A NEW EFFICIENT ELLIPSE DETECTION METHOD", Yonghong Xie & Qiang Ji
			 */
			class EllipsesDetector: public IShapeDetector<TEllipse>{
				public:
					virtual std::vector<std::shared_ptr<TEllipse> > detect(float *in, int w, int h);
			};
		}
	}	
}
