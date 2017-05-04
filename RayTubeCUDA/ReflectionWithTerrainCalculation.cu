#include "ReflectionWithTerrainCalculation.cuh"
#include <time.h>

//单位化向量
 __device__ void RWTC_UnitizeVector(float *a, float *b, float *c)
{
	float length = sqrt((*a) * (*a) + (*b) * (*b) + (*c) * (*c));
	*a = *a / length;
	*b = *b / length;
	*c = *c / length;
}

 
//计算两点间距离
__device__ float RWTC_CalculateDistanceOfBipartite(Point a,Point b)
{
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
}


//计算反射射线方向向量
__device__ Point RWTC_GetReflectedVector(Point d,Point n)
{
	Point reflectedVector;
	float temp = 2 * (d.x * n.x + d.y * n.y + d.z * n.z);
	reflectedVector.x = d.x - temp * n.x;
	reflectedVector.y = d.y - temp * n.y;
	reflectedVector.z = d.z - temp * n.z;
	return reflectedVector;
}

//计算反射
__device__ void RWTC_CalculateTempReflectedRays(Face *face, Ray incidentray, 
	Ray *tempReflectedRay, int faceCountPerBlock, int *identifier, float *tempDistance, 
	int faceNumber)
{
//face面    数据类型Face *
//incidentray入射向量    数据类型Ray
//tempReflectedRay反射向量    数据类型Ray *
//faceCountPerBlock每次测得面的数目    数据类型int
//identifier 用于确定是否村在反射射线    数据类型int *
//tempDistance 距离短则为实际的反射射线    数据类型float *
//distanceNumber距离编号
	int i=0;
	float t=50000;//距离
	for(;i<faceCountPerBlock;i++)
	{
		float a1 = -incidentray.direction.x, a2 = -incidentray.direction.y, a3 = -incidentray.direction.z;
		float b1 = face[i].B.x - face[i].A.x, b2 = face[i].B.y - face[i].A.y, b3 = face[i].B.z - face[i].A.z;
		float c1 = face[i].C.x - face[i].A.x, c2 = face[i].C.y - face[i].A.y, c3 = face[i].C.z - face[i].A.z;
		float x1 = incidentray.originalPoint.x - face[i].A.x, x2 = incidentray.originalPoint.y - face[i].A.y, x3 = incidentray.originalPoint.z - face[i].A.z;
		float denominator = a1 * (b2 * c3 - b3 * c2) - b1 * (a2 * c3 - a3 * c2) + c1 * (a2 * b3 - a3 * b2);
		float t_numerator = x1 * (b2 * c3 - b3 * c2) - b1 * (x2 * c3 - x3 * c2) + c1 * (x2 * b3 - x3 * b2);
		float u_numerator = a1 * (x2 * c3 - x3 * c2) - x1 * (a2 * c3 - a3 * c2) + c1 * (a2 * x3 - a3 * x2);
		float v_numerator = a1 * (b2 * x3 - b3 * x2) - b1 * (a2 * x3 - a3 * x2) + x1 * (a2 * b3 - a3 * b2);
		if(abs(denominator) > 0.000001)
		{
			float u = u_numerator / denominator;
			float v = v_numerator / denominator;					
			if(t_numerator/denominator<t && t_numerator / denominator > 0)
			{//有交点且交点在平面内，且距离近(需要考虑因为计算误差导致一次反射点在平面下的问题，会很快打到平面上)
				t = t_numerator / denominator;
				(*tempReflectedRay).originalPoint.x = incidentray.originalPoint.x + incidentray.direction.x * t;
				(*tempReflectedRay).originalPoint.y = incidentray.originalPoint.y + incidentray.direction.y * t;
				(*tempReflectedRay).originalPoint.z = incidentray.originalPoint.z + incidentray.direction.z * t;
				Point n,d;
				n.x = b2 * c3 - b3 * c2; n.y = b3 * c1 - b1 * c3; n.z = b1 * c2 - c1 * b2;
				d.x = incidentray.direction.x; d.y = incidentray.direction.y; d.z = incidentray.direction.z;
				RWTC_UnitizeVector(&n.x, &n.y, &n.z);
				(*tempReflectedRay).direction = RWTC_GetReflectedVector(d, n);
				if((u>0)&&(u<1)&&((u+v)>0)&&((u+v)<1)&&v>0&&v<1)
				{
					*identifier = 1;
					faceNumber = i;
				}
				else
				{
					*identifier = 0;
				}
			}
		}
		else
		{
			*identifier = 0;
		}
	}
}


__device__ int GetMaxNumber(int a, int b, int c)
{
	if(a>b)
	{
		if(a>c)
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
		if(b>c)
		{
			return b;
		}
		else
		{
			return c;
		}
	}

}
//GPU中计算反射射线管
//face面    变量类型Face *
//incidentrayTubes入射射线管    变量类型TriangleRayTube *
//ReflectedRayTubes反射射线管    变量类型TriangleRayTube *
//faceCountPerBlock每个block面的数目，数据过大会出错    变量类型int
//identifier 判断是否有反射射线管只有1和0    变量类型int *
//temDistance 判断距离大小用于找到最近的反射点（远的不会被打到）   变量类型float*
__global__ void CalculateTempTriangleReflectedRayTubesOnGPU(Face *face, 
	TriangleRayTube *incidentrayTubes, TriangleRayTube *tempReflectedRayTubes, 
	int faceCountPerBlock, int *identifier, float *tempDistance)
{

	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int identifier1=0, identifier2=0, identifier3=0;
	
	int reflectedFace1 = -1, reflectedFace2 = -1, reflectedFace3 = -1;
	int identifierNumber = 3 * i;
	
	
	

	//RWTC_CalculateTempReflectedRays(face, incidentrayTubes[i].ray1, &tempReflectedRayTubes[i].ray1, faceCountPerBlock, &identifier1, tempDistance, faceNumber1);
	//RWTC_CalculateTempReflectedRays(face, incidentrayTubes[i].ray2, &tempReflectedRayTubes[i].ray2, faceCountPerBlock, &identifier2, tempDistance, faceNumber2);
	//RWTC_CalculateTempReflectedRays(face, incidentrayTubes[i].ray3, &tempReflectedRayTubes[i].ray3, faceCountPerBlock, &identifier3, tempDistance, faceNumber3);
	if(identifier[identifierNumber] == 0)
	{
		tempReflectedRayTubes[i].ray1 = RWTC_CalculateReflectedRayWithTerrainOnGPU(incidentrayTubes[i].ray1, face, faceCountPerBlock, &reflectedFace1, &identifier1);
		identifier[identifierNumber] = identifier1;
	}
	if(identifier[identifierNumber+1] == 0)
	{
		tempReflectedRayTubes[i].ray2 = RWTC_CalculateReflectedRayWithTerrainOnGPU(incidentrayTubes[i].ray2, face, faceCountPerBlock, &reflectedFace2, &identifier2);
		identifier[identifierNumber+1] = identifier2;
	}
	if(identifier[identifierNumber+2] == 0)
	{
		tempReflectedRayTubes[i].ray3 = RWTC_CalculateReflectedRayWithTerrainOnGPU(incidentrayTubes[i].ray3, face, faceCountPerBlock, &reflectedFace3, &identifier3);
		identifier[identifierNumber+2] = identifier3;
	}
}





//将CPU中的数据传到GPU中计算,并将算出来的结果拷回GPU
cudaError_t GetTriangleReflectedRayTubes(TriangleRayTube *incidentRayTubes,TriangleRayTube *tempReflectedRayTubes,
	Face *face,unsigned int rayTubeCount,unsigned int faceCount,int *identifier,
	float *tempDistance, FILE *fp)
{






	clock_t totalTime = 0, startTime = 0, endTime = 0;
	Face *device_face = 0;
	float *device_tempDistance = 0;
	int *device_identifier = 0;
	TriangleRayTube *device_incidentRayTubes = 0;
	TriangleRayTube *device_tempReflectedRayTubes = 0;
	cudaError_t cudaStatus; 



	startTime = clock();
//	cudaEventRecord(start, 0);
	fprintf(fp, "cudaMalloc的起始时间：%dms\n", startTime);
//	fprintf(fp, "cudaMalloc的起始时间：%fus\n", start);
	cudaStatus = cudaMalloc((void**)&device_incidentRayTubes, rayTubeCount * sizeof(TriangleRayTube));
	if(cudaStatus != cudaSuccess)
	{
		fprintf(stderr, "device_rayTubes cudaMalloc error!");
		goto Error;
	}
	cudaStatus = cudaMalloc((void**)&device_tempReflectedRayTubes, rayTubeCount * sizeof(TriangleRayTube));
	if(cudaStatus != cudaSuccess)
	{
		fprintf(stderr, "device_rayTubes cudaMalloc error!");
		goto Error;
	}
	cudaStatus = cudaMalloc((void**)&device_face, faceCount * sizeof(Face));
	if(cudaStatus != cudaSuccess)
	{
		fprintf(stderr, "face cudaMalloc error!");
		goto Error;
	}
	cudaStatus = cudaMalloc((void**)&device_identifier, 3 * rayTubeCount * sizeof(int));
	if(cudaStatus != cudaSuccess)
	{
		fprintf(stderr, "identifier cudaMalloc error!");
		goto Error;
	}
	cudaStatus = cudaMalloc((void**)&device_tempDistance, 3 * rayTubeCount * sizeof(float));
	if(cudaStatus != cudaSuccess)
	{
		fprintf(stderr, "tempDistance cudaMalloc error!");
		goto Error;
	}



	endTime = clock();
//	cudaEventRecord(stop, 0);
//	cudaEventElapsedTime(&elapsedTime, start, stop);
	fprintf(fp, "cudaMalloc的终止时间：%dms\n", endTime);
//	fprintf(fp, "cudaMalloc的终止时间：%fus\n", stop);
//	fprintf(fp, "cudaMalloc的用时为：%fus\n", elapsedTime);


	startTime = clock();
//	cudaEventRecord(start, 0);
	fprintf(fp,"CPU数据拷到GPU的起始时间：%dms\n", startTime);
//	fprintf(fp,"CPU数据拷到GPU的起始时间：%fus\n", start);
	cudaStatus = cudaMemcpy(device_face, face, faceCount * sizeof(Face), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) 
	{
        fprintf(stderr, "face Memcpy failed!");
        goto Error;
    }
	cudaStatus = cudaMemcpy(device_incidentRayTubes, incidentRayTubes, rayTubeCount * sizeof(TriangleRayTube), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) 
	{
        fprintf(stderr,"incidentRayTubes Memcpy failed!");
        goto Error;
    }
	cudaStatus = cudaMemcpy(device_identifier, identifier, 3 * rayTubeCount * sizeof(int), cudaMemcpyHostToDevice);
	if (cudaStatus != cudaSuccess) 
	{
        fprintf(stderr,"identifier Memcpy failed!");
        goto Error;
    }
	cudaStatus = cudaMemcpy(device_tempReflectedRayTubes, tempReflectedRayTubes, rayTubeCount * sizeof(TriangleRayTube), cudaMemcpyHostToDevice);
	if (cudaStatus != cudaSuccess) 
	{
        fprintf(stderr,"tempReflectedRayTubes Memcpy failed!");
        goto Error;
    }
	//dim3 DimGrid(5,15);
	//dim3 DimBlock(24,36);
	//CaculatetempReflectedRays<<<DimGrid,DimBlock>>>(device_face,device_incidentRays,device_tempReflectedRays,faceNumber);
	const int num_blocks = 32;
	const int num_threads = 640;





	endTime = clock();
//	cudaEventRecord(stop, 0);
//	cudaEventElapsedTime(&elapsedTime, start, stop);
	fprintf(fp, "CPU数据拷到GPU的终止时间：%dms\n", endTime);
//	fprintf(fp, "CPU数据拷到GPU的终止时间：%fus\n", stop);
//	fprintf(fp, "CPU数据拷到GPU的用时为：%fus\n", elapsedTime);

	cudaEvent_t start, stop;
	float elapsedTime = 0.0;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	startTime = clock();
	cudaEventRecord(start, 0);
	fprintf(fp,"核函数运行的起始时间：%dms\n", startTime);
//	fprintf(fp,"核函数运行的起始时间：%fus\n", start);


	CalculateTempTriangleReflectedRayTubesOnGPU<<<num_blocks, num_threads>>>(device_face, device_incidentRayTubes, device_tempReflectedRayTubes, faceCount, device_identifier, device_tempDistance);
	//cudaStatus = cudaDeviceSynchronize();


	cudaEventRecord(stop, 0);
	cudaEventSynchronize(start);    
	cudaEventSynchronize(stop);	endTime = clock();
	cudaEventElapsedTime(&elapsedTime, start, stop);
	fprintf(fp, "核函数运行的终止时间：%fms\n", elapsedTime);
	fprintf(fp, "核函数运行的终止时间：%dms\n", endTime);
//  fprintf(fp, "核函数运行的终止时间：%fus\n", stop);

	cudaEventDestroy(start);
	cudaEventDestroy(stop);


	startTime = clock();
//	cudaEventRecord(start, 0);
	fprintf(fp,"GPU数据拷到CPU的起始时间：%dms\n", startTime);
//	fprintf(fp,"GPU数据拷到CPU的起始时间：%fus\n", start);
    if (cudaStatus != cudaSuccess)
	{
        fprintf(stderr, "cudaDeviceSynchronize returned error!", cudaStatus);
        goto Error;
    }

	cudaStatus = cudaMemcpy(identifier, device_identifier, 3 * rayTubeCount * sizeof(int), cudaMemcpyDeviceToHost);     
	if(cudaStatus != cudaSuccess)
	{
		fprintf(stderr, "isRefletedRaysExist cudaMemcpy error!");
		goto Error;
	}

	cudaStatus = cudaMemcpy(tempReflectedRayTubes, device_tempReflectedRayTubes, rayTubeCount * sizeof(TriangleRayTube), cudaMemcpyDeviceToHost);     
	if(cudaStatus != cudaSuccess)
	{
		fprintf(stderr, "tempTriangleReflectedRayTubes cudaMemcpy error!");
		goto Error;
	}

	cudaStatus = cudaMemcpy(tempDistance,device_tempDistance, 3 * rayTubeCount*sizeof(int),cudaMemcpyDeviceToHost);     
	if(cudaStatus != cudaSuccess)
	{
		fprintf(stderr, "tempDistance cudaMemcpy error!");
		goto Error;
	}
	endTime = clock();
//	cudaEventRecord(stop, 0);
//	cudaEventElapsedTime(&elapsedTime, start, stop);
	fprintf(fp, "GPU数据拷到CPU的终止时间：%dms\n", endTime);
//	fprintf(fp, "GPU数据拷到CPU的终止时间：%fus\n", stop);
//	fprintf(fp, "GPU数据拷到CPU的终止时间：%fus\n", elapsedTime);
Error:

	startTime = clock();
//	cudaEventRecord(start, 0);
	fprintf(fp,"cudaFree的起始时间：%dms\n", startTime);
//	fprintf(fp,"cudaFree的起始时间：%fus\n", start);
	cudaFree(device_incidentRayTubes);
	cudaFree(device_tempReflectedRayTubes);
	cudaFree(device_face);
	cudaFree(device_identifier);
	cudaFree(device_tempDistance);


	endTime = clock();
//	cudaEventRecord(stop, 0);
//	cudaEventElapsedTime(&elapsedTime, start, stop);
	fprintf(fp, "cudaFree的终止时间：%dms\n", endTime);
//	fprintf(fp, "cudaFree的终止时间：%fus\n", stop);
//	fprintf(fp, "cudaFree的终止时间：%fus\n", elapsedTime);


	return cudaStatus;
}

int CalculateTriangleReflectedRayTubes(const unsigned int rayTubeCount, 
	unsigned int faceCount, Face *p_face, TriangleRayTube *incidentRayTubes, 
	TriangleRayTube *ReflectedRayTubes, int *isReflectedRayTubesExist, float *distance)
{
//rayTubeCount射线管数目    变量类型const unsigned int
//faceCount面的数目    变量类型unsigned int
//p_face面类型指针    变量类型Face *
//incidentRayTubes入射射线管    变量类型RayTube *
//ReflectedRayTubes反射射线管    变量类型RayTube *
//isReflectedRayTubesExist反射射线管是否存在    变量类型int *
//distance距离最近的才是反射射线    变量类型float *
	static TriangleRayTube tempRayTubes[20480];
	static int identifier[61440];
	static float tempDistance[61440];
	//TriangleRayTube tempRayTubes[20480];
	//int identifier[20480];
	//float tempDistance[61440];

	memset(identifier, 0, 61440*sizeof(int));
	const int faceCountPerTime = 100;

	int time = faceCount / faceCountPerTime;
	int remainder = faceCount % faceCountPerTime;
	cudaError_t cudaStatus;
	FILE *fp;


	fp = fopen("GPU时间.txt","w+");




	clock_t totalTime = 0, startTime = 0, endTime = 0;
	startTime = clock();
	printf("起始时间：%dms\n", startTime);
	for(int i = 0; i <= time; i++)
	{
		fprintf(fp, "当前是第%d次运行的结果\n", i+1);
		if(i < time)
		{
			cudaStatus = GetTriangleReflectedRayTubes(incidentRayTubes, tempRayTubes, p_face, rayTubeCount, faceCountPerTime, identifier, tempDistance, fp);

		}
		else
		{
			cudaStatus = GetTriangleReflectedRayTubes(incidentRayTubes, tempRayTubes, p_face, rayTubeCount, remainder, identifier, tempDistance, fp);						
		}

		p_face += faceCountPerTime;		
	}
	endTime = clock();

	printf("终止时间：%dms\n", endTime);
	fclose(fp);
	totalTime = endTime - startTime;
	printf("在GPU上一共跑的用时为%dms\n", totalTime);



	for(int j = 0; j < rayTubeCount; j++)
	{
		isReflectedRayTubesExist[j] = identifier[3*j] * identifier[3*j+1] * identifier[3*j+2];
		if(isReflectedRayTubesExist[j] == 1)
		{				
			ReflectedRayTubes[j] = tempRayTubes[j];					
		}
	}
	endTime = clock();
	printf("终止时间2：%dms\n", endTime);
	totalTime = endTime - startTime;
	printf("在GPU上一共跑的用时为%ldms\n", totalTime);
	return 0;
}

__global__ void CalculateTempQuadrangleReflectedRayTubesOnGPU(Face *face, 
	QuadrangleRayTube *incidentrayTubes, QuadrangleRayTube *tempReflectedRayTubes, 
	int faceCountPerBlock, int *identifier, float *tempDistance)
{

	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int identifier1, identifier2, identifier3,identifier4;
	int distanceNumber = 4 * i;
	RWTC_CalculateTempReflectedRays(face, incidentrayTubes[i].ray1, &tempReflectedRayTubes[i].ray1 , faceCountPerBlock, &identifier1, tempDistance, distanceNumber);
	RWTC_CalculateTempReflectedRays(face, incidentrayTubes[i].ray2, &tempReflectedRayTubes[i].ray2, faceCountPerBlock, &identifier2, tempDistance, distanceNumber+1);
	RWTC_CalculateTempReflectedRays(face, incidentrayTubes[i].ray3, &tempReflectedRayTubes[i].ray3, faceCountPerBlock, &identifier3, tempDistance, distanceNumber+2);
	RWTC_CalculateTempReflectedRays(face, incidentrayTubes[i].ray3, &tempReflectedRayTubes[i].ray4, faceCountPerBlock, &identifier4, tempDistance, distanceNumber+3);
	identifier[i] = identifier1 * identifier2 * identifier3 * identifier4;
}



cudaError_t GetQuadrangleReflectedRayTubes(QuadrangleRayTube *incidentRayTubes,QuadrangleRayTube *tempReflectedRayTubes,
	Face *face,unsigned int rayTubeCount,unsigned int faceCount,int *identifier,
	float *tempDistance)
{

	Face *device_face = 0;
	float *device_tempDistance = 0;
	int *device_identifier = 0;
	QuadrangleRayTube *device_incidentRayTubes = 0;
	QuadrangleRayTube *device_tempReflectedRayTubes = 0;
	cudaError_t cudaStatus; 
	cudaStatus = cudaMalloc((void**)&device_incidentRayTubes, rayTubeCount * sizeof(QuadrangleRayTube));
	if(cudaStatus != cudaSuccess)
	{
		fprintf(stderr, "device_rayTubes cudaMalloc error!");
		goto Error;
	}
	cudaStatus = cudaMalloc((void**)&device_tempReflectedRayTubes, rayTubeCount * sizeof(QuadrangleRayTube));
	if(cudaStatus != cudaSuccess)
	{
		fprintf(stderr, "device_rayTubes cudaMalloc error!");
		goto Error;
	}
	cudaStatus = cudaMalloc((void**)&device_face, faceCount * sizeof(Face));
	if(cudaStatus != cudaSuccess)
	{
		fprintf(stderr, "face cudaMalloc error!");
		goto Error;
	}
	cudaStatus = cudaMalloc((void**)&device_identifier, rayTubeCount * sizeof(int));
	if(cudaStatus != cudaSuccess)
	{
		fprintf(stderr, "identifier cudaMalloc error!");
		goto Error;
	}
	cudaStatus = cudaMalloc((void**)&device_tempDistance, 4 * rayTubeCount * sizeof(float));
	if(cudaStatus != cudaSuccess)
	{
		fprintf(stderr, "tempDistance cudaMalloc error!");
		goto Error;
	}
	cudaStatus = cudaMemcpy(device_face, face, faceCount * sizeof(Face), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) 
	{
        fprintf(stderr, "face Memcpy failed!");
        goto Error;
    }
	cudaStatus = cudaMemcpy(device_incidentRayTubes, incidentRayTubes, rayTubeCount * sizeof(QuadrangleRayTube), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) 
	{
        fprintf(stderr,"incidentRayTubes Memcpy failed!");
        goto Error;
    }
	//dim3 DimGrid(5,15);
	//dim3 DimBlock(24,36);
	//CaculatetempReflectedRays<<<DimGrid,DimBlock>>>(device_face,device_incidentRays,device_tempReflectedRays,faceNumber);
	const int num_blocks = 32;
	const int num_threads = 640;




	CalculateTempQuadrangleReflectedRayTubesOnGPU<<<num_blocks, num_threads>>>(device_face, device_incidentRayTubes, device_tempReflectedRayTubes, faceCount, device_identifier, device_tempDistance);



	cudaStatus = cudaDeviceSynchronize();
    if (cudaStatus != cudaSuccess)
	{
        fprintf(stderr, "cudaDeviceSynchronize returned error!", cudaStatus);
        goto Error;
    }
	cudaStatus = cudaMemcpy(tempReflectedRayTubes, device_tempReflectedRayTubes, rayTubeCount * sizeof(QuadrangleRayTube), cudaMemcpyDeviceToHost);     
	if(cudaStatus != cudaSuccess)
	{
		fprintf(stderr, "tempQuadrangleRayTubeReflectedRayTubes cudaMemcpy error!");
		goto Error;
	}
	cudaStatus = cudaMemcpy(identifier, device_identifier, rayTubeCount * sizeof(int), cudaMemcpyDeviceToHost);     
	if(cudaStatus != cudaSuccess)
	{
		fprintf(stderr, "isRefletedRaysExist cudaMemcpy error!");
		goto Error;
	}
	cudaStatus = cudaMemcpy(tempDistance,device_tempDistance, 3 * rayTubeCount*sizeof(int),cudaMemcpyDeviceToHost);     
	if(cudaStatus != cudaSuccess)
	{
		fprintf(stderr, "tempDistance cudaMemcpy error!");
		goto Error;
	}
Error:
	cudaFree(device_incidentRayTubes);
	cudaFree(device_tempReflectedRayTubes);
	cudaFree(device_face);
	cudaFree(device_identifier);
	cudaFree(device_tempDistance);
	return cudaStatus;
}


int CalculateQuadrangleReflectedRayTubes(const unsigned int rayTubeCount, 
	unsigned int faceCount, Face *p_face, QuadrangleRayTube *incidentRayTubes, 
	QuadrangleRayTube *ReflectedRayTubes, int *isReflectedRayTubesExist, float *distance)
{
//rayTubeCount射线管数目    变量类型const unsigned int
//faceCount面的数目    变量类型unsigned int
//p_face面类型指针    变量类型Face *
//incidentRayTubes入射射线管    变量类型RayTube *
//ReflectedRayTubes反射射线管    变量类型RayTube *
//isReflectedRayTubesExist反射射线管是否存在    变量类型int *
//distance距离最近的才是反射射线    变量类型float *
	static QuadrangleRayTube tempRayTubes[20480];
	static int identifier[20480];
	static float tempDistance[81920];
	//TriangleRayTube tempRayTubes[20480];
	//int identifier[20480];
	//float tempDistance[61440];


	const int faceCountPerTime = 3000;

	int time = faceCount / faceCountPerTime;
	int remainder = faceCount % faceCountPerTime;
	cudaError_t cudaStatus;

	for(int i = 0; i <= time; i++)
	{
		if(i < time)
		{
			cudaStatus = GetQuadrangleReflectedRayTubes(incidentRayTubes, tempRayTubes, p_face, rayTubeCount, faceCountPerTime, identifier, tempDistance);
		}
		else
		{
			cudaStatus = GetQuadrangleReflectedRayTubes(incidentRayTubes, tempRayTubes, p_face, rayTubeCount, remainder, identifier, tempDistance);
		}
		for(int j = 0; j < rayTubeCount; j++)
		{
			int distanceNumber = 4 * j;
			if(identifier[j] == 1 && tempDistance[distanceNumber] < distance[distanceNumber])
			{				
				ReflectedRayTubes[j] = tempRayTubes[j];
				distance[distanceNumber] = tempDistance[distanceNumber];
				isReflectedRayTubesExist[j] = 1;					
			}
			if(identifier[j] == 1 && tempDistance[++distanceNumber] < distance[distanceNumber])
			{				
				ReflectedRayTubes[j] = tempRayTubes[j];
				distance[distanceNumber] = tempDistance[distanceNumber];
				isReflectedRayTubesExist[j] = 1;					
			}
			if(identifier[j] == 1 && tempDistance[++distanceNumber] < distance[distanceNumber])
			{				
				ReflectedRayTubes[j] = tempRayTubes[j];
				distance[distanceNumber] = tempDistance[distanceNumber];
				isReflectedRayTubesExist[j] = 1;					
			}

		}
		p_face += faceCountPerTime;		
	}
	return 0;
}


__device__ Ray RWTC_CalculateReflectedRayWithTerrainOnGPU(Ray incidentRay,Face *face,int faceCount,int *reflectedFace,int *flag)
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
				RWTC_UnitizeVector(&n.x,&n.y,&n.z);
				reflectedRay.direction=RWTC_GetReflectedVector(incidentRay.direction,n);
			}
		}
	}
	return reflectedRay;
}