#include <math.h>
#include "Face.h"
#include "BasicToolOnCPU.h"
#include <iostream>
using namespace std;
#define PI 3.1415926

//float CPURWTC_CalculateDistanceOfBipartite(Point a,Point b);
//void CPURWTC_UnitizeVector(float *a, float *b, float *c);

//计算反射射线方向向量
//Point CPURWTC_GetReflectedVector(Point d,Point n);

//计算反射
void CPURWTC_CalculatetempReflectedRays(Face *face, Ray incidentray, 
	Ray *tempReflectedRay, int faceCountPerBlock, int *identifier, float *tempDistance, 
	int distanceNumber);

//CPU中计算反射射线管
void CPUCalculateTempTriangleReflectedRayTubes(Face *face, 
	TriangleRayTube *incidentrayTubes, TriangleRayTube *ReflectedRayTubes, 
	int faceCountPerBlock, int *identifier, float *tempDistance);


int CPUCalculateTriangleReflectedRayTubes(const unsigned int rayTubeCount,
	unsigned int faceCount, Face *p_face, TriangleRayTube *incidentRayTubes,
	TriangleRayTube *ReflectedRayTubes,int *isReflectedRaysExist,float *distance);

void CPUCalculateTempQuadrangleReflectedRayTubes(Face *face, 
	QuadrangleRayTube *incidentrayTubes, QuadrangleRayTube *tempReflectedRayTubes, 
	int faceCountPerBlock, int *identifier, float *tempDistance);

int CPUCalculateQuadrangleReflectedRayTubes(const unsigned int rayTubeCount, 
	unsigned int faceCount, Face *p_face, QuadrangleRayTube *incidentRayTubes, 
	QuadrangleRayTube *ReflectedRayTubes, int *isReflectedRayTubesExist, float *distance);
