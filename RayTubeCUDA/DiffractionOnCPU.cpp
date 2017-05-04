#include "DiffractionOnCPU.h"

//round����
double round(double r)
{
    return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
}

//��λ������(Ҫ��)
Point GetNormalizationVectorOnCPU(Point vector)
{
	float sum=sqrt(pow(vector.x,2)+pow(vector.y,2)+pow(vector.z,2));
	Point unitVector;
	unitVector.x=vector.x/sum;
	unitVector.y=vector.y/sum;
	unitVector.z=vector.z/sum;
	return unitVector;
}

//��������
Point GetReverseVectorOnCPU(Point vector)
{
	vector.x=-vector.x;
	vector.y=-vector.y;
	vector.z=-vector.z;
	return vector;
}



//��һ���������һ����ĶԳƵ�
Point GetSymmetryPointOnCPU(Point pointToCal, Point centerPoint)
{
	Point result;
	result.x= 2*centerPoint.x-pointToCal.x;
	result.y= 2*centerPoint.y-pointToCal.y;
	result.z= 2*centerPoint.z-pointToCal.z;
	return result;
}

//��������ģ
float MagOnCPU(Point vector)
{
	return sqrt(pow(vector.x,2)+pow(vector.y,2)+pow(vector.z,2));
}

//����
float DotMultiplyOnCPU(Point vector1, Point vector2)
{
	return vector1.x*vector2.x+vector1.y*vector2.y+vector1.z*vector2.z;
}

//����
Point CrossMultiplyOnCPU(Point vector1, Point vector2)
{
	Point result;
	float xTemp, yTemp, zTemp, multipled;
	xTemp = vector1.y*vector2.z-vector1.z*vector2.y;
	yTemp = vector1.z*vector2.x-vector1.x*vector2.z;
	zTemp = vector1.x*vector2.y-vector2.y*vector1.x;
	multipled = sqrt(pow(xTemp,2)+pow(yTemp,2)+pow(zTemp,2));
	if(multipled==0)
	{
		result.x=0;
		result.y=0;
		result.z=0;
	}
	else
	{
		result.x=xTemp/multipled;
		result.y=yTemp/multipled;
		result.z=zTemp/multipled;
	}
	if(abs(result.x)<0.000001)
		result.x=0;
	if(abs(result.y)<0.000001)
		result.y=0;
	if(abs(result.z)<0.000001)
		result.z=0;
	return result;
}

//���ƽ�淨����
Point GetNormalVectorOnCPU(Face face)
{
	Point vectorAB;
	Point vectorBC;
	vectorAB.x=face.B.x-face.A.x;
	vectorAB.y=face.B.y-face.A.y;
	vectorAB.z=face.B.z-face.A.z;
	vectorBC.x=face.C.x-face.B.x;
	vectorBC.y=face.C.y-face.B.y;
	vectorBC.z=face.C.z-face.B.z;
	return CrossMultiplyOnCPU(vectorAB,vectorBC);
}

//��ƽ�淽��Ax+By+Cz+D=0���ĸ�ϵ��A,B,C,D
float* GetFaceEquationFactorOnCPU(Face face)
{
	float para[4];
	Point unitVector=GetNormalVectorOnCPU(face);
	unitVector=GetNormalizationVectorOnCPU(unitVector);
	para[0]=unitVector.x;
	para[1]=unitVector.y;
	para[2]=unitVector.z;
	para[3]=-(unitVector.x*face.A.x+unitVector.y*face.A.y+unitVector.z*face.A.z);
	return para;
}

//�жϵ��Ƿ���ƽ����
bool JudgeIfPointInFaceOnCPU(Point point, Face face)
{
	float* faceFactor=(float*)malloc(4);//ƽ�淽�̵�4��ϵ��ABCD
	faceFactor=GetFaceEquationFactorOnCPU(face);
	float faceEquation=faceFactor[0]*point.x+faceFactor[1]*point.y+faceFactor[2]*point.z+faceFactor[3];
	if(abs(faceEquation<0.001))
	{
//		delete [] faceFactor;
		return true;
	}
	else
	{
//		delete [] faceFactor;
		return false;
	}
}

//�ж����������Ƿ�ƽ����ͬ��
bool IsParallelAndSamedirectionOnCPU(Point vector1, Point vector2)
{
	float temp=(vector1.x*vector2.x+vector1.y*vector2.y+vector1.z*vector2.z)/(MagOnCPU(vector1)*MagOnCPU(vector2));
	if(abs(temp-1)<=0.00001)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//�õ�һ����ķ���������p1Ϊ�µ㣬p2Ϊ�ϵ㣬p1ָ��p2��
Point GetDirectionVectorOfEdgeOnCPU(Edge edge)
{
	Point direction;
	direction.x=edge.p2.x-edge.p1.x;
	direction.y=edge.p2.y-edge.p1.y;
	direction.z=edge.p2.z-edge.p1.z;
	return direction;
}

//�ж��������Ƿ�Ϊͬһ��
bool IsTwoPointSameOnCPU(Point A,Point B)
{
	if ((abs(A.x-B.x)<0.00001)&&(abs(A.y-B.y)<0.00001)&&(abs(A.z-B.z)<0.00001))
	{
		return true;
	}
	return false;
}

//�ж�һ�����Ƿ�Ϊһ��������Ķ���֮һ
bool IsTheSamePointOfAdjacentExistOnCPU(Point A,Face face)
{
	if (IsTwoPointSameOnCPU(A,face.A)||IsTwoPointSameOnCPU(A,face.B)||IsTwoPointSameOnCPU(A,face.C))
	{
		return true;
	}
	return false;
}

//�õ�������������Ĺ�����
Edge GetEdgeOfAdjacentFacesOnCPU(Face face1,Face face2)
{
	Edge edge;
	if(IsTheSamePointOfAdjacentExistOnCPU(face1.A,face2)&&IsTheSamePointOfAdjacentExistOnCPU(face1.B,face2)&&IsTheSamePointOfAdjacentExistOnCPU(face1.C,face2))
	{
		//��ʾ��������Ϊͬһ������
		edge.p1.x=6666;
		edge.p1.y=6666;
		edge.p1.z=6666;
		edge.p2.x=6666;
		edge.p2.y=6666;
		edge.p2.z=6666;
		printf("������������Ϊͬһ�����棡\n");
		return edge;
	}
	else if (IsTheSamePointOfAdjacentExistOnCPU(face1.A,face2)&&IsTheSamePointOfAdjacentExistOnCPU(face1.B,face2))
	{
		edge.p1=face1.A;
		edge.p2=face1.B;
		return edge;
	}
	else if (IsTheSamePointOfAdjacentExistOnCPU(face1.A,face2)&&IsTheSamePointOfAdjacentExistOnCPU(face1.C,face2))
	{
		edge.p1=face1.A;
		edge.p2=face1.C;
		return edge;
	}
	else if (IsTheSamePointOfAdjacentExistOnCPU(face1.B,face2)&&IsTheSamePointOfAdjacentExistOnCPU(face1.C,face2))
	{
		edge.p1=face1.B;
		edge.p2=face1.C;
		return edge;
	}
	else
	{
		//��ʾ���������޹�����
		edge.p1.x=8888;
		edge.p1.y=8888;
		edge.p1.z=8888;
		edge.p2.x=8888;
		edge.p2.y=8888;
		edge.p2.z=8888;
		printf("�ж�������ʱ�����������������޹����⣡\n");
		return edge;
	}
}

//����������г���һ�������ʣ��һ��
Point GetRemainPointOnFaceOnCPU(Face face, Edge sameEdge)
{
	bool flag[3];
	for(int i=0;i<3;i++)
	{
		flag[i]=true;
	}
	//����6��if��������Ϊ�棬����ʣ��һ��
	if(IsTwoPointSameOnCPU(face.A, sameEdge.p1))
	{
		flag[0]=false;
	}
	if(IsTwoPointSameOnCPU(face.B, sameEdge.p1))
	{
		flag[1]=false;
	}
	if(IsTwoPointSameOnCPU(face.C, sameEdge.p1))
	{
		flag[2]=false;
	}
	if(IsTwoPointSameOnCPU(face.A, sameEdge.p2))
	{
		flag[0]=false;
	}
	if(IsTwoPointSameOnCPU(face.B, sameEdge.p2))
	{
		flag[1]=false;
	}
	if(IsTwoPointSameOnCPU(face.C, sameEdge.p2))
	{
		flag[2]=false;
	}
	if(flag[0])
	{
		return face.A;
	}
	if(flag[1])
	{
		return face.B;
	}
	if(flag[2])
	{
		return face.C;
	}
	printf("���������г��������һ��GetRemainPointOnFace()����\n");
}
//���ֱ����һ�㵽��ֱ�ߵĴ��㣬pΪֱ����һ�㣬���ش�������
//Point GetDropFoot(Point p)
//{
//	
//}

//�����ⷢ��㵽��������¹�����������
Ray CreateNewInRayOnCPU(Point originPoint, Point diffractionPoint)
{
	Ray newInRay;
	newInRay.originalPoint=originPoint;
	newInRay.direction.x=diffractionPoint.x-originPoint.x;
	newInRay.direction.y=diffractionPoint.y-originPoint.y;
	newInRay.direction.z=diffractionPoint.z-originPoint.z;
	return newInRay;
}

//������ֱ�ߵļн� �����������ڻ�ȡ���������ߵļн�
float GetAngleOfTwoStraightLinesOnCPU(Point vector1, Point vector2)
{
	float dotMultiplied= DotMultiplyOnCPU(vector1,vector2);
	float tempValue= sqrt(pow(vector1.x,2)+pow(vector1.y,2)+pow(vector1.z,2))*sqrt(pow(vector2.x,2)+pow(vector2.y,2)+pow(vector2.z,2));
	if(tempValue==0)
	{
		//��Ϊ���ܴ�LOG���˴����Լ�����
		return 90;
	}
	float nTemp=dotMultiplied/tempValue;
	if(nTemp<-1)
	{
		nTemp=-1;
	}
	if(nTemp>1)
	{
		nTemp=1;
	}
	float phase = (acos(nTemp)/PI)*(180.0);
	return phase;
}

//������Բ��Բ�� ��������������������ѡһ������������ͬ���Ҳ��������ĵ�,һ��ѡ�������ĳ������,���н�Ϊ90��ʱ����������ΪԲ�̣�Բ��Ϊ�����
Point GetCircleCenterPointOnCPU(float angleOfRayAndSameLine,Ray incidentRay, Edge sameEdge, Point diffractionPoint)
{
	Point sameEdgePoint1=sameEdge.p1;
	Point sameEdgePoint2=sameEdge.p2;
	if(abs(angleOfRayAndSameLine-90)<0.0001)//�����������ֱ
	{
		return diffractionPoint;
	}
	//��������պ�Ϊ������Ķ��㣬�򱶳������������ߣ���������������ֱ�������ߣ�����ΪԲ��
	else if(IsTwoPointSameOnCPU(diffractionPoint,sameEdge.p1)||IsTwoPointSameOnCPU(diffractionPoint,sameEdge.p2))
	{
		if(IsTwoPointSameOnCPU(diffractionPoint,sameEdge.p1))
		{
			Point p;
			p=GetSymmetryPointOnCPU(incidentRay.originalPoint, sameEdge.p1);//������
			//�������򹫹�������ֱ��������
			Point origin;
			origin.x=sameEdge.p2.x;
			origin.y=sameEdge.p2.y;
			origin.z=sameEdge.p2.z;
			Point direction;
			direction.x=sameEdge.p1.x-sameEdge.p2.x;
			direction.y=sameEdge.p1.y-sameEdge.p2.y;
			direction.z=sameEdge.p1.z-sameEdge.p2.z;
			Point end;
			end.x=origin.x+direction.x*100;
			end.y=origin.y+direction.y*100;
			end.z=origin.z+direction.z*100;
			float dx = origin.x-end.x;
			float dy = origin.y-end.y;
			float dz = origin.z-end.z;
			float u = (p.x - origin.x) * (origin.x - end.x) + (p.y - origin.y) * (origin.y - end.y) + (p.z - origin.z) * (origin.z - end.z);
            u = u / ((dx * dx) + (dy * dy) + (dz * dz));
			Point result;
			result.x = origin.x + u*dx;
			result.y = origin.y + u*dy;
			result.z = origin.z + u*dz;
			return result;
		}
		if(IsTwoPointSameOnCPU(diffractionPoint,sameEdge.p2))
		{
			Point p;
			p=GetSymmetryPointOnCPU(incidentRay.originalPoint, sameEdge.p2);//������
			//�������򹫹�������ֱ��������
			Point origin;
			origin.x=sameEdge.p1.x;
			origin.y=sameEdge.p1.y;
			origin.z=sameEdge.p1.z;
			Point direction;
			direction.x=sameEdge.p2.x-sameEdge.p1.x;
			direction.y=sameEdge.p2.y-sameEdge.p1.y;
			direction.z=sameEdge.p2.z-sameEdge.p1.z;
			Point end;
			end.x=origin.x+direction.x*100;
			end.y=origin.y+direction.y*100;
			end.z=origin.z+direction.z*100;
			float dx = origin.x-end.x;
			float dy = origin.y-end.y;
			float dz = origin.z-end.z;
			float u = (p.x - origin.x) * (origin.x - end.x) + (p.y - origin.y) * (origin.y - end.y) + (p.z - origin.z) * (origin.z - end.z);
            u = u / ((dx * dx) + (dy * dy) + (dz * dz));
			Point result;
			result.x = origin.x + u*dx;
			result.y = origin.y + u*dy;
			result.z = origin.z + u*dz;
			return result;
		}
	}
	else//��Բ����Ϊ����������ͬһ��Ķ���
	{
		Point sameEdgeVector;
		sameEdgeVector.x=sameEdgePoint2.x-sameEdgePoint1.x;
		sameEdgeVector.y=sameEdgePoint2.y-sameEdgePoint1.y;
		sameEdgeVector.z=sameEdgePoint2.z-sameEdgePoint1.z;
		float dotMultiplied=DotMultiplyOnCPU(incidentRay.direction, sameEdgeVector);
		if(dotMultiplied>0)//������ͬ��
		{
			return sameEdgePoint2;
		}
		if(dotMultiplied<0)//����������
		{
			return sameEdgePoint1;
		}
		else
		{
			Point tmp;
			tmp.x=5555;
			tmp.y=5555;
			tmp.z=5555;
			return tmp;
		}
	}
}

//������Բ�İ뾶
float GetCircleRadiusOnCPU(float angleOfRayAndSameLine, Point diffractionPoint, Point circleCenterPoint)
{
	float circleRadius;
	if(abs(angleOfRayAndSameLine-90)<0.0001)//���ߴ�ֱ������
	{
		circleRadius=1;
	}
	else
	{
		//tan��Ϊ�۽�ʱ���п��ܳ��ָ������������������������ߵķ������������������ԼӾ���ֵ
		circleRadius=abs(CalculateDistanceOfBipartite(diffractionPoint,circleCenterPoint)*tan(angleOfRayAndSameLine*PI/180));
	}
	return circleRadius;
}

//��Բ����ƽ���ϵ�һ������(����) �������Ϊ��������������е�����һ����
Point GetVectorInThePlaneOfCircleOnCPU(Edge sameEdge, Face oneAdjacentTriangle)
{
	/*Point diffractionEdgePoint1 = sameEdge.p1;
	Point diffractionEdgePoint2 = sameEdge.p2;
	if(diffractionEdgePoint2.x-diffractionEdgePoint1.x==0 && diffractionEdgePoint2.y-diffractionEdgePoint1.y==0)
	{
		Point unitCircleVectorU;
		unitCircleVectorU.x=1;
		unitCircleVectorU.y=0;
		unitCircleVectorU.z=0;
		return unitCircleVectorU;
	}
	else
	{*/
		return GetNormalizationVectorOnCPU(GetNormalVectorOnCPU(oneAdjacentTriangle));
	//}
}

//����Բ��ƽ���ϵ�һ����ƽ�淨������ƽ��ĳ������������������
Point GetAnotherVectorInThePlaneOfCircleOnCPU(Edge sameEdge, Point vectorU)
{
	Point diffractionEdgePoint1 = sameEdge.p1;
	Point diffractionEdgePoint2 = sameEdge.p2;
	Point sameEdgeVector;
	sameEdgeVector.x=diffractionEdgePoint2.x-diffractionEdgePoint1.x;
	sameEdgeVector.y=diffractionEdgePoint2.y-diffractionEdgePoint1.y;
	sameEdgeVector.z=diffractionEdgePoint2.z-diffractionEdgePoint1.z;
	/*if(diffractionEdgePoint2.x-diffractionEdgePoint1.x==0 && diffractionEdgePoint2.y-diffractionEdgePoint1.y==0)
	{
		Point unitCircleVectorV;
		unitCircleVectorV.x=0;
		unitCircleVectorV.y=1;
		unitCircleVectorV.z=0;
		return unitCircleVectorV;
	}
	else*/
	{
		//����һ����1������������������������������Ҳ��Բ���ڵ�ƽ����
		Point circleVectorV;
		circleVectorV.x=sameEdgeVector.y*vectorU.z-sameEdgeVector.z*vectorU.y;
		circleVectorV.y=sameEdgeVector.z*vectorU.x-sameEdgeVector.x*vectorU.z;
		circleVectorV.z=sameEdgeVector.x*vectorU.y-sameEdgeVector.y*vectorU.x;
		return GetNormalizationVectorOnCPU(circleVectorV);
	}
}

//��Բ��������Ľ��㣨���䣩
/// <param name="terTri">������</param>
/// <param name="centerPoint">Բ��</param>
/// <param name="radius">Բ�İ뾶</param>
/// <param name="vectorU">Բ����ƽ���һ������U������ƽ��ķ�������ֱ</param>
/// <param name="vectorV">Բ����ƽ���һ������V,����U��ƽ�淨�����໥��ֱ</param>
/// <param name="triPoint">��������������������˵������һ����</param>
/// <returns>����Բ��������Ľ���</returns>
Point GetCrossPointOfCircleWithTerOnCPU(Face terTri, Point centerPoint, float radius, Point vectorU, Point vectorV, Point triPoint)
{
	//������ �������õ�һ���ڸ߿յ�����
	Point defaultPoint;
	defaultPoint.x=7777;
	defaultPoint.y=7777;
	defaultPoint.z=7777;
	//�ο�Rays.cs�е�ͬ�����������巽���ڸöδ�����
	Point crossPoints[4];
	bool crossPointFlags[4];
	for(int i = 0; i < 4; i++)//����־λ����ֵ��Ĭ��ȫΪtrue��֮��������޳�
	{
		crossPointFlags[i]=true;
	}
	if(radius<1)//�뾶̫С�����ж������������ð뾶��С��1
	{
		radius=1;
	}
	Point triVector= GetNormalVectorOnCPU(terTri);//��ȡ������ķ�����
	float D = -(triVector.x*terTri.A.x+triVector.y*terTri.A.y+triVector.z*terTri.A.z);
	float M = radius*(triVector.x*vectorU.x+triVector.y*vectorU.y+triVector.z*vectorU.z);
	float N = radius*(triVector.x*vectorV.x+triVector.y*vectorV.y+triVector.z*vectorV.z);
	float sint[2];
	sint[0]=round(sqrt(pow(N,2)/(pow(M,2)+pow(N,2))));
	sint[1]=round(-sqrt(pow(N,2)/(pow(M,2)+pow(N,2))));
	for(int i=0; i<2; i++)//���ڷֱ治��Ƕ�t�ķ�Χ�����Խ�cost��1-sint^2�����ķ����󣬲��õ�4���㣬������������ƽ����
	{
		if((-1<=sint[i])&&(sint[i]<=1))//��ķ�Χ��-1��1֮��ʱ
		{
			
			float cost[2];
			cost[0]=sqrt(1-pow(sint[i],2));
			cost[1]=-sqrt(1-pow(sint[i],2));
			Point crossPoint1;
			crossPoint1.x=centerPoint.x+radius*(vectorU.x*sint[i]+vectorV.x*cost[0]);
			crossPoint1.y=centerPoint.y+radius*(vectorU.y*sint[i]+vectorV.y*cost[0]);
			crossPoint1.z=centerPoint.z+radius*(vectorU.z*sint[i]+vectorV.z*cost[0]);
			crossPoints[2*i] = crossPoint1;
			Point crossPoint2;
			crossPoint2.x=centerPoint.x+radius*(vectorU.x*sint[i]+vectorV.x*cost[1]);
			crossPoint2.y=centerPoint.y+radius*(vectorU.y*sint[i]+vectorV.y*cost[1]);
			crossPoint2.z=centerPoint.z+radius*(vectorU.z*sint[i]+vectorV.z*cost[1]);
			crossPoints[2*i + 1] = crossPoint2;
		}
		else
		{
			//����
			printf("��Բ�������潻��ʱ����\n");
			return defaultPoint;
		}
	}
	//ɾ������������ƽ���ϵĽ���
	for(int i=0;i<4;i++)
	{
		if(!JudgeIfPointInFaceOnCPU(crossPoints[i], terTri))
		{
			crossPointFlags[i] = false;
		}
	}
	//���ڵ�thi��Ϊ90��ʱ����õĵ������������ظ��ĵ㣬��Ҫ����ɾ��
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3-i;j++)
		{
			if(IsTwoPointSameOnCPU(crossPoints[j], crossPoints[j+1]))
			{
				if(crossPointFlags[j]&&crossPointFlags[j+1])//��ͬ�ĵ��޳�����һ��
				{
					crossPointFlags[j+1]=false;
				}
			}
		}
	}
	//Բ���������������һ��Ľ���
	for(int i=0;i<3;i++)
	{
		for(int j=i+1;j<4;j++)
		{
			if(crossPointFlags[i]&&crossPointFlags[j])//ȡͬ���
			{
				if(CalculateDistanceOfBipartite(crossPoints[i],triPoint)<CalculateDistanceOfBipartite(crossPoints[j],triPoint))
				{
					return crossPoints[i];
				}
				else
				{
					return crossPoints[j];
				}
			}
		}
	}
	//else if((!cp1)&&cp2)
	//{
	//	crossPoint2=GetSymmetryPointOnCPU(crossPoint2, centerPoint);//�Ե�ȡ��
	//	return crossPoint2;
	//}
	{
		//����
		return defaultPoint;
	}
}

//������Բ����������ķֲ���������ϵ
/// <param name="unitVectorU">Բ����ƽ����һ������U</param>
/// <param name="unitVectorV">Բ����ƽ����һ������V</param>
/// <param name="circleCenterPoint">Բ�̵�Բ��</param>
/// <param name="crossPointOfFace0AndCircle">Բ����������0����ǰ�棩�Ľ���</param>
/// <param name="crossPointOfFace1AndCircle">Բ����������1�������棩�Ľ���</param>
/// <param name="verticalVector">Բ����ƽ��ķ����������µ�Բ��ָ���ϵ�Բ��</param>
/// <param name="angleOfTriangles">����������ļн�</param>
void SetUnitVectorVnCirclePlaneOnCPU(Point &unitVectorU, Point &unitVectorV, Point circleCenterPoint,
	Point crossPointOfFace0AndCircle, Point crossPointOfFace1AndCircle, Point verticalVector, float angleOfTriangles)
{
	unitVectorU.x=crossPointOfFace0AndCircle.x-circleCenterPoint.x;
	unitVectorU.y=crossPointOfFace0AndCircle.y-circleCenterPoint.y;
	unitVectorU.z=crossPointOfFace0AndCircle.z-circleCenterPoint.z;
	unitVectorU=GetNormalizationVectorOnCPU(unitVectorU);//����0��ȡvectorU
	unitVectorV=GetNormalizationVectorOnCPU(GetReverseVectorOnCPU(CrossMultiplyOnCPU(unitVectorU,verticalVector)));
	if(!IsParallelAndSamedirectionOnCPU(verticalVector,CrossMultiplyOnCPU(unitVectorU,unitVectorV)))//��֤���յѿ�������ϵ��ȡ
	{
		unitVectorV=GetReverseVectorOnCPU(unitVectorV);
	}
	Point otherVectorU;
	otherVectorU.x=crossPointOfFace1AndCircle.x-circleCenterPoint.x;
	otherVectorU.y=crossPointOfFace1AndCircle.y-circleCenterPoint.y;
	otherVectorU.z=crossPointOfFace1AndCircle.z-circleCenterPoint.z;
	otherVectorU=GetNormalizationVectorOnCPU(otherVectorU);
	if(angleOfTriangles < 90)
	{
		if(abs(GetAngleOfTwoStraightLinesOnCPU(unitVectorV,otherVectorU) + angleOfTriangles - 90)<0.00001)
		{
			unitVectorU = otherVectorU;
			unitVectorV = CrossMultiplyOnCPU(verticalVector,unitVectorU);
		}
	}
	else if(angleOfTriangles>90)
	{
		if(abs(angleOfTriangles - GetAngleOfTwoStraightLinesOnCPU(unitVectorV,otherVectorU) - 90)<0.00001)
		{
			unitVectorU = otherVectorU;
			unitVectorV = CrossMultiplyOnCPU(verticalVector,unitVectorU);
		}
	}
	else
	{
		if(IsParallelAndSamedirectionOnCPU(unitVectorV,otherVectorU))
		{
			unitVectorV = GetReverseVectorOnCPU(unitVectorU);
			unitVectorU=otherVectorU;
		}
	}
}

//��������ĽǶ���Բ�ϵ�һ�� arcAngle�Ի����Ʊ�ʾ
Point GetPointInCircumferenceOnCPU(Point circleCenterPoint, float circleRadius, Point unitVectorU, Point unitVectorV, float arcAngle)
{
	//����������ʱ�룩ת
	Point circumferencePoint;
	circumferencePoint.x = circleCenterPoint.x + circleRadius * (unitVectorU.x * cos(arcAngle) + unitVectorV.x * sin(arcAngle));
	circumferencePoint.y = circleCenterPoint.y + circleRadius * (unitVectorU.y * cos(arcAngle) + unitVectorV.y * sin(arcAngle));
	circumferencePoint.z = circleCenterPoint.z + circleRadius * (unitVectorU.z * cos(arcAngle) + unitVectorV.z * sin(arcAngle));
	return circumferencePoint;
}

//���������߽��� ���������������ⷢ��㣩
Point GetIntersectionOfTwoRaysOnCPU(Ray ray1,Ray ray2)
{
	  float a1=ray1.direction.x,a2=ray1.direction.y,a3=ray1.direction.z;
	  float b1=-ray2.direction.x,b2=-ray2.direction.y,b3=-ray2.direction.z;
	  float c1=ray2.originalPoint.x-ray1.originalPoint.x,c2=ray2.originalPoint.y-ray1.originalPoint.y,c3=ray2.originalPoint.z-ray1.originalPoint.z;
	  float det=a1*(b2*c3-b3*c2)-b1*(a2*c3-a3*c2)+c1*(a2*b3-a3*b2);
	  Point intersection;
	  if(abs(det)<0.00001)
	  {
		  if (c1*b2-c2*b1<0.00001)
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

//��ĳһ���ǶȻ��ֻ������Բ�ϵĵ�
Point *GetcircumPointOfTheCircleOnCPU(Edge sameEdge, Point circleCenterPoint, float circleRadius, 
	Point unitCircleVectorU, Point unitCircleVectorV, float circleRange, int circumPointCount)
{
	float angleOfTwoCircumPoint = circleRange * PI /(circumPointCount * 180);//ȡ��ĽǶ�
	struct Point *circumPoints=new struct Point[circumPointCount];
	//��һ����ȡһ��΢С�Ƕȣ����������غ�
	circumPoints[0] = GetPointInCircumferenceOnCPU(circleCenterPoint, circleRadius, unitCircleVectorU, unitCircleVectorV, 0.01);
	//��Բ��ȡ�㣬���붯̬����
	for (int i = 1; i < circumPointCount-1; i++)
	{
		circumPoints[i] = GetPointInCircumferenceOnCPU(circleCenterPoint, circleRadius, unitCircleVectorU, unitCircleVectorV, i * angleOfTwoCircumPoint);
	}
	//���һ�������һ��΢С�Ƕȣ����������غ�
	circumPoints[circumPointCount-1] = GetPointInCircumferenceOnCPU(circleCenterPoint, circleRadius, 
		unitCircleVectorU, unitCircleVectorV, circleRange * PI/180-0.01);
	return circumPoints;
}

void GenerateDiffractionRaysOnCPU(TriangleRayTube *incidentRayTubes, Edge *diffractionEdge, 
	Face *diffractionFace1, Face *diffractionFace2, QuadrangleRayTube *diffractionRayTubes, unsigned const int diffractionRayCount,
	unsigned const int raysToBeDiffractedCnt)
{
	//int maxidx = sizeof(incidentRayTubes)/sizeof(incidentRayTubes[0]);
	int maxidx = raysToBeDiffractedCnt;
	struct Ray *diffractionRays1 = new struct Ray[maxidx*diffractionRayCount];
	struct Ray *diffractionRays2 = new struct Ray[maxidx*diffractionRayCount];
	for(int idx=0; idx<maxidx; idx++)
	{
		Point originPoint = GetIntersectionOfTwoRaysOnCPU(incidentRayTubes[idx].ray1, incidentRayTubes[idx].ray2);//�������ⷢ���
		Ray inRay1=CreateNewInRayOnCPU(originPoint, diffractionEdge[idx].p1);//�����ⷢ��㵽�����1���¹���һ��������
		Ray inRay2=CreateNewInRayOnCPU(originPoint, diffractionEdge[idx].p2);//�����ⷢ��㵽�����2���¹���һ��������
		float angleOfRay1AndEdge=GetAngleOfTwoStraightLinesOnCPU(inRay1.direction, GetDirectionVectorOfEdgeOnCPU(diffractionEdge[idx]));//��ȡ����1�����ߵļн�
		float angleOfRay2AndEdge=GetAngleOfTwoStraightLinesOnCPU(inRay2.direction, GetDirectionVectorOfEdgeOnCPU(diffractionEdge[idx]));//��ȡ����2�����ߵļн�
		Edge sameEdge=GetEdgeOfAdjacentFacesOnCPU(diffractionFace1[idx],diffractionFace2[idx]);//���ߵ������⣬diffractionEdgeΪ���Ӽ�
		if(sameEdge.p1.x==8888)
		{
			printf("CPU��˺��������������������治���ڵ������\n");
		}
		Point circleCenterPoint1 = GetCircleCenterPointOnCPU(angleOfRay1AndEdge, inRay1, sameEdge, diffractionEdge[idx].p1);//��������ѡһ������������ͬ���Ҳ��������ĵ�,һ��ѡ�������ĳ������,���н�Ϊ90��ʱ����������ΪԲ�̣�Բ��Ϊ�����
		Point circleCenterPoint2 = GetCircleCenterPointOnCPU(angleOfRay2AndEdge, inRay2, sameEdge, diffractionEdge[idx].p2);
		float circleRadius1 = GetCircleRadiusOnCPU(angleOfRay1AndEdge, diffractionEdge[idx].p1, circleCenterPoint1);//����Բ1�İ뾶
		float circleRadius2 = GetCircleRadiusOnCPU(angleOfRay2AndEdge, diffractionEdge[idx].p2, circleCenterPoint2);//����Բ2�İ뾶
		Point circleVectorU1 = GetVectorInThePlaneOfCircleOnCPU(sameEdge, diffractionFace1[idx]);//����Բ����ƽ���ϵ�һ������
		Point circleVectorU2 = circleVectorU1;//���һ����������diffractionFace1����Ϊֻ��Ҫ��������һ��������
		Point circleVectorV1 = GetAnotherVectorInThePlaneOfCircleOnCPU(sameEdge, circleVectorU1);//����һ����ǰ����������������������������������������Ҳ��Բ���ڵ�ƽ����
		Point circleVectorV2 = GetAnotherVectorInThePlaneOfCircleOnCPU(sameEdge, circleVectorU2);
		//������Բ������������Ľ���
		Point triPoint1;
		Point triPoint2;
		triPoint1 = GetRemainPointOnFaceOnCPU(diffractionFace1[idx], sameEdge);
		triPoint2 = GetRemainPointOnFaceOnCPU(diffractionFace2[idx], sameEdge);
		Point crossPointOfFace1AndCircle1 = GetCrossPointOfCircleWithTerOnCPU(diffractionFace1[idx], circleCenterPoint1, 
			circleRadius1, circleVectorU1, circleVectorV1, triPoint1);//Բ1��������1һ��Ľ���
		Point crossPointOfFace2AndCircle1 = GetCrossPointOfCircleWithTerOnCPU(diffractionFace2[idx], circleCenterPoint1, 
			circleRadius1, circleVectorU1, circleVectorV1, triPoint2);//Բ1��������2һ��Ľ���
		Point crossPointOfFace1AndCircle2 = GetCrossPointOfCircleWithTerOnCPU(diffractionFace1[idx], circleCenterPoint2, 
			circleRadius2, circleVectorU2, circleVectorV2, triPoint1);//Բ2��������1һ��Ľ���
		Point crossPointOfFace2AndCircle2 = GetCrossPointOfCircleWithTerOnCPU(diffractionFace2[idx], circleCenterPoint2, 
			circleRadius2, circleVectorU2, circleVectorV2, triPoint2);//Բ2��������2һ��Ľ���
		Point circlePointVector1;
		Point circlePointVector2;
		if(IsTwoPointSameOnCPU(circleCenterPoint1, diffractionEdge[idx].p1))//�����������Բ�ģ���ȡ������Ϊ��������
		{
			circlePointVector1 = GetNormalizationVectorOnCPU(GetDirectionVectorOfEdgeOnCPU(sameEdge));
		}
		else//�������㲻����Բ�ģ�����������Ϊ��������
		{
			circlePointVector1.x = circleCenterPoint1.x - diffractionEdge[idx].p1.x;
			circlePointVector1.y = circleCenterPoint1.y - diffractionEdge[idx].p1.y;
			circlePointVector1.z = circleCenterPoint1.z - diffractionEdge[idx].p1.z;
			circlePointVector1 = GetNormalizationVectorOnCPU(circlePointVector1);
		}
		if(IsTwoPointSameOnCPU(circleCenterPoint2, diffractionEdge[idx].p2))//�����������Բ�ģ���ȡ������Ϊ��������
		{
			circlePointVector2 = GetNormalizationVectorOnCPU(GetDirectionVectorOfEdgeOnCPU(sameEdge));
		}
		else//�������㲻����Բ�ģ�����������Ϊ��������
		{
			circlePointVector2.x = circleCenterPoint2.x - diffractionEdge[idx].p2.x;
			circlePointVector2.y = circleCenterPoint2.y - diffractionEdge[idx].p2.y;
			circlePointVector2.z = circleCenterPoint2.z - diffractionEdge[idx].p2.z;
			circlePointVector2 = GetNormalizationVectorOnCPU(circlePointVector2);
		}
		SetUnitVectorVnCirclePlaneOnCPU(circleVectorU1, circleVectorV1, circleCenterPoint1, 
			crossPointOfFace1AndCircle1, crossPointOfFace2AndCircle1, circlePointVector1, 90);//�����µ�����ϵ
		SetUnitVectorVnCirclePlaneOnCPU(circleVectorU2, circleVectorV2, circleCenterPoint2, 
			crossPointOfFace1AndCircle2, crossPointOfFace2AndCircle2, circlePointVector1, 90);
		struct Point* circumPoints1 = new struct Point[diffractionRayCount];//�����1��Ӧ��Բ���ϵĵ�
		struct Point* circumPoints2 = new struct Point[diffractionRayCount];//�����2��Ӧ��Բ���ϵĵ�
		circumPoints1 = GetcircumPointOfTheCircleOnCPU(sameEdge, circleCenterPoint1, circleRadius1, circleVectorU1, circleVectorV1, 270, diffractionRayCount);
		circumPoints2 = GetcircumPointOfTheCircleOnCPU(sameEdge, circleCenterPoint2, circleRadius2, circleVectorU2, circleVectorV2, 270, diffractionRayCount);
		//������Ȧ�������� ÿ��thread�е�ÿ������㣨�������㣩����diffractionRayCount���������ߣ���������㹲����2*diffractionRayCount���������ߣ���������Ԥ�����������
		unsigned int j = 0;//���ڱ�ʾԲ���ϵĵ������circumPoints���
		for(unsigned int i = idx * diffractionRayCount; i < (idx+1) * diffractionRayCount; i++)
		{
			diffractionRays1[i].originalPoint = diffractionEdge[idx].p1;//�����1
			diffractionRays2[i].originalPoint = diffractionEdge[idx].p2;//�����2
			diffractionRays1[i].direction.x = circumPoints1[j].x - diffractionRays1[i].originalPoint.x;//�����1����������һ����������
			diffractionRays1[i].direction.y = circumPoints1[j].y - diffractionRays1[i].originalPoint.y;
			diffractionRays1[i].direction.z = circumPoints1[j].z - diffractionRays1[i].originalPoint.z;
			diffractionRays2[i].direction.x = circumPoints2[j].x - diffractionRays2[i].originalPoint.x;//�����2����������һ����������
			diffractionRays2[i].direction.y = circumPoints2[j].y - diffractionRays2[i].originalPoint.y;
			diffractionRays2[i].direction.z = circumPoints2[j].z - diffractionRays2[i].originalPoint.z;
			j++;
		}

		//�������߹�
		unsigned int k = idx * diffractionRayCount;
		for(unsigned int i = idx * (diffractionRayCount-1); i < (idx+1) * (diffractionRayCount - 1); i++)
		{
			//�������ǽ��ܹ���
			diffractionRayTubes[i].ray1 = diffractionRays1[k];
			diffractionRayTubes[i].ray2 = diffractionRays1[k+1];
			diffractionRayTubes[i].ray3 = diffractionRays2[k+1];
			diffractionRayTubes[i].ray4 = diffractionRays2[k];
			//����Ϊ·���ڵ�ļ�¼
			if(incidentRayTubes[idx].path.nodeLevel==0)
			{
				diffractionRayTubes[i].path.nodeLevel = 1;
				diffractionRayTubes[i].path.node1.point1 = sameEdge.p1;
				diffractionRayTubes[i].path.node1.point2 = sameEdge.p2;
			}
			if(incidentRayTubes[idx].path.nodeLevel==1)
			{
				diffractionRayTubes[i].path.node1 = incidentRayTubes[idx].path.node1;
				diffractionRayTubes[i].path.nodeLevel = 2;
				diffractionRayTubes[i].path.node2.point1 = sameEdge.p1;
				diffractionRayTubes[i].path.node2.point2 = sameEdge.p2;
			}
			if(incidentRayTubes[idx].path.nodeLevel==2)
			{
				diffractionRayTubes[i].path.node1 = incidentRayTubes[idx].path.node1;
				diffractionRayTubes[i].path.node2 = incidentRayTubes[idx].path.node2;
				diffractionRayTubes[i].path.nodeLevel = 3;
				diffractionRayTubes[i].path.node3.point1 = sameEdge.p1;
				diffractionRayTubes[i].path.node3.point2 = sameEdge.p2;
			}
			k++;
		}
		delete [] circumPoints1;
		delete [] circumPoints2;
	}
	delete[] diffractionRays1;
	delete[] diffractionRays2;
}