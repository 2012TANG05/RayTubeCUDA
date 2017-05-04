//#include "cuda_runtime.h"
//#include "device_launch_parameters.h"
//#include <math.h>
//#include "Face.h"
//#include "BasicTool.h"
//
//using namespace std;
//#define PI 3.1415926;
//
//__device__ void RADC_UitizeVector(float *a,float *b,float *c)//单位化向量
//{
//	float length=sqrt((*a)*(*a)+(*b)*(*b)+(*c)*(*c));
//	*a=*a/length;
//	*b=*b/length;
//	*c=*c/length; 
//}
//
//__device__ Point RADC_GetReflectedVector(Point d,Point n)//计算反射射线方向向量
//{
//	Point reflectedPoint;
//	float temp=2*(d.x*n.x+d.y*n.y+d.z*n.z);
//	reflectedPoint.x=d.x-temp*n.x;
//	reflectedPoint.y=d.y-temp*n.y;
//	reflectedPoint.z=d.z-temp*n.z;
//	return reflectedPoint;
//}
//__device__ Ray RADC_CalculateReflectedRay(Ray incidentRay,Face *face,int faceCount,int *reflectedFace,int *flag)
//{
//	Ray reflectedRay;
//	float t=50000;
//	for (int i=0;i<faceCount;i++)
//	{
//		float a1=-incidentRay.direction.x,a2=-incidentRay.direction.y,a3=-incidentRay.direction.z;
//		float b1=face[i]. B.x-face[i].A.x,b2=face[i].B.y-face[i].A.y,b3=face[i].B.z-face[i].A.z;
//		float c1=face[i]. C.x-face[i].A.x,c2=face[i].C.y-face[i].A.y,c3=face[i].C.z-face[i].A.z;
//		float x1=incidentRay.originalPoint.x-face[i].A.x,x2=incidentRay.originalPoint.y-face[i].A.y,x3=incidentRay.originalPoint.z-face[i].A.z;
//		float denominator=a1*(b2*c3-b3*c2)-b1*(a2*c3-a3*c2)+c1*(a2*b3-a3*b2);
//		float t_numerator=x1*(b2*c3-b3*c2)-b1*(x2*c3-x3*c2)+c1*(x2*b3-x3*b2);
//		float u_numerator=a1*(x2*c3-x3*c2)-x1*(a2*c3-a3*c2)+c1*(a2*c3-a3*c2);
//		float v_numerator=a1*(b2*x3-b3*x2)-b1*(a2*x3-a3*x2)+x1*(a2*b3-a3*b2);
//		if (abs(denominator)>0.000001)
//		{
//			float u=u_numerator/denominator;
//			float v=v_numerator/denominator;
//			if(t_numerator/denominator<t&&t_numerator/denominator>1)
//			{
//				t=t_numerator/denominator;
//				reflectedRay.originalPoint.x=incidentRay.originalPoint.x+incidentRay.direction.x*t;
//				reflectedRay.originalPoint.y=incidentRay.originalPoint.y+incidentRay.direction.y*t;
//				reflectedRay.originalPoint.z=incidentRay.originalPoint.z+incidentRay.direction.z*t;
//				Point n;
//				n.x=b2*c3-b3*c2;n.y=b3*c1-b1*c3;n.z=b1*c2-c1*b2;
//				RADC_UitizeVector(&n.x,&n.y,&n.z);
//				reflectedRay.direction=RADC_GetReflectedVector(incidentRay.direction,n);
//				*reflectedFace=i;
//				if (u>0&&u<1&&(u+v)>0&&(u+v)<1)
//				{
//					*flag=1;
//				}
//			}
//		}
//	}
//	return reflectedRay;
//}
//
//
//
//__device__ bool JudgeTwoTriangleFacesOnTheSameQuadrangle(int faceNumber1,int faceNumber2)
//{
//	int a=faceNumber1%12;
//	int b=faceNumber2%12;
//	if (faceNumber1==faceNumber2)
//	{
//		return true;
//	}
//	if (a%2==0)
//	{
//		if (b==a+1)
//		{
//			return  true;
//		}
//	}
//	if (a%2==1)
//	{
//		if (b==a-1)
//		{
//			return true;
//		}
//	}
//	return false;
//}
//
//__device__ bool JudgeTwoTriangleFacesOnTheAdjacentQuadrangle(int faceNumber1,int faceNumber2)
//{
//	int a=faceNumber1%12;
//	int b=faceNumber2%12;
//	if (a%2==0&&a!=0)
//	{
//		if (b==a-1)
//		{
//			return  true;
//		}
//	}
//	if (a%2==1&&a!=7)
//	{
//		if (b==a+1)
//		{
//			return true;
//		}
//	}
//	if ((a==0&&b==7)||(a==7&&b==0))
//	{
//		return true;
//	}
//	return false;
//}
//
//__device__ int GetRemainingOneNumber(int a,int b)
//{
//	return 3-(a+b);
//}
//
//__device__ void GetRemainingTwoNumber(int a,int *b,int *c)
//{
//	if (a==0)
//	{
//		*b=1;
//		*c=2;
//	}
//	if (a==1)
//	{
//		*b=0;
//		*c=2;
//	}
//	if (a==2)
//	{
//		*b=0;
//		*c=1;
//	}
//}
//
//__device__ void GetReflectedRayTubeWhenOneReflectedRayExist(int flagNumber,Ray *incidentRays,TriangleRayTube *reflectedRayTubes,Ray *reflectedRays,int *reflectedFace,Face *face,int i,int *reflectionExist,int *diffractionExist,int *faceNumber1,int *faceNumber2)
//{
//	int tempFace,tempFlag;
//	int remainingNumber1,int remainingNumber2;
//	GetRemainingTwoNumber(flagNumber,&remainingNumber1,&remainingNumber2);
//	reflectedRayTubes[2*i].ray1=reflectedRays[flagNumber];
//	reflectedRayTubes[2*i].ray2=RADC_CalculateReflectedRay(incidentRays[remainingNumber1],&face[reflectedFace[flagNumber]],1,&tempFace,&tempFlag);
//	reflectedRayTubes[2*i].ray3=RADC_CalculateReflectedRay(incidentRays[remainingNumber2],&face[reflectedFace[flagNumber]],1,&tempFace,&tempFlag);
//	reflectedRayTubes[2*i+1].ray1=reflectedRays[flagNumber];
//	reflectedRayTubes[2*i+1].ray2=RADC_CalculateReflectedRay(incidentRays[remainingNumber1],&face[reflectedFace[flagNumber]],1,&tempFace,&tempFlag);
//	reflectedRayTubes[2*i+1].ray3=RADC_CalculateReflectedRay(incidentRays[remainingNumber2],&face[reflectedFace[flagNumber]],1,&tempFace,&tempFlag);
//	diffractionExist[i]=0;
//	reflectionExist[i]=1;
//	faceNumber1[i]=reflectedFace[flagNumber];
//	faceNumber2[i]=reflectedFace[flagNumber];
//}
//
//__device__ void GetReflectedRayTubeWhenTwoReflectedRayExist(int flagNumber1,int flagNumber2,Ray *incidentRays,TriangleRayTube *reflectedRayTubes,Ray *reflectedRays,int *reflectedFace,Face *face,int i,int *reflectionExist,int *diffractionExist,int *faceNumber1,int *faceNumber2)
//{
//	int tempFace,tempflag=0;
//	int remainingNumber=GetRemainingOneNumber(flagNumber1,flagNumber2);
//	reflectedRayTubes[2*i].ray1=reflectedRays[flagNumber1];
//	reflectedRayTubes[2*i].ray2=RADC_CalculateReflectedRay(incidentRays[flagNumber2],&face[reflectedFace[flagNumber1]],1,&tempFace,&tempflag);
//	reflectedRayTubes[2*i].ray3=RADC_CalculateReflectedRay(incidentRays[remainingNumber],&face[reflectedFace[flagNumber1]],1,&tempFace,&tempflag);
//	reflectedRayTubes[2*i+1].ray1=reflectedRays[flagNumber2];
//	reflectedRayTubes[2*i+1].ray2=RADC_CalculateReflectedRay(incidentRays[flagNumber1],&face[reflectedFace[flagNumber2]],1,&tempFace,&tempflag);
//	reflectedRayTubes[2*i+1].ray3=RADC_CalculateReflectedRay(incidentRays[remainingNumber],&face[reflectedFace[flagNumber2]],1,&tempFace,&tempflag);
//	if (JudgeTwoTriangleFacesOnTheAdjacentQuadrangle(reflectedFace[flagNumber1],reflectedFace[flagNumber2]))
//	{
//		diffractionExist[i]=1;
//	}
//	else
//	{
//		diffractionExist[i]=0;
//	}
//	reflectionExist[i]=1;
//	faceNumber1[i]=reflectedFace[flagNumber1];
//	faceNumber2[i]=reflectedFace[flagNumber2];
//}
//
//__device__ void GetReflectedRayTubeWhenThreeReflectedRayExist(int flagNumber1,int flagNumber2,Ray *incidentRays,TriangleRayTube *reflectedRayTubes,Ray *reflectedRays,int *reflectedFace,Face *face,int i,int *reflectionExist,int *diffractionExist,int *faceNumber1,int *faceNumber2)
//{
//	int tempFace,tempflag=0;
//	int remainingNumber=GetRemainingOneNumber(flagNumber1,flagNumber2);
//	reflectedRayTubes[2*i].ray1=reflectedRays[flagNumber1];
//	reflectedRayTubes[2*i].ray2=reflectedRays[flagNumber2];
//	reflectedRayTubes[2*i].ray3=RADC_CalculateReflectedRay(incidentRays[remainingNumber],&face[reflectedFace[flagNumber1]],1,&tempFace,&tempflag);
//	reflectedRayTubes[2*i+1].ray1=RADC_CalculateReflectedRay(incidentRays[flagNumber1],&face[reflectedFace[remainingNumber]],1,&tempFace,&tempflag);
//	reflectedRayTubes[2*i+1].ray2=RADC_CalculateReflectedRay(incidentRays[flagNumber2],&face[reflectedFace[remainingNumber]],1,&tempFace,&tempflag);
//	reflectedRayTubes[2*i+1].ray3=reflectedRays[remainingNumber];
//	diffractionExist[i]=1;
//	reflectionExist[i]=1;
//	if (JudgeTwoTriangleFacesOnTheAdjacentQuadrangle(reflectedFace[remainingNumber],reflectedFace[flagNumber1]))
//	{
//		faceNumber1[i]=reflectedFace[flagNumber1];
//	}
//	else
//	{
//		faceNumber1[i]=reflectedFace[flagNumber2];
//	}
//	faceNumber2[i]=reflectedFace[remainingNumber];
//}
//
//__device__ bool JudgeTwoPointsAreSame(Point A,Point B)
//{
//	if ((A.x==B.x)&&(A.y==B.y)&&(A.z==B.z))
//	{
//		return true;
//	}
//	return false;
//}
//
//__device__ bool JudgeIsTheSamePointOfAdjacentExist(Point A,Face face)
//{
//	if (JudgeTwoPointsAreSame(A,face.A)||JudgeTwoPointsAreSame(A,face.B)||JudgeTwoPointsAreSame(A,face.C))
//	{
//		return true;
//	}
//	return false;
//}
//
//__device__ Edge GetEdgeOfCube(Face face1,Face face2)
//{
//	Edge edge;
//	if (JudgeIsTheSamePointOfAdjacentExist(face1.A,face2))
//	{
//		edge.p1=face1.A;
//		if (JudgeIsTheSamePointOfAdjacentExist(face1.B,face2))
//		{
//			edge.p2=face1.B;
//			return edge;
//		}
//		else
//		{
//			edge.p2=face1.C;
//			return edge;
//		}
//	}
//	else
//	{
//		edge.p1=face1.A;
//		edge.p2=face1.B;
//		return edge;
//	}
//}
//__device__ float CalculateDistanceOnXOY(Point a,Point b)
//{
//	return  sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
//}
//
////__device__ Point GetHighestPointOfAFace(Face face)
////{
////	if (face.A.z>face.B.z)
////	{
////		if (face.A.z>face.C.z)
////		{
////			return face.A;
////		}
////		else
////		{
////			return face.C;
////		}
////	}
////	else
////	{
////		if (face.B.z>face.C.z)
////		{
////			return face.B;
////		}
////		else
////		{
////			return face.C;
////		}
////	}
////}
//__device__ void SortPointsOfEdge(Edge *edge)
//{
//	Point tempPoint;
//	if ((*edge).p1.z>(*edge).p2.z)
//	{
//		return;
//	}
//	else
//	{
//		tempPoint=(*edge).p1;
//		(*edge).p1=(*edge).p2;
//		(*edge).p2=tempPoint;		
//	}
//}
//__device__ Edge GetDiffractionEdgeWhenTwoReflectedRaysExist(Face face1,Face face2,Point reflectionPoint1,Point reflectionPoint2,Point VitualReflectionPoint)
//{
//	Point diffractionPoint1;
//	Point diffractionPoint2;
//	Edge edge=GetEdgeOfCube(face1,face2);
//	float x1=CalculateDistanceOnXOY(reflectionPoint1,edge.p1);
//	float x2=CalculateDistanceOnXOY(reflectionPoint2,edge.p1);
//	float z=reflectionPoint1.z+(reflectionPoint2.z-reflectionPoint1.z)*(x1/(x1+x2));
//	SortPointsOfEdge(&edge);
//	diffractionPoint1.x=edge.p1.x;
//	diffractionPoint1.y=edge.p1.y;
//	diffractionPoint1.z=z;
//	if (VitualReflectionPoint.z>z)
//	{
//		diffractionPoint2=edge.p1;;
//	}
//	else
//	{
//		diffractionPoint2=edge.p2;
//	}
//	Edge diffractionEdge;
//	diffractionEdge.p1=diffractionPoint1;
//	diffractionEdge.p2=diffractionPoint2;
//	SortPointsOfEdge(&diffractionEdge);
//	return diffractionEdge;
//}
//
//__device__ Edge GetDiffractionEdgeWhenThreeReflectedRaysExist(Face face1,Face face2,Point reflectionPoint1,Point reflectionPoint2,Point reflectionPointOnOtherFace)
//{
//	Point diffractionPoint1,diffractionPoint2;
//	Edge edge=GetEdgeOfCube(face1,face2);
//	float x1=CalculateDistanceOnXOY(reflectionPoint1,edge.p1);
//	float x2=CalculateDistanceOnXOY(reflectionPoint2,edge.p1);
//	float x3=CalculateDistanceOnXOY(reflectionPointOnOtherFace,edge.p1);
//	float z1=reflectionPoint1.z+(reflectionPointOnOtherFace.z-reflectionPoint1.z)*(x1/(x1+x3));
//	float z2=reflectionPoint2.z+(reflectionPointOnOtherFace.z-reflectionPoint2.z)*(x2/(x2+x3));
//	Edge diffractionEdge;
//	diffractionEdge.p1.x=edge.p1.x;
//	diffractionEdge.p1.y=edge.p1.y;
//	diffractionEdge.p1.z=z1;
//	diffractionEdge.p2.x=edge.p1.x;
//	diffractionEdge.p2.y=edge.p1.y;
//	diffractionEdge.p2.z=z2;
//	SortPointsOfEdge(&diffractionEdge);
//	return diffractionEdge;
//}
//
//__global__ void CalculateReflectionAndDiffractionWithBuildingFace(TriangleRayTube *incidentRayTubes,int faceCount,int rayTubeCount,TriangleRayTube *reflectedRayTubes,Face *face,Edge *diffractionEdge,int *faceNumber1,int *faceNumber2,int *reflectionExist,int *diffractionExist)
//{
//	int i=blockIdx.x * blockDim.x+threadIdx.x;
//	int reflectedFace[3];
//	int flag[3]={0,0,0};
//	Ray reflectedRays[3],incidentRays[3];
//	incidentRays[0]=incidentRayTubes[i].ray1;
//	incidentRays[0]=incidentRayTubes[i].ray2;
//	incidentRays[0]=incidentRayTubes[i].ray3;
//	reflectedRays[0]=RADC_CalculateReflectedRay(incidentRays[0],face,faceCount,&reflectedFace[0],&flag[0]);
//	reflectedRays[1]=RADC_CalculateReflectedRay(incidentRays[1],face,faceCount,&reflectedFace[1],&flag[1]);
//	reflectedRays[2]=RADC_CalculateReflectedRay(incidentRays[2],face,faceCount,&reflectedFace[2],&flag[2]);
//	if (flag[0]+flag[1]+flag[2]==0)
//	{
//		reflectionExist[i]=0;
//		diffractionExist[i]=0;
//		faceNumber1[i]=0;
//		faceNumber2[i]=0;
//	}
//	if (flag[0]+flag[1]+flag[2]==1)
//	{
//		for (int j=0;j<3;j++)
//		{
//			if (flag[j]==1)
//			{
//				GetReflectedRayTubeWhenOneReflectedRayExist(j,incidentRays,reflectedRayTubes,reflectedRays,reflectedFace,face,i,reflectionExist,diffractionExist,faceNumber1,faceNumber2);
//			}
//		}
//	}
//	if (flag[0]+flag[1]+flag[2]==2)
//	{
//		if (flag[0]==0)
//		{
//			if (JudgeTwoTriangleFacesOnTheSameQuadrangle(reflectedFace[1],reflectedFace[2]))
//			{
//				GetReflectedRayTubeWhenOneReflectedRayExist(1,incidentRays,reflectedRayTubes,reflectedRays,reflectedFace,face,i,reflectionExist,diffractionExist,faceNumber1,faceNumber2);
//			}
//			else
//			{
//				GetReflectedRayTubeWhenTwoReflectedRayExist(1,2,incidentRays,reflectedRayTubes,reflectedRays,reflectedFace,face,i,reflectionExist,diffractionExist,faceNumber1,faceNumber2);
//				if (JudgeTwoTriangleFacesOnTheAdjacentQuadrangle(reflectedFace[1],reflectedFace[2]))
//				{
//					diffractionEdge[i]=GetDiffractionEdgeWhenTwoReflectedRaysExist(face[reflectedFace[1]],face[reflectedFace[2]],reflectedRays[1].originalPoint,reflectedRays[2].originalPoint,reflectedRays[0].originalPoint);
//				}
//			}
//		}
//		if (flag[1]==0)
//		{
//			if (JudgeTwoTriangleFacesOnTheSameQuadrangle(reflectedFace[0],reflectedFace[2]))
//			{
//				GetReflectedRayTubeWhenOneReflectedRayExist(0,incidentRays,reflectedRayTubes,reflectedRays,reflectedFace,face,i,reflectionExist,diffractionExist,faceNumber1,faceNumber2);
//			}
//			else
//			{
//				GetReflectedRayTubeWhenTwoReflectedRayExist(0,2,incidentRays,reflectedRayTubes,reflectedRays,reflectedFace,face,i,reflectionExist,diffractionExist,faceNumber1,faceNumber2);
//				if (JudgeTwoTriangleFacesOnTheAdjacentQuadrangle(reflectedFace[0],reflectedFace[2]))
//				{
//					diffractionEdge[i]=GetDiffractionEdgeWhenTwoReflectedRaysExist(face[reflectedFace[0]],face[reflectedFace[2]],reflectedRays[0].originalPoint,reflectedRays[2].originalPoint,reflectedRays[1].originalPoint);
//				}
//			}
//
//		}
//		if (flag[2]==0)
//		{
//			if (JudgeTwoTriangleFacesOnTheSameQuadrangle(reflectedFace[0],reflectedFace[1]))
//			{
//				GetReflectedRayTubeWhenOneReflectedRayExist(0,incidentRays,reflectedRayTubes,reflectedRays,reflectedFace,face,i,reflectionExist,diffractionExist,faceNumber1,faceNumber2);
//			}
//			else
//			{
//				GetReflectedRayTubeWhenTwoReflectedRayExist(0,1,incidentRays,reflectedRayTubes,reflectedRays,reflectedFace,face,i,reflectionExist,diffractionExist,faceNumber1,faceNumber2);
//				if (JudgeTwoTriangleFacesOnTheAdjacentQuadrangle(reflectedFace[0],reflectedFace[1]))
//				{
//					diffractionEdge[i]=GetDiffractionEdgeWhenTwoReflectedRaysExist(face[reflectedFace[0]],face[reflectedFace[1]],reflectedRays[0].originalPoint,reflectedRays[1].originalPoint,reflectedRays[2].originalPoint);
//				}
//			}			
//
//		}
//	}
//	if (flag[0]+flag[1]+flag[2]==3)
//	{
//		if (JudgeTwoTriangleFacesOnTheSameQuadrangle(reflectedFace[0],reflectedFace[1])&&JudgeTwoTriangleFacesOnTheSameQuadrangle(reflectedFace[0],reflectedFace[2]))
//		{
//			reflectedRayTubes[2*i].ray1=reflectedRays[0];
//			reflectedRayTubes[2*i].ray2=reflectedRays[1];
//			reflectedRayTubes[2*i].ray3=reflectedRays[2];
//			reflectedRayTubes[2*i+1].ray1=reflectedRays[0];
//			reflectedRayTubes[2*i+1].ray2=reflectedRays[1];
//			reflectedRayTubes[2*i+1].ray3=reflectedRays[2];
//			reflectionExist[i]=1;
//			diffractionExist[i]=0;
//		}
//		if (JudgeTwoTriangleFacesOnTheSameQuadrangle(reflectedFace[0],reflectedFace[1])&&(JudgeTwoTriangleFacesOnTheAdjacentQuadrangle(reflectedFace[0],reflectedFace[2])))
//		{
//			GetReflectedRayTubeWhenThreeReflectedRayExist(0,1,incidentRays,reflectedRayTubes,reflectedRays,reflectedFace,face,i,reflectionExist,diffractionExist,faceNumber1,faceNumber2);
//			GetDiffractionEdgeWhenThreeReflectedRaysExist(face[reflectedFace[0]],face[reflectedFace[1]],reflectedRays[0].originalPoint,reflectedRays[1].originalPoint,reflectedRays[2].originalPoint);
//		}
//		if (JudgeTwoTriangleFacesOnTheSameQuadrangle(reflectedFace[0],reflectedFace[2])&&(JudgeTwoTriangleFacesOnTheAdjacentQuadrangle(reflectedFace[0],reflectedFace[1])))
//		{
//			GetReflectedRayTubeWhenThreeReflectedRayExist(0,2,incidentRays,reflectedRayTubes,reflectedRays,reflectedFace,face,i,reflectionExist,diffractionExist,faceNumber1,faceNumber2);
//			GetDiffractionEdgeWhenThreeReflectedRaysExist(face[reflectedFace[0]],face[reflectedFace[2]],reflectedRays[0].originalPoint,reflectedRays[2].originalPoint,reflectedRays[1].originalPoint);
//		}
//		if (JudgeTwoTriangleFacesOnTheSameQuadrangle(reflectedFace[1],reflectedFace[2])&&(JudgeTwoTriangleFacesOnTheAdjacentQuadrangle(reflectedFace[0],reflectedFace[1])))
//		{
//			GetReflectedRayTubeWhenThreeReflectedRayExist(1,2,incidentRays,reflectedRayTubes,reflectedRays,reflectedFace,face,i,reflectionExist,diffractionExist,faceNumber1,faceNumber2);
//			GetDiffractionEdgeWhenThreeReflectedRaysExist(face[reflectedFace[1]],face[reflectedFace[2]],reflectedRays[1].originalPoint,reflectedRays[2].originalPoint,reflectedRays[0].originalPoint);
//		}
//	}
//}
//
//
//
//cudaError_t  GetReflectionAndDiffractionWithBuildingFace(TriangleRayTube *incidentRayTubes,int faceCount,int rayTubeCount,Face *buildingFace,TriangleRayTube *reflectedRayTubes,Edge *diffractionEdge,int *faceNumber1,int *faceNumber2,int *diffractionExist,int *reflectionExist)
//{
//	const int pointPerFace=9;
//	cudaError_t cudaStatus;
//
//	TriangleRayTube *device_incidentRayTubes=0;
//	TriangleRayTube *device_reflectedRayTubes=0;
//	Edge *device_edge=0;
//	Face *device_face=0;
//	//float *device_distance=0;
//	int *device_faceNumber1=0;
//	int *device_faceNumber2=0;
//	int *device_diffractionExist=0;
//	int *device_reflectionExist=0;
//	cudaStatus=cudaSetDevice(0);
//	if (cudaStatus!=cudaSuccess)
//	{
//		fprintf(stderr,"CUDA capable GPU is not available!");
//		goto Error;
//	}
//
//	cudaStatus=cudaMalloc((void**)&device_incidentRayTubes,rayTubeCount*sizeof(TriangleRayTube));
//	if (cudaStatus!=cudaSuccess)
//	{
//		fprintf(stderr,"device_incidentRayTubes cudaMalloc error!");
//		goto Error;
//	}
//	cudaStatus=cudaMalloc((void**)&device_reflectedRayTubes,rayTubeCount*2*sizeof(TriangleRayTube));
//	if (cudaStatus!=cudaSuccess)
//	{
//		fprintf(stderr,"device_reflectedRayTubes cudaMalloc error!");
//
//		goto Error;
//	}
//
//	cudaStatus=cudaMalloc((void**)&device_edge,rayTubeCount*sizeof(Edge));
//	if (cudaStatus!=cudaSuccess)
//	{
//		fprintf(stderr,"device_edge cudaMalloc error!");
//		goto Error;
//	}
//	cudaStatus=cudaMalloc((void**)&device_face,faceCount*sizeof(Face));
//	if (cudaStatus!=cudaSuccess)
//	{
//		fprintf(stderr,"device_face cudaMalloc error!");
//		goto Error;
//	}
//	cudaStatus=cudaMalloc((void**)&device_faceNumber1,rayTubeCount*sizeof(int));
//	if (cudaStatus!=cudaSuccess)
//	{
//		fprintf(stderr,"device_faceNumber1 cudaMalloc error!");
//		goto Error;
//	}
//	cudaStatus=cudaMalloc((void**)&device_faceNumber2,rayTubeCount*sizeof(int));
//	if (cudaStatus!=cudaSuccess)
//	{
//		fprintf(stderr,"device_faceNumber2 cudaMalloc error!");
//		goto Error;
//	}
//	cudaStatus=cudaMalloc((void**)&device_diffractionExist,rayTubeCount*sizeof(int));
//	if (cudaStatus!=cudaSuccess)
//	{
//		fprintf(stderr,"device_diffractionExist cudaMalloc error!");
//		goto Error;
//	}
//	cudaStatus=cudaMalloc((void**)&device_reflectionExist,rayTubeCount*sizeof(int));
//	if (cudaStatus!=cudaSuccess)
//	{
//		fprintf(stderr,"device_reflectionExis cudaMalloc error!");
//		goto Error;
//	}
//
//	cudaStatus=cudaMemcpy(device_incidentRayTubes,incidentRayTubes,rayTubeCount*sizeof(TriangleRayTube),cudaMemcpyHostToDevice);
//	if (cudaStatus!=cudaSuccess)
//	{
//		fprintf(stderr,"incidentRayTubes Memcpy failed!");
//	}
//	cudaStatus=cudaMemcpy(device_face,buildingFace,faceCount*sizeof(Face),cudaMemcpyHostToDevice);
//	if (cudaStatus!=cudaSuccess)
//	{
//		fprintf(stderr,"face Memcpy failed!");
//	}
//
//	const int num_blocks=32;
//	const int num_threads=640;
//
//	CalculateReflectionAndDiffractionWithBuildingFace<<<num_blocks,num_threads>>>(device_incidentRayTubes,faceCount,rayTubeCount,device_reflectedRayTubes,device_face,device_edge,device_faceNumber1,device_faceNumber2,device_reflectionExist,device_diffractionExist);
//
//	cudaStatus=cudaMemcpy(reflectedRayTubes,device_reflectedRayTubes,rayTubeCount*2*sizeof(TriangleRayTube),cudaMemcpyDeviceToHost);
//	if (cudaStatus!=cudaSuccess)
//	{
//		fprintf(stderr,"reflectedRayTubes cudaMemcpy failed!");
//	}
//	cudaStatus=cudaMemcpy(faceNumber1,device_faceNumber1,rayTubeCount*sizeof(int),cudaMemcpyDeviceToHost);
//	if (cudaStatus!=cudaSuccess)
//	{
//		fprintf(stderr,"faceNumber1 cudaMemcpy failed!");
//	}
//	cudaStatus=cudaMemcpy(faceNumber2,device_faceNumber2,rayTubeCount*sizeof(int),cudaMemcpyDeviceToHost);
//	if (cudaStatus!=cudaSuccess)
//	{
//		fprintf(stderr,"faceNumber2 cudaMemcpy failed!");
//	}
//	cudaStatus=cudaMemcpy(diffractionEdge,device_edge,rayTubeCount*sizeof(Edge),cudaMemcpyDeviceToHost);
//	if (cudaStatus!=cudaSuccess)
//	{
//		fprintf(stderr,"diffractionEdge cudaMemcpy failed!");
//	}
//	cudaStatus=cudaMemcpy(reflectionExist,device_reflectionExist,rayTubeCount*sizeof(int),cudaMemcpyDeviceToHost);
//	if (cudaStatus!=cudaSuccess)
//	{
//		fprintf(stderr,"reflectionExist cudaMemcpy failed!");
//	}
//	cudaStatus=cudaMemcpy(diffractionExist,device_diffractionExist,rayTubeCount*sizeof(int),cudaMemcpyDeviceToHost);
//	if (cudaStatus!=cudaSuccess)
//	{
//		fprintf(stderr,"diffractionExist cudaMemcpy failed!");
//	}
//	return cudaStatus;
//Error:
//	cudaFree(device_incidentRayTubes);
//	cudaFree(device_reflectedRayTubes);
//	cudaFree(device_edge);
//	cudaFree(device_face);
//	cudaFree(device_faceNumber1);
//	cudaFree(device_faceNumber2);
//	cudaFree(device_reflectionExist);
//	cudaFree(device_diffractionExist);
//}
