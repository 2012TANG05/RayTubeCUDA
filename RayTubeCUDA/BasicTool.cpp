#include "BasicTool.h"


double AngleToRad(double angle)//角度转弧度
{
	double temp;
	temp=angle*PI;
	return temp/180;
}

float CalculateDistanceOfBipartite(Point a,Point b)//计算两点间距离
{
	return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y)+(a.z-b.z)*(a.z-b.z));
}