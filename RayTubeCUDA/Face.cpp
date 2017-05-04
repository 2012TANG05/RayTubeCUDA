#include "Face.h"

CFace::CFace(CPoint A, CPoint B, CPoint C)
{
	this->A=A;
	this->B=B;
	this->C=C;
};

CPoint CFace::GetA()
{
	return A;
}

CPoint CFace::GetB()
{
	return B;
}

CPoint CFace::GetC()
{
	return C;
}

void CFace::SetA(CPoint A)
{
	this->A=A;
}

void CFace::SetB(CPoint B)
{
	this->B=B;
}

void CFace::SetC(CPoint C)
{
	this->C=C;
}

void CFace::Print()
{
	printf("PointA(%.3f,%.3f,%.3f);\nPointB(%.3f,%.3f,%.3f);\nPointC(%.3f,%.3f,%.3f).\n", 
	A.GetX(), A.GetY(), A.GetZ(), 
	B.GetX(), B.GetY(), B.GetZ(), 
	C.GetX(), C.GetY(), C.GetZ());
}

