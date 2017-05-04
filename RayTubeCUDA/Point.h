#pragma once

class CPoint
{
	private:
		float x;
		float y;
		float z;
	public:
		CPoint(float x=0,float y=0,float z=0);
		float GetX();
		float GetY();
		float GetZ();
		void SetX(float x);
		void SetY(float y);
		void SetZ(float z);
};