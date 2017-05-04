#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <math.h>
#include "Face.h"
#include "BasicTool.cuh"

using namespace std;
#define PI 3.1415926;

__device__ void RWBC_UitizeVector(float *a,float *b,float *c);
//__device__ Ray RWBC_CalculateReflectedRay(Ray incidentRay,Face *face,int faceCount,int *reflectedFace,int *flag)
__device__ Point RWBC_GetReflectedVector(Point d,Point n);
__device__ Ray RWBC_CalculateReflectedRay(Ray incidentRay,Face *face,int faceCount,int *reflectedFace,int *flag);
__device__ bool RWBC_JudgeTwoTriangleFacesOnTheSameQuadrangle(int faceNumber1,int faceNumber2);
__device__ bool RWBC_JudgeTwoTriangleFacesOnTheAdjacentQuadrangle(int faceNumber1,int faceNumber2);
__device__ void RWBC_GetThreeRemainingNumbers(int number,int *remainingNumber1,int *remainingNumber2,int *remainingNumber3);
__device__ void RWBC_GetOneRemainingNumber(int number1,int number2,int number3,int *remainingNumber);
__device__ void GetReflectedRayTubesWhenTwoFlagsEqualOne(int flagNumber1,int flagNumber2,int *reflectedFace,Ray *reflectedRays,Ray *incidentRays,QuadrangleRayTube *reflectedRayTubes,Face *face,int *reflectionExist,int i);
__device__ void GetReflectedRayTubesWhenThreeFlagsEqualOne(int flagNumber1,int flagNumber2,int flagNumber3,int *reflectedFace,Ray *reflectedRays,Ray *incidentRays,QuadrangleRayTube *reflectedRayTubes,QuadrangleRayTube *incidentRayTubes,Face *face,int *reflectionExist,int i);
__device__ void RWBC_AddPathNodeToReflectedRayTube(QuadrangleRayTube *incidentRayTubes,QuadrangleRayTube *reflectedRayTubes,int i,Face face1,Face face2);
__global__ void CalculateReflectionWithBuildingFace(QuadrangleRayTube *incidentRayTubes,int faceCount,int rayTubeCount,QuadrangleRayTube *reflectedRayTubes,Face *face,int *reflectionExist);
cudaError_t  GetReflectionWithBuildingFace(QuadrangleRayTube *incidentRayTubes,int faceCount,int rayTubeCount,Face *buildingFace,QuadrangleRayTube *reflectedRayTubes,int *reflectionExist);