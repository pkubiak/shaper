#include "libshaper.hpp"
#include "edge_detectors.hpp"
#include "filters.hpp"
#include "../common/fs.hpp"
#include<vector>
using namespace std;


namespace shaper{
	namespace singleton{
		
		void detect(char *path){
			vector<float*> channels;
			int width, height;
			float *image = NULL;
			
			shaper::fs::load(path, &channels, &width, &height);

			if(channels.size() == 1){
				image = channels[0];
			}else
			if(channels.size() == 3){
				shaper::singleton::filters::PALColorReduction palColorReduction;
				//convert to gray scale
				palColorReduction.apply({channels[0], channels[1], channels[2]}, {&image}, width, height);
				
				free(channels[0]);
				free(channels[1]);
				free(channels[2]);
			}else
				throw "Unsupported number of channels";
			
			
			shaper::singleton::edge_detectors::CannyEdgeDetector cannyEdgeDetector;
			
			float *edges = NULL;
			
			cannyEdgeDetector.apply({image}, {&edges}, width, height);
			free(image);
			
			shaper::singleton::filters::normalize(edges, width, height);
			
			printf("%p\n",edges);
			shaper::fs::save("canny.pgm", "PGM", {edges}, width, height);
		}
	}
}
