#pragma once
#include <stdio.h>
#include <iostream>
#include "Terrain.h"
#include "BuildingReader.h"
#include "BuildingTriangulation.h"
#include "ObjectToArray.h"
#include "ReflectionAndDiffractionCalculation.h"
#include "ReflectionWithBuildingCalculation.h"
#include "ReflectionWithTerrainCalculation.h"
#include "BasicTool.cuh"
#include "Transmitter.h"
#include <string>
#include <fstream>
#include <math.h>
#include "Transition.h"
#include "DiffractionOnCPU.h"
#include "IsRaytubesReceived.h"
;
using namespace std;

void ReceiveTriangleRayTubesOnCPU(TriangleRayTube *rayTubes, TriangleRayTube *&receivedRayTubes, Point receiver, 
	const int rayTubeCount, int &receivedRayTubeCnt);

void ReceiveQuadrangleRayTubesOnCPU(QuadrangleRayTube *rayTubes, QuadrangleRayTube *&receivedRayTubes, Point receiver, 
	const int rayTubeCount, int &receivedRayTubeCnt);

void BuildingReflectionAndJudgeDiffractionOnCPU(TriangleRayTube *incidentRayTubes, TriangleRayTube *&reflectedRayTubes, 
	const int incidentRayTubeCnt, int &reflectedRayTubeCnt, int &diffractingRayTubeCnt, const int buildingFaceCnt, Face *buildingFaces, 
	Edge *&diffractionEdge, Face *&face1, Face *&face2, TriangleRayTube *&incidentDiffractingRayTubes);

void BuildingDiffractionOnCPU(TriangleRayTube *incidentRayTubes, Edge *&diffractionEdge, Face *&diffractionFace1, Face *&diffractionFace2,
	QuadrangleRayTube *&diffractionRayTubes, unsigned const int diffractionRayCnt, unsigned const int RaysToBeDiffractedCnt);

void BuildingReflection_QuaOnCPU(QuadrangleRayTube *incidentRayTubes, const int buildingFaceCnt, const int RayTubeCnt, int &reflectedRayTubeCnt,
	Face *buildingFaces, QuadrangleRayTube *&reflectedRayTubes);

void TerReflection_TriOnCPU(TriangleRayTube *incidentRayTubes, TriangleRayTube *&reflectedRayTubes, Face *faces,
	const int incidentRayTubeCnt, const int faceCnt, int &reflectedRayTubeCnt);

void TerReflection_QuaOnCPU(QuadrangleRayTube *incidentRayTubes, QuadrangleRayTube *&reflectedRayTubes, Face *faces,
	const int incidentRayTubeCnt, const int faceCnt, int &reflectedRayTubeCnt);