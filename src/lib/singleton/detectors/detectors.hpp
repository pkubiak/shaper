#pragma once
#include<vector>
#include<string>
#include<type_traits>
#include<cmath>
#include<memory>

namespace shaper{
	namespace singleton{
		namespace detectors{
			
			class IShape{
				public:
					virtual std::string toString() = 0;
			};
			
			class TPoint: public IShape{
				private:
					float _x,_y;
				public:
					static const char *type;
					
					TPoint() = delete;
					//TPoint(const TPoint& ) = default;
					TPoint(float x, float y);
					virtual std::string toString();
			};
			
			/*class Segment: public IShape{
				public:
					static const char type = "Segment";
					
					Segment() = delete;
					Segment(Point, Point);
					virtual std::string toString();
			}*/
			
			class TEllipse: public IShape{
				private:
					TPoint f1, f2;
					float e;
					
				public:
					TEllipse() = delete;
					TEllipse(TPoint , TPoint , float );
					virtual std::string toString();
			};
			
			template <typename T>
			class IShapeDetector{
				public:
					static_assert(std::is_base_of<IShape, T>::value, "IShapeDetector - Template class must inherit from IShape");
					
					virtual std::vector<std::shared_ptr<T> > detect(float *in, int w, int h);
			};
			
		}
	}
}
