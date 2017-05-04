#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
;
using namespace std;
#define PI 3.1415926;

struct Point//定义点
{
	float x;
	float y;
	float z;
};

struct Face//定义三角面
{
	Point A;
	Point B;
	Point C;
};

struct Ray//定义射线
{
	Point originalPoint;
	Point direction;
};

struct TriangleRayTube//定义三角形射线管
{
	Ray ray1;
	Ray ray2;
	Ray ray3;

};

struct QuadrangleRayTube//定义四边形射线管
{
	Ray ray1;
	Ray ray2;
	Ray ray3;
	Ray ray4;
};

struct Edge
{
	Point p1;
	Point p2;
};

double AngleToRad(double angle);
float CalculateDistanceOfBipartite(Point a,Point b);