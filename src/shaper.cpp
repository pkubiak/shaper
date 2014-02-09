#include "lib/libshaper.hpp"
#include<cstdio>

int main(int argc, char* argv[]){
	try{
		shaper::detect(argv[1]);
	}catch(const char *error){
		printf("Exception: %s\n",error);
		return 1;
	}
	return 0;
}
