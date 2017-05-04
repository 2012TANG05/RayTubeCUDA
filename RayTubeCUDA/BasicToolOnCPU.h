#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include "BasicTool.cuh"
;
using namespace std;
#define PI 3.1415926;

struct HPoint//定义点
{
	float x;
	float y;
	float z;
};

struct HFace//定义三角面
{
	HPoint A;
	HPoint B;
	HPoint C;
};

struct HRay//定义射线
{
	HPoint originalPoint;
	HPoint direction;
};

struct HPathNode//路径节点
{
	HPoint point1;
	HPoint point2;
	HPoint point3;
};

struct HPath
{
	HPathNode node1;
	HPathNode node2;
	HPathNode node3;
	int nodeLevel;
};

struct HTriangleRayTube//定义三角形射线管
{
	HRay ray1;
	HRay ray2;
	HRay ray3;
	HPath path;
};

struct HQuadrangleRayTube//定义四边形射线管
{
	HRay ray1;
	HRay ray2;
	HRay ray3;
	HRay ray4;
	HPath path;
};

struct HEdge
{
	HPoint p1;
	HPoint p2;
};

double AngleToRad(double angle);
float CalculateDistanceOfBipartite(HPoint a,HPoint b);
void UitizeVector(float *a,float *b,float *c);
Point GetReflectedVector(Point d,Point n);
Ray CalculateReflectedRayOnCPU(Ray incidentRay,Face *face,int faceCount,int *reflectedFace,int *flag);
Ray CalculateVirtualReflectedRayOnCPU(Ray incidentRay,Face face);
Ray CalculateReflectedRayWithTerrainOnCPU(Ray incidentRay,Face *face,int faceCount,int *reflectedFace,int *flag);
Point GetIntersectionOfRays(Ray ray1,Ray ray2);