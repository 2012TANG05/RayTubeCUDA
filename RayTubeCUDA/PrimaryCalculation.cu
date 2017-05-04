#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <math.h>
#include "Face.h"
#include "BasicTool.h"

using namespace std;
#define PI 3.1415926;

 __device__ void PC_UnitizeVector(float *a,float *b,float *c)//单位化向量
{
	float length=sqrt((*a)*(*a)+(*b)*(*b)+(*c)*(*c));
	*a=*a/length;
	*b=*b/length;
	*c=*c/length;
}
 
 __device__ Vector PC_GetReflectedVector(Vector d,Vector n)//计算反射射线方向向量
{
	Vector reflectedVector;
	float temp=2*(d.x*n.x+d.y*n.y+d.z*n.z);
	reflectedVector.x=d.x-temp*n.x;
	reflectedVector.y=d.y-temp*n.y;
	reflectedVector.z=d.z-temp*n.z;
	return reflectedVector;
}


 //__global__ void CalculationReflectionAndDiffractionWithBuildingFace(TriangleRayTube *incidentRayTubes,TriangleRayTube *reflectedRayTubes,float *CFace,Edge *diffractionEdge,float *distance,int faceCount,int rayTubeCount)
 __global__ void CalculationReflectionAndDiffractionWithBuildingFace(int *a)
 {
	int i=blockIdx.x*blockDim.x+threadIdx.x;
	*a=i;

}



cudaError_t  GetReflectionAndDiffractionWithBuildingFace(TriangleRayTube *originalRayTubes,TriangleRayTube *reflectedRayTubes,float *buildingFace,bool *isDiffractionExist,int buildingFaceCount,int rayTubeCount)
{
	const int pointPerFace=9;
	cudaError_t cudaStatus;

	TriangleRayTube *device_incidentRayTubes=0;
	TriangleRayTube *device_reflectedRayTubes=0;
	Edge *device_edge=0;
	float *device_face=0;
	float *device_distance=0;
	int *a=0;


	cudaStatus=cudaSetDevice(0);
	if (cudaStatus!=cudaSuccess)
	{
		fprintf(stderr,"CUDA capable GPU is not available!");
		goto Error;
	}

	cudaStatus=cudaMalloc((void**)&device_incidentRayTubes,rayTubeCount*sizeof(TriangleRayTube));
	if (cudaStatus!=cudaSuccess)
	{
		fprintf(stderr,"device_incidentRayTubes cudaMalloc error!");
		goto Error;
	}
	cudaStatus=cudaMalloc((void**)&device_reflectedRayTubes,rayTubeCount*2*sizeof(TriangleRayTube));
	if (cudaStatus!=cudaSuccess)
	{
		fprintf(stderr,"device_reflectedRayTubes cudaMalloc error!");

		goto Error;
	}
	cudaStatus=cudaMalloc((void**)&device_distance,rayTubeCount*3*sizeof(TriangleRayTube));
	if (cudaStatus!=cudaSuccess)
	{
		fprintf(stderr,"device_distance cudaMalloc error!");
		goto Error;
	}
	cudaStatus=cudaMalloc((void**)&device_edge,rayTubeCount*sizeof(Edge));
	if (cudaStatus!=cudaSuccess)
	{
		fprintf(stderr,"device_edge cudaMalloc error!");
		goto Error;
	}
	cudaStatus=cudaMalloc((void**)&device_face,buildingFaceCount*pointPerFace*sizeof(float));
	cudaStatus=cudaMalloc((void**)&a,20480*sizeof(int));

	const int num_blocks=32;
	const int num_threads=640;
	CalculationReflectionAndDiffractionWithBuildingFace<<<num_blocks,num_threads>>>(a);
	return cudaStatus;
Error:
	;

	
}