#include "CalCulation.cuh"

//单位化向量
__device__ Point C_UnitizeVectorOnGPU(Point vector)
{
	float sum=sqrt(pow(vector.x,2)+pow(vector.y,2)+pow(vector.z,2));
	Point unitVector;
	unitVector.x=vector.x/sum;
	unitVector.y=vector.y/sum;
	unitVector.z=vector.z/sum;
	return unitVector;
}

//求反向向量
__device__ Point GetReverseVector(Point vector)
{
	vector.x=-vector.x;
	vector.y=-vector.y;
	vector.z=-vector.z;
	return vector;
}

//求两点之间距离
__device__ float CalculateDistanceOfBipartiteOnGPU(Point p1, Point p2)
{
	return sqrt(pow(p1.x-p2.x, 2) + pow(p1.y-p2.y, 2) + pow(p1.z-p2.z, 2));
}


//求一个点关于另一个点的对称点
__device__ Point GetSymmetryPoint(Point pointToCal, Point centerPoint)
{
	Point result;
	result.x= 2*centerPoint.x-pointToCal.x;
	result.y= 2*centerPoint.y-pointToCal.y;
	result.z= 2*centerPoint.z-pointToCal.z;
	return result;
}

//求向量的模
__device__ float Mag(Point vector)
{
	return sqrt(pow(vector.x,2)+pow(vector.y,2)+pow(vector.z,2));
}

//求点乘
__device__ float DotMultiply(Point vector1, Point vector2)
{
	return vector1.x*vector2.x+vector1.y*vector2.y+vector1.z*vector2.z;
}

//求叉乘
__device__ Point CrossMultiply(Point vector1, Point vector2)
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

//获得平面法向量
__device__ Point GetNormalVector(Face face)
{
	Point vectorAB;
	Point vectorBC;
	vectorAB.x=face.B.x-face.A.x;
	vectorAB.y=face.B.y-face.A.y;
	vectorAB.z=face.B.z-face.A.z;
	vectorBC.x=face.C.x-face.B.x;
	vectorBC.y=face.C.y-face.B.y;
	vectorBC.z=face.C.z-face.B.z;
	return CrossMultiply(vectorAB,vectorBC);
}

//求平面方程Ax+By+Cz+D=0的四个系数A,B,C,D
__device__ float* GetFaceEquationFactor(Face face)
{
	float para[4];
	Point unitVector=GetNormalVector(face);
	unitVector=C_UnitizeVectorOnGPU(unitVector);
	para[0]=unitVector.x;
	para[1]=unitVector.y;
	para[2]=unitVector.z;
	para[3]=-(unitVector.x*face.A.x+unitVector.y*face.A.y+unitVector.z*face.A.z);
	return para;
}

//判断点是否在平面内
__device__ bool JudgeIfPointInFace(Point point, Face face)
{
	float* faceFactor=(float*)malloc(4);//平面方程的4个系数ABCD
	faceFactor=GetFaceEquationFactor(face);
	float faceEquation=faceFactor[0]*point.x+faceFactor[1]*point.y+faceFactor[2]*point.z+faceFactor[3];
	if(abs(faceEquation<0.001))
	{
		delete [] faceFactor;
		return true;
	}
	else
	{
		delete [] faceFactor;
		return false;
	}
}

//判断两个向量是否平行且同向
__device__ bool IsParallelAndSamedirection(Point vector1, Point vector2)
{
	float temp=(vector1.x*vector2.x+vector1.y*vector2.y+vector1.z*vector2.z)/(Mag(vector1)*Mag(vector2));
	if(abs(temp-1)<=0.00001)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//得到一条棱的方向向量（p1为下点，p2为上点，p1指向p2）
__device__ Point GetDirectionVectorOfEdge(Edge edge)
{
	Point direction;
	direction.x=edge.p2.x-edge.p1.x;
	direction.y=edge.p2.y-edge.p1.y;
	direction.z=edge.p2.z-edge.p1.z;
	return direction;
}

//判断两个点是否为同一点
__device__ bool IsTwoPointSame(Point A,Point B)
{
	if ((abs(A.x-B.x)<0.00001)&&(abs(A.y-B.y)<0.00001)&&(abs(A.z-B.z)<0.00001))
	{
		return true;
	}
	return false;
}

//判断一个点是否为一个三角面的顶点之一
__device__ bool IsTheSamePointOfAdjacentExist(Point A,Face face)
{
	if (IsTwoPointSame(A,face.A)||IsTwoPointSame(A,face.B)||IsTwoPointSame(A,face.C))
	{
		return true;
	}
	return false;
}

//得到相邻两三角面的公共棱
__device__ Edge GetEdgeOfAdjacentFaces(Face face1,Face face2)
{
	Edge edge;
	if(IsTheSamePointOfAdjacentExist(face1.A,face2)&&IsTheSamePointOfAdjacentExist(face1.B,face2)&&IsTheSamePointOfAdjacentExist(face1.C,face2))
	{
		//表示两三角面为同一三角面
		edge.p1.x=6666;
		edge.p1.y=6666;
		edge.p1.z=6666;
		edge.p2.x=6666;
		edge.p2.y=6666;
		edge.p2.z=6666;
		return edge;
	}
	else if (IsTheSamePointOfAdjacentExist(face1.A,face2)&&IsTheSamePointOfAdjacentExist(face1.B,face2))
	{
		edge.p1=face1.A;
		edge.p2=face1.B;
		return edge;
	}
	else if (IsTheSamePointOfAdjacentExist(face1.A,face2)&&IsTheSamePointOfAdjacentExist(face1.C,face2))
	{
		edge.p1=face1.A;
		edge.p2=face1.C;
		return edge;
	}
	else if (IsTheSamePointOfAdjacentExist(face1.B,face2)&&IsTheSamePointOfAdjacentExist(face1.C,face2))
	{
		edge.p1=face1.B;
		edge.p2=face1.C;
		return edge;
	}
	else
	{
		//表示两三角面无公共棱
		edge.p1.x=8888;
		edge.p1.y=8888;
		edge.p1.z=8888;
		edge.p2.x=8888;
		edge.p2.y=8888;
		edge.p2.z=8888;
		return edge;
	}
}

//获得直线外一点到该直线的垂足，p为直线外一点，返回垂足坐标
__device__ Point GetDropFoot(Point p)
{
	
}


//获得三角面中除了一条棱外的剩余一点
__device__ Point GetRemainPointOnFace(Face face, Edge sameEdge)
{
	bool flag[3];
	for(int i=0;i<3;i++)
	{
		flag[i]=true;
	}
	//下列6个if会有两个为真，留下剩余一点
	if(IsTwoPointSame(face.A, sameEdge.p1))
	{
		flag[0]=false;
	}
	if(IsTwoPointSame(face.B, sameEdge.p1))
	{
		flag[1]=false;
	}
	if(IsTwoPointSame(face.C, sameEdge.p1))
	{
		flag[2]=false;
	}
	if(IsTwoPointSame(face.A, sameEdge.p2))
	{
		flag[0]=false;
	}
	if(IsTwoPointSame(face.B, sameEdge.p2))
	{
		flag[1]=false;
	}
	if(IsTwoPointSame(face.C, sameEdge.p2))
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
	printf("找三角面中除公共棱的一点GetRemainPointOnFace()出错！\n");
}

//从虚拟发射点到绕射点重新构造入射射线
__device__ Ray CreateNewInRay(Point originPoint, Point diffractionPoint)
{
	Ray newInRay;
	newInRay.originalPoint=originPoint;
	newInRay.direction.x=diffractionPoint.x-originPoint.x;
	newInRay.direction.y=diffractionPoint.y-originPoint.y;
	newInRay.direction.z=diffractionPoint.z-originPoint.z;
	return newInRay;
}

//求两条直线的夹角 在绕射中用于获取射线与劈边的夹角
__device__ float GetAngleOfTwoStraightLines(Point vector1, Point vector2)
{
	float dotMultiplied= DotMultiply(vector1,vector2);
	float tempValue= sqrt(pow(vector1.x,2)+pow(vector1.y,2)+pow(vector1.z,2))*sqrt(pow(vector2.x,2)+pow(vector2.y,2)+pow(vector2.z,2));
	if(tempValue==0)
	{
		//因为不能打LOG，此处用以检查错误
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

//求所设圆的圆心 在绕射中用于在劈边上选一个与绕射射线同向且不是绕射点的点,一般选公共棱的某个顶点,当夹角为90度时，绕射射线为圆盘，圆心为绕射点
__device__ Point GetCircleCenterPoint(float angleOfRayAndSameLine,Ray incidentRay, Edge sameEdge, Point diffractionPoint)
{
	Point sameEdgePoint1=sameEdge.p1;
	Point sameEdgePoint2=sameEdge.p2;
	if(abs(angleOfRayAndSameLine-90)<0.0001)//如果两向量垂直
	{
		return diffractionPoint;
	}
	//如果绕射点刚好为公共棱的顶点，则倍长虚拟入射射线，并向绕射棱所在直线作垂线，垂足为圆心
	else if(IsTwoPointSame(diffractionPoint,sameEdge.p1)||IsTwoPointSame(diffractionPoint,sameEdge.p2))
	{
		if(IsTwoPointSame(diffractionPoint,sameEdge.p1))
		{
			Point p;
			p=GetSymmetryPoint(incidentRay.originalPoint, sameEdge.p1);//倍长点
			//倍长点向公共棱所在直线做垂足
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
		if(IsTwoPointSame(diffractionPoint,sameEdge.p2))
		{
			Point p;
			p=GetSymmetryPoint(incidentRay.originalPoint, sameEdge.p2);//倍长点
			//倍长点向公共棱所在直线做垂足
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
	else//将圆心设为与绕射射线同一侧的顶点
	{
		Point sameEdgeVector;
		sameEdgeVector.x=sameEdgePoint2.x-sameEdgePoint1.x;
		sameEdgeVector.y=sameEdgePoint2.y-sameEdgePoint1.y;
		sameEdgeVector.z=sameEdgePoint2.z-sameEdgePoint1.z;
		float dotMultiplied=DotMultiply(incidentRay.direction, sameEdgeVector);
		if(dotMultiplied>0)//两向量同向
		{
			return sameEdgePoint2;
		}
		if(dotMultiplied<0)//两向量反向
		{
			return sameEdgePoint1;
		}
	}
}

//求所设圆的半径
__device__ float GetCircleRadius(float angleOfRayAndSameLine, Point diffractionPoint, Point circleCenterPoint)
{
	float circleRadius;
	if(abs(angleOfRayAndSameLine-90)<0.0001)//射线垂直打到棱上
	{
		circleRadius=1;
	}
	else
	{
		//tan角为钝角时，有可能出现负数，导致最终生成绕射射线的方向向量符号有误，所以加绝对值
		circleRadius=abs(CalculateDistanceOfBipartiteOnGPU(diffractionPoint,circleCenterPoint)*tan(angleOfRayAndSameLine*PI/180));
	}
	return circleRadius;
}

//求圆所在平面上的一个向量(绕射) 输入的面为组成劈的两个面中的任意一个面
__device__ Point GetVectorInThePlaneOfCircle(Edge sameEdge, Face oneAdjacentTriangle)
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
		return C_UnitizeVectorOnGPU(GetNormalVector(oneAdjacentTriangle));
	//}
}

//求在圆的平面上的一个与平面法向量，平面某个向量都正交的向量
__device__ Point GetAnotherVectorInThePlaneOfCircle(Edge sameEdge, Point vectorU)
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
		//再求一个与1和劈边向量都正交的向量，该向量也在圆所在的平面上
		Point circleVectorV;
		circleVectorV.x=sameEdgeVector.y*vectorU.z-sameEdgeVector.z*vectorU.y;
		circleVectorV.y=sameEdgeVector.z*vectorU.x-sameEdgeVector.x*vectorU.z;
		circleVectorV.z=sameEdgeVector.x*vectorU.y-sameEdgeVector.y*vectorU.x;
		return C_UnitizeVectorOnGPU(circleVectorV);
	}
}

//求圆与三角面的交点（绕射）
/// <param name="terTri">三角面</param>
/// <param name="centerPoint">圆心</param>
/// <param name="radius">圆的半径</param>
/// <param name="vectorU">圆所在平面的一个向量U，其与平面的法向量垂直</param>
/// <param name="vectorV">圆所在平面的一个向量V,其与U及平面法向量相互垂直</param>
/// <param name="triPoint">三角面除了绕射棱两个端点外的另一个点</param>
/// <returns>返回圆与三角面的交点</returns>
__device__ Point GetCrossPointOfCircleWithTer(Face terTri, Point centerPoint, float radius, Point vectorU, Point vectorV, Point triPoint)
{
	//测试用 求解出错，得到一条在高空的射线
	Point defaultPoint;
	defaultPoint.x=7777;
	defaultPoint.y=7777;
	defaultPoint.z=7777;
	//参考Rays.cs中的同名方法，具体方程在该段代码中
	Point crossPoints[4];
	bool crossPointFlags[4];
	for(int i = 0; i < 4; i++)//给标志位赋初值，默认全为true，之后再逐个剔除
	{
		crossPointFlags[i]=true;
	}
	if(radius<1)//半径太小导致判断有误差，所以设置半径不小于1
	{
		radius=1;
	}
	Point triVector= GetNormalVector(terTri);//获取三角面的法向量
	float D = -(triVector.x*terTri.A.x+triVector.y*terTri.A.y+triVector.z*terTri.A.z);
	float M = radius*(triVector.x*vectorU.x+triVector.y*vectorU.y+triVector.z*vectorU.z);
	float N = radius*(triVector.x*vectorV.x+triVector.y*vectorV.y+triVector.z*vectorV.z);
	float sint[2];
	sint[0]=round(sqrt(pow(N,2)/(pow(M,2)+pow(N,2))));
	sint[1]=round(-sqrt(pow(N,2)/(pow(M,2)+pow(N,2))));
	for(int i=0; i<2; i++)//由于分辨不清角度t的范围，所以将cost用1-sint^2开根的方法求，并得到4个点，其中两个不在平面内
	{
		if((-1<=sint[i])&&(sint[i]<=1))//解的范围在-1到1之间时
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
			//出错
			printf("求圆周与劈面交点时出错！\n");
			return defaultPoint;
		}
	}
	//删除不在三角面平面上的交点
	for(int i=0;i<4;i++)
	{
		if(!JudgeIfPointInFace(crossPoints[i], terTri))
		{
			crossPointFlags[i] = false;
		}
	}
	//由于当thi角为90度时，求得的点中有两个是重复的点，故要将其删除
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3-i;j++)
		{
			if(IsTwoPointSame(crossPoints[j], crossPoints[j+1]))
			{
				if(crossPointFlags[j]&&crossPointFlags[j+1])//相同的点剔除其中一个
				{
					crossPointFlags[j+1]=false;
				}
			}
		}
	}
	//圆与三角面或者在其一侧的交点
	for(int i=0;i<3;i++)
	{
		for(int j=i+1;j<4;j++)
		{
			if(crossPointFlags[i]&&crossPointFlags[j])//取同侧点
			{
				if(CalculateDistanceOfBipartiteOnGPU(crossPoints[i],triPoint)<CalculateDistanceOfBipartiteOnGPU(crossPoints[j],triPoint))
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
	//	crossPoint2=GetSymmetryPointOnCPU(crossPoint2, centerPoint);//对点取反
	//	return crossPoint2;
	//}
	{
		//出错
		return defaultPoint;
	}
}

//根据在圆盘上绕射面的分布设置坐标系
/// <param name="unitVectorU">圆所在平面上一个向量U</param>
/// <param name="unitVectorV">圆所在平面上一个向量V</param>
/// <param name="circleCenterPoint">圆盘的圆心</param>
/// <param name="crossPointOfFace0AndCircle">圆盘与绕射面0的交点</param>
/// <param name="crossPointOfFace1AndCircle">圆盘与绕射面1的交点</param>
/// <param name="verticalVector">圆所在平面的法向量，由下底圆心指向上底圆心</param>
/// <param name="angleOfTriangles">两个绕射面的夹角</param>
__device__ void SetUnitVectorVnCirclePlane(Point &unitVectorU, Point &unitVectorV, Point circleCenterPoint,
	Point crossPointOfFace0AndCircle, Point crossPointOfFace1AndCircle, Point verticalVector, float angleOfTriangles)
{
	unitVectorU.x=crossPointOfFace0AndCircle.x-circleCenterPoint.x;
	unitVectorU.y=crossPointOfFace0AndCircle.y-circleCenterPoint.y;
	unitVectorU.z=crossPointOfFace0AndCircle.z-circleCenterPoint.z;
	unitVectorU=C_UnitizeVectorOnGPU(unitVectorU);
	unitVectorV=C_UnitizeVectorOnGPU(GetReverseVector(CrossMultiply(unitVectorU,verticalVector)));
	if(!IsParallelAndSamedirection(verticalVector,CrossMultiply(unitVectorU,unitVectorV)))
	{
		unitVectorV=GetReverseVector(unitVectorV);
	}
	Point otherVectorU;
	otherVectorU.x=crossPointOfFace1AndCircle.x-circleCenterPoint.x;
	otherVectorU.y=crossPointOfFace1AndCircle.y-circleCenterPoint.y;
	otherVectorU.z=crossPointOfFace1AndCircle.z-circleCenterPoint.z;
	otherVectorU=C_UnitizeVectorOnGPU(otherVectorU);
	if(angleOfTriangles < 90)
	{
		if(abs(GetAngleOfTwoStraightLines(unitVectorV,otherVectorU) + angleOfTriangles - 90)<0.00001)
		{
			unitVectorU = otherVectorU;
			unitVectorV = CrossMultiply(verticalVector,unitVectorU);
		}
	}
	else if(angleOfTriangles>90)
	{
		if(abs(angleOfTriangles - GetAngleOfTwoStraightLines(unitVectorV,otherVectorU) - 90)<0.00001)
		{
			unitVectorU = otherVectorU;
			unitVectorV = CrossMultiply(verticalVector,unitVectorU);
		}
	}
	else
	{
		if(IsParallelAndSamedirection(unitVectorV,otherVectorU))
		{
			unitVectorV = GetReverseVector(unitVectorU);
			unitVectorU=otherVectorU;
		}
	}
}

//根据输入的角度求圆上的一点 arcAngle以弧度制表示
__device__ Point GetPointInCircumference(Point circleCenterPoint, float circleRadius, Point unitVectorU, Point unitVectorV, float arcAngle)
{
	Point circumferencePoint;
	circumferencePoint.x = circleCenterPoint.x + circleRadius * (unitVectorU.x * cos(arcAngle) + unitVectorV.x * sin(arcAngle));
	circumferencePoint.y = circleCenterPoint.y + circleRadius * (unitVectorU.y * cos(arcAngle) + unitVectorV.y * sin(arcAngle));
	circumferencePoint.z = circleCenterPoint.z + circleRadius * (unitVectorU.z * cos(arcAngle) + unitVectorV.z * sin(arcAngle));
	return circumferencePoint;
}

//计算两射线交点 （用于求绕射虚拟发射点）
__device__ Point GetIntersectionOfTwoRays(Ray ray1,Ray ray2)
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

//以某一个角度划分获得所设圆上的点
__device__ Point *GetcircumPointOfTheCircle(Edge sameEdge, Point circleCenterPoint, float circleRadius, 
	Point unitCircleVectorU, Point unitCircleVectorV, float circleRange, int circumPointCount)
{
	float angleOfTwoCircumPoint = circleRange * PI /(circumPointCount * 180);//取点的角度
	struct Point *circumPoints=new struct Point[circumPointCount];
	//第一个点取一个微小角度，避免与面重合
	circumPoints[0] = GetPointInCircumference(circleCenterPoint, circleRadius, unitCircleVectorU, unitCircleVectorV, 0.01);
	//在圆上取点，存入动态数组
	for (int i = 1; i < circumPointCount-1; i++)
	{
		circumPoints[i] = GetPointInCircumference(circleCenterPoint, circleRadius, unitCircleVectorU, unitCircleVectorV, i * angleOfTwoCircumPoint);
	}
	//最后一个点减少一个微小角度，避免与面重合
	circumPoints[circumPointCount-1] = GetPointInCircumference(circleCenterPoint, circleRadius, 
		unitCircleVectorU, unitCircleVectorV, circleRange * PI/180-0.01);
	return circumPoints;
}

//生成绕射射线的核函数 diffractionEdge是绕射点p1和绕射点p2之间的部分 diffractionRayCount表示每个绕射点生成绕射射线的条数
__global__ void GenerateDiffractionRays(TriangleRayTube *incidentRayTubes, Edge *diffractionEdge, Face *diffractionFace1, Face *diffractionFace2, 
	Ray *diffractionRays1, Ray *diffractionRays2, QuadrangleRayTube *diffractionRayTubes, unsigned const int diffractionRayCount)
{
	//该核函数中，diffractionEdge不用作棱的功能，只用作两个绕射点。 只有核函数的局部变量sameEdge用作绕射棱的功能
	int idx=(blockIdx.x*blockDim.x)+threadIdx.x;
	
	Point originPoint = GetIntersectionOfTwoRays(incidentRayTubes[idx].ray1, incidentRayTubes[idx].ray2);//计算虚拟发射点
	Ray inRay1=CreateNewInRay(originPoint, diffractionEdge[idx].p1);//从虚拟发射点到绕射点1重新构造一条入射线
	Ray inRay2=CreateNewInRay(originPoint, diffractionEdge[idx].p2);//从虚拟发射点到绕射点2重新构造一条入射线
	float angleOfRay1AndEdge=GetAngleOfTwoStraightLines(inRay1.direction, GetDirectionVectorOfEdge(diffractionEdge[idx]));//获取射线1与劈边的夹角
	float angleOfRay2AndEdge=GetAngleOfTwoStraightLines(inRay2.direction, GetDirectionVectorOfEdge(diffractionEdge[idx]));//获取射线2与劈边的夹角
	Edge sameEdge=GetEdgeOfAdjacentFaces(diffractionFace1[idx],diffractionFace2[idx]);//劈边的完整棱，diffractionEdge为其子集
	Point circleCenterPoint1 = GetCircleCenterPoint(angleOfRay1AndEdge, inRay1, sameEdge, diffractionEdge[idx].p1);//在劈边上选一个与绕射射线同向且不是绕射点的点,一般选公共棱的某个顶点,当夹角为90度时，绕射射线为圆盘，圆心为绕射点
	Point circleCenterPoint2 = GetCircleCenterPoint(angleOfRay2AndEdge, inRay2, sameEdge, diffractionEdge[idx].p2);
	float circleRadius1 = GetCircleRadius(angleOfRay1AndEdge, diffractionEdge[idx].p1, circleCenterPoint1);//所作圆1的半径
	float circleRadius2 = GetCircleRadius(angleOfRay2AndEdge, diffractionEdge[idx].p2, circleCenterPoint2);//所作圆2的半径
	Point circleVectorU1 = GetVectorInThePlaneOfCircle(sameEdge, diffractionFace1[idx]);//先求圆所在平面上的一个向量
	Point circleVectorU2 = circleVectorU1;//最后一个参数都是diffractionFace1，因为只需要劈的其中一个三角面
	Point circleVectorV1 = GetAnotherVectorInThePlaneOfCircle(sameEdge, circleVectorU1);//再求一个与前面所求向量和劈边向量都正交的向量，该向量也在圆所在的平面上
	Point circleVectorV2 = GetAnotherVectorInThePlaneOfCircle(sameEdge, circleVectorU2);
	//求两个圆与两个三角面的交点
	Point triPoint1;
	Point triPoint2;
	triPoint1 = GetRemainPointOnFace(diffractionFace1[idx], sameEdge);
	triPoint2 = GetRemainPointOnFace(diffractionFace2[idx], sameEdge);
	Point crossPointOfFace1AndCircle1 = GetCrossPointOfCircleWithTer(diffractionFace1[idx], circleCenterPoint1, 
		circleRadius1, circleVectorU1, circleVectorV1, triPoint1);//圆1与三角面1一侧的交点
	Point crossPointOfFace2AndCircle1 = GetCrossPointOfCircleWithTer(diffractionFace2[idx], circleCenterPoint1, 
		circleRadius1, circleVectorU1, circleVectorV1, triPoint2);//圆1与三角面2一侧的交点
	Point crossPointOfFace1AndCircle2 = GetCrossPointOfCircleWithTer(diffractionFace1[idx], circleCenterPoint2, 
		circleRadius2, circleVectorU2, circleVectorV2, triPoint1);//圆2与三角面1一侧的交点
	Point crossPointOfFace2AndCircle2 = GetCrossPointOfCircleWithTer(diffractionFace2[idx], circleCenterPoint2, 
		circleRadius2, circleVectorU2, circleVectorV2, triPoint2);//圆2与三角面2一侧的交点
	Point circlePointVector1;
	Point circlePointVector2;
	if(IsTwoPointSame(circleCenterPoint1, diffractionEdge[idx].p1))//如果绕射点等于圆心，则取绕射棱为方向向量
	{
		circlePointVector1 = C_UnitizeVectorOnGPU(GetDirectionVectorOfEdge(sameEdge));
	}
	else//如果绕射点不等于圆心，则两点连线为方向向量
	{
		circlePointVector1.x = circleCenterPoint1.x - diffractionEdge[idx].p1.x;
		circlePointVector1.y = circleCenterPoint1.y - diffractionEdge[idx].p1.y;
		circlePointVector1.z = circleCenterPoint1.z - diffractionEdge[idx].p1.z;
		circlePointVector1 = C_UnitizeVectorOnGPU(circlePointVector1);
	}
	if(IsTwoPointSame(circleCenterPoint2, diffractionEdge[idx].p2))//如果绕射点等于圆心，则取绕射棱为方向向量
	{
		circlePointVector2 = C_UnitizeVectorOnGPU(GetDirectionVectorOfEdge(sameEdge));
	}
	else//如果绕射点不等于圆心，则两点连线为方向向量
	{
		circlePointVector2.x = circleCenterPoint2.x - diffractionEdge[idx].p2.x;
		circlePointVector2.y = circleCenterPoint2.y - diffractionEdge[idx].p2.y;
		circlePointVector2.z = circleCenterPoint2.z - diffractionEdge[idx].p2.z;
		circlePointVector2 = C_UnitizeVectorOnGPU(circlePointVector2);
	}
	SetUnitVectorVnCirclePlane(circleVectorU1, circleVectorV1, circleCenterPoint1, 
		crossPointOfFace1AndCircle1, crossPointOfFace2AndCircle1, circlePointVector1, 90);//建立新的坐标系
	SetUnitVectorVnCirclePlane(circleVectorU2, circleVectorV2, circleCenterPoint2, 
		crossPointOfFace1AndCircle2, crossPointOfFace2AndCircle2, circlePointVector1, 90);
	struct Point* circumPoints1 = new struct Point[diffractionRayCount];//绕射点1对应的圆周上的点
	struct Point* circumPoints2 = new struct Point[diffractionRayCount];//绕射点2对应的圆周上的点
	circumPoints1 = GetcircumPointOfTheCircle(sameEdge, circleCenterPoint1, circleRadius1, circleVectorU1, circleVectorV1, 270, diffractionRayCount);
	circumPoints2 = GetcircumPointOfTheCircle(sameEdge, circleCenterPoint2, circleRadius2, circleVectorU2, circleVectorV2, 270, diffractionRayCount);
    //生成两圈绕射射线 每个thread中的每个绕射点（共两个点）生成diffractionRayCount条绕射射线，两个绕射点共生成2*diffractionRayCount条绕射射线，存入两个预先申请的数组
	unsigned int j = 0;//用于表示圆周上的点的数组circumPoints序号
	for(unsigned int i = idx * diffractionRayCount; i < (idx+1) * diffractionRayCount; i++)
	{
		diffractionRays1[i].originalPoint = diffractionEdge[idx].p1;//绕射点1
		diffractionRays2[i].originalPoint = diffractionEdge[idx].p2;//绕射点2
		diffractionRays1[i].direction.x = circumPoints1[j].x - diffractionRays1[i].originalPoint.x;//绕射点1产生的其中一条绕射射线
        diffractionRays1[i].direction.y = circumPoints1[j].y - diffractionRays1[i].originalPoint.y;
		diffractionRays1[i].direction.z = circumPoints1[j].z - diffractionRays1[i].originalPoint.z;
		diffractionRays2[i].direction.x = circumPoints2[j].x - diffractionRays2[i].originalPoint.x;//绕射点2产生的其中一条绕射射线
        diffractionRays2[i].direction.y = circumPoints2[j].y - diffractionRays2[i].originalPoint.y;
		diffractionRays2[i].direction.z = circumPoints2[j].z - diffractionRays2[i].originalPoint.z;
		j++;
	}

	//建立射线管
	unsigned int k = idx * diffractionRayCount;
	for(unsigned int i = idx * (diffractionRayCount-1); i < (idx+1) * (diffractionRayCount - 1); i++)
	{
		//这四行是建管过程
		diffractionRayTubes[i].ray1 = diffractionRays1[k];
		diffractionRayTubes[i].ray2 = diffractionRays1[k+1];
		diffractionRayTubes[i].ray3 = diffractionRays2[k+1];
		diffractionRayTubes[i].ray4 = diffractionRays2[k];
		//下面为路径节点的记录
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
	//测试用begin
	/*f1[idx]=circleRadius1;
	f2[idx]=circleRadius2;
	Point U1=crossPointOfFace1AndCircle1;
	Point U2=crossPointOfFace2AndCircle1;
	test1[idx].x=U1.x;
	test1[idx].y=U1.y;
	test1[idx].z=U1.z;
	test2[idx].x=U2.x;
	test2[idx].y=U2.y;
	test2[idx].z=U2.z;*/
	//测试用end
}

//diffractionRayCount表示每个绕射点生成绕射射线的条数 RaysToBeDiffractedCount表示入射射线的条数（incidentRayTubes的个数）
cudaError_t GetDiffractionRaysInGPU(TriangleRayTube *incidentRayTubes, Edge *diffractionEdge, Face *diffractionFace1, Face *diffractionFace2, 
	Ray *diffractionRays1, Ray *diffractionRays2, QuadrangleRayTube *diffractionRayTubes,
	unsigned const int diffractionRayCount, unsigned const int RaysToBeDiffractedCount)
{
	cudaError_t cudaStatus;
	TriangleRayTube *d_incidentRayTubes = 0;
	Edge *d_diffractionEdge = 0;
	Face *d_diffractionFace1 = 0;
	Face *d_diffractionFace2 = 0;
	Ray *d_diffractionRays1 = 0;
	Ray *d_diffractionRays2 = 0;
	QuadrangleRayTube *d_diffractionRayTubes = 0;
	/*Point *d_test1;
	Point *d_test2;
	float *d_f1;
	float *d_f2;*/

	cudaStatus=cudaSetDevice(0);
	if (cudaStatus!=cudaSuccess)
	{
		fprintf(stderr,"CUDA capable GPU is not available!");
		goto Error;
	}

	cudaStatus = cudaMalloc((void**)&d_incidentRayTubes, RaysToBeDiffractedCount*sizeof(TriangleRayTube));
	if(cudaStatus!=cudaSuccess)
	{
		fprintf(stderr, "d_incidentRayTubes cudaMalloc error!");
		goto Error;
	}
	cudaStatus = cudaMalloc((void**)&d_diffractionEdge, RaysToBeDiffractedCount*sizeof(Edge));
	if(cudaStatus!=cudaSuccess)
	{
		fprintf(stderr, "d_diffractionEdge cudaMalloc error!");
		goto Error;
	}
	cudaStatus = cudaMalloc((void**)&d_diffractionFace1, RaysToBeDiffractedCount*sizeof(Face));
	if(cudaStatus!=cudaSuccess)
	{
		fprintf(stderr, "d_diffractionFace1 cudaMalloc error!");
		goto Error;
	}
	cudaStatus = cudaMalloc((void**)&d_diffractionFace2, RaysToBeDiffractedCount*sizeof(Face));
	if(cudaStatus!=cudaSuccess)
	{
		fprintf(stderr, "d_diffractionFace2 cudaMalloc error!");
		goto Error;
	}
	cudaStatus = cudaMalloc((void**)&d_diffractionRays1, diffractionRayCount*RaysToBeDiffractedCount*sizeof(Ray));
	if(cudaStatus!=cudaSuccess)
	{
		fprintf(stderr, "d_diffractionRays1 cudaMalloc error!");
		goto Error;
	}
	cudaStatus = cudaMalloc((void**)&d_diffractionRays2, diffractionRayCount*RaysToBeDiffractedCount*sizeof(Ray));
	if(cudaStatus!=cudaSuccess)
	{
		fprintf(stderr, "d_diffractionRays2 cudaMalloc error!");
		goto Error;
	}
	cudaStatus = cudaMalloc((void**)&d_diffractionRayTubes, (diffractionRayCount-1)*RaysToBeDiffractedCount*sizeof(QuadrangleRayTube));
	if(cudaStatus!=cudaSuccess)
	{
		fprintf(stderr, "d_diffractionRayTubes cudaMalloc error!");
		goto Error;
	}

	//测试用begin
	/*cudaStatus = cudaMalloc((void**)&d_test1, RaysToBeDiffractedCount*sizeof(Point));
	if(cudaStatus!=cudaSuccess)
	{
		fprintf(stderr, "d_test1 cudaMalloc error!");
		goto Error;
	}
	cudaStatus = cudaMalloc((void**)&d_test2, RaysToBeDiffractedCount*sizeof(Point));
	if(cudaStatus!=cudaSuccess)
	{
		fprintf(stderr, "d_test2 cudaMalloc error!");
		goto Error;
	}
	cudaStatus = cudaMalloc((void**)&d_f1, RaysToBeDiffractedCount*sizeof(float));
	if(cudaStatus!=cudaSuccess)
	{
		fprintf(stderr, "d_f1 cudaMalloc error!");
		goto Error;
	}
	cudaStatus = cudaMalloc((void**)&d_f2, RaysToBeDiffractedCount*sizeof(float));
	if(cudaStatus!=cudaSuccess)
	{
		fprintf(stderr, "d_f2 cudaMalloc error!");
		goto Error;
	}*/
	//测试用end
	cudaStatus = cudaMemcpy(d_incidentRayTubes, incidentRayTubes, RaysToBeDiffractedCount*sizeof(TriangleRayTube), cudaMemcpyHostToDevice);
	if(cudaStatus!=cudaSuccess)
	{
		fprintf(stderr,"incidentRayTubes Memcpy failed!");
	}
	cudaStatus = cudaMemcpy(d_diffractionEdge, diffractionEdge, RaysToBeDiffractedCount*sizeof(Edge), cudaMemcpyHostToDevice);
	if(cudaStatus!=cudaSuccess)
	{
		fprintf(stderr,"diffractionEdge Memcpy failed!");
	}
	cudaStatus = cudaMemcpy(d_diffractionFace1, diffractionFace1, RaysToBeDiffractedCount*sizeof(Face), cudaMemcpyHostToDevice);
	if(cudaStatus!=cudaSuccess)
	{
		fprintf(stderr,"diffractionFace1 Memcpy failed!");
	}
	cudaStatus = cudaMemcpy(d_diffractionFace2, diffractionFace2, RaysToBeDiffractedCount*sizeof(Face), cudaMemcpyHostToDevice);
	if(cudaStatus!=cudaSuccess)
	{
		fprintf(stderr,"diffractionFace2 Memcpy failed!");
	}

	const int num_blocks=1;
	const int num_threads=RaysToBeDiffractedCount;
	GenerateDiffractionRays<<<num_blocks, num_threads>>>(d_incidentRayTubes, d_diffractionEdge, d_diffractionFace1, d_diffractionFace2, 
	d_diffractionRays1, d_diffractionRays2, d_diffractionRayTubes, diffractionRayCount);
	cudaStatus = cudaMemcpy(diffractionRays1, d_diffractionRays1, diffractionRayCount*RaysToBeDiffractedCount*sizeof(Ray),cudaMemcpyDeviceToHost);
	if(cudaStatus!=cudaSuccess)
	{
		fprintf(stderr,"diffractionRays1 Memcpy failed!");
	}
	cudaStatus = cudaMemcpy(diffractionRays2, d_diffractionRays2, diffractionRayCount*RaysToBeDiffractedCount*sizeof(Ray),cudaMemcpyDeviceToHost);
	if(cudaStatus!=cudaSuccess)
	{
		fprintf(stderr,"diffractionRays2 Memcpy failed!");
	}
	cudaStatus = cudaMemcpy(diffractionRayTubes, d_diffractionRayTubes, (diffractionRayCount-1)*RaysToBeDiffractedCount*sizeof(QuadrangleRayTube),cudaMemcpyDeviceToHost);
	if(cudaStatus!=cudaSuccess)
	{
		fprintf(stderr,"diffractionRayTubes Memcpy failed!");
	}
	//测试用begin
	/*cudaStatus = cudaMemcpy(test1, d_test1, RaysToBeDiffractedCount*sizeof(Point),cudaMemcpyDeviceToHost);
	if(cudaStatus!=cudaSuccess)
	{
		fprintf(stderr,"test1 Memcpy failed!");
	}
	cudaStatus = cudaMemcpy(test2, d_test2, RaysToBeDiffractedCount*sizeof(Point),cudaMemcpyDeviceToHost);
	if(cudaStatus!=cudaSuccess)
	{
		fprintf(stderr,"test2 Memcpy failed!");
	}
	cudaStatus = cudaMemcpy(f1, d_f1, RaysToBeDiffractedCount*sizeof(float),cudaMemcpyDeviceToHost);
	if(cudaStatus!=cudaSuccess)
	{
		fprintf(stderr,"f1 Memcpy failed!");
	}
	cudaStatus = cudaMemcpy(f2, d_f2, RaysToBeDiffractedCount*sizeof(float),cudaMemcpyDeviceToHost);
	if(cudaStatus!=cudaSuccess)
	{
		fprintf(stderr,"f1 Memcpy failed!");
	}*/
	//测试用end
	return cudaStatus;
Error:
	cudaFree(d_incidentRayTubes);
	cudaFree(d_diffractionEdge);
	cudaFree(d_diffractionFace1);
	cudaFree(d_diffractionFace2);
	cudaFree(d_diffractionRays1);
	cudaFree(d_diffractionRays2);
	cudaFree(d_diffractionRayTubes);
	/*cudaFree(d_test1);
	cudaFree(d_test2);
	cudaFree(d_f1);
	cudaFree(d_f2);*/
}