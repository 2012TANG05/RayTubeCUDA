#include "Point.h"

CPoint::CPoint(float x,float y,float z)
{
	this->x=x;
	this->y=y;
	this->z=z;
}

float CPoint::GetX()
{
	return x;
}

float CPoint::GetY()
{
	return y;
}

float CPoint::GetZ()
{
	return z;
}

void CPoint::SetX(float x)
{
	this->x=x;
}

void CPoint::SetY(float y)
{
	this->y=y;
}

void CPoint::SetZ(float z)
{
	this->z=z;
}