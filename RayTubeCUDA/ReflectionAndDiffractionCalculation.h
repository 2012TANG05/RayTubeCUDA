#include <math.h>
#include "Face.h"
#include "BasicTool.cuh"
#include "ReflectionWithBuildingCalculation.h"
using namespace std;
#define PI 3.1415926;

//Ray CalculateReflectedRayAndDiffractionOnCPU(Ray incidentRay,Face *face,int faceCount,int *reflectedFace,int *flag);
//bool JudgeTwoTriangleFacesOnTheSameQuadrangle(int faceNumber1,int faceNumber2);
//bool JudgeTwoTriangleFacesOnTheAdjacentQuadrangle(int faceNumber1,int faceNumber2);
int GetRemainingOneNumberOnCPU(int a,int b);
void GetRemainingTwoNumberOnCPU(int a,int *b,int *c);
void AddPathNodeToReflectedRayTube(TriangleRayTube *incidentRayTubes,TriangleRayTube *reflectedRayTubes,int i,Face face1,Face face2);
void GetReflectedRayTubeWhenOneReflectedRayExist(int flagNumber,Ray *incidentRays,TriangleRayTube *reflectedRayTubes,Ray *reflectedRays,int *reflectedFace,Face *face,int i,int *reflectionExist,int *diffractionExist,int *faceNumber1,int *faceNumber2);
void GetReflectedRayTubeWhenTwoReflectedRayExist(int flagNumber1,int flagNumber2,Ray *incidentRays,TriangleRayTube *reflectedRayTubes,Ray *reflectedRays,int *reflectedFace,Face *face,int i,int *reflectionExist,int *diffractionExist,int *faceNumber1,int *faceNumber2);
void GetReflectedRayTubeWhenThreeReflectedRayExist(int flagNumber1,int flagNumber2,Ray *incidentRays,TriangleRayTube *reflectedRayTubes,Ray *reflectedRays,int *reflectedFace,Face *face,int i,int *reflectionExist,int *diffractionExist,int *faceNumber1,int *faceNumber2);
bool JudgeTwoPointsAreSame(Point A,Point B);
bool JudgeIsTheSamePointOfAdjacentExist(Point A,Face face);
Edge GetEdgeOfCube(Face face1,Face face2);
float CalculateDistanceOnXOY(Point a,Point b);
void SortPointsOfEdge(Edge *edge);
void AntiSortPointsOfEdge(Edge *edge);
Edge GetDiffractionEdgeWhenTwoReflectedRaysExist(Face face1,Face face2,Point reflectionPoint1,Point reflectionPoint2,Point VitualReflectionPoint,int *diffractionExist);
Edge GetDiffractionEdgeWhenThreeReflectedRaysExist(Face face1,Face face2,Point reflectionPoint1,Point reflectionPoint2,Point reflectionPointOnOtherFace,int *diffractionExist);
void CalculateReflectionAndDiffractionWithBuildingFaceOnCPU(TriangleRayTube *incidentRayTubes,int faceCount,int rayTubeCount,TriangleRayTube *reflectedRayTubes,Face *face,Edge *diffractionEdge,int *faceNumber1,int *faceNumber2,int *reflectionExist,int *diffractionExist);
int GetMaxNumberOnCPU(int a,int b,int c);
int GetMinNumberOnCPU(int a,int b,int c);