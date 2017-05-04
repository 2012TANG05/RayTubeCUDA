#include "BasicToolOnCPU.h"


double HAngleToRad(double angle)//角度转弧度
{
	double temp;
	temp=angle*PI;
	return temp/180;
}

float HCalculateDistanceOfBipartite(HPoint a,HPoint b)//计算两点间距离
{
	return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y)+(a.z-b.z)*(a.z-b.z));
}

void UitizeVector(float *a,float *b,float *c)//归一化向量
{
	float length=sqrt((*a)*(*a)+(*b)*(*b)+(*c)*(*c));
	*a=*a/length;
	*b=*b/length;
	*c=*c/length; 
}

Point GetReflectedVector(Point d,Point n)//根据反射面的单位法向量和入射向量得到反射向量
{
	Point reflectedPoint;
	float temp=2*(d.x*n.x+d.y*n.y+d.z*n.z);
	reflectedPoint.x=d.x-temp*n.x;
	reflectedPoint.y=d.y-temp*n.y;
	reflectedPoint.z=d.z-temp*n.z;
	return reflectedPoint;
}

Ray CalculateReflectedRayOnCPU(Ray incidentRay,Face *face,int faceCount,int *reflectedFace,int *flag)//单条射线与所有三角面判交
{
	Ray reflectedRay;
	Point defaultPoint;
	defaultPoint.x=0;defaultPoint.y=0;defaultPoint.z=0;
	reflectedRay.originalPoint=defaultPoint;
	reflectedRay.direction=defaultPoint;
	float t=50000;
 	for (int i=0;i<faceCount;i++)
	{
		float a1=-incidentRay.direction.x,a2=-incidentRay.direction.y,a3=-incidentRay.direction.z;
		float b1=face[i]. B.x-face[i].A.x,b2=face[i].B.y-face[i].A.y,b3=face[i].B.z-face[i].A.z;
		float c1=face[i]. C.x-face[i].A.x,c2=face[i].C.y-face[i].A.y,c3=face[i].C.z-face[i].A.z;
		float x1=incidentRay.originalPoint.x-face[i].A.x,x2=incidentRay.originalPoint.y-face[i].A.y,x3=incidentRay.originalPoint.z-face[i].A.z;
		float denominator=a1*(b2*c3-b3*c2)-b1*(a2*c3-a3*c2)+c1*(a2*b3-a3*b2);
		float t_numerator=x1*(b2*c3-b3*c2)-b1*(x2*c3-x3*c2)+c1*(x2*b3-x3*b2);
		float u_numerator=a1*(x2*c3-x3*c2)-x1*(a2*c3-a3*c2)+c1*(a2*x3-a3*x2);
		float v_numerator=a1*(b2*x3-b3*x2)-b1*(a2*x3-a3*x2)+x1*(a2*b3-a3*b2);
		if (abs(denominator)>0.000001)
		{
			float u=u_numerator/denominator;
			float v=v_numerator/denominator;
			int remain=i%12;
 			if(t_numerator/denominator<t&&t_numerator/denominator>1&&u>=0&&u<=1&&v>0&&(u+v)<1&&v<=1&&remain<8)
			{
				*flag=1;
				*reflectedFace=i;
				t=t_numerator/denominator;
				reflectedRay.originalPoint.x=u*b1+v*c1+face[i].A.x;
				reflectedRay.originalPoint.y=u*b2+v*c2+face[i].A.y;
				reflectedRay.originalPoint.z=u*b3+v*c3+face[i].A.z;
				Point n;
				n.x=b2*c3-b3*c2;n.y=b3*c1-b1*c3;n.z=b1*c2-c1*b2;
				UitizeVector(&n.x,&n.y,&n.z);
				reflectedRay.direction=GetReflectedVector(incidentRay.direction,n);
			}
		}
	}
	return reflectedRay;
}

Ray CalculateVirtualReflectedRayOnCPU(Ray incidentRay,Face face)//得到单条射线与某个三角面的虚拟反射射线
{
	Ray reflectedRay;
	Point defaultPoint;
	defaultPoint.x=0;defaultPoint.y=0;defaultPoint.z=0;
	reflectedRay.originalPoint=defaultPoint;
	reflectedRay.direction=defaultPoint;
	float t=50000;
	float a1=-incidentRay.direction.x,a2=-incidentRay.direction.y,a3=-incidentRay.direction.z;
	float b1=face.B.x-face.A.x,b2=face.B.y-face.A.y,b3=face.B.z-face.A.z;
	float c1=face.C.x-face.A.x,c2=face.C.y-face.A.y,c3=face.C.z-face.A.z;
	float x1=incidentRay.originalPoint.x-face.A.x,x2=incidentRay.originalPoint.y-face.A.y,x3=incidentRay.originalPoint.z-face.A.z;
	float denominator=a1*(b2*c3-b3*c2)-b1*(a2*c3-a3*c2)+c1*(a2*b3-a3*b2);
	float t_numerator=x1*(b2*c3-b3*c2)-b1*(x2*c3-x3*c2)+c1*(x2*b3-x3*b2);
	float u_numerator=a1*(x2*c3-x3*c2)-x1*(a2*c3-a3*c2)+c1*(a2*x3-a3*x2);
	float v_numerator=a1*(b2*x3-b3*x2)-b1*(a2*x3-a3*x2)+x1*(a2*b3-a3*b2);
	if (abs(denominator)>0.000001)
	{
		float u=u_numerator/denominator; 
		float v=v_numerator/denominator;
		if(t_numerator/denominator<t&&t_numerator/denominator>1)
		{
			t=t_numerator/denominator;
			reflectedRay.originalPoint.x=u*b1+v*c1+face.A.x;
			reflectedRay.originalPoint.y=u*b2+v*c2+face.A.y;
			reflectedRay.originalPoint.z=u*b3+v*c3+face.A.z;
			Point n;
			n.x=b2*c3-b3*c2;n.y=b3*c1-b1*c3;n.z=b1*c2-c1*b2;
			UitizeVector(&n.x,&n.y,&n.z);
			reflectedRay.direction=GetReflectedVector(incidentRay.direction,n);
		}
	}
	return reflectedRay;
}

Ray CalculateReflectedRayWithTerrainOnCPU(Ray incidentRay,Face *face,int faceCount,int *reflectedFace,int *flag)//单条射线与所有三角面判交
{
	Ray reflectedRay;
	Point defaultPoint;
	defaultPoint.x=0;defaultPoint.y=0;defaultPoint.z=0;
	reflectedRay.originalPoint=defaultPoint;
	reflectedRay.direction=defaultPoint;
	float t=50000;
 	for (int i=0;i<faceCount;i++)
	{
		float a1=-incidentRay.direction.x,a2=-incidentRay.direction.y,a3=-incidentRay.direction.z;
		float b1=face[i]. B.x-face[i].A.x,b2=face[i].B.y-face[i].A.y,b3=face[i].B.z-face[i].A.z;
		float c1=face[i]. C.x-face[i].A.x,c2=face[i].C.y-face[i].A.y,c3=face[i].C.z-face[i].A.z;
		float x1=incidentRay.originalPoint.x-face[i].A.x,x2=incidentRay.originalPoint.y-face[i].A.y,x3=incidentRay.originalPoint.z-face[i].A.z;
		float denominator=a1*(b2*c3-b3*c2)-b1*(a2*c3-a3*c2)+c1*(a2*b3-a3*b2);
		float t_numerator=x1*(b2*c3-b3*c2)-b1*(x2*c3-x3*c2)+c1*(x2*b3-x3*b2);
		float u_numerator=a1*(x2*c3-x3*c2)-x1*(a2*c3-a3*c2)+c1*(a2*x3-a3*x2);
		float v_numerator=a1*(b2*x3-b3*x2)-b1*(a2*x3-a3*x2)+x1*(a2*b3-a3*b2);
		if (abs(denominator)>0.000001)
		{
			float u=u_numerator/denominator;
			float v=v_numerator/denominator;
 			if(t_numerator/denominator<t&&t_numerator/denominator>1&&u>=0&&u<=1&&v>0&&(u+v)<1&&v<=1)
			{
				*flag=1;
				*reflectedFace=i;
				t=t_numerator/denominator;
				reflectedRay.originalPoint.x=u*b1+v*c1+face[i].A.x;
				reflectedRay.originalPoint.y=u*b2+v*c2+face[i].A.y;
				reflectedRay.originalPoint.z=u*b3+v*c3+face[i].A.z;
				Point n;
				n.x=b2*c3-b3*c2;n.y=b3*c1-b1*c3;n.z=b1*c2-c1*b2;
				UitizeVector(&n.x,&n.y,&n.z);
				reflectedRay.direction=GetReflectedVector(incidentRay.direction,n);
			}
		}
	}
	return reflectedRay;
}

//计算两射线交点 （用于求绕射虚拟发射点）
Point GetIntersectionOfRays(Ray ray1,Ray ray2)
{
	  float a1=ray1.direction.x,a2=ray1.direction.y,a3=ray1.direction.z;
	  float b1=-ray2.direction.x,b2=-ray2.direction.y,b3=-ray2.direction.z;
	  float c1=ray2.originalPoint.x-ray1.originalPoint.x,c2=ray2.originalPoint.y-ray1.originalPoint.y,c3=ray2.originalPoint.z-ray1.originalPoint.z;
	  float det=a1*(b2*c3-b3*c2)-b1*(a2*c3-a3*c2)+c1*(a2*b3-a3*b2);
	  Point intersection;
	  intersection.x=0;intersection.y=0;intersection.z=0;
	  if(abs(det)<0.001)
	  {
		  if (abs(c1*b2-c2*b1)<0.001)
		  {
			  intersection.x=ray1.originalPoint.x;
			  intersection.y=ray1.originalPoint.y;
			  intersection.z=ray1.originalPoint.z;

		  }
		  else if ((abs(b2*a1-b1*a2)>0.00001))
		  {

			  float u=(c1*b2-c2*b1)/(b2*a1-b1*a2);
			  //float v=(c2*a1-c1*a2)/(b2*a1-b1*a2);
			  intersection.x=ray1.originalPoint.x+u*a1;
			  intersection.y=ray1.originalPoint.y+u*a2;
			  intersection.z=ray1.originalPoint.z+u*a3;
		  }
	  }
	  else
	  {
		  intersection.x=0;intersection.y=0;intersection.z=0;
	  }
	  return intersection;
}