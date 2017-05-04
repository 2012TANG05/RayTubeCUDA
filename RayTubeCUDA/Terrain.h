#pragma once
#include <vector>
#include "Face.h"
#include "StringTool.h"
#include <fstream>
using namespace std;

class Terrain
{
private:
	vector<CFace> faces;
	int faceCnt;
	CPoint GetOnePointOnTerrain(string line);
public:
	Terrain(string pathTer);
	int GetFaceCount();
	vector<CFace> GetFaces();
};