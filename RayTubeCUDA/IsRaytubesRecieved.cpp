#include"IsRaytubesRecieved.h"


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
		isexist[i] = IsRayTubesReceived(triangRayTube[i].ray1, triangRayTube[i].ray2, triangRayTube[i].ray3, reciever);
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
		int Raytubereceived1 = IsRayTubesReceived(quadrangleRayTube[i].ray1, quadrangleRayTube[i].ray2, quadrangleRayTube[i].ray3, reciever);
		int Raytubereceived2 = IsRayTubesReceived(quadrangleRayTube[i].ray2, quadrangleRayTube[i].ray3, quadrangleRayTube[i].ray4, reciever);
		int Raytubereceived3 = IsRayTubesReceived(quadrangleRayTube[i].ray1, quadrangleRayTube[i].ray2, quadrangleRayTube[i].ray4, reciever);
		int Raytubereceived4 = IsRayTubesReceived(quadrangleRayTube[i].ray1, quadrangleRayTube[i].ray3, quadrangleRayTube[i].ray4, reciever);
		if(Raytubereceived1 + Raytubereceived2 + Raytubereceived3 + Raytubereceived4 == 0)
			isexist[i] = false;
		else
			isexist[i] = true;
	}
} 

bool  IsRayTubesReceived(Ray ray1, Ray ray2, Ray ray3, Point reciever)
{
	 
	float x = reciever.x - ray1.originalPoint.x - ray2.originalPoint.x - ray3.originalPoint.x;
	float y = reciever.y - ray1.originalPoint.y - ray2.originalPoint.y - ray3.originalPoint.y;
	float z = reciever.z - ray1.originalPoint.z - ray2.originalPoint.z - ray3.originalPoint.z;
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
		if(u >0 && v > 0 && t>0)
			return true;
	}
	return false;

}


void QuadrangleRayTubesReceived(QuadrangleRayTube *quadrangleRayTube, Point reciever, int rayTubeCount)
{

}