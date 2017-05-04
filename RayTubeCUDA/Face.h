#pragma once
#include "Point.h"
#include <stdio.h>
class CFace
{
private:
	CPoint A;
	CPoint B;
	CPoint C;
public:
	CFace(CPoint A=CPoint(0,0,0), CPoint B=CPoint(0,0,0), CPoint C=CPoint(0,0,0));
	CPoint GetA();
	CPoint GetB();
	CPoint GetC();
	void SetA(CPoint A);
	void SetB(CPoint B);
	void SetC(CPoint C);
	void Print();
};