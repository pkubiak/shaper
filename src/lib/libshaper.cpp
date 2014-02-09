#include "libshaper.hpp"

#ifdef LIBSHAPER_SINGLETON
	#include "singleton/libshaper.hpp"
#endif

namespace shaper{
	
	void detect(char *path){
		#ifdef LIBSHAPER_SINGLETON
			shaper::singleton::detect(path);
		#endif
	}
}
