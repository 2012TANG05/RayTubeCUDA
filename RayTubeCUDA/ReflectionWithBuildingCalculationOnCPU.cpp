#include "ReflectionWithBuildingCalculationOnCPU.h"

 bool JudgeTwoTriangleFacesOnTheSameQuadrangleOnCPU(int faceNumber1,int faceNumber2)
{
	int a=faceNumber1%12;
	int b=faceNumber2%12;
	if (faceNumber1==faceNumber2)
	{
		return true;
	} 
	if (a%2==0)
	{
		if (b==a+1)
		{
			return  true;
		}
	}
	if (a%2==1)
	{
		if (b==a-1)
		{
			return true;
		}
	}
	return false;
}

bool JudgeTwoTriangleFacesOnTheAdjacentQuadrangleOnCPU(int faceNumber1,int faceNumber2)
{
	if (abs(faceNumber1-faceNumber2)>=12)
	{
		return false;
	}
	else
	{
		int a=faceNumber1%12;
		int b=faceNumber2%12;
		if (a%2==0&&a!=0)
		{
			if (b==a-1)
			{
				return  true;
			}
		}
		if (a%2==1&&a!=7)
		{
			if (b==a+1)
			{
				return true;
			}
		}
		if ((a==0&&b==7)||(a==7&&b==0))
		{
			return true;
		}
		return false;
	}
}

//
void GetThreeRemainingNumbersOnCPU(int number,int *remainingNumber1,int *remainingNumber2,int *remainingNumber3)
{
	if (number==0)
	{
		*remainingNumber1=1;
		*remainingNumber2=2;
		*remainingNumber3=3;
	}
	if (number==1)
	{
		*remainingNumber1=0;
		*remainingNumber2=2;
		*remainingNumber3=3;
	}
	if (number==2)
	{
		*remainingNumber1=0;
		*remainingNumber2=1;
		*remainingNumber3=3;
	}
	if (number==3)
	{
		*remainingNumber1=0;
		*remainingNumber2=1;
		*remainingNumber3=2;
	}
}

void GetOneRemainingNumberOnCPU(int number1,int number2,int number3,int *remainingNumber)
{
	*remainingNumber=6-number1-number2-number3;
}


void GetReflectedRayTubesWhenTwoFlagsEqualOneOnCPU(int flagNumber1,int flagNumber2,int *reflectedFace,Ray *reflectedRays,Ray *incidentRays,QuadrangleRayTube *reflectedRayTubes,Face *face,int *reflectionExist,int i)
{
	if (JudgeTwoTriangleFacesOnTheSameQuadrangleOnCPU(reflectedFace[flagNumber1],reflectedFace[flagNumber2]))
	{
		reflectionExist[i]=1;
		reflectedRayTubes[2*i].ray1=reflectedRays[0];
		reflectedRayTubes[2*i].ray2=reflectedRays[1];
		reflectedRayTubes[2*i].ray3=reflectedRays[2];
		reflectedRayTubes[2*i].ray4=reflectedRays[3];
		reflectedRayTubes[2*i+1].ray1=reflectedRays[0];
		reflectedRayTubes[2*i+1].ray2=reflectedRays[1];
		reflectedRayTubes[2*i+1].ray3=reflectedRays[2];
		reflectedRayTubes[2*i+1].ray4=reflectedRays[3];
	}
	if (JudgeTwoTriangleFacesOnTheAdjacentQuadrangleOnCPU(reflectedFace[flagNumber1],reflectedFace[flagNumber2]))
	{
		reflectionExist[i]=1;
		int remainingNumber1,remainingNumber2,remainingNumber3;
		GetThreeRemainingNumbersOnCPU(flagNumber1,&remainingNumber1,&remainingNumber2,&remainingNumber3);
		reflectedRayTubes[2*i].ray1=reflectedRays[flagNumber1];
		reflectedRayTubes[2*i].ray2=reflectedRays[remainingNumber1];
		reflectedRayTubes[2*i].ray3=reflectedRays[remainingNumber2];
		reflectedRayTubes[2*i].ray4=reflectedRays[remainingNumber2];
		GetThreeRemainingNumbersOnCPU(flagNumber2,&remainingNumber1,&remainingNumber2,&remainingNumber3);
		reflectedRayTubes[2*i+1].ray1=reflectedRays[flagNumber2];
		reflectedRayTubes[2*i+1].ray2=reflectedRays[remainingNumber1];
		reflectedRayTubes[2*i+1].ray3=reflectedRays[remainingNumber2];
		reflectedRayTubes[2*i+1].ray4=reflectedRays[remainingNumber3];

	}	
}

void GetReflectedRayTubesWhenThreeFlagsEqualOneOnCPU(int flagNumber1,int flagNumber2,int flagNumber3,int *reflectedFace,Ray *reflectedRays,Ray *incidentRays,QuadrangleRayTube *reflectedRayTubes,QuadrangleRayTube *incidentRayTubes,Face *face,int *reflectionExist,int i)
{
	//int remainingNumber;
	//GetOneRemainingNumberOnCPU(flagNumber1,flagNumber2,flagNumber3,&remainingNumber);
	if (JudgeTwoTriangleFacesOnTheSameQuadrangleOnCPU(reflectedFace[flagNumber1],reflectedFace[flagNumber2]))
	{
		AddPathNodeToReflectedRayTubeOnCPU(incidentRayTubes,reflectedRayTubes,i,face[reflectedFace[flagNumber1]],face[reflectedFace[flagNumber3]]);
		GetReflectedRayTubesWhenTwoFlagsEqualOneOnCPU(flagNumber1,flagNumber3,reflectedFace,reflectedRays,incidentRays,reflectedRayTubes,face,reflectionExist,i);
	}
	if (JudgeTwoTriangleFacesOnTheSameQuadrangleOnCPU(reflectedFace[flagNumber1],reflectedFace[flagNumber3]))
	{
		AddPathNodeToReflectedRayTubeOnCPU(incidentRayTubes,reflectedRayTubes,i,face[reflectedFace[flagNumber2]],face[reflectedFace[flagNumber3]]);
		GetReflectedRayTubesWhenTwoFlagsEqualOneOnCPU(flagNumber2,flagNumber3,reflectedFace,reflectedRays,incidentRays,reflectedRayTubes,face,reflectionExist,i);
	}
	if (JudgeTwoTriangleFacesOnTheSameQuadrangleOnCPU(reflectedFace[flagNumber2],reflectedFace[flagNumber3]))
	{
		AddPathNodeToReflectedRayTubeOnCPU(incidentRayTubes,reflectedRayTubes,i,face[reflectedFace[flagNumber1]],face[reflectedFace[flagNumber2]]);
		GetReflectedRayTubesWhenTwoFlagsEqualOneOnCPU(flagNumber1,flagNumber2,reflectedFace,reflectedRays,incidentRays,reflectedRayTubes,face,reflectionExist,i);
	}
}

void AddPathNodeToReflectedRayTubeOnCPU(QuadrangleRayTube *incidentRayTubes,QuadrangleRayTube *reflectedRayTubes,int i,Face face1,Face face2)
{
	if(incidentRayTubes[i].path.nodeLevel=0)
	{
		reflectedRayTubes[2*i].path.nodeLevel=1;
		reflectedRayTubes[2*i].path.node1.point1=face1.A;
		reflectedRayTubes[2*i].path.node1.point2=face1.B;
		reflectedRayTubes[2*i].path.node1.point3=face1.C;
		reflectedRayTubes[2*i+1].path.nodeLevel=1;
		reflectedRayTubes[2*i+1].path.node1.point1=face2.A;
		reflectedRayTubes[2*i+1].path.node1.point2=face2.B;
		reflectedRayTubes[2*i+1].path.node1.point3=face2.C;
	}
	if(incidentRayTubes[i].path.nodeLevel=1)
	{
		reflectedRayTubes[2*i].path.node1=incidentRayTubes[i].path.node1;
		reflectedRayTubes[2*i].path.nodeLevel=2;
		reflectedRayTubes[2*i].path.node2.point1=face1.A;
		reflectedRayTubes[2*i].path.node2.point2=face1.B;
		reflectedRayTubes[2*i].path.node2.point3=face1.C;
		reflectedRayTubes[2*i+1].path.nodeLevel=2;
		reflectedRayTubes[2*i+1].path.node1=incidentRayTubes[i].path.node1;
		reflectedRayTubes[2*i+1].path.node2.point1=face2.A;
		reflectedRayTubes[2*i+1].path.node2.point2=face2.B;
		reflectedRayTubes[2*i+1].path.node2.point3=face2.C;
	}
	if(incidentRayTubes[i].path.nodeLevel=2)
	{
		reflectedRayTubes[2*i].path.node1=incidentRayTubes[i].path.node1;
		reflectedRayTubes[2*i].path.node2=incidentRayTubes[i].path.node2;
		reflectedRayTubes[2*i].path.nodeLevel=3;
		reflectedRayTubes[2*i].path.node3.point1=face1.A;
		reflectedRayTubes[2*i].path.node3.point2=face1.B;
		reflectedRayTubes[2*i].path.node3.point3=face1.C;
		reflectedRayTubes[2*i+1].path.nodeLevel=3;
		reflectedRayTubes[2*i+1].path.node1=incidentRayTubes[i].path.node1;
		reflectedRayTubes[2*i+1].path.node2=incidentRayTubes[i].path.node2;
		reflectedRayTubes[2*i+1].path.node3.point1=face2.A;
		reflectedRayTubes[2*i+1].path.node3.point2=face2.B;
		reflectedRayTubes[2*i+1].path.node3.point3=face2.C;
	}
}

void CalculateReflectionWithBuildingFaceOnCPU(QuadrangleRayTube *incidentRayTubes,int faceCount,int rayTubeCount,QuadrangleRayTube *reflectedRayTubes,Face *face,int *reflectionExist)
{
	for (int i=0;i<rayTubeCount;i++)
	{
		int reflectedFace[4];
		int flag[4]={0,0,0,0};
		Ray reflectedRays[4],incidentRays[4];
		incidentRays[0]=incidentRayTubes[i].ray1;
		incidentRays[1]=incidentRayTubes[i].ray2;
		incidentRays[2]=incidentRayTubes[i].ray3;
		incidentRays[3]=incidentRayTubes[i].ray4;
		reflectedRays[0]=CalculateReflectedRayOnCPU(incidentRays[0],face,faceCount,&reflectedFace[0],&flag[0]);
		reflectedRays[1]=CalculateReflectedRayOnCPU(incidentRays[1],face,faceCount,&reflectedFace[1],&flag[1]);
		reflectedRays[2]=CalculateReflectedRayOnCPU(incidentRays[2],face,faceCount,&reflectedFace[2],&flag[2]);
		reflectedRays[3]=CalculateReflectedRayOnCPU(incidentRays[3],face,faceCount,&reflectedFace[3],&flag[3]);
		if (flag[0]+flag[1]+flag[2]+flag[3]==0)
		{
			reflectionExist[i]=0;
		}
		if (flag[0]+flag[1]+flag[2]+flag[3]==1)
		{
			reflectionExist[i]=1;
			reflectedRayTubes[2*i].ray1=reflectedRays[0];
			reflectedRayTubes[2*i].ray2=reflectedRays[1];
			reflectedRayTubes[2*i].ray3=reflectedRays[2];
			reflectedRayTubes[2*i].ray4=reflectedRays[3];
			reflectedRayTubes[2*i+1].ray1=reflectedRays[0];
			reflectedRayTubes[2*i+1].ray2=reflectedRays[1];
			reflectedRayTubes[2*i+1].ray3=reflectedRays[2];
			reflectedRayTubes[2*i+1].ray4=reflectedRays[3];
			for (int j=0;j<4;j++)
			{
				if (flag[j]==1)
				{
					AddPathNodeToReflectedRayTubeOnCPU(incidentRayTubes,reflectedRayTubes,i,face[reflectedFace[j]],face[reflectedFace[j]]);
				}
			}
		}
		if (flag[0]+flag[1]+flag[2]+flag[3]==2)
		{
			int m,n;
			for (int j=0;j<4;j++)
			{
				if (flag[j]==1)
				{
					m=j;
					for (int t=j+1;t<4;t++)
					{
						if(flag[t]==1)
						{
							n=t;
						}
					}
				}
			}
			GetReflectedRayTubesWhenTwoFlagsEqualOneOnCPU(m,n,reflectedFace,reflectedRays,incidentRays,reflectedRayTubes,face,reflectionExist,i);
			AddPathNodeToReflectedRayTubeOnCPU(incidentRayTubes,reflectedRayTubes,i,face[reflectedFace[m]],face[reflectedFace[n]]);
		}
		if (flag[0]+flag[1]+flag[2]+flag[3]==3)
		{
			int j=0;
			int remainingNumber1,remainingNumber2,remainingNumber3;
			for (;j<4;j++)
			{
				if (flag[j]==0)
				{
					break;
				}
			}
			GetThreeRemainingNumbersOnCPU(j,&remainingNumber1,&remainingNumber2,&remainingNumber3);
			GetReflectedRayTubesWhenThreeFlagsEqualOneOnCPU(remainingNumber1,remainingNumber2,remainingNumber3,reflectedFace,reflectedRays,incidentRays,reflectedRayTubes,incidentRayTubes,face,reflectionExist,i);

		}
		if (flag[0]+flag[1]+flag[2]+flag[3]==4)
		{
			if (JudgeTwoTriangleFacesOnTheSameQuadrangleOnCPU(reflectedFace[0],reflectedFace[1]))
			{
				if (JudgeTwoTriangleFacesOnTheAdjacentQuadrangleOnCPU(reflectedFace[0],reflectedFace[2]))
				{
					AddPathNodeToReflectedRayTubeOnCPU(incidentRayTubes,reflectedRayTubes,i,face[reflectedFace[0]],face[reflectedFace[2]]);
					GetReflectedRayTubesWhenTwoFlagsEqualOneOnCPU(0,2,reflectedFace,reflectedRays,incidentRays,reflectedRayTubes,face,reflectionExist,i);
				}
				else
				{
					AddPathNodeToReflectedRayTubeOnCPU(incidentRayTubes,reflectedRayTubes,i,face[reflectedFace[1]],face[reflectedFace[3]]);
					GetReflectedRayTubesWhenTwoFlagsEqualOneOnCPU(1,3,reflectedFace,reflectedRays,incidentRays,reflectedRayTubes,face,reflectionExist,i);
				}
			}
			if (JudgeTwoTriangleFacesOnTheSameQuadrangleOnCPU(reflectedFace[0],reflectedFace[3]))
			{
				if (JudgeTwoTriangleFacesOnTheAdjacentQuadrangleOnCPU(reflectedFace[0],reflectedFace[1]))
				{
					AddPathNodeToReflectedRayTubeOnCPU(incidentRayTubes,reflectedRayTubes,i,face[reflectedFace[0]],face[reflectedFace[1]]);
					GetReflectedRayTubesWhenTwoFlagsEqualOneOnCPU(0,1,reflectedFace,reflectedRays,incidentRays,reflectedRayTubes,face,reflectionExist,i);
				}
				else
				{
					AddPathNodeToReflectedRayTubeOnCPU(incidentRayTubes,reflectedRayTubes,i,face[reflectedFace[2]],face[reflectedFace[3]]);
					GetReflectedRayTubesWhenTwoFlagsEqualOneOnCPU(2,3,reflectedFace,reflectedRays,incidentRays,reflectedRayTubes,face,reflectionExist,i);
				}
			}
		}
	}
}
	
	


 