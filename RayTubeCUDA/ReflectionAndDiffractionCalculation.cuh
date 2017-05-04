#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <math.h>
#include "Face.h"
#include "BasicTool.cuh"

using namespace std;
#define PI 3.1415926;

__device__ void RADC_UitizeVector(float *a,float *b,float *c);

__device__ Point RADC_GetReflectedVector(Point d,Point n);

__device__ Ray RADC_CalculateReflectedRay(Ray incidentRay,Face *face,int faceCount,int *reflectedFace,int *flag);

__device__ bool JudgeTwoTriangleFacesOnTheSameQuadrangle(int faceNumber1,int faceNumber2);
__device__ bool JudgeTwoTriangleFacesOnTheAdjacentQuadrangle(int faceNumber1,int faceNumber2);
__device__ int GetRemainingOneNumber(int a,int b);
__device__ void GetRemainingTwoNumber(int a,int *b,int *c);
__device__ void GetReflectedRayTubeWhenOneReflectedRayExist(int flagNumber,Ray *incidentRays,TriangleRayTube *reflectedRayTubes,Ray *reflectedRays,int *reflectedFace,Face *face,int i,int *reflectionExist,int *diffractionExist,int *faceNumber1,int *faceNumber2);
__device__ void GetReflectedRayTubeWhenTwoReflectedRayExist(int flagNumber1,int flagNumber2,Ray *incidentRays,TriangleRayTube *reflectedRayTubes,Ray *reflectedRays,int *reflectedFace,Face *face,int i,int *reflectionExist,int *diffractionExist,int *faceNumber1,int *faceNumber2);
__device__ void GetReflectedRayTubeWhenThreeReflectedRayExist(int flagNumber1,int flagNumber2,Ray *incidentRays,TriangleRayTube *reflectedRayTubes,Ray *reflectedRays,int *reflectedFace,Face *face,int i,int *reflectionExist,int *diffractionExist,int *faceNumber1,int *faceNumber2);
__device__ bool JudgeTwoPointsAreSame(Point A,Point B);
__device__ bool JudgeIsTheSamePointOfAdjacentExist(Point A,Face face);
__device__ Edge GetEdgeOfCube(Face face1,Face face2);
__device__ float CalculateDistanceOnXOY(Point a,Point b);
__device__ void SortPointsOfEdge(Edge *edge);
__device__ Edge GetDiffractionEdgeWhenTwoReflectedRaysExist(Face face1,Face face2,Point reflectionPoint1,Point reflectionPoint2,Point VitualReflectionPoint,int *diffractionExist);
__global__ void CalculateReflectionAndDiffractionWithBuildingFace(TriangleRayTube *incidentRayTubes,int faceCount,int rayTubeCount,TriangleRayTube *reflectedRayTubes,Face *face,Edge *diffractionEdge,int *faceNumber1,int *faceNumber2,int *reflectionExist,int *diffractionExist);
__device__ Edge GetDiffractionEdgeWhenThreeReflectedRaysExist(Face face1,Face face2,Point reflectionPoint1,Point reflectionPoint2,Point reflectionPointOnOtherFace,int *diffractionExist);
__device__ int RADC_GetMaxNumber(int a,int b,int c);
__device__ int RADC_GetMinNumber(int a,int b,int c);
cudaError_t  GetReflectionAndDiffractionWithBuildingFace(TriangleRayTube *incidentRayTubes,int faceCount,int rayTubeCount,Face *buildingFace,TriangleRayTube *reflectedRayTubes,Edge *diffractionEdge,int *faceNumber1,int *faceNumber2,int *diffractionExist,int *reflectionExist);