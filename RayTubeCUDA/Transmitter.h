#pragma once
#include <fstream>
#include <iostream>
#include "BasicTool.cuh"
#include "StringTool.h"
#include "Point.h"

using namespace std;

void GenerateOriginalRays(Point txPosition,TriangleRayTube *originalRayTubes,int tessellationFrequency,float height);
void CalculateAllVertexesOfIcosahedron(Point centerPoint,Point *vertexes);
TriangleRayTube GetTriangleRayTube(Point vertex1,Point vertex2,Point vertex3,Point txPosition);

void GetVertexesOnEdge(Point vertex1,Point vertex2,int tessellationFrequency,Point *vertexes);
void GetVertexesOnEachLine(Point startVertex,Point direction,int tessellationFrequency,int floor,Point *vertexes);
void GetAllVertexesOnSigleFace(Point vertex1,Point vertex2,Point vertex3,Point txPosition,int tessellationFrequency,Point *vertexes);

void GetAllRayTubesOnSingleFace(TriangleRayTube **sp_rayTubes,Point vertex1,Point vertex2,Point vertex3,Point txPosition,int tessellationFrequency);
void GetRayTubesOnEachFloor(TriangleRayTube *p_rayTubes,Point *vertexes,int floor,Point txPosition);

class Transmitter
{
private:
	Point txPosition;
	char *txFilePath;
public:
	Transmitter(char *txFilePath);
	Point GetPosition();
	void SetPosition(Point position);
	float GetHeight();
};