#include"IsRaytubesRecieved.cuh"




//三角形射线管是否打在接收机上
 bool IsTriangleRayTubesReceived(TriangleRayTube triangRayTube, Point Receiver)
{
	return IsRayTubesReceived(triangRayTube.ray1, triangRayTube.ray2, triangRayTube.ray3, Receiver);
}


//四边形射线管是否打在接收机上
 bool IsTQuadrangleRayTubesReceived(QuadrangleRayTube QuadrangleRayTube, Point Receiver)
{
	int Raytubereceive1 = IsRayTubesReceived(QuadrangleRayTube.ray1, QuadrangleRayTube.ray2, QuadrangleRayTube.ray3, Receiver);
	int Raytubereceive2 = IsRayTubesReceived(QuadrangleRayTube.ray2, QuadrangleRayTube.ray3, QuadrangleRayTube.ray4, Receiver);
	int Raytubereceive3 = IsRayTubesReceived(QuadrangleRayTube.ray1, QuadrangleRayTube.ray2, QuadrangleRayTube.ray4, Receiver);
	int Raytubereceive4 = IsRayTubesReceived(QuadrangleRayTube.ray1, QuadrangleRayTube.ray3, QuadrangleRayTube.ray4, Receiver);
	if(Raytubereceive1 + Raytubereceive2 + Raytubereceive3 + Raytubereceive4 == 0)
		return false;
	return true;
} 

bool  IsRayTubesReceived(Ray ray1, Ray ray2, Ray ray3, Point receive)
{
	 
	float x = receive.x - ray1.originalPoint.x - ray2.originalPoint.x - ray3.originalPoint.x;
	float y = receive.y - ray1.originalPoint.y - ray2.originalPoint.y - ray3.originalPoint.y;
	float z = receive.z - ray1.originalPoint.z - ray2.originalPoint.z - ray3.originalPoint.z;
	double Denominator = ray1.direction.x * ray2.direction.y * ray3.direction.z 
		- ray1.direction.x * ray3.direction.y * ray2.direction.z + ray2.direction.x 
		* ray3.direction.y * ray1.direction.z - ray2.direction.x * ray1.direction.y * ray3.direction.z
		 + ray3.direction.x * ray1.direction.y * ray2.direction.z - ray3.direction.x * ray2.direction.y * ray1.direction.z;
	double uNumerator = x * ray2.direction.y * ray3.direction.z - x * ray3.direction.y * ray2.direction.z
		+ ray2.direction.x * ray3.direction.y * z - ray2.direction.x * y * ray3.direction.z
		+ ray1.direction.x * y * ray3.direction.z - ray1.direction.x * ray2.direction.y * z;
	double vNumerator = ray1.direction.x * y * ray3.direction.z - ray1.direction.x * ray3.direction.y * z
		+ x * ray3.direction.y * ray1.direction.z - x * ray1.direction.y * ray3.direction.z
		+ ray3.direction.x * ray1.direction.y * z - ray1.direction.x * y * ray3.direction.z;
	double tNumerator = ray1.direction.x * ray2.direction.y * z - ray1.direction.x * y * ray2.direction.z
		+ ray2.direction.x * y * ray1.direction.z - ray2.direction.x * ray1.direction.y * z
		+ x * ray1.direction.y * ray2.direction.z - x * ray2.direction.y * ray3.direction.z;


	if(abs(Denominator)>0.01)
	{
		double u = uNumerator / Denominator;
		double v = vNumerator / Denominator;
		double t = tNumerator / Denominator;
		if(u >0 && v > 0 && t>0)
			return true;
	}

	return false;

}



