#include "Transition.h"

//���Ӵ�С����
void SortTwoNumbers(int &a,int &b)
{
	if (a>b)
	{
		return;
	}
	else
	{
		int c=0;
		c=a;
		a=b;
		b=c;
	}
}

//�õ������ɵķ������߹ܸ���
int GetNewReflectedRayTubesCount(int *isReflectionExist,int reflectedRayTubesCount)
{
	int newReflectedRayTubesCount=0;
	for (int i=0;i<reflectedRayTubesCount;i++)
	{
		if (isReflectionExist[i]==1)
		{
			newReflectedRayTubesCount++;
		}
		
	}
	return newReflectedRayTubesCount;
}

//�õ�����������������
int GetDiffractionEdgeCount(int *isDiffractionExsit,int incidentRayTubesCount,int *faceNumber1,int *faceNumber2)
{
	int diffractionExistIncidentRayTubesCount=0;
	for (int i=0;i<incidentRayTubesCount;i++)
	{
		if (isDiffractionExsit[i]==1&&faceNumber1[i]!=faceNumber2[i]&&abs(faceNumber1[i]-faceNumber2[i])<4)
		{
			diffractionExistIncidentRayTubesCount++;
		}
	}
	return diffractionExistIncidentRayTubesCount;
}

//�õ����յ������߹�����
int GetReceivedRayTubeCount(bool *isReceived, int receivedRayTubeCount)
{
	int Cnt = 0;
	for (int i=0;i<receivedRayTubeCount;i++)
	{
		if (isReceived[i])
		{
			Cnt++;
		}
	}
	return Cnt;
}

void FilterReceivedRayTubes_Tri(TriangleRayTube *receivedRayTubes, TriangleRayTube *newReceivedRayTubes, int receivedRayTubeCount, int newReceivedRayTubesCount, bool *isReceived)
{
	for(int i = 0, j = 0; i < receivedRayTubeCount; i++)
	{
		if(isReceived[i])
		{
			newReceivedRayTubes[j]=receivedRayTubes[i];
			j++;
		}
	}
}

void FilterReceivedRayTubes_Qua(QuadrangleRayTube *receivedRayTubes, QuadrangleRayTube *newReceivedRayTubes, int receivedRayTubeCount, int newReceivedRayTubesCount, bool *isReceived)
{
	for(int i = 0, j = 0; i < receivedRayTubeCount; i++)
	{
		if(isReceived[i])
		{
			newReceivedRayTubes[j]=receivedRayTubes[i];
			j++;
		}
	}
}

void FilterReflectedRayTubes(TriangleRayTube *reflectedRayTubes,TriangleRayTube *newReflectedRayTubes,int newReflectedRayTubesCount,int reflectedRayTubesCount,int *isReflectionExist)
{
	for (int i=0,j=0;i<reflectedRayTubesCount;i++)
	{
		if (isReflectionExist[i]==1)
		{
			newReflectedRayTubes[j]=reflectedRayTubes[i];
			j++;
		}
	}	
}

void FilterDiffractionExistIncidentRayTubes(TriangleRayTube *incidentRayTubes,TriangleRayTube *newIncidentRayTubes,Edge *edge,Edge *newEdge,int *faceNumber1,int *newFaceNumber1,int *faceNumber2,int *newFaceNumber2,int incidentRayTubesCount,int *isDiffractionExist)
{
	for (int i=0,j=0;i<incidentRayTubesCount;i++)
	{
		SortTwoNumbers(faceNumber1[i],faceNumber2[i]);
		if (isDiffractionExist[i]==1)
		{
			if (faceNumber1[i]-faceNumber2[i]<4&&faceNumber1[i]-faceNumber2[i]>0)
			{
				newIncidentRayTubes[j]=incidentRayTubes[i];
				if (faceNumber1[i]-faceNumber2[i]==1)
				{
					newFaceNumber1[j]=faceNumber1[i];
					newFaceNumber2[j]=faceNumber2[i];
				}
				if (faceNumber1[i]-faceNumber2[i]==2)
				{
					if (faceNumber1[i]%2==1)
					{
						newFaceNumber1[j]=faceNumber1[i]-1;
						newFaceNumber2[j]=faceNumber2[i];
					}
					else
					{
						newFaceNumber1[j]=faceNumber1[i];
						newFaceNumber2[j]=faceNumber2[i]+1;
					}
				}
				if (faceNumber1[i]-faceNumber2[i]==3)
				{
					newFaceNumber1[j]=faceNumber1[i]-1;
					newFaceNumber2[j]=faceNumber2[i]+1;
				}
				newEdge[j]=edge[i];
				j++;
			}
			else
			{
				cout<<"��������������"<<endl;
				break;
			}
		}
	}	
}

//facesΪ���������棻newface1��newface2��ɸѡ��������棻facenum1��������1��Ӧ������飬facenum2ͬ��faceCntΪɸѡ����������ĸ���
void FilterDiffractionFaces(Face *faces, Face *newFaces1, Face *newFaces2, int *faceNumber1, int *faceNumber2, int wedgeCnt)
{
	if(wedgeCnt==0)
	{
		return;
	}
	for(int i=0; i<wedgeCnt; i++)
	{
		newFaces1[i]=faces[faceNumber1[i]];
		newFaces2[i]=faces[faceNumber2[i]];
	}
}

void FilterReflectedRayTubes_Qua(QuadrangleRayTube *reflectedRayTubes, QuadrangleRayTube *newReflectedRayTubes, int newReflectedRayTubesCount,int reflectedRayTubesCount,int *isReflectionExist)
{
	for (int i=0,j=0;i<reflectedRayTubesCount;i++)
	{
		if (isReflectionExist[i]==1)
		{
			newReflectedRayTubes[j]=reflectedRayTubes[i];
			j++;
		}
	}
}

//-------------------
//CPU���㲿��
//-------------------
void FilterReceivedRayTubes_Tri_CPU(TriangleRayTube *receivedRayTubes, TriangleRayTube *newReceivedRayTubes, int receivedRayTubeCount, int newReceivedRayTubesCount, bool *isReceived)
{
	for(int i = 0, j = 0; i < receivedRayTubeCount; i++)
	{
		if(isReceived[i])
		{
			newReceivedRayTubes[j]=receivedRayTubes[i];
			j++;
		}
	}
}

void FilterReceivedRayTubes_Qua_CPU(QuadrangleRayTube *receivedRayTubes, QuadrangleRayTube *newReceivedRayTubes, int receivedRayTubeCount, int newReceivedRayTubesCount, bool *isReceived)
{
	for(int i = 0, j = 0; i < receivedRayTubeCount; i++)
	{
		if(isReceived[i])
		{
			newReceivedRayTubes[j]=receivedRayTubes[i];
			j++;
		}
	}
}

void FilterReflectedRayTubes_CPU(TriangleRayTube *reflectedRayTubes,TriangleRayTube *newReflectedRayTubes,int newReflectedRayTubesCount,int reflectedRayTubesCount,int *isReflectionExist)
{
	for (int i=0,j=0;i<reflectedRayTubesCount;i++)
	{
		if (isReflectionExist[i]==1)
		{
			newReflectedRayTubes[j]=reflectedRayTubes[i];
			j++;
		}
	}	
}

void FilterDiffractionExistIncidentRayTubes_CPU(TriangleRayTube *incidentRayTubes,TriangleRayTube *newIncidentRayTubes,Edge *edge,Edge *newEdge,int *faceNumber1,int *newFaceNumber1,int *faceNumber2,int *newFaceNumber2,int incidentRayTubesCount,int *isDiffractionExist)
{
	for (int i=0,j=0;i<incidentRayTubesCount;i++)
	{
		if (isDiffractionExist[i]==1)
		{
			newIncidentRayTubes[j]=incidentRayTubes[i];
			newFaceNumber1[j]=faceNumber1[i];
			newFaceNumber2[j]=faceNumber2[i];
			newEdge[j]=edge[i];
			j++;
		}
	}	
}

//facesΪ���������棻newface1��newface2��ɸѡ��������棻facenum1��������1��Ӧ������飬facenum2ͬ��faceCntΪɸѡ����������ĸ���
void FilterDiffractionFaces_CPU(Face *faces, Face *newFaces1, Face *newFaces2, int *faceNumber1, int *faceNumber2, int wedgeCnt)
{
	if(wedgeCnt==0)
	{
		return;
	}
	for(int i=0; i<wedgeCnt; i++)
	{
		newFaces1[i]=faces[faceNumber1[i]];
		newFaces2[i]=faces[faceNumber2[i]];
	}
}

void FilterReflectedRayTubes_Qua_CPU(QuadrangleRayTube *reflectedRayTubes, QuadrangleRayTube *newReflectedRayTubes, int newReflectedRayTubesCount,int reflectedRayTubesCount,int *isReflectionExist)
{
	for (int i=0,j=0;i<reflectedRayTubesCount;i++)
	{
		if (isReflectionExist[i]==1)
		{
			newReflectedRayTubes[j]=reflectedRayTubes[i];
			j++;
		}
	}
}