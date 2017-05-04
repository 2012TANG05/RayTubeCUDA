#pragma once
#include "Building.h"
#include <vector>
#include "Face.h"
using namespace std;

class BuildingTriangulation
{
private:
	vector<CFace> buildingFaces;
public:
	BuildingTriangulation(vector<Building> buildings);
	vector<CFace> GetBuildingFaces();
};