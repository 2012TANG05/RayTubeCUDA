#include "IsRaytubesReceived.cuh"
//三角形射线管是否打在接收机上



__device__ bool IRR_IsTriangleRayTubesReceived(TriangleRayTube triangRayTube, Point Receiver)
{
	return IRR_IsRayTubesReceived(triangRayTube.ray1, triangRayTube.ray2, triangRayTube.ray3, Receiver, 0);
}


//四边形射线管是否打在接收机上
__device__ bool IRR_IsTQuadrangleRayTubesReceived(QuadrangleRayTube QuadrangleRayTube, Point Receiver)
{
	int RaytubeReceived1 = IsDiffractionRayTubesReceivedOnGPU(QuadrangleRayTube.ray1, QuadrangleRayTube.ray2, QuadrangleRayTube.ray3, Receiver);
	int RaytubeReceived2 = IsDiffractionRayTubesReceivedOnGPU(QuadrangleRayTube.ray2, QuadrangleRayTube.ray3, QuadrangleRayTube.ray4, Receiver);
	int RaytubeReceived3 = IsDiffractionRayTubesReceivedOnGPU(QuadrangleRayTube.ray1, QuadrangleRayTube.ray2, QuadrangleRayTube.ray4, Receiver);
	int RaytubeReceived4 = IsDiffractionRayTubesReceivedOnGPU(QuadrangleRayTube.ray1, QuadrangleRayTube.ray3, QuadrangleRayTube.ray4, Receiver);
	if(RaytubeReceived1 + RaytubeReceived2 + RaytubeReceived3 + RaytubeReceived4 == 0)
		return false;
	return true;
} 
//用于判断三条射线（一个三角形射线管）是否打在接收点上
__device__ bool  IRR_IsRayTubesReceived(Ray ray1, Ray ray2, Ray ray3, Point receive, int flag)
{
	Point virtualOriginalPoint = CGetIntersectionOfTwoRaysOnGPU(ray1, ray2); 
	float x = receive.x - virtualOriginalPoint.x;
	float y = receive.y - virtualOriginalPoint.y;
	float z = receive.z - virtualOriginalPoint.z;
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
		{	
			if(u >0 && v > 0 && t>0)
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

//三角射线管判断接收的核函数
__global__ void CalculateIsTriangleRayTubesReceivedOnGPU(TriangleRayTube *rayTubes, 
	Point receive, bool *identifier,int rayTubeCount)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	if(i<rayTubeCount)
	{	identifier[i] = IRR_IsTriangleRayTubesReceived(rayTubes[i], receive);
		/*if(identifier[i] == true)
			printf("%d\n",i);*/
	}
}
//四边形射线管判断是否接收的核函数
__global__ void CalculateIsQuadrangleRayTubesReceivedOnGPU(QuadrangleRayTube *rayTubes, 
	Point receive, bool *identifier, int rayTubeCount)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	if(i<rayTubeCount)
	identifier[i] = IRR_IsTQuadrangleRayTubesReceived(rayTubes[i], receive);
}

//将CPU中的数据传到GPU中
cudaError_t GetTriangleRayTubesReceived(TriangleRayTube *rayTubes,bool *identifier, Point receive, int rayTubeCount)
{
	TriangleRayTube *device_rayTubes;
	bool *device_identifier;
//	Point device_receive;
	cudaError_t cudaStatus;
	cudaStatus = cudaMalloc((void**)&device_rayTubes, rayTubeCount * sizeof(TriangleRayTube));
	if(cudaStatus != cudaSuccess)
	{
		fprintf(stderr, "device_rayTubes cudaMalloc error!\n");
		goto Error;
	}
	cudaStatus = cudaMalloc((void**)&device_identifier, rayTubeCount *sizeof(bool));
	if(cudaStatus != cudaSuccess)
	{
		fprintf(stderr, "device_identifier cudaMalloc error!\n");
		goto Error;
	}
	cudaStatus = cudaMemcpy(device_rayTubes, rayTubes, rayTubeCount * sizeof(TriangleRayTube), cudaMemcpyHostToDevice);
	if(cudaStatus != cudaSuccess)
	{
		fprintf(stderr, "rayTubes Memcpy failed!\n");
		goto Error;
	}
	cudaStatus = cudaMemcpy(device_identifier, identifier, rayTubeCount * sizeof(bool), cudaMemcpyHostToDevice);
	if(cudaStatus != cudaSuccess)
	{
		fprintf(stderr, "identifier Memcpy failed!\n");
		goto Error;
	}
	const int num_blocks = 32;
	const int num_threads = 640;
	CalculateIsTriangleRayTubesReceivedOnGPU<<<num_blocks, num_threads>>>(device_rayTubes, receive, device_identifier, rayTubeCount);
	cudaStatus = cudaDeviceSynchronize();
    if (cudaStatus != cudaSuccess)
	{
        fprintf(stderr, "cudaDeviceSynchronize returned error!", cudaStatus);
        goto Error;
    }
	cudaStatus = cudaMemcpy(identifier, device_identifier, rayTubeCount * sizeof(bool), cudaMemcpyDeviceToHost);
	if(cudaStatus != cudaSuccess)
	{
		fprintf(stderr, "identifier Memcpy Error!\n");
		goto Error;
	}


	
	Error:
		cudaFree(device_rayTubes);
		cudaFree(device_identifier);

	return cudaStatus;
}



cudaError_t GetQuadrangleRayTubesReceived(QuadrangleRayTube *rayTubes,bool *identifier, Point receive, int rayTubeCount)
{
	QuadrangleRayTube *device_rayTubes;
	bool *device_identifier;
//	Point device_receive;
	cudaError_t cudaStatus;
	cudaStatus = cudaMalloc((void**)&device_rayTubes, rayTubeCount * sizeof(QuadrangleRayTube));
	if(cudaStatus != cudaSuccess)
	{
		fprintf(stderr, "device_rayTubes cudaMalloc error!\n");
		goto Error;
	}
	cudaStatus = cudaMalloc((void**)&device_identifier, rayTubeCount *sizeof(int));
	if(cudaStatus != cudaSuccess)
	{
		fprintf(stderr, "device_identifier cudaMalloc error!\n");
		goto Error;
	}
	cudaStatus = cudaMemcpy(device_rayTubes, rayTubes, rayTubeCount * sizeof(QuadrangleRayTube), cudaMemcpyHostToDevice);
	if(cudaStatus != cudaSuccess)
	{
		fprintf(stderr, "rayTubes Memcpy failed!\n");
		goto Error;
	}
	cudaStatus = cudaMemcpy(device_identifier, identifier, rayTubeCount * sizeof(bool), cudaMemcpyHostToDevice);
	if(cudaStatus != cudaSuccess)
	{
		fprintf(stderr, "identifier Memcpy failed!\n");
		goto Error;
	}
	const int num_blocks = 32;
	const int num_threads = 640;
	CalculateIsQuadrangleRayTubesReceivedOnGPU<<<num_blocks, num_threads>>>(device_rayTubes, receive, device_identifier, rayTubeCount);
	cudaStatus = cudaDeviceSynchronize();
    if (cudaStatus != cudaSuccess)
	{
        fprintf(stderr, "cudaDeviceSynchronize returned error!", cudaStatus);
        goto Error;
    }
	cudaStatus = cudaMemcpy(identifier, device_identifier, rayTubeCount * sizeof(bool), cudaMemcpyDeviceToHost);
	if(cudaStatus != cudaSuccess)
	{
		fprintf(stderr, "identifier Memcpy Error!\n");
		goto Error;
	}


	
	Error:
		cudaFree(device_rayTubes);
		cudaFree(device_identifier);

	return cudaStatus;
}

//判断绕射射线管是否打在打在接收机上
__device__ bool IsDiffractionRayTubesReceivedOnGPU(Ray ray1, Ray ray2, Ray ray3, Point reciever)
{
	Point originalPoint;
	Ray temRay1, temRay2, temRay3, temRay4, temRay5, temRay6;
	if(ray1.originalPoint.x == ray2.originalPoint.x && ray1.originalPoint.y == ray2.originalPoint.y && ray1.originalPoint.z == ray2.originalPoint.z)
	{
		originalPoint=ray1.originalPoint;
		GetNewRayOnGPU(&temRay1, &temRay2, &temRay3, &temRay4, ray1, ray2, ray3);
		if(CalculateIsDiffractionRecievedOnGPU(temRay1, temRay2, temRay3, reciever, originalPoint) == true)
			return true;
		else
			return IRR_IsRayTubesReceived(ray1, ray2, temRay4, reciever, 1);

	}
	else if(ray1.originalPoint.x == ray3.originalPoint.x && ray1.originalPoint.y == ray3.originalPoint.y && ray1.originalPoint.z == ray3.originalPoint.z)
	{
		originalPoint=ray1.originalPoint;
		GetNewRayOnGPU(&temRay1, &temRay3, &temRay2, &temRay4, ray1, ray3, ray2);
		if(CalculateIsDiffractionRecievedOnGPU(temRay3, temRay1, temRay2, reciever, originalPoint) == true)
			return true;
		else
			return IRR_IsRayTubesReceived(ray1, ray3, temRay4, reciever, 1);
	}
	else if(ray3.originalPoint.x == ray2.originalPoint.x && ray3.originalPoint.y == ray2.originalPoint.y && ray3.originalPoint.z == ray2.originalPoint.z)
	{
		originalPoint=ray2.originalPoint;
		GetNewRayOnGPU(&temRay3, &temRay2, &temRay1, &temRay4, ray3, ray2, ray1);
		if(CalculateIsDiffractionRecievedOnGPU(temRay3, temRay2, temRay1, reciever, originalPoint) == true)
			return true;
		else
			return IRR_IsRayTubesReceived(ray3, ray2, temRay4, reciever, 1);
	}
	else
	{
		printf("error:绕射射线管没有找到交点\n");
		return false;
	}
}


//得到新的射线管
__device__ void GetNewRayOnGPU(Ray *temRay1, Ray *temRay2, Ray *temRay3, Ray *temRay4, Ray ray1, Ray ray2, Ray ray3)
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

//计算绕射射线管是否接收到
__device__ bool CalculateIsDiffractionRecievedOnGPU(Ray temRay1, Ray temRay2, Ray temRay3, Point reciever,Point originalPoint)
{
	Point intersection1, intersection2 ,intersection3;
	float x1 = temRay1.originalPoint.x, x2 = temRay2.originalPoint.x, x3 = temRay3.originalPoint.x;
	float y1 = temRay1.originalPoint.y, y2 = temRay2.originalPoint.y, y3 = temRay3.originalPoint.y;
	float z1 = temRay1.originalPoint.z, z2 = temRay2.originalPoint.z, z3 = temRay3.originalPoint.z;
	float A = (y2 - y1) * (z3 - z1) - (z2 - z1) * (y3 - y1);
	float B = (x3 - x1) * (z2 - z1) - (x2 - x1) * (z3 - z1);
	float C = (x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1);
	float D = -(A * reciever.x + B * reciever.y + C * reciever.z);
	if(JudgeStraightAndFaceOnGPU(A, B, C, D, temRay1, originalPoint, &intersection1) == true 
		&&JudgeStraightAndFaceOnGPU(A, B, C, D, temRay2, originalPoint, &intersection2) == true 
		&&JudgeStraightAndFaceOnGPU(A, B, C, D, temRay3, originalPoint, &intersection3) == true)
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
__device__ bool JudgeStraightAndFaceOnGPU(float A, float B, float C, float D, Ray ray, Point originalPoint, Point *intersection)
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


__device__ Point CGetIntersectionOfTwoRaysOnGPU(Ray ray1,Ray ray2)
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