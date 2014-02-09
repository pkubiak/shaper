#pragma once
#include "filters.hpp"

namespace shaper{
	namespace singleton{
		namespace edge_detectors{

			class IEdgeDetector: shaper::singleton::filters::IFilter<1, 1>{
				//public:
					//virtual void apply(std::array<float*, 1> INs, std::array<float**, 1> OUTs, int w, int h);
			};

			class CannyEdgeDetector: public IEdgeDetector{
				public:
					virtual void apply(std::array<float*, 1> INs, std::array<float**, 1> OUTs, int w, int h);
			};

		}
	}
}
