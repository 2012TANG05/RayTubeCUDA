#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <math.h>
#include "Face.h"
#include "BasicTool.cuh"

using namespace std;
#define PI 3.1415926;

__device__ float RWTC_CalculateDistanceOfBipartite(Point a,Point b);
__device__ void RWTC_UnitizeVector(float *a, float *b, float *c);

//计算反射射线方向向量
__device__ Point RWTC_GetReflectedVector(Point d,Point n);

//计算反射
__device__ void RWTC_CalculatetempReflectedRays(Face *face, Ray incidentray, 
	Ray *tempReflectedRay, int faceCountPerBlock, int *identifier, float *tempDistance, 
	int distanceNumber);

//GPU中计算反射射线管
__global__ void CalculateTempTriangleReflectedRayTubesOnGPU(Face *face, 
	TriangleRayTube *incidentrayTubes, TriangleRayTube *ReflectedRayTubes, 
	int faceCountPerBlock, int *identifier, float *tempDistance);

__global__ void CalculateTempQuadrangleReflectedRayTubesOnGPU(Face *face, 
	QuadrangleRayTube *incidentrayTubes, QuadrangleRayTube *tempReflectedRayTubes, 
	int faceCountPerBlock, int *identifier, float *tempDistance);

//
int CalculateTriangleReflectedRayTubes(const unsigned int rayTubeCount,
	unsigned int faceCount, Face *p_face, TriangleRayTube *incidentRayTubes,
	TriangleRayTube *ReflectedRayTubes,int *isReflectedRaysExist,float *distance);

int CalculateQuadrangleReflectedRayTubes(const unsigned int rayTubeCount, 
	unsigned int faceCount, Face *p_face, QuadrangleRayTube *incidentRayTubes, 
	QuadrangleRayTube *ReflectedRayTubes, int *isReflectedRayTubesExist, float *distance);

//将CPU中的数据传到GPU中
cudaError_t GetTriangleReflectedRayTubes(TriangleRayTube *incidentRayTubes,TriangleRayTube *tempReflectedRayTubes,
	Face *face,unsigned int rayTubeCount,unsigned int faceCount,int *identifier,
	float *tempDistance, FILE *fp);

cudaError_t GetQuadrangleReflectedRayTubes(QuadrangleRayTube *incidentRayTubes,QuadrangleRayTube *tempReflectedRayTubes,
	Face *face,unsigned int rayTubeCount,unsigned int faceCount,int *identifier,
	float *tempDistance);

__device__ Ray RWTC_CalculateReflectedRayWithTerrainOnGPU(Ray incidentRay,Face *face,int faceCount,int *reflectedFace,int *flag);