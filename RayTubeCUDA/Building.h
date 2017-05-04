#pragma once
#include <string>
using namespace std;

class Building
{
private:
	string name;
	float locate_x;//这个中心点为长方体底面的中心点
	float locate_y;
	float locate_z;
	float length;//x轴方向
	float width;//y轴方向
	float height;//z轴方向
public:
	float GetLocate_X();
	float GetLocate_Y();
	float GetLocate_Z();
	float GetLength();
	float GetWidth();
	float GetHeight();
	string GetName();
	void SetName(string Name);
	void SetLocate_X(float locate_X);
	void SetLocate_Y(float locate_Y);
	void SetLocate_Z(float locate_Z);
	void SetLength(float length);
	void SetWidth(float width);
	void SetHeight(float height);
};