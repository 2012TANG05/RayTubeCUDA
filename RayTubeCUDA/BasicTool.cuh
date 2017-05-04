#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

using namespace std;
#define PI 3.1415926

__device__ __host__ struct Point//�����
{
	float x;
	float y;
	float z;
};

__device__ __host__ struct Face//����������
{
	Point A;
	Point B;
	Point C;
};

__device__ __host__ struct Ray//��������
{
	Point originalPoint;
	Point direction;
};

__device__ __host__ struct PathNode//·���ڵ�
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

__device__ __host__ struct TriangleRayTube//�������������߹�
{
	Ray ray1;
	Ray ray2;
	Ray ray3;
	Path path;
};

__device__ __host__ struct QuadrangleRayTube //�����ı������߹�
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


double AngleToRad(double angle);//�Ƕȱ仡��
float CalculateDistanceOfBipartite(Point a,Point b);//��������֮��ľ���
void UitizeVector(float *a,float *b,float *c);//��λ������
Point GetReflectedVector(Point d,Point n);//��������
Ray CalculateReflectedRayOnCPU(Ray incidentRay,Face *face,int faceCount,int *reflectedFace,int *flag);//���㷴������
Ray CalculateVirtualReflectedRayOnCPU(Ray incidentRay,Face face);
Ray CalculateReflectedRayWithTerrainOnCPU(Ray incidentRay,Face *face,int faceCount,int *reflectedFace,int *flag);
Point GetIntersectionOfRays(Ray ray1,Ray ray2);//����������ߵĽ���
bool JudgePointEqual(Point a, Point b);//�ж��������Ƿ����
void ExchangeTwoRay(Ray *ray1, Ray *ray2);//������������
