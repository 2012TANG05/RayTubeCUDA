#include "ReflectionAndDiffractionCalculation.h"


//编号0 1 2里得到剩下的号
int GetRemainingOneNumberOnCPU(int a,int b)
{
	return 3-(a+b);
}

//编号0 1 2里得到剩下的两个号
 void GetRemainingTwoNumberOnCPU(int a,int *b,int *c)
{
	if (a==0)
	{
		*b=1;
		*c=2;
	}
	if (a==1)
	{
		*b=0;
		*c=2;
	}
	if (a==2)
	{
		*b=0;
		*c=1;
	}
}


//给射线管添加节点
void AddPathNodeToReflectedRayTubeOnCPU(TriangleRayTube *incidentRayTubes,TriangleRayTube *reflectedRayTubes,int i,Face face1,Face face2)
{
	if(incidentRayTubes[i].path.nodeLevel==0)
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
	if(incidentRayTubes[i].path.nodeLevel==1)
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
	if(incidentRayTubes[i].path.nodeLevel==2)
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

//当射线管有且仅有一条射线存在反射时得到反射射线管
void GetReflectedRayTubeWhenOneReflectedRayExistOnCPU(int flagNumber,Ray *incidentRays,TriangleRayTube *reflectedRayTubes,Ray *reflectedRays,int *reflectedFace,Face *face,int i,int *reflectionExist,int *diffractionExist,int *faceNumber1,int *faceNumber2)
{
	int remainingNumber1;
	int remainingNumber2;
	GetRemainingTwoNumberOnCPU(flagNumber,&remainingNumber1,&remainingNumber2);
	reflectedRayTubes[2*i].ray1=reflectedRays[flagNumber];
	reflectedRayTubes[2*i].ray2=CalculateVirtualReflectedRayOnCPU(incidentRays[remainingNumber1],face[reflectedFace[flagNumber]]);
	reflectedRayTubes[2*i].ray3=CalculateVirtualReflectedRayOnCPU(incidentRays[remainingNumber2],face[reflectedFace[flagNumber]]);
	reflectedRayTubes[2*i+1].ray1=reflectedRays[flagNumber];
	reflectedRayTubes[2*i+1].ray2=CalculateVirtualReflectedRayOnCPU(incidentRays[remainingNumber1],face[reflectedFace[flagNumber]]);
	reflectedRayTubes[2*i+1].ray3=CalculateVirtualReflectedRayOnCPU(incidentRays[remainingNumber2],face[reflectedFace[flagNumber]]);
	diffractionExist[i]=0;
	reflectionExist[i]=1;
	faceNumber1[i]=reflectedFace[flagNumber];
	faceNumber2[i]=reflectedFace[flagNumber];
}

//当射线管有且仅有两条射线存在反射时得到反射射线管
void GetReflectedRayTubeWhenTwoReflectedRayExistOnCPU(int flagNumber1,int flagNumber2,Ray *incidentRays,TriangleRayTube *reflectedRayTubes,Ray *reflectedRays,int *reflectedFace,Face *face,int i,int *reflectionExist,int *diffractionExist,int *faceNumber1,int *faceNumber2)
{
	int remainingNumber=GetRemainingOneNumberOnCPU(flagNumber1,flagNumber2);
	reflectedRayTubes[2*i].ray1=reflectedRays[flagNumber1];
	reflectedRayTubes[2*i].ray2=CalculateVirtualReflectedRayOnCPU(incidentRays[flagNumber2],face[reflectedFace[flagNumber1]]);
	reflectedRayTubes[2*i].ray3=CalculateVirtualReflectedRayOnCPU(incidentRays[remainingNumber],face[reflectedFace[flagNumber1]]);
	reflectedRayTubes[2*i+1].ray1=reflectedRays[flagNumber2];
	reflectedRayTubes[2*i+1].ray2=CalculateVirtualReflectedRayOnCPU(incidentRays[flagNumber1],face[reflectedFace[flagNumber2]]);
	reflectedRayTubes[2*i+1].ray3=CalculateVirtualReflectedRayOnCPU(incidentRays[remainingNumber],face[reflectedFace[flagNumber2]]);
	if (JudgeTwoTriangleFacesOnTheAdjacentQuadrangleOnCPU(reflectedFace[flagNumber1],reflectedFace[flagNumber2]))
	{
		diffractionExist[i]=1;
	}
	else
	{
		diffractionExist[i]=0;
	}
	reflectionExist[i]=1;
	faceNumber1[i]=reflectedFace[flagNumber1];
	faceNumber2[i]=reflectedFace[flagNumber2];
}

//当射线管所有三条射线均存在反射时得到反射射线管
void GetReflectedRayTubeWhenThreeReflectedRayExistOnCPU(int flagNumber1,int flagNumber2,Ray *incidentRays,TriangleRayTube *reflectedRayTubes,Ray *reflectedRays,int *reflectedFace,Face *face,int i,int *reflectionExist,int *diffractionExist,int *faceNumber1,int *faceNumber2)
{
	int remainingNumber=GetRemainingOneNumberOnCPU(flagNumber1,flagNumber2);
	reflectedRayTubes[2*i].ray1=reflectedRays[flagNumber1];
	reflectedRayTubes[2*i].ray2=reflectedRays[flagNumber2];
	reflectedRayTubes[2*i].ray3=CalculateVirtualReflectedRayOnCPU(incidentRays[remainingNumber],face[reflectedFace[flagNumber1]]);
	reflectedRayTubes[2*i+1].ray1=CalculateVirtualReflectedRayOnCPU(incidentRays[flagNumber1],face[reflectedFace[remainingNumber]]);
	reflectedRayTubes[2*i+1].ray2=CalculateVirtualReflectedRayOnCPU(incidentRays[flagNumber2],face[reflectedFace[remainingNumber]]);
	reflectedRayTubes[2*i+1].ray3=reflectedRays[remainingNumber];
	diffractionExist[i]=1;
	reflectionExist[i]=1;
	if (JudgeTwoTriangleFacesOnTheAdjacentQuadrangleOnCPU(reflectedFace[remainingNumber],reflectedFace[flagNumber1]))
	{
		faceNumber1[i]=reflectedFace[flagNumber1];
	}
	else
	{
		faceNumber1[i]=reflectedFace[flagNumber2];
	}
	faceNumber2[i]=reflectedFace[remainingNumber];
}

//判断A、B两个点是否相同
bool JudgeTwoPointsAreSameOnCPU(Point A,Point B)
{
	if ((A.x==B.x)&&(A.y==B.y)&&(A.z==B.z))
	{
		return true;
	}
	return false;
}

//判断点A是否是三角面face的某个顶点
bool JudgeIsThePointOnTheFaceOnCPU(Point A,Face face)
{
	if (JudgeTwoPointsAreSameOnCPU(A,face.A)||JudgeTwoPointsAreSameOnCPU(A,face.B)||JudgeTwoPointsAreSameOnCPU(A,face.C))
	{
		return true;
	}
	return false;
}

//得到两个相邻面的公共棱
Edge GetEdgeOfCubeOnCPU(Face face1,Face face2)
{
	Edge edge;
	if (JudgeIsThePointOnTheFaceOnCPU(face1.A,face2))
	{
		edge.p1=face1.A;
		if (JudgeIsThePointOnTheFaceOnCPU(face1.B,face2))
		{
			edge.p2=face1.B;
			return edge;
		}
		else
		{
			edge.p2=face1.C;
			return edge;
		}
	}
	else
	{
		edge.p1=face1.B;
		edge.p2=face1.C;
		return edge;
	}
}

//计算AB两点的水平距离
float CalculateDistanceOnXOYOnCPU(Point a,Point b)
{
	return  sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}

//将棱的两个点按z坐标从大到小排
void SortPointsOfEdgeOnCPU(Edge *edge)
{
	Point tempPoint;
	if ((*edge).p1.z>(*edge).p2.z)
	{
		return;
	}
	else
	{
		tempPoint=(*edge).p1;
		(*edge).p1=(*edge).p2;
		(*edge).p2=tempPoint;		
	}
}

//将棱的两个点按z坐标从小到大排
void AntiSortPointsOfEdgeOnCPU(Edge *edge)
{
	Point tempPoint;
	if ((*edge).p1.z<(*edge).p2.z)
	{
		return;
	}
	else
	{
		tempPoint=(*edge).p1;
		(*edge).p1=(*edge).p2;
		(*edge).p2=tempPoint;		
	}

}


Edge GetDiffractionEdgeWhenTwoReflectedRaysExistOnCPU(Face face1,Face face2,Point reflectionPoint1,Point reflectionPoint2,Point VitualReflectionPoint,int *diffractionExist)
{
	Point diffractionPoint1;
	Point diffractionPoint2;
	Edge edge=GetEdgeOfCubeOnCPU(face1,face2);
	float x1=CalculateDistanceOnXOYOnCPU(reflectionPoint1,edge.p1);
	float x2=CalculateDistanceOnXOYOnCPU(reflectionPoint2,edge.p1);
	float z=reflectionPoint1.z+(reflectionPoint2.z-reflectionPoint1.z)*(x1/(x1+x2));
	SortPointsOfEdgeOnCPU(&edge);
	diffractionPoint1.x=edge.p1.x;
	diffractionPoint1.y=edge.p1.y;
	diffractionPoint1.z=z;
	if (VitualReflectionPoint.z>z)
	{
		diffractionPoint2=edge.p1;;
	}
	else
	{
		diffractionPoint2=edge.p2;
	}
	if (abs(edge.p1.z-edge.p2.z)<=0.1)
	{
		*diffractionExist=0;
		return edge;
	}
	Edge diffractionEdge;
	diffractionEdge.p1=diffractionPoint1;
	diffractionEdge.p2=diffractionPoint2;
	AntiSortPointsOfEdgeOnCPU(&diffractionEdge);
	return diffractionEdge;
}

Edge GetDiffractionEdgeWhenThreeReflectedRaysExistOnCPU(Face face1,Face face2,Point reflectionPoint1,Point reflectionPoint2,Point reflectionPointOnOtherFace,int *diffractionExist)
{
	Point diffractionPoint1,diffractionPoint2;
	Edge edge=GetEdgeOfCubeOnCPU(face1,face2);
	float x1=CalculateDistanceOnXOYOnCPU(reflectionPoint1,edge.p1);
	float x2=CalculateDistanceOnXOYOnCPU(reflectionPoint2,edge.p1);
	float x3=CalculateDistanceOnXOYOnCPU(reflectionPointOnOtherFace,edge.p1);
	float z1=reflectionPoint1.z+(reflectionPointOnOtherFace.z-reflectionPoint1.z)*(x1/(x1+x3));
	float z2=reflectionPoint2.z+(reflectionPointOnOtherFace.z-reflectionPoint2.z)*(x2/(x2+x3));
	Edge diffractionEdge;
	if (abs(z1-z2)<0.1)
	{
		*diffractionExist=0;
		return edge;
	}
	diffractionEdge.p1.x=edge.p1.x;
	diffractionEdge.p1.y=edge.p1.y;
	diffractionEdge.p1.z=z1;
	diffractionEdge.p2.x=edge.p1.x;
	diffractionEdge.p2.y=edge.p1.y;
	diffractionEdge.p2.z=z2;
	SortPointsOfEdgeOnCPU(&diffractionEdge);
	return diffractionEdge;
}

int GetMaxNumberOnCPU(int a,int b,int c)
{
	if (a>b)
	{
		if (a>c)
		{
			return a;
		}
		else
		{
			return c;
		}
	}
	else
	{
		if (b>c)
		{
			return b;
		}
		else
		{
			return c;
		}
	}
}

int GetMinNumberOnCPU(int a,int b,int c)
{
	if (a<b)
	{
		if (a<c)
		{
			return a;
		}
		else
		{
			return c;
		}
	}
	else
	{
		if (b<c)
		{
			return b;
		}
		else
		{
			return c;
		}
	}
}

void CalculateReflectionAndDiffractionWithBuildingFaceOnCPU(TriangleRayTube *incidentRayTubes,int faceCount,int rayTubeCount,TriangleRayTube *reflectedRayTubes,Face *face,Edge *diffractionEdge,int *faceNumber1,int *faceNumber2,int *reflectionExist,int *diffractionExist)
{
	for (int i=0;i<rayTubeCount;i++)
	{
		int reflectedFace[3]={0,0,0};
		int flag[3]={0,0,0};
		int difference=0;
		Ray reflectedRays[3],incidentRays[3];
		incidentRays[0]=incidentRayTubes[i].ray1;
		incidentRays[1]=incidentRayTubes[i].ray2;
		incidentRays[2]=incidentRayTubes[i].ray3;
		reflectedRays[0]=CalculateReflectedRayOnCPU(incidentRays[0],face,faceCount,&reflectedFace[0],&flag[0]);
		reflectedRays[1]=CalculateReflectedRayOnCPU(incidentRays[1],face,faceCount,&reflectedFace[1],&flag[1]);
		reflectedRays[2]=CalculateReflectedRayOnCPU(incidentRays[2],face,faceCount,&reflectedFace[2],&flag[2]);
		difference=GetMaxNumberOnCPU(reflectedFace[0],reflectedFace[1],reflectedFace[2])-GetMinNumberOnCPU(reflectedFace[0],reflectedFace[1],reflectedFace[2]);
		if (flag[0]+flag[1]+flag[2]==0||difference>=12)
		{
			reflectionExist[i]=0;
			diffractionExist[i]=0;
			faceNumber1[i]=0;
			faceNumber2[i]=0;
		}
		else
		{
			if (flag[0]+flag[1]+flag[2]==1)
			{
				reflectionExist[i]=1;
				diffractionExist[i]=0;	
				for (int j=0;j<3;j++)
				{
					if (flag[j]==1)
					{
						GetReflectedRayTubeWhenOneReflectedRayExistOnCPU(j,incidentRays,reflectedRayTubes,reflectedRays,reflectedFace,face,i,reflectionExist,diffractionExist,faceNumber1,faceNumber2);
						AddPathNodeToReflectedRayTubeOnCPU(incidentRayTubes,reflectedRayTubes,i,face[reflectedFace[j]],face[reflectedFace[j]]);
					}
				}
			}
			if (flag[0]+flag[1]+flag[2]==2)
			{
				if (flag[0]==0)
				{
					AddPathNodeToReflectedRayTubeOnCPU(incidentRayTubes,reflectedRayTubes,i,face[reflectedFace[1]],face[reflectedFace[2]]);
					if (JudgeTwoTriangleFacesOnTheSameQuadrangleOnCPU(reflectedFace[1],reflectedFace[2]))
					{
						GetReflectedRayTubeWhenOneReflectedRayExistOnCPU(1,incidentRays,reflectedRayTubes,reflectedRays,reflectedFace,face,i,reflectionExist,diffractionExist,faceNumber1,faceNumber2);

					}
					else
					{
						GetReflectedRayTubeWhenTwoReflectedRayExistOnCPU(1,2,incidentRays,reflectedRayTubes,reflectedRays,reflectedFace,face,i,reflectionExist,diffractionExist,faceNumber1,faceNumber2);
						//AddPathNodeToReflectedRayTubeOnCPU(incidentRayTubes,reflectedRayTubes,i,face[reflectedFace[1]],face[reflectedFace[2]]);
						if (JudgeTwoTriangleFacesOnTheAdjacentQuadrangleOnCPU(reflectedFace[1],reflectedFace[2]))
						{
							diffractionEdge[i]=GetDiffractionEdgeWhenTwoReflectedRaysExistOnCPU(face[reflectedFace[1]],face[reflectedFace[2]],reflectedRays[1].originalPoint,reflectedRays[2].originalPoint,reflectedRays[0].originalPoint,&diffractionExist[i]);
						}
					}
				}
				if (flag[1]==0)
				{
					AddPathNodeToReflectedRayTubeOnCPU(incidentRayTubes,reflectedRayTubes,i,face[reflectedFace[0]],face[reflectedFace[2]]);
					if (JudgeTwoTriangleFacesOnTheSameQuadrangleOnCPU(reflectedFace[0],reflectedFace[2]))
					{
						GetReflectedRayTubeWhenOneReflectedRayExistOnCPU(0,incidentRays,reflectedRayTubes,reflectedRays,reflectedFace,face,i,reflectionExist,diffractionExist,faceNumber1,faceNumber2);
					}
					else
					{
						GetReflectedRayTubeWhenTwoReflectedRayExistOnCPU(0,2,incidentRays,reflectedRayTubes,reflectedRays,reflectedFace,face,i,reflectionExist,diffractionExist,faceNumber1,faceNumber2);
						if (JudgeTwoTriangleFacesOnTheAdjacentQuadrangleOnCPU(reflectedFace[0],reflectedFace[2]))
						{
							diffractionEdge[i]=GetDiffractionEdgeWhenTwoReflectedRaysExistOnCPU(face[reflectedFace[0]],face[reflectedFace[2]],reflectedRays[0].originalPoint,reflectedRays[2].originalPoint,reflectedRays[1].originalPoint,&diffractionExist[i]);
						}
					}

				}
				if (flag[2]==0)
				{
					AddPathNodeToReflectedRayTubeOnCPU(incidentRayTubes,reflectedRayTubes,i,face[reflectedFace[0]],face[reflectedFace[1]]);
					if (JudgeTwoTriangleFacesOnTheSameQuadrangleOnCPU(reflectedFace[0],reflectedFace[1]))
					{
						GetReflectedRayTubeWhenOneReflectedRayExistOnCPU(0,incidentRays,reflectedRayTubes,reflectedRays,reflectedFace,face,i,reflectionExist,diffractionExist,faceNumber1,faceNumber2);
					}
					else
					{
						GetReflectedRayTubeWhenTwoReflectedRayExistOnCPU(0,1,incidentRays,reflectedRayTubes,reflectedRays,reflectedFace,face,i,reflectionExist,diffractionExist,faceNumber1,faceNumber2);
						if (JudgeTwoTriangleFacesOnTheAdjacentQuadrangleOnCPU(reflectedFace[0],reflectedFace[1]))
						{
							diffractionEdge[i]=GetDiffractionEdgeWhenTwoReflectedRaysExistOnCPU(face[reflectedFace[0]],face[reflectedFace[1]],reflectedRays[0].originalPoint,reflectedRays[1].originalPoint,reflectedRays[2].originalPoint,&diffractionExist[i]);
						}
					}			

				}
			}
			if (flag[0]+flag[1]+flag[2]==3)
			{
				if (JudgeTwoTriangleFacesOnTheSameQuadrangleOnCPU(reflectedFace[0],reflectedFace[1])&&JudgeTwoTriangleFacesOnTheSameQuadrangleOnCPU(reflectedFace[0],reflectedFace[2]))
				{
					reflectedRayTubes[2*i].ray1=reflectedRays[0];
					reflectedRayTubes[2*i].ray2=reflectedRays[1];
					reflectedRayTubes[2*i].ray3=reflectedRays[2];
					reflectedRayTubes[2*i+1].ray1=reflectedRays[0];
					reflectedRayTubes[2*i+1].ray2=reflectedRays[1];
					reflectedRayTubes[2*i+1].ray3=reflectedRays[2];
					reflectionExist[i]=1;
					diffractionExist[i]=0;
					AddPathNodeToReflectedRayTubeOnCPU(incidentRayTubes,reflectedRayTubes,i,face[reflectedFace[0]],face[reflectedFace[1]]);
				}
				else{	
					if (JudgeTwoTriangleFacesOnTheSameQuadrangleOnCPU(reflectedFace[0],reflectedFace[1]))
					{
						AddPathNodeToReflectedRayTubeOnCPU(incidentRayTubes,reflectedRayTubes,i,face[reflectedFace[0]],face[reflectedFace[2]]);
						GetReflectedRayTubeWhenThreeReflectedRayExistOnCPU(0,1,incidentRays,reflectedRayTubes,reflectedRays,reflectedFace,face,i,reflectionExist,diffractionExist,faceNumber1,faceNumber2);
						if(JudgeTwoTriangleFacesOnTheAdjacentQuadrangleOnCPU(reflectedFace[1],reflectedFace[2]))
						{				
							diffractionEdge[i]=GetDiffractionEdgeWhenThreeReflectedRaysExistOnCPU(face[reflectedFace[1]],face[reflectedFace[2]],reflectedRays[0].originalPoint,reflectedRays[1].originalPoint,reflectedRays[2].originalPoint,&diffractionExist[i]);
						}
						else
						{
							diffractionEdge[i]=GetDiffractionEdgeWhenThreeReflectedRaysExistOnCPU(face[reflectedFace[0]],face[reflectedFace[2]],reflectedRays[0].originalPoint,reflectedRays[1].originalPoint,reflectedRays[2].originalPoint,&diffractionExist[i]);
						}

					}
					if (JudgeTwoTriangleFacesOnTheSameQuadrangleOnCPU(reflectedFace[0],reflectedFace[2]))
					{
						AddPathNodeToReflectedRayTubeOnCPU(incidentRayTubes,reflectedRayTubes,i,face[reflectedFace[0]],face[reflectedFace[1]]);
						GetReflectedRayTubeWhenThreeReflectedRayExistOnCPU(0,2,incidentRays,reflectedRayTubes,reflectedRays,reflectedFace,face,i,reflectionExist,diffractionExist,faceNumber1,faceNumber2);
						if(JudgeTwoTriangleFacesOnTheAdjacentQuadrangleOnCPU(reflectedFace[0],reflectedFace[1]))
						{				
							diffractionEdge[i]=GetDiffractionEdgeWhenThreeReflectedRaysExistOnCPU(face[reflectedFace[0]],face[reflectedFace[1]],reflectedRays[0].originalPoint,reflectedRays[2].originalPoint,reflectedRays[1].originalPoint,&diffractionExist[i]);
						}
						else
						{
							diffractionEdge[i]=GetDiffractionEdgeWhenThreeReflectedRaysExistOnCPU(face[reflectedFace[1]],face[reflectedFace[2]],reflectedRays[0].originalPoint,reflectedRays[2].originalPoint,reflectedRays[1].originalPoint,&diffractionExist[i]);
						}

					}
					if (JudgeTwoTriangleFacesOnTheSameQuadrangleOnCPU(reflectedFace[1],reflectedFace[2]))
					{
						AddPathNodeToReflectedRayTubeOnCPU(incidentRayTubes,reflectedRayTubes,i,face[reflectedFace[1]],face[reflectedFace[0]]);
						GetReflectedRayTubeWhenThreeReflectedRayExistOnCPU(1,2,incidentRays,reflectedRayTubes,reflectedRays,reflectedFace,face,i,reflectionExist,diffractionExist,faceNumber1,faceNumber2);
						if(JudgeTwoTriangleFacesOnTheAdjacentQuadrangleOnCPU(reflectedFace[0],reflectedFace[1]))
						{				
							diffractionEdge[i]=GetDiffractionEdgeWhenThreeReflectedRaysExistOnCPU(face[reflectedFace[0]],face[reflectedFace[1]],reflectedRays[1].originalPoint,reflectedRays[2].originalPoint,reflectedRays[0].originalPoint,&diffractionExist[i]);
						}
						else
						{
							diffractionEdge[i]=GetDiffractionEdgeWhenThreeReflectedRaysExistOnCPU(face[reflectedFace[0]],face[reflectedFace[2]],reflectedRays[1].originalPoint,reflectedRays[2].originalPoint,reflectedRays[0].originalPoint,&diffractionExist[i]);
						}			
					}
				}
			}	
		}
	}	
}



