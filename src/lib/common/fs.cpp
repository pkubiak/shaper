#include "fs.hpp"

#ifdef SHAPER_SUPPORT_PNM
	#include "fs.pnm.hpp"
#endif

#ifdef SHAPER_SUPPORT_PNG
	#include "fs.png.hpp"
#endif

#include<cstring>
#include<vector>
#include<initializer_list>
using namespace std;

void shaper::fs::load(char *path, vector<float*> *channels, int *w, int *h){
	*w = 0;
	*h = 0;
	
	#ifdef SHAPER_SUPPORT_PNM
	{
		int retval = shaper::fs::pnm::loadPNM(path, channels, w, h);
		if(retval == 0)
			return ;
	}
	#endif
	
	throw "Unsupported input file type";
}


void shaper::fs::save(char *path, char* format, vector<float*> channels, int w, int h){
	#ifdef SHAPER_SUPPORT_PNM
	if(strcmp(format, "PBM")==0){
		if(channels.size()!=1)
			throw "Wrong number of channels for `PBM` format";
		shaper::fs::pnm::savePBM(path, {channels[0]}, w, h);
		return ;
	}else
	if(strcmp(format, "PGM")==0){
		if(channels.size()!=1)
			throw "Wrong number of channels for `PGM`format";
		shaper::fs::pnm::savePGM(path, {channels[0]}, w, h);
		return ;
	}else
	if(strcmp(format, "PNM")==0){
		if(channels.size()!=3)
			throw "Wrong number of channels for `PNM` format";
		shaper::fs::pnm::savePNM(path, {channels[0], channels[1], channels[2]}, w, h);
		return ;
	}
	#endif
	throw "Unsupported output file type";
}

