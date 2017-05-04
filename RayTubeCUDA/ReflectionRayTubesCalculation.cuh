#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <math.h>
#include "Face.h"
#include "BasicTool.cuh"

using namespace std;
#define PI 3.1415926;

__device__ float RCalculateDistanceOfBipartite(Point a,Point b);
__host__ __device__ void RUnitizeVector(float *a, float *b, float *c);

//���㷴�����߷�������
__device__ Point RGetReflectedVector(Point d,Point n);

//���㷴��
__device__ void CalculatetempReflectedRays(Face *face, Ray incidentray, 
	Ray *tempReflectedRay, int faceCountPerBlock, int *identifier, float *tempDistance, 
	int distanceNumber);

//GPU�м��㷴�����߹�
__global__ void CalculatetempReflectedRayTubesOnGPU(Face *face, 
	TriangleRayTube *incidentrayTubes, TriangleRayTube *ReflectedRayTubes, 
	int faceCountPerBlock, int *identifier, float *tempDistance);

//
int CalculateReflectedRayTubes(const unsigned int rayTubeCount,
	unsigned int faceCount, Face *p_face, TriangleRayTube *incidentRayTubes,
	TriangleRayTube *ReflectedRayTubes,int *isReflectedRaysExist,float *distance);

//��CPU�е����ݴ���GPU��
cudaError_t GetReflectedRayTubes(TriangleRayTube *incidentRayTubes,TriangleRayTube *tempReflectedRayTubes,
	Face *face,unsigned int rayTubeCount,unsigned int faceCount,int *identifier,
	float *tempDistance);