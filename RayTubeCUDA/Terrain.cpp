#include "Terrain.h"


//��������<CFace>��ǩ�µ�һ����
CPoint Terrain::GetOnePointOnTerrain(string line)
{
	float x,y,z;
	int tmpCnt=0;
	vector<string> tmpRet;
	tmpRet=split(line," ");
	for(vector<string>::iterator it=tmpRet.begin(); it<tmpRet.end(); it++)
	{
		if(tmpCnt==0)
		{
			x=atof(it->c_str());
		}
		if(tmpCnt==1)
		{
			y=atof(it->c_str());
		}
		if(tmpCnt==2)
		{
			z=atof(it->c_str());
			
		}
		tmpCnt++;
	}
	return CPoint(x,y,z);
}

Terrain::Terrain(string pathTer)
{
	ifstream confTer(pathTer.c_str());
	if(!confTer)
	{
		printf("Open terrain file \"%s\" failed.", pathTer.c_str());
		throw "Open terrain file failed!";
	}
	else
	{
		string line;
		int position;
		int flag=0;//��ʾ��ter�ļ���һ��<CFace>��ǩ�У���ǰ��������
		CFace tmpFace;
		while(getline(confTer,line))
		{
			position=line.find("nVertices");
			if(position!=line.npos)
			{
				flag=1;
				continue;
			}
			if(flag>0)
			{
				if(flag==1)
				{
					CPoint pointA=GetOnePointOnTerrain(line);
					tmpFace.SetA(pointA);
				}
				if(flag==2)
				{
					CPoint pointB=GetOnePointOnTerrain(line);
					tmpFace.SetB(pointB);
				}
				if(flag==3)
				{
					CPoint pointC=GetOnePointOnTerrain(line);
					tmpFace.SetC(pointC);
				}
				flag++;
				if(flag==4)
				{
					flag=0;
					faces.push_back(tmpFace);
				}
			}
		}
		faceCnt=(int)faces.size();	
		printf("�������ȡ��ϣ� ������һ���� %d ��������,", faceCnt);			
	}
}

int Terrain::GetFaceCount()
{
	return faceCnt;
}

vector<CFace> Terrain::GetFaces()
{
	return faces;
}