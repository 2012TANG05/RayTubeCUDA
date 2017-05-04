#include <math.h>
#include "BasicTool.cuh"
#include <iostream>
using namespace std;
#define PI 3.1415926



//CPU中计算反射射线管
void CalculateTempTriangleReflectedRayTubesOnCPU(Face *face, 
	TriangleRayTube *incidentrayTubes, TriangleRayTube *ReflectedRayTubes, 
	int faceCountPerBlock, int *identifier, float *tempDistance);


int CalculateTriangleReflectedRayTubesOnCPU(const unsigned int rayTubeCount,
	unsigned int faceCount, Face *p_face, TriangleRayTube *incidentRayTubes,
	TriangleRayTube *ReflectedRayTubes,int *isReflectedRaysExist,float *distance);




void CPUCalculateTempQuadrangleReflectedRayTubes(Face *face, 
	QuadrangleRayTube *incidentrayTubes, QuadrangleRayTube *tempReflectedRayTubes, 
	int faceCountPerBlock, int *identifier, float *tempDistance);

int CPUCalculateQuadrangleReflectedRayTubes(const unsigned int rayTubeCount, 
	unsigned int faceCount, Face *p_face, QuadrangleRayTube *incidentRayTubes, 
	QuadrangleRayTube *ReflectedRayTubes, int *isReflectedRayTubesExist, float *distance);
