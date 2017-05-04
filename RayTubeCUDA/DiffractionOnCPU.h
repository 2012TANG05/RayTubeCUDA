#pragma once
#include <math.h>
#include "Face.h"
#include "BasicTool.cuh"
using namespace std;
#define PI 3.1415926

void GenerateDiffractionRaysOnCPU(TriangleRayTube *incidentRayTubes, Edge *diffractionEdge, 
	Face *diffractionFace1, Face *diffractionFace2, QuadrangleRayTube *diffractionRayTubes, unsigned const int diffractionRayCount,
	unsigned const int raysToBeDiffractedCnt);