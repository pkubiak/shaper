


extern "C"
__global__ void RGBtoL(CUDeviceptr in, CUDeviceptr out, float R, float G, float B){
	int k = threadIdx.x;
	int w = blockDim.x*gridDim.x;
	
	int base = 
	
	for(int i=0;i<32;i++)
		out[base+i*w] = 
	
}
