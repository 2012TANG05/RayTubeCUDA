#pragma once
#include <vector>
#include "Building.h"
#include <fstream>
#include "StringTool.h"
using namespace std;

class BuildingReader
{
private:
	string buildingPath;
	vector<Building> buildings;
	void Read();
public:
	BuildingReader(string buildingPath);
	vector<Building> GetBuildings();
};