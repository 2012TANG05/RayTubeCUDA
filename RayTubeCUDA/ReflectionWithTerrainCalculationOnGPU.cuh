#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <math.h>
#include "Face.h"
#include "BasicTool.cuh"

using namespace std;
#define PI 3.1415926

extern __device__ void RWTC_UnitizeVectorOnGPU(float *a, float *b, float *c);

//���㷴�����߷�������
__device__ Point GetReflectedVectorOnGPU(Point d,Point n);

//���㷴��
__device__ Ray CalculateReflectedRayWithTerrainOnGPU(Ray incidentRay,Face *face,int faceCount,int *reflectedFace,int *flag);

//
int CalculateTriangleReflectedRayTubesWithTerrainOnGPU(const unsigned int rayTubeCount,
	unsigned int faceCount, Face *p_face, TriangleRayTube *incidentRayTubes,
	TriangleRayTube *ReflectedRayTubes,int *isReflectedRaysExist,float *distance);

//��CPU�е����ݴ���GPU��
cudaError_t GetTriangleReflectedRayTubes(TriangleRayTube *incidentRayTubes,TriangleRayTube *tempReflectedRayTubes,
	Face *face,unsigned int rayTubeCount,unsigned int faceCount,int *identifier,
	float *tempDistance);

//GPU�м��㷴�����߹�
__global__ void CalculateTempTriangleReflectedRayTubesWithTerrainOnGPU(Face *face, 
	TriangleRayTube *incidentrayTubes, TriangleRayTube *ReflectedRayTubes, 
	int faceCountPerBlock, int *identifier, float *tempDistance);

//�жϲ��ı䷴������
__device__ void JudgeAndChangeTempReflectedRay(Ray *ray, Ray tempRay, float *Distance, float tempDistance, int *identifier, int tempIdentifier, int i, int j);



























int CalculateQuadrangleReflectedRayTubes(const unsigned int rayTubeCount, 
	unsigned int faceCount, Face *p_face, QuadrangleRayTube *incidentRayTubes, 
	QuadrangleRayTube *ReflectedRayTubes, int *isReflectedRayTubesExist, float *distance);

cudaError_t GetQuadrangleReflectedRayTubes(QuadrangleRayTube *incidentRayTubes,QuadrangleRayTube *tempReflectedRayTubes,
	Face *face,unsigned int rayTubeCount,unsigned int faceCount,int *identifier,
	float *tempDistance);

__global__ void CalculateTempQuadrangleReflectedRayTubesOnGPU(Face *face, 
	QuadrangleRayTube *incidentrayTubes, QuadrangleRayTube *tempReflectedRayTubes, 
	int faceCountPerBlock, int *identifier, float *tempDistance);