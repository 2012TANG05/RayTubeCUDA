#pragma once
#include <fstream>
#include <iostream>
#include "BasicTool.cuh"
#include "StringTool.h"
#include "Point.h"

using namespace std;

class Receiver
{
private:
	Point rxPosition;
	char *rxFilePath;
public:
	Receiver(char *rxFilePath);
	Point GetPosition();
	void SetPosition(Point position);
	float GetHeight();
};
Point GenerateReceiver(Point tempReceiver, float height);