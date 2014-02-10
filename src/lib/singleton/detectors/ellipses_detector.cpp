#include "ellipses_detector.hpp"
#include "detectors.hpp"
#include <vector>
#include <utility>
#include <algorithm>
#include <memory>
#include <cstdio>
using namespace std;


namespace shaper{
	namespace singleton{
		namespace detectors{
			std::vector<shared_ptr<TEllipse> > EllipsesDetector::detect(float *in, int w, int h){
				vector<std::shared_ptr<TEllipse> > detected;
				
				float minDst = powf(50.0f, 2.0f);
				int minPoints = 50;
				float axis_range = 1.0f;
				
				vector<pair<int, int> > V;
				
				for(int y=0;y<h;y++)
					for(int x=0;x<w;x++)
						if(in[x+w*y]>0.5f)
							V.push_back(make_pair(x,y));
				
				
				
				for(int p1 = 0;p1<V.size();p1++){
					printf("%d/%d\n", p1, V.size());
					for(int p2 = p1+1; p2<V.size();p2++){
						float dst = (V[p1].first-V[p2].first)*(V[p1].first-V[p2].first) + 
							(V[p1].second-V[p2].second)*(V[p1].second-V[p2].second);
						if(dst > minDst){
							float x0 = 0.5f*(V[p1].first+V[p2].first);
							float y0 = 0.5f*(V[p1].second+V[p2].second);
							
							float a = 0.5*sqrtf(dst);
							vector<pair<float, int> > P;
							
							for(int p3 = 0; p3< V.size(); p3++)
								if(p3!=p1&&p3!=p2){
									
									float d = sqrtf(powf(V[p3].first-x0, 2.0f)+powf(V[p3].second-y0, 2.0f));
									if(d<a&&d>0.5*minDst){
										float f1 = sqrtf(powf(V[p1].first-V[p3].first, 2.0f) + powf(V[p1].second-V[p3].second, 2.0f));
										float f2 = sqrtf(powf(V[p2].first-V[p3].first, 2.0f) + powf(V[p2].second-V[p3].second, 2.0f));
										float f = (f1<f2?f1:f2);
										
										float cost = (a*a + d*d - f*f) / (2*a*d);
										float b = sqrtf(a*a*d*d*(1-cost*cost)/(a*a-d*d*cost*cost));
										P.push_back(make_pair(b, p3));
									}
								}
								
							sort(P.begin(),P.end());
							int it=0, maxV = 0, maxI = -1;
							for(int i=0;i<P.size();i++){
								while(it<i&&P[i].first-P[it].first>axis_range)it++;
								if(i-it+1 > maxV){
									maxV = i-it+1;
									maxI = i;
								}
							}
							
							if(maxV > minPoints){
								float b = 0.0f;
								for(int i=0;i<maxV;i++){
									b+=P[maxI-i].first;
								}
								b/=(float)maxV;
								detected.push_back(std::make_shared<TEllipse>(
									TPoint((float)V[p1].first, (float)V[p1].second), 
									TPoint((float)V[p2].first, (float)V[p2].second),
									sqrtf(1-b*b/(a*a))
								));
								return detected;
							}
						}
					}
				}
				return detected;
			}
		}
	}	
}
