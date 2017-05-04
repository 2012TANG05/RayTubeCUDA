#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <math.h>
#include "Face.h"
#include "BasicTool.cuh"
#include "ReflectionWithTerrainCalculationOnGPU.cuh"

using namespace std;
#define PI 3.1415926

//单位化向量
__device__ void RWBC_UitizeVectorOnGPU(float *a,float *b,float *c);

//计算反射射线方向向量
__device__ Point RWBC_GetReflectedVectorOnGPU(Point d,Point n);

//计算反射射线
__device__ Ray RWBC_CalculateReflectedRayOnGPU(Ray incidentRay,Face *face,int faceCount,int *reflectedFace,int *flag);


__device__ bool RWBC_JudgeTwoTriangleFacesOnTheSameQuadrangle(int faceNumber1,int faceNumber2);
__device__ bool RWBC_JudgeTwoTriangleFacesOnTheAdjacentQuadrangle(int faceNumber1,int faceNumber2);
__device__ void RWBC_GetThreeRemainingNumbersOnGPU(int number,int *remainingNumber1,int *remainingNumber2,int *remainingNumber3);
__device__ void RWBC_GetOneRemainingNumberOnGPU(int number1,int number2,int number3,int *remainingNumber);
__device__ void GetReflectedRayTubesWhenTwoFlagsEqualOneOnGPU(int flagNumber1,int flagNumber2,int *reflectedFace,Ray *reflectedRays,Ray *incidentRays,QuadrangleRayTube *reflectedRayTubes,Face *face,int *reflectionExist,int i);
__device__ void GetReflectedRayTubesWhenThreeFlagsEqualOneOnGPU(int flagNumber1,int flagNumber2,int flagNumber3,int *reflectedFace,Ray *reflectedRays,Ray *incidentRays,QuadrangleRayTube *reflectedRayTubes,QuadrangleRayTube *incidentRayTubes,Face *face,int *reflectionExist,int i);
__device__ void RWBC_AddPathNodeToReflectedRayTubeOnGPU(QuadrangleRayTube *incidentRayTubes,QuadrangleRayTube *reflectedRayTubes,int i,Face face1,Face face2);
__global__ void CalculateReflectionWithBuildingFaceOnGPU(QuadrangleRayTube *incidentRayTubes,int faceCount,int rayTubeCount,QuadrangleRayTube *reflectedRayTubes,Face *face,int *reflectionExist);
cudaError_t  GetReflectionWithBuildingFaceOnGPU(QuadrangleRayTube *incidentRayTubes,int faceCount,int rayTubeCount,Face *buildingFace,QuadrangleRayTube *reflectedRayTubes,int *reflectionExist);