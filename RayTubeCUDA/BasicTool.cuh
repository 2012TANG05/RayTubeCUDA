#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

using namespace std;
#define PI 3.1415926

__device__ __host__ struct Point//定义点
{
	float x;
	float y;
	float z;
};

__device__ __host__ struct Face//定义三角面
{
	Point A;
	Point B;
	Point C;
};

__device__ __host__ struct Ray//定义射线
{
	Point originalPoint;
	Point direction;
};

__device__ __host__ struct PathNode//路径节点
{
	Point point1;
	Point point2;
	Point point3;
};

__device__ __host__ struct Path
{
	PathNode node1;
	PathNode node2;
	PathNode node3;
	int nodeLevel;
};

__device__ __host__ struct TriangleRayTube//定义三角形射线管
{
	Ray ray1;
	Ray ray2;
	Ray ray3;
	Path path;
};

__device__ __host__ struct QuadrangleRayTube //定义四边形射线管
{
	Ray ray1;
	Ray ray2;
	Ray ray3;
	Ray ray4;
	Path path;
};

__device__ __host__ struct Edge
{
	Point p1;
	Point p2;
};


double AngleToRad(double angle);//角度变弧度
float CalculateDistanceOfBipartite(Point a,Point b);//计算两点之间的距离
void UitizeVector(float *a,float *b,float *c);//单位化向量
Point GetReflectedVector(Point d,Point n);//求反射向量
Ray CalculateReflectedRayOnCPU(Ray incidentRay,Face *face,int faceCount,int *reflectedFace,int *flag);//计算反射射线
Ray CalculateVirtualReflectedRayOnCPU(Ray incidentRay,Face face);
Ray CalculateReflectedRayWithTerrainOnCPU(Ray incidentRay,Face *face,int faceCount,int *reflectedFace,int *flag);
Point GetIntersectionOfRays(Ray ray1,Ray ray2);//获得两个射线的交点
bool JudgePointEqual(Point a, Point b);//判断两个点是否相等
void ExchangeTwoRay(Ray *ray1, Ray *ray2);//交换两条射线
