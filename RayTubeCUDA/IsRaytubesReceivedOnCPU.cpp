#include"IsRaytubesReceivedOnCPU.h"


//三角形射线管是否打在接收机上(三角形射线管判交入口)
void IsTriangleRayTubesReceivedOnCPU(TriangleRayTube *triangRayTube, Point receiver,int rayTubeCount, bool *isexist)
{
	int i = 0;
	if(triangRayTube == NULL)
	{	
		printf("error: triangRayTube is NULL!\n");
		return;
	}
	for(i = 0;i<rayTubeCount;i++)
	{
		isexist[i] = ChooseReceiveMethodByIntersectionOnCPU(triangRayTube[i].ray1, triangRayTube[i].ray2, triangRayTube[i].ray3, receiver);
		if(isexist[i] == 1)
			printf("%d\n", i);
	}
	
}


//四边形射线管是否打在接收机上（四边形射线管判交入口）
void IsQuadrangleRayTubesReceivedOnCPU(QuadrangleRayTube *quadrangleRayTube, Point receiver, int rayTubeCount, bool *isexist)
{
	int i = 0;
	if(quadrangleRayTube == NULL)
	{	
		printf("error: quadrangleRayTube is NULL!\n");
		return;
	}
	for(i = 0;i<rayTubeCount;i++)
	{
		int RaytubeReceived1 = ChooseReceiveMethodByIntersectionOnCPU(quadrangleRayTube[i].ray1, quadrangleRayTube[i].ray2, quadrangleRayTube[i].ray3, receiver);
		int RaytubeReceived2 = ChooseReceiveMethodByIntersectionOnCPU(quadrangleRayTube[i].ray2, quadrangleRayTube[i].ray3, quadrangleRayTube[i].ray4, receiver);
		int RaytubeReceived3 = ChooseReceiveMethodByIntersectionOnCPU(quadrangleRayTube[i].ray1, quadrangleRayTube[i].ray2, quadrangleRayTube[i].ray4, receiver);
		int RaytubeReceived4 = ChooseReceiveMethodByIntersectionOnCPU(quadrangleRayTube[i].ray1, quadrangleRayTube[i].ray3, quadrangleRayTube[i].ray4, receiver);
		if(RaytubeReceived1 + RaytubeReceived2 + RaytubeReceived3 + RaytubeReceived4 == 0)
			isexist[i] = false;
		else
			isexist[i] = true;
	}
} 


//射线管接收判断（最终）
bool IsRayTubesWithSameStartPointReceivedOnCPU(Ray ray1, Ray ray2, Ray ray3, Point receiver, int flag)
{
	Point virtualOriginalPoint = ray1.originalPoint;
	float x = receiver.x - virtualOriginalPoint.x;
	float y = receiver.y - virtualOriginalPoint.y;
	float z = receiver.z - virtualOriginalPoint.z;
	double Denominator = ray1.direction.x * ray2.direction.y * ray3.direction.z 
		- ray1.direction.x * ray3.direction.y * ray2.direction.z + ray2.direction.x 
		* ray3.direction.y * ray1.direction.z - ray2.direction.x * ray1.direction.y * ray3.direction.z
		+ ray3.direction.x * ray1.direction.y * ray2.direction.z - ray3.direction.x * ray2.direction.y * ray1.direction.z;
	double uNumerator = x * ray2.direction.y * ray3.direction.z - x * ray3.direction.y * ray2.direction.z
		+ ray2.direction.x * ray3.direction.y * z - ray2.direction.x * y * ray3.direction.z
		+ ray3.direction.x * y * ray2.direction.z - ray3.direction.x * ray2.direction.y * z;
	double vNumerator = ray1.direction.x * y * ray3.direction.z - ray1.direction.x * ray3.direction.y * z
		+ x * ray3.direction.y * ray1.direction.z - x * ray1.direction.y * ray3.direction.z
		+ ray3.direction.x * ray1.direction.y * z - ray3.direction.x * y * ray1.direction.z;
	double tNumerator = ray1.direction.x * ray2.direction.y * z - ray1.direction.x * y * ray2.direction.z
		+ ray2.direction.x * y * ray1.direction.z - ray2.direction.x * ray1.direction.y * z
		+ x * ray1.direction.y * ray2.direction.z - x * ray2.direction.y * ray1.direction.z;


	if(abs(Denominator)>0.000001)
	{
		double u = uNumerator / Denominator;
		double v = vNumerator / Denominator;
		double t = tNumerator / Denominator;
		if(flag == 0)
		{	if(u >=0 && v >= 0 && t>=0)
				return true;
		}
		else
		{
			if(u>=0 && u<=flag && v>=0 && v<=flag && t>=0 && t<=flag)
				return true;
		}
		
	}
	return false;

}


//中间方法
bool ChooseReceiveMethodByIntersectionOnCPU(Ray ray1, Ray ray2, Ray ray3, Point receiver)
{
	Ray temRay1 = ray1, temRay2 = ray2, temRay3 = ray3, temRay4;
	int flag = 0;
	int judges = JudgeIntersectionOfThreeRayOnCPU(&temRay1, &temRay2, &temRay3);
	if(judges == 0)
	{
		return IsRayTubesWithoutSameStartPointReceivedOnCPU(temRay1, temRay2, temRay3, receiver);
	}
	else if(judges == 3)
	{
		return IsRayTubesWithSameStartPointReceivedOnCPU(temRay1, temRay2, temRay3, receiver, flag);		
	}
	else if(judges == 1)
	{
		while(judges == 1)//防止出现一个起点加上方向向量后正好是第三条射线的起点的情况
		{
			flag = 1;
			GetNewRayTubeOnCPU(&temRay1, &temRay2, &temRay3, &temRay4, flag);
			judges = JudgeIntersectionOfThreeRayOnCPU(&temRay1, &temRay2, &temRay3);
		}
		if(IsRayTubesWithoutSameStartPointReceivedOnCPU(temRay1, temRay2, temRay3, receiver) == true)
			return true;
		else
			return IsRayTubesWithSameStartPointReceivedOnCPU(temRay1, temRay2, temRay4, receiver, flag);
	}
	
}

//判断射线管内的射线是否有交点并返回交点个数
int JudgeIntersectionOfThreeRayOnCPU(Ray *ray1, Ray *ray2, Ray *ray3)
{
	if(JudgePointEqual((*ray1).originalPoint, (*ray2).originalPoint) == true 
		&& JudgePointEqual((*ray2).originalPoint, (*ray3).originalPoint) == true ) 
	{//三条射线起于一点
		return 3;
	}
	else if(JudgePointEqual((*ray1).originalPoint, (*ray2).originalPoint) == true)
	{//第一二条起于一点
		return 1;
	}
	else if(JudgePointEqual((*ray1).originalPoint, (*ray3).originalPoint) == true)
	{//第一三条起于一点，将第三条射线与第二条射线位置交换，便于后面运算
		ExchangeTwoRay(ray2, ray3);
		return 1;
	}
	else if(JudgePointEqual((*ray2).originalPoint, (*ray3).originalPoint) == true)
	{//第二三条起于一点，将第一条射线于第三条射线位置交换，便于后面运算
		ExchangeTwoRay(ray1, ray3);
		return 1;
	}
	else
		return 0;
}


void GetNewRayTubeOnCPU(Ray *temRay1, Ray *temRay2, Ray *temRay3, Ray *temRay4, int flag)
{
	(*temRay4).originalPoint = (*temRay1).originalPoint;
	(*temRay4).direction.x = (*temRay3).originalPoint.x - (*temRay1).originalPoint.x;
	(*temRay4).direction.y = (*temRay3).originalPoint.y - (*temRay1).originalPoint.y;
	(*temRay4).direction.z = (*temRay3).originalPoint.z - (*temRay1).originalPoint.z;
	(*temRay1).originalPoint.x = (*temRay1).originalPoint.x + (*temRay1).direction.x * flag;
	(*temRay1).originalPoint.y = (*temRay1).originalPoint.y + (*temRay1).direction.y * flag;
	(*temRay1).originalPoint.z = (*temRay1).originalPoint.z + (*temRay1).direction.z * flag;
	(*temRay2).originalPoint.x = (*temRay2).originalPoint.x + (*temRay2).direction.x * flag;
	(*temRay2).originalPoint.y = (*temRay2).originalPoint.y + (*temRay2).direction.y * flag;
	(*temRay2).originalPoint.z = (*temRay2).originalPoint.z + (*temRay2).direction.z * flag;
}



bool IsRayTubesWithoutSameStartPointReceivedOnCPU(Ray temRay1, Ray temRay2, Ray temRay3, Point receiver)
{
	Point intersection1, intersection2 ,intersection3;
	float x1 = temRay1.originalPoint.x, x2 = temRay2.originalPoint.x, x3 = temRay3.originalPoint.x;
	float y1 = temRay1.originalPoint.y, y2 = temRay2.originalPoint.y, y3 = temRay3.originalPoint.y;
	float z1 = temRay1.originalPoint.z, z2 = temRay2.originalPoint.z, z3 = temRay3.originalPoint.z;
	float A = (y2 - y1) * (z3 - z1) - (z2 - z1) * (y3 - y1);
	float B = (x3 - x1) * (z2 - z1) - (x2 - x1) * (z3 - z1);
	float C = (x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1);
	float D = -(A * receiver.x + B * receiver.y + C * receiver.z);
	if(JudgeIntersectionOfStraightAndFaceOnCPU(A, B, C, D, temRay1, &intersection1) == true 
		&&JudgeIntersectionOfStraightAndFaceOnCPU(A, B, C, D, temRay2, &intersection2) == true 
		&&JudgeIntersectionOfStraightAndFaceOnCPU(A, B, C, D, temRay3, &intersection3) == true)
	{
		float x0 = receiver.x - intersection1.x, x1 = intersection2.x - intersection1.x, x2 = intersection3.x - intersection1.x;
		float y0 = receiver.y - intersection1.y, y1 = intersection2.y - intersection1.y, y2 = intersection3.y - intersection1.y;
		float z0 = receiver.z - intersection1.z, z1 = intersection2.z - intersection1.z, z2 = intersection3.z - intersection1.z;
		float denominator = x1 * y2 - x2 * y1;
		if(abs(denominator) < 0.000001)
			return false;
		float u_numerator = x0 * y2 - x2 * y0;
		float v_numerator = x1 * y0 - x0 * y1;
		float u = u_numerator / denominator;
		float v = v_numerator / denominator;
		if(u>=0 && u<=1 && v>=0 && v<=1 && u+v>=0 && u+v<=1)
			return true;

	}
	return false;

}


//判断直线与面是否有交点并返回
//列直线方程和平面方程联立求解
bool JudgeIntersectionOfStraightAndFaceOnCPU(float A, float B, float C, float D, Ray ray, Point *intersection)
{
	float x0 = ray.originalPoint.x, x1 = ray.direction.x;
	float y0 = ray.originalPoint.y, y1 = ray.direction.y;
	float z0 = ray.originalPoint.z, z1 = ray.direction.z;
	float denominator = A * x1 * x1 + B * x1 * y1 + C * x1 * z1;
	if(abs(denominator) < 0.000001)
		return false;
	float x_numerator = -D * x1 * x1 - B * x1 * (x1 * y0 - y1 * x0) - C * x1 * (x1 * z0 - z1 * x0);
	float x = x_numerator / denominator;
	float y_numerator = A * x1 * (x1 * y0 - y1 * x0) - D * y1 * x1 - C * y1 * (x1 * z0 - z1 * x0) + C * z1 * (x1 * y0 - y1 * x0);
	float z_numerator = A * x1 * (x1 * z0 - z1 * x0) - B * z1 * (x1 * y0 - y1 * x0) + B * y1 * (x1 * z0 - z1 * x0) - D * x1 * z1;
	float y = y_numerator / denominator;
	float z = z_numerator / denominator;
	if((x - x0) / x1 < 0)
		return false;
	(*intersection).x = x;
	(*intersection).y = y;
	(*intersection).z = z;
	return true;
}

