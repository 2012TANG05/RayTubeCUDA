#include "BasicTool.h"


double AngleToRad(double angle)//�Ƕ�ת����
{
	double temp;
	temp=angle*PI;
	return temp/180;
}

float CalculateDistanceOfBipartite(Point a,Point b)//������������
{
	return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y)+(a.z-b.z)*(a.z-b.z));
}