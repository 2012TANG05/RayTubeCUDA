#include "BuildingReader.h"


BuildingReader::BuildingReader(string buildingPath)
{
	this->buildingPath=buildingPath;
}


void BuildingReader::Read()
{
	ifstream confBuilding(buildingPath.c_str());
	if(!confBuilding)
	{
		printf("Open building file \"%s\" failed.", buildingPath.c_str());
		throw "Open building file failed!";
	}
	else
	{
		string line;
		Building currentBuilding;
		while(getline(confBuilding,line))
		{
			vector<string> tmpLine=split(line, " ");
			if(tmpLine[0]=="begin_<building>")
			{
				currentBuilding.SetName(tmpLine[1]);
			}
			if(tmpLine[0]=="length")
			{
				currentBuilding.SetLength(strtod(tmpLine[1].c_str(),NULL));
			}
				if(tmpLine[0]=="width")
			{
				currentBuilding.SetWidth(strtod(tmpLine[1].c_str(),NULL));
			}
			if(tmpLine[0]=="height")
			{
				currentBuilding.SetHeight(strtod(tmpLine[1].c_str(),NULL));
			}
			if(tmpLine[0]=="locate_x")
			{
				currentBuilding.SetLocate_X(strtod(tmpLine[1].c_str(),NULL));
			}
			if(tmpLine[0]=="locate_y")
			{
				currentBuilding.SetLocate_Y(strtod(tmpLine[1].c_str(),NULL));
			}
			if(tmpLine[0]=="locate_z")
			{
				currentBuilding.SetLocate_Z(strtod(tmpLine[1].c_str(),NULL));
			}
			if(tmpLine[0]=="end_<building>")
			{
				buildings.push_back(currentBuilding);
			}
		}
	}
}

vector<Building> BuildingReader::GetBuildings()
{
	Read();
	return buildings;
}