#include"IsRaytubesReceived.h"


//三角形射线管是否打在接收机上
void IsTriangleRayTubesReceived(TriangleRayTube *triangRayTube, Point reciever,int rayTubeCount, bool *isexist)
{
	int i = 0;
	if(triangRayTube == NULL)
	{	
		printf("error: triangRayTube is NULL!\n");
		return;
	}
	for(i = 0;i<rayTubeCount;i++)
	{
		isexist[i] = IsRayTubesReceived(triangRayTube[i].ray1, triangRayTube[i].ray2, triangRayTube[i].ray3, reciever, 0);
		//if(isexist[i] == 1)
		//	printf("%d\n",i);
	}
	
}


//四边形射线管是否打在接收机上
void IsQuadrangleRayTubesReceived(QuadrangleRayTube *quadrangleRayTube, Point reciever, int rayTubeCount, bool *isexist)
{
	int i = 0;
	if(quadrangleRayTube == NULL)
	{	
		printf("error: quadrangleRayTube is NULL!\n");
		return;
	}
	for(i = 0;i<rayTubeCount;i++)
	{
		int RaytubeReceived1 = IsDiffractionRayTubesReceivedOnCPU(quadrangleRayTube[i].ray1, quadrangleRayTube[i].ray2, quadrangleRayTube[i].ray3, reciever);
		int RaytubeReceived2 = IsDiffractionRayTubesReceivedOnCPU(quadrangleRayTube[i].ray2, quadrangleRayTube[i].ray3, quadrangleRayTube[i].ray4, reciever);
		int RaytubeReceived3 = IsDiffractionRayTubesReceivedOnCPU(quadrangleRayTube[i].ray1, quadrangleRayTube[i].ray2, quadrangleRayTube[i].ray4, reciever);
		int RaytubeReceived4 = IsDiffractionRayTubesReceivedOnCPU(quadrangleRayTube[i].ray1, quadrangleRayTube[i].ray3, quadrangleRayTube[i].ray4, reciever);
		if(RaytubeReceived1 + RaytubeReceived2 + RaytubeReceived3 + RaytubeReceived4 == 0)
			isexist[i] = false;
		else
			isexist[i] = true;
	}
} 

bool  IsRayTubesReceived(Ray ray1, Ray ray2, Ray ray3, Point reciever, int flag)
{
	Point virtualOriginalPoint = GetIntersectionOfRays(ray1, ray2);
	float x = reciever.x - virtualOriginalPoint.x;
	float y = reciever.y - virtualOriginalPoint.y;
	float z = reciever.z - virtualOriginalPoint.z;
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
		{	if(u >0 && v > 0 && t>0)
				return true;
		}
		else if(flag == 1)
		{
			if(u>0 && u<1 && v>0 && v<1 && t>0 && t<1)
				return true;
		}
		
	}
	return false;

}
bool IsDiffractionRayTubesReceivedOnCPU(Ray ray1, Ray ray2, Ray ray3, Point reciever)
{
	Point originalPoint;
	Ray temRay1, temRay2, temRay3, temRay4, temRay5, temRay6;
	if(ray1.originalPoint.x == ray2.originalPoint.x && ray1.originalPoint.y == ray2.originalPoint.y && ray1.originalPoint.z == ray2.originalPoint.z)
	{
		originalPoint=ray1.originalPoint;
		GetNewRayOnCPU(&temRay1, &temRay2, &temRay3, &temRay4, ray1, ray2, ray3);
		if(CalculateIsDiffractionRecievedOnCPU(temRay1, temRay2, temRay3, reciever, originalPoint) == true)
			return true;
		else
			return IsRayTubesReceived(ray1, ray2, temRay4, reciever, 1);

	}
	else if(ray1.originalPoint.x == ray3.originalPoint.x && ray1.originalPoint.y == ray3.originalPoint.y && ray1.originalPoint.z == ray3.originalPoint.z)
	{
		originalPoint=ray1.originalPoint;
		GetNewRayOnCPU(&temRay1, &temRay3, &temRay2, &temRay4, ray1, ray3, ray2);
		if(CalculateIsDiffractionRecievedOnCPU(temRay3, temRay1, temRay2, reciever, originalPoint) == true)
			return true;
		else
			return IsRayTubesReceived(ray1, ray3, temRay4, reciever, 1);
	}
	else if(ray3.originalPoint.x == ray2.originalPoint.x && ray3.originalPoint.y == ray2.originalPoint.y && ray3.originalPoint.z == ray2.originalPoint.z)
	{
		originalPoint=ray2.originalPoint;
		GetNewRayOnCPU(&temRay3, &temRay2, &temRay1, &temRay4, ray3, ray2, ray1);
		if(CalculateIsDiffractionRecievedOnCPU(temRay3, temRay2, temRay1, reciever, originalPoint) == true)
			return true;
		else
			return IsRayTubesReceived(ray3, ray2, temRay4, reciever, 1);
	}
	else
	{
		printf("error:绕射射线管没有找到交点\n");
		return false;
	}
}

void GetNewRayOnCPU(Ray *temRay1, Ray *temRay2, Ray *temRay3, Ray *temRay4, Ray ray1, Ray ray2, Ray ray3)
{
	(*temRay1).originalPoint.x = ray1.originalPoint.x + ray1.direction.x;
	(*temRay1).originalPoint.y = ray1.originalPoint.y + ray1.direction.y;
	(*temRay1).originalPoint.z = ray1.originalPoint.z + ray1.direction.z;
	(*temRay2).originalPoint.x = ray2.originalPoint.x + ray2.direction.x;
	(*temRay2).originalPoint.y = ray2.originalPoint.y + ray2.direction.y;
	(*temRay2).originalPoint.z = ray2.originalPoint.z + ray2.direction.z;
	(*temRay3).originalPoint = ray3.originalPoint;
	(*temRay1).direction = ray1.direction;
	(*temRay2).direction = ray2.direction;
	(*temRay3).direction = ray3.direction;
	(*temRay4).originalPoint = ray1.originalPoint;
	(*temRay4).direction.x = ray3.originalPoint.x - ray1.originalPoint.x;
	(*temRay4).direction.y = ray3.originalPoint.y - ray1.originalPoint.y;
	(*temRay4).direction.z = ray3.originalPoint.z - ray1.originalPoint.z;

}

bool CalculateIsDiffractionRecievedOnCPU(Ray temRay1, Ray temRay2, Ray temRay3, Point reciever,Point originalPoint)
{
	Point intersection1, intersection2 ,intersection3;
	float x1 = temRay1.originalPoint.x, x2 = temRay2.originalPoint.x, x3 = temRay3.originalPoint.x;
	float y1 = temRay1.originalPoint.y, y2 = temRay2.originalPoint.y, y3 = temRay3.originalPoint.y;
	float z1 = temRay1.originalPoint.z, z2 = temRay2.originalPoint.z, z3 = temRay3.originalPoint.z;
	float A = (y2 - y1) * (z3 - z1) - (z2 - z1) * (y3 - y1);
	float B = (x3 - x1) * (z2 - z1) - (x2 - x1) * (z3 - z1);
	float C = (x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1);
	float D = -(A * reciever.x + B * reciever.y + C * reciever.z);
	if(JudgeStraightAndFaceOnCPU(A, B, C, D, temRay1, originalPoint, &intersection1) == true 
		&&JudgeStraightAndFaceOnCPU(A, B, C, D, temRay2, originalPoint, &intersection2) == true 
		&&JudgeStraightAndFaceOnCPU(A, B, C, D, temRay3, originalPoint, &intersection3) == true)
	{
		float x0 = reciever.x - intersection1.x, x1 = intersection2.x - intersection1.x, x2 = intersection3.x - intersection1.x;
		float y0 = reciever.y - intersection1.y, y1 = intersection2.y - intersection1.y, y2 = intersection3.y - intersection1.y;
		float z0 = reciever.z - intersection1.z, z1 = intersection2.z - intersection1.z, z2 = intersection3.z - intersection1.z;
		float denominator = x1 * y2 - x2 * y1;
		if(abs(denominator) < 0.000001)
			return false;
		float u_numerator = x0 * y2 - x2 * y0;
		float v_numerator = x1 * y0 - x0 * y1;
		float u = u_numerator / denominator;
		float v = v_numerator / denominator;
		if(u>0 && u<1 && v>0 && v<1)
			return true;
	}
	return false;

}

//判断直线与面是否有交点并返回
bool JudgeStraightAndFaceOnCPU(float A, float B, float C, float D, Ray ray, Point originalPoint, Point *intersection)
{
	float x0 = ray.originalPoint.x, x1 = ray.direction.x;
	float y0 = ray.originalPoint.y, y1 = ray.direction.y;
	float z0 = ray.originalPoint.z, z1 = ray.direction.z;
	float denominator = A * x1 * x1 + B * x1 * y1 + C * x1 * z1;
	if(abs(denominator) < 0.000001)
		return false;
	float x_numerator = -D * x1 * x1 + B * x1 * (x1 * y0 - y1 * x0) + C * x1 * (x1 * z0 - z1 * x0);
	float x = x_numerator / denominator;
	if((x - x0) / x1 < 0)
		return false;
	float y_numerator = -A * x1 * (x1 * y0 - y1 * x0) + D * y1 * (x1 * z0 - z1 * x0) + C * y1 * (x1 * z0 - z1 * x0) - C * z1 * (x1 * y0 - y1 * x0);
	float z_numerator = -A * x1 * (x1 * z0 - z1 * x0) + B * z1 * (x1 * y0 - y1 * x0) - B * y1 * (x1 * z0 - z1 * x0) - D * x1 * z1;
	float y = y_numerator / denominator;
	float z = z_numerator / denominator;
	(*intersection).x = x;
	(*intersection).y = y;
	(*intersection).z = z;
	return true;
}




void QuadrangleRayTubesReceived(QuadrangleRayTube *quadrangleRayTube, Point reciever, int rayTubeCount)
{

}