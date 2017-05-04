#include "BuildingTriangulation.h"

BuildingTriangulation::BuildingTriangulation(vector<Building> buildings)
{
	int buildingCnt=buildings.size();
	for(int i=0; i<buildingCnt; i++)
	{
		//F=Front B=Behind L=Left R=Right U=Up D=Down 每三个面确定一个长方体顶点
		CPoint FLU;
		CPoint FLD;
		CPoint FRU;
		CPoint FRD;
		CPoint BLU;
		CPoint BRU;
		CPoint BLD;
		CPoint BRD;
		FLU.SetX(buildings[i].GetLocate_X()-(buildings[i].GetLength())/2);
		FLU.SetY(buildings[i].GetLocate_Y()-(buildings[i].GetWidth())/2);
		FLU.SetZ(buildings[i].GetLocate_Z()+buildings[i].GetHeight());
		FRU.SetX(buildings[i].GetLocate_X()+(buildings[i].GetLength())/2);
		FRU.SetY(buildings[i].GetLocate_Y()-(buildings[i].GetWidth())/2);
		FRU.SetZ(buildings[i].GetLocate_Z()+buildings[i].GetHeight());
		FLD.SetX(buildings[i].GetLocate_X()-(buildings[i].GetLength())/2);
		FLD.SetY(buildings[i].GetLocate_Y()-(buildings[i].GetWidth())/2);
		FLD.SetZ(buildings[i].GetLocate_Z());
		FRD.SetX(buildings[i].GetLocate_X()+(buildings[i].GetLength())/2);
		FRD.SetY(buildings[i].GetLocate_Y()-(buildings[i].GetWidth())/2);
		FRD.SetZ(buildings[i].GetLocate_Z());
		BLU.SetX(buildings[i].GetLocate_X()-(buildings[i].GetLength())/2);
		BLU.SetY(buildings[i].GetLocate_Y()+(buildings[i].GetWidth())/2);
		BLU.SetZ(buildings[i].GetLocate_Z()+buildings[i].GetHeight());
		BRU.SetX(buildings[i].GetLocate_X()+(buildings[i].GetLength())/2);
		BRU.SetY(buildings[i].GetLocate_Y()+(buildings[i].GetWidth())/2);
		BRU.SetZ(buildings[i].GetLocate_Z()+buildings[i].GetHeight());
		BLD.SetX(buildings[i].GetLocate_X()-(buildings[i].GetLength())/2);
		BLD.SetY(buildings[i].GetLocate_Y()+(buildings[i].GetWidth())/2);
		BLD.SetZ(buildings[i].GetLocate_Z());
		BRD.SetX(buildings[i].GetLocate_X()+(buildings[i].GetLength())/2);
		BRD.SetY(buildings[i].GetLocate_Y()+(buildings[i].GetWidth())/2);
		BRD.SetZ(buildings[i].GetLocate_Z());
		CFace F1=CFace(FLU,FRU,FLD);
		CFace F2=CFace(FRU,FLD,FRD);
		CFace R1=CFace(FRU,BRU,FRD);
		CFace R2=CFace(BRU,FRD,BRD);
		CFace B1=CFace(BRU,BRD,BLD);
		CFace B2=CFace(BRU,BLD,BLU);
		CFace L1=CFace(BLU,BLD,FLD);
		CFace L2=CFace(BLU,FLD,FLU);
		CFace U1=CFace(BLU,FLU,BRU);
		CFace U2=CFace(BRU,FLU,FRU);
		CFace D1=CFace(BRD,BLD,FLD);
		CFace D2=CFace(BRD,FLD,FRD);
		this->buildingFaces.push_back(F1);
		this->buildingFaces.push_back(F2);
		this->buildingFaces.push_back(R1);
		this->buildingFaces.push_back(R2);
		this->buildingFaces.push_back(B1);
		this->buildingFaces.push_back(B2);
		this->buildingFaces.push_back(L1);
		this->buildingFaces.push_back(L2);
		this->buildingFaces.push_back(U1);
		this->buildingFaces.push_back(U2);
		this->buildingFaces.push_back(D1);
		this->buildingFaces.push_back(D2);
	}
}

vector<CFace> BuildingTriangulation::GetBuildingFaces()
{
	return buildingFaces;
}