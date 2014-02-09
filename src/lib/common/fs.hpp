#pragma once
#include<vector>
#include<initializer_list>

namespace shaper{
	namespace fs{
		void load(char *path, std::vector<float*> *channels, int *w, int *h);

		void save(char *path, char* format, std::vector<float*> channels, int w, int h);
	}
}
