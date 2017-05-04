#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
;
using namespace std;
#define PI 3.1415926;

struct Point//�����
{
	float x;
	float y;
	float z;
};

struct Face//����������
{
	Point A;
	Point B;
	Point C;
};

struct Ray//��������
{
	Point originalPoint;
	Point direction;
};

struct TriangleRayTube//�������������߹�
{
	Ray ray1;
	Ray ray2;
	Ray ray3;

};

struct QuadrangleRayTube//�����ı������߹�
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