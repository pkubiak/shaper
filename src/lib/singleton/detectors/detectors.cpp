#include<string>
#include "detectors.hpp"
#include "boost/lexical_cast.hpp"

using boost::lexical_cast;

namespace shaper{
	namespace singleton{
		namespace detectors{
			
			
			TPoint::TPoint(float x, float y):_x(x), _y(y){
			}
			
			std::string TPoint::toString(){
				std::string result = "(" + boost::lexical_cast<std::string>(this->_x) + ", " + boost::lexical_cast<std::string>(this->_y) + ")";

				return result;
			}
			
			TEllipse::TEllipse(TPoint foci1, TPoint foci2, float eccentricity):f1(foci1), f2(foci2), e(eccentricity){
			}
			
			std::string TEllipse::toString(){
				std::string result = "([" + this->f1.toString() + ", " + this->f2.toString() + "], " + boost::lexical_cast<std::string>(this->e) + ")";
				return result;
			}
		}
	}
}
