#include<cstdlib>
#include<cstdio>
#include<ctype.h>
#include<cmath>
#include<vector>
#include<array>
#include "fs.pnm.hpp"
using namespace std;

namespace shaper{
namespace fs{
namespace pnm{
	
int loadPNM(char *path, std::vector<float*> *data, int *width, int *height){
	FILE *f;
	f = fopen(path, "rb");
	if(f==NULL)
		return -1;
	
	char H[2];
	
	fread(H, 2, 1, f);
	if(H[0] == 'P'){
		
		//color ascii PPM
		if(H[1] == '3'){
			char line[105];int num = -3;
			long w,h,bpp;
			float *res[3];
			
			while(fgets(line, 100, f)){
				if(line[0]!='#'){
					char *p = line;
					while(*p){
						if(isdigit(*p)){
							long val = strtol(p, &p, 10);
							if(num == -3)w = val;else
							if(num == -2)h = val;else
							if(num == -1)bpp = val;else{
								if(num<3*w*h)
									res[num%3][num/3] = float(val)/float(bpp);
							}
							if(num == -2){
								res[0] = (float*)malloc(w*h*sizeof(float));
								res[1] = (float*)malloc(w*h*sizeof(float));
								res[2] = (float*)malloc(w*h*sizeof(float));
								
								for(int i=0;i<w*h;i++)
									res[0][i] = res[1][i] = res[2][i] = 0.0f;
							}
							num++;
						}else
							p++;
					}
				}
			}
			printf("P3; w=%d; h=%d; b=%d; r=%d\n",w,h,bpp,num-3*w*h);
			if(num-3*w*h)
				throw std::runtime_error("Malformed PNM input");
			data->push_back(res[0]);
			data->push_back(res[1]);
			data->push_back(res[2]);
			
			*width = w;
			*height = h;
			fclose(f);
			return 0;
		}else{
			fclose(f);
			return -1;
		}
	}else
		return -1;
	
}

int savePBM(char *path, std::array<float*, 1> channels, int w, int h){
	FILE *f;
	f = fopen(path,"w");
	if(f==NULL)
		return -1;
	
	fprintf(f,"P1\n%d %d\n",w,h);
	
	for(int i=0;i<w*h;){
		for(int j=0;j<30&&i<w*h;j++,i++)
			fprintf(f, "%d ", (channels[0][i]>0.0001?1:0));
		fprintf(f, "\n");
	}
	fclose(f);
	return 0;
}

int savePGM(char *path, std::array<float*, 1> channels, int w, int h){
	FILE *f;
	f = fopen(path,"w");
	if(f==NULL)
		return -1;
	
	fprintf(f,"P2\n%d %d\n%d\n",w,h,255);
		
	for(int i=0;i<w*h;){
		for(int j=0;j<10&&i<w*h;j++,i++)
			fprintf(f, "%d ",  (channels[0][i]<0.0f?0:(channels[0][i]>1.0f?255:(int)(255.0f*channels[0][i]))));
		fprintf(f, "\n");
	}
	fclose(f);
	return 0;
}
int savePNM(char *path,std::array<float*, 3> channels, int w, int h){
	FILE *f;
	f = fopen(path,"w");
	if(f==NULL)
		return -1;

	fprintf(f,"P3\n%d %d\n%d\n",w,h,255);		
	
	for(int i=0;i<w*h;){
		for(int j=0;j<18&&i<w*h;j++,i++)
			fprintf(f, "%d %d %d ", (channels[0][i]<0.0f?0:(channels[0][i]>1.0f?255:(int)(255.0f*channels[0][i]))),
			(channels[1][i]<0.0f?0:(channels[1][i]>1.0f?255:(int)(255.0f*channels[1][i]))),
			(channels[2][i]<0.0f?0:(channels[2][i]>1.0f?255:(int)(255.0f*channels[2][i]))));
		fprintf(f, "\n");
	}
	
	fclose(f);
	return 0;
}
}

}
}
