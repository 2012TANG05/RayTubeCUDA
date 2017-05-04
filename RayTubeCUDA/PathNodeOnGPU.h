#pragma once
#include <stdio.h>
#include <iostream>
#include "Terrain.h"
#include "BuildingReader.h"
#include "BuildingTriangulation.h"
#include "ObjectToArray.h"
#include "ReflectionAndDiffractionWithBuildingCalculationOnGPU.cuh"
#include "ReflectionWithBuildingCalculationOnGPU.cuh"
#include "ReflectionWithTerrainCalculationOnGPU.cuh"
#include "BasicTool.cuh"
#include "Transmitter.h"
#include <string>
#include <fstream>
#include <math.h>
#include "Transition.h"
#include "Calculation.cuh"
#include "DiffractionOnCPU.h"
#include "IsRaytubesReceivedOnGPU.cuh"
;
using namespace std;

void ReceiveTriangleRayTubes(TriangleRayTube *rayTubes, TriangleRayTube *&receivedRayTubes, Point receiver, 
	const int rayTubeCount, int &receivedRayTubeCnt);

void ReceiveQuadrangleRayTubes(QuadrangleRayTube *rayTubes, QuadrangleRayTube *&receivedRayTubes, Point receiver, 
	const int rayTubeCount, int &receivedRayTubeCnt);

void BuildingReflectionAndJudgeDiffraction(TriangleRayTube *incidentRayTubes, TriangleRayTube *&reflectedRayTubes, 
	const int incidentRayTubeCnt, int &reflectedRayTubeCnt, int &diffractingRayTubeCnt, const int buildingFaceCnt, Face *buildingFaces, 
	Edge *&diffractionEdge, Face *&face1, Face *&face2, TriangleRayTube *&incidentDiffractingRayTubes);

void BuildingDiffraction(TriangleRayTube *incidentRayTubes, Edge *&diffractionEdge, Face *&diffractionFace1, Face *&diffractionFace2,
	QuadrangleRayTube *&diffractionRayTubes, unsigned const int diffractionRayCnt, unsigned const int RaysToBeDiffractedCnt);

void BuildingReflection_Qua(QuadrangleRayTube *incidentRayTubes, const int buildingFaceCnt, const int RayTubeCnt, int &reflectedRayTubeCnt,
	Face *buildingFaces, QuadrangleRayTube *&reflectedRayTubes);

void TerReflection_Tri(TriangleRayTube *incidentRayTubes, TriangleRayTube *&reflectedRayTubes, Face *faces,
	const int incidentRayTubeCnt, const int faceCnt, int &reflectedRayTubeCnt);

void TerReflection_Qua(QuadrangleRayTube *incidentRayTubes, QuadrangleRayTube *&reflectedRayTubes, Face *faces,
	const int incidentRayTubeCnt, const int faceCnt, int &reflectedRayTubeCnt);