#pragma once

#include<cstdlib>
#include<cstdio>
#include<ctype.h>
#include<cmath>
#include<vector>
#include<array>

namespace shaper{
	namespace fs{
		namespace pnm{
			int loadPNM(char *path, std::vector<float*> *data, int *width, int *height);

			int savePBM(char *path, std::array<float*, 1> channels, int w, int h);
			int savePGM(char *path, std::array<float*, 1> channels, int w, int h);
			int savePNM(char *path, std::array<float*, 3> channels, int w, int h);
		}
	}
}

