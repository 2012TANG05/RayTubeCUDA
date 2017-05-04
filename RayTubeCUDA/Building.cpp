#include "Building.h"

float Building::GetLocate_X()
{
	return locate_x;
}

float Building::GetLocate_Y()
{
	return locate_y;
}

float Building::GetLocate_Z()
{
	return locate_z;
}

float Building::GetLength()
{
	return length;
}

float Building::GetWidth()
{
	return width;
}

float Building::GetHeight()
{
	return height;
}

string Building::GetName()
{
	return name;
}

void Building::SetName(string name)
{
	this->name=name;
}

void Building::SetLength(float length)
{
	this->length=length;
}

void Building::SetWidth(float width)
{
	this->width=width;
}

void Building::SetHeight(float height)
{
	this->height=height;
}

void Building::SetLocate_X(float locate_x)
{
	this->locate_x=locate_x;
}

void Building::SetLocate_Y(float locate_y)
{
	this->locate_y=locate_y;
}

void Building::SetLocate_Z(float locate_z)
{
	this->locate_z=locate_z;
}