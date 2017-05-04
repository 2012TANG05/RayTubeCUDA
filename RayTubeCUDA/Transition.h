#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include "BasicTool.cuh"

int GetNewReflectedRayTubesCount(int *isReflectionExist,int reflectedRayTubesCount);
int GetDiffractionEdgeCount(int *isDiffractionExist,int incidentRayTubesCount,int *tmp_faceNumber1,int *tmp_faceNumber2);
int GetReceivedRayTubeCount(bool *isReceived, int receivedRayTubeCount);
void FilterReceivedRayTubes_Tri(TriangleRayTube *receivedRayTubes, TriangleRayTube *newReceivedRayTubes, int receivedRayTubeCount, int newReceivedRayTubesCount, bool *isReceived);
void FilterReceivedRayTubes_Qua(QuadrangleRayTube *receivedRayTubes, QuadrangleRayTube *newReceivedRayTubes, int receivedRayTubeCount, int newReceivedRayTubesCount, bool *isReceived);
void FilterReflectedRayTubes(TriangleRayTube *reflectedRayTubes,TriangleRayTube *newReflectedRayTubes,int newReflectedRayTubesCount,int reflectedRayTubesCount,int *isReflectionExist);
void FilterReflectedRayTubes_Qua(QuadrangleRayTube *reflectedRayTubes, QuadrangleRayTube *newReflectedRayTubes, int newReflectedRayTubesCount,int reflectedRayTubesCount,int *isReflectionExist);
void FilterDiffractionExistIncidentRayTubes(TriangleRayTube *incidentRayTubes,TriangleRayTube *newIncidentRayTubes,Edge *edge,Edge *newEdge,int *faceNumber1,int *newFaceNumber1,int *faceNumber2,int *newFaceNumber2,int incidentRayTubesCount,int *isDiffractionExist);
void FilterDiffractionFaces(Face *faces, Face *newFaces1, Face *newFaces2, int *faceNumber1, int *faceNumber2, int wedgeCnt);

void FilterReceivedRayTubes_Tri_CPU(TriangleRayTube *receivedRayTubes, TriangleRayTube *newReceivedRayTubes, int receivedRayTubeCount, int newReceivedRayTubesCount, bool *isReceived);
void FilterReceivedRayTubes_Qua_CPU(QuadrangleRayTube *receivedRayTubes, QuadrangleRayTube *newReceivedRayTubes, int receivedRayTubeCount, int newReceivedRayTubesCount, bool *isReceived);
void FilterReflectedRayTubes_CPU(TriangleRayTube *reflectedRayTubes,TriangleRayTube *newReflectedRayTubes,int newReflectedRayTubesCount,int reflectedRayTubesCount,int *isReflectionExist);
void FilterDiffractionExistIncidentRayTubes_CPU(TriangleRayTube *incidentRayTubes,TriangleRayTube *newIncidentRayTubes,Edge *edge,Edge *newEdge,int *faceNumber1,int *newFaceNumber1,int *faceNumber2,int *newFaceNumber2,int incidentRayTubesCount,int *isDiffractionExist);
void FilterDiffractionFaces_CPU(Face *faces, Face *newFaces1, Face *newFaces2, int *faceNumber1, int *faceNumber2, int wedgeCnt);
void FilterReflectedRayTubes_Qua_CPU(QuadrangleRayTube *reflectedRayTubes, QuadrangleRayTube *newReflectedRayTubes, int newReflectedRayTubesCount,int reflectedRayTubesCount,int *isReflectionExist);