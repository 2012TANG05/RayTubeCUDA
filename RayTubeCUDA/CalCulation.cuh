#pragma once
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <math.h>
#include "Face.h"
#include "BasicTool.cuh"
using namespace std;
#define PI 3.1415926

//单位化向量
__device__ Point C_UnitizeVectorOnGPU(Point vector);
//求两点之间距离
__device__ float CalculateDistanceOfBipartiteOnGPU(Point p1, Point p2);
//求向量的模
__device__ float Mag(Point vector);
//求点乘
__device__ float DotMultiply(Point vector1, Point vector2);
//求叉乘
__device__ Point CrossMultiply(Point vector1, Point vector2);
//获得平面法向量
__device__ Point GetNormalVector(Face face);
//求平面方程Ax+By+Cz+D=0的四个系数A,B,C,D
__device__ float* GetFaceEquationFactor(Face face);
//判断点是否在平面内
__device__ bool JudgeIfPointInFace(Point point, Face face);
//判断两个向量是否平行且同向
__device__ bool IsParallelAndSamedirection(Point vector1, Point vector2);
//得到一条棱的方向向量（p1为下点，p2为上点，p1指向p2）
__device__ Point GetDirectionVectorOfEdge(Edge edge);
//判断两个点是否为同一点
__device__ bool IsTwoPointSame(Point A,Point B);
//判断一个点是否为一个三角面的顶点之一
__device__ bool IsTheSamePointOfAdjacentExist(Point A,Face face);
//得到相邻两三角面的公共棱
__device__ Edge GetEdgeOfAdjacentFaces(Face face1,Face face2);
//从虚拟发射点到绕射点重新构造入射射线
__device__ Ray CreateNewInRay(Point originPoint, Point diffractionPoint);
//求两条直线的夹角 在绕射中用于获取射线与劈边的夹角
__device__ float GetAngleOfTwoStraightLines(Point vector1, Point vector2);
//求所设圆的圆心 在绕射中用于在劈边上选一个与绕射射线同向且不是绕射点的点,一般选公共棱的某个顶点,当夹角为90度时，绕射射线为圆盘，圆心为绕射点
__device__ Point GetCircleCenterPoint(float angleOfRayAndSameLine,Ray incidentRay, Edge sameEdge, Point diffractionPoint);
//求所设圆的半径
__device__ float GetCircleRadius(float angleOfRayAndSameLine, Point diffractionPoint, Point circleCenterPoint);
//求圆所在平面上的一个向量(绕射) 输入的面为组成劈的两个面中的任意一个面
__device__ Point GetVectorInThePlaneOfCircle(Edge sameEdge, Point circleCenterPoint, Face oneAdjacentTriangle);
//求在圆的平面上的一个与平面法向量，平面某个向量都正交的向量
__device__ Point GetAnotherVectorInThePlaneOfCircle(Edge sameEdge, Point vectorU);
//求圆与三角面的交点（绕射）
__device__ Point GetCrossPointOfCircleWithTer(Face terTri, Point centerPoint, float radius, Point vectorU, Point vectorV, Point triPoint);
//根据在圆盘上绕射面的分布设置坐标系
__device__ void SetUnitVectorVnCirclePlane(Point &unitVectorU, Point &unitVectorV, Point circleCenterPoint,
	Point crossPointOfFace0AndCircle, Point crossPointOfFace1AndCircle, Point verticalVector, float angleOfTriangles);
//根据输入的角度求圆上的一点 arcAngle以弧度制表示
__device__ Point GetPointInCircumference(Point circleCenterPoint, float circleRadius, Point unitVectorU, Point unitVectorV, float arcAngle);
//以某一个角度划分获得所设圆上的点
__device__ Point *GetcircumPointOfTheCircle(Edge sameEdge, Point circleCenterPoint, float circleRadius, 
	Point unitCircleVectorU, Point unitCircleVectorV, float circleRange, int circumPointCount);
//生成绕射射线的核函数 diffractionEdge是绕射点p1和绕射点p2之间的部分 diffractionRayCount表示每个绕射点生成绕射射线的条数
__global__ void GenerateDiffractionRays(TriangleRayTube *incidentRayTubes, Edge *diffractionEdge, Face *diffractionFace1, Face *diffractionFace2, 
	Ray *diffractionRays1, Ray *diffractionRays2, QuadrangleRayTube *diffractionRayTubes, unsigned const int diffractionRayCount);
//diffractionRayCount表示每个绕射点生成绕射射线的条数 RaysToBeDiffractedCount表示入射射线的条数（originPoint的个数）
cudaError_t GetDiffractionRaysInGPU(TriangleRayTube *incidentRayTubes, Edge *diffractionEdge, Face *diffractionFace1, Face *diffractionFace2, 
	Ray *diffractionRays1, Ray *diffractionRays2, QuadrangleRayTube *diffractionRayTubes,
	unsigned const int diffractionRayCount, unsigned const int RaysToBeDiffractedCount);