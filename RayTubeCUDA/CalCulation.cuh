#pragma once
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <math.h>
#include "Face.h"
#include "BasicTool.cuh"
using namespace std;
#define PI 3.1415926

//��λ������
__device__ Point C_UnitizeVectorOnGPU(Point vector);
//������֮�����
__device__ float CalculateDistanceOfBipartiteOnGPU(Point p1, Point p2);
//��������ģ
__device__ float Mag(Point vector);
//����
__device__ float DotMultiply(Point vector1, Point vector2);
//����
__device__ Point CrossMultiply(Point vector1, Point vector2);
//���ƽ�淨����
__device__ Point GetNormalVector(Face face);
//��ƽ�淽��Ax+By+Cz+D=0���ĸ�ϵ��A,B,C,D
__device__ float* GetFaceEquationFactor(Face face);
//�жϵ��Ƿ���ƽ����
__device__ bool JudgeIfPointInFace(Point point, Face face);
//�ж����������Ƿ�ƽ����ͬ��
__device__ bool IsParallelAndSamedirection(Point vector1, Point vector2);
//�õ�һ����ķ���������p1Ϊ�µ㣬p2Ϊ�ϵ㣬p1ָ��p2��
__device__ Point GetDirectionVectorOfEdge(Edge edge);
//�ж��������Ƿ�Ϊͬһ��
__device__ bool IsTwoPointSame(Point A,Point B);
//�ж�һ�����Ƿ�Ϊһ��������Ķ���֮һ
__device__ bool IsTheSamePointOfAdjacentExist(Point A,Face face);
//�õ�������������Ĺ�����
__device__ Edge GetEdgeOfAdjacentFaces(Face face1,Face face2);
//�����ⷢ��㵽��������¹�����������
__device__ Ray CreateNewInRay(Point originPoint, Point diffractionPoint);
//������ֱ�ߵļн� �����������ڻ�ȡ���������ߵļн�
__device__ float GetAngleOfTwoStraightLines(Point vector1, Point vector2);
//������Բ��Բ�� ��������������������ѡһ������������ͬ���Ҳ��������ĵ�,һ��ѡ�������ĳ������,���н�Ϊ90��ʱ����������ΪԲ�̣�Բ��Ϊ�����
__device__ Point GetCircleCenterPoint(float angleOfRayAndSameLine,Ray incidentRay, Edge sameEdge, Point diffractionPoint);
//������Բ�İ뾶
__device__ float GetCircleRadius(float angleOfRayAndSameLine, Point diffractionPoint, Point circleCenterPoint);
//��Բ����ƽ���ϵ�һ������(����) �������Ϊ��������������е�����һ����
__device__ Point GetVectorInThePlaneOfCircle(Edge sameEdge, Point circleCenterPoint, Face oneAdjacentTriangle);
//����Բ��ƽ���ϵ�һ����ƽ�淨������ƽ��ĳ������������������
__device__ Point GetAnotherVectorInThePlaneOfCircle(Edge sameEdge, Point vectorU);
//��Բ��������Ľ��㣨���䣩
__device__ Point GetCrossPointOfCircleWithTer(Face terTri, Point centerPoint, float radius, Point vectorU, Point vectorV, Point triPoint);
//������Բ����������ķֲ���������ϵ
__device__ void SetUnitVectorVnCirclePlane(Point &unitVectorU, Point &unitVectorV, Point circleCenterPoint,
	Point crossPointOfFace0AndCircle, Point crossPointOfFace1AndCircle, Point verticalVector, float angleOfTriangles);
//��������ĽǶ���Բ�ϵ�һ�� arcAngle�Ի����Ʊ�ʾ
__device__ Point GetPointInCircumference(Point circleCenterPoint, float circleRadius, Point unitVectorU, Point unitVectorV, float arcAngle);
//��ĳһ���ǶȻ��ֻ������Բ�ϵĵ�
__device__ Point *GetcircumPointOfTheCircle(Edge sameEdge, Point circleCenterPoint, float circleRadius, 
	Point unitCircleVectorU, Point unitCircleVectorV, float circleRange, int circumPointCount);
//�����������ߵĺ˺��� diffractionEdge�������p1�������p2֮��Ĳ��� diffractionRayCount��ʾÿ������������������ߵ�����
__global__ void GenerateDiffractionRays(TriangleRayTube *incidentRayTubes, Edge *diffractionEdge, Face *diffractionFace1, Face *diffractionFace2, 
	Ray *diffractionRays1, Ray *diffractionRays2, QuadrangleRayTube *diffractionRayTubes, unsigned const int diffractionRayCount);
//diffractionRayCount��ʾÿ������������������ߵ����� RaysToBeDiffractedCount��ʾ�������ߵ�������originPoint�ĸ�����
cudaError_t GetDiffractionRaysInGPU(TriangleRayTube *incidentRayTubes, Edge *diffractionEdge, Face *diffractionFace1, Face *diffractionFace2, 
	Ray *diffractionRays1, Ray *diffractionRays2, QuadrangleRayTube *diffractionRayTubes,
	unsigned const int diffractionRayCount, unsigned const int RaysToBeDiffractedCount);