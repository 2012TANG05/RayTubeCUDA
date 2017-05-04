#include <math.h>
#include "Face.h"
#include "BasicTool.cuh"
#include "ReflectionWithBuildingCalculationOnCPU.h"
using namespace std;
#define PI 3.1415926

int GetRemainingOneNumberOnCPU(int a,int b);
void GetRemainingTwoNumberOnCPU(int a,int *b,int *c);
void AddPathNodeToReflectedRayTubeOnCPU(TriangleRayTube *incidentRayTubes,TriangleRayTube *reflectedRayTubes,int i,Face face1,Face face2);
void GetReflectedRayTubeWhenOneReflectedRayExistOnCPU(int flagNumber,Ray *incidentRays,TriangleRayTube *reflectedRayTubes,Ray *reflectedRays,int *reflectedFace,Face *face,int i,int *reflectionExist,int *diffractionExist,int *faceNumber1,int *faceNumber2);
void GetReflectedRayTubeWhenTwoReflectedRayExistOnCPU(int flagNumber1,int flagNumber2,Ray *incidentRays,TriangleRayTube *reflectedRayTubes,Ray *reflectedRays,int *reflectedFace,Face *face,int i,int *reflectionExist,int *diffractionExist,int *faceNumber1,int *faceNumber2);
void GetReflectedRayTubeWhenThreeReflectedRayExistOnCPU(int flagNumber1,int flagNumber2,Ray *incidentRays,TriangleRayTube *reflectedRayTubes,Ray *reflectedRays,int *reflectedFace,Face *face,int i,int *reflectionExist,int *diffractionExist,int *faceNumber1,int *faceNumber2);
bool JudgeTwoPointsAreSameOnCPU(Point A,Point B);
bool JudgeIsTheSamePointOfAdjacentExistOnCPU(Point A,Face face);
Edge GetEdgeOfCubeOnCPU(Face face1,Face face2);
float CalculateDistanceOnXOYOnCPU(Point a,Point b);
void SortPointsOfEdgeOnCPU(Edge *edge);
void AntiSortPointsOfEdgeOnCPU(Edge *edge);
Edge GetDiffractionEdgeWhenTwoReflectedRaysExistOnCPU(Face face1,Face face2,Point reflectionPoint1,Point reflectionPoint2,Point VitualReflectionPoint,int *diffractionExist);
Edge GetDiffractionEdgeWhenThreeReflectedRaysExistOnCPU(Face face1,Face face2,Point reflectionPoint1,Point reflectionPoint2,Point reflectionPointOnOtherFace,int *diffractionExist);
void CalculateReflectionAndDiffractionWithBuildingFaceOnCPU(TriangleRayTube *incidentRayTubes,int faceCount,int rayTubeCount,TriangleRayTube *reflectedRayTubes,Face *face,Edge *diffractionEdge,int *faceNumber1,int *faceNumber2,int *reflectionExist,int *diffractionExist);
int GetMaxNumberOnCPU(int a,int b,int c);
int GetMinNumberOnCPU(int a,int b,int c);