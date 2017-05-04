#include "ReflectionWithTerrainCalculationOnCPU.h"


//GPU中计算反射射线管
//face面    变量类型Face *
//incidentrayTubes入射射线管    变量类型TriangleRayTube *
//ReflectedRayTubes反射射线管    变量类型TriangleRayTube *
//faceCountPerBlock每个block面的数目，数据过大会出错    变量类型int
//identifier 判断是否有反射射线管只有1和0    变量类型int *
//temDistance 判断距离大小用于找到最近的反射点（远的不会被打到）   变量类型float*
void CalculateTempTriangleReflectedRayTubesOnCPU(Face *face, 
	TriangleRayTube *incidentrayTubes, TriangleRayTube *tempReflectedRayTubes, 
	int faceCountPerBlock, int *identifier, float *tempDistance)
{	
	int i;
	int reflectedFace1 = -1, reflectedFace2 = -1, reflectedFace3 = -1;//反射面，暂时未用到
	for(i=0; i<20480; i++)
	{
		int identifier1 = 0, identifier2 = 0, identifier3 = 0;
		int distanceNumber = 3 * i;
		tempReflectedRayTubes[i].ray1 = CalculateReflectedRayWithTerrainOnCPU(incidentrayTubes[i].ray1, face, faceCountPerBlock, &reflectedFace1, &identifier1);
		tempReflectedRayTubes[i].ray2 = CalculateReflectedRayWithTerrainOnCPU(incidentrayTubes[i].ray2, face, faceCountPerBlock, &reflectedFace2, &identifier2);
		tempReflectedRayTubes[i].ray3 = CalculateReflectedRayWithTerrainOnCPU(incidentrayTubes[i].ray3, face, faceCountPerBlock, &reflectedFace3, &identifier3);
		identifier[distanceNumber] = identifier1;
		identifier[distanceNumber+1] = identifier2;
		identifier[distanceNumber+2] = identifier3;
	}
}


//rayTubeCount射线管数目    变量类型const unsigned int
//faceCount面的数目    变量类型unsigned int
//p_face面类型指针    变量类型Face *
//incidentRayTubes入射射线管    变量类型RayTube *
//ReflectedRayTubes反射射线管    变量类型RayTube *
//isReflectedRayTubesExist反射射线管是否存在    变量类型int *
//distance距离最近的才是反射射线    变量类型float *
int CalculateTriangleReflectedRayTubesOnCPU(const unsigned int rayTubeCount, 
	unsigned int faceCount, Face *p_face, TriangleRayTube *incidentRayTubes, 
	TriangleRayTube *ReflectedRayTubes, int *isReflectedRayTubesExist, float *distance)
{
	static TriangleRayTube tempRayTubes[20480];
	static int identifier[61440];
	static float tempDistance[61440];
	const int faceCountPerTime = 3000;

	//int time = faceCount / faceCountPerTime;
	//int remainder = faceCount % faceCountPerTime;
	CalculateTempTriangleReflectedRayTubesOnCPU(p_face, incidentRayTubes, tempRayTubes, faceCount, identifier, tempDistance);
	for (int i=0; i<rayTubeCount; i++)
	{
		isReflectedRayTubesExist[i] = identifier[3*i] * identifier[3*i+1] * identifier[3*i+2];
		if (isReflectedRayTubesExist[i] == 1)
		{
			ReflectedRayTubes[i] = tempRayTubes[i];
		}
	}
	//for(int i = 0; i <= time; i++)
	//{
	//	if(i < time)
	//	{
	//		CalculateTempTriangleReflectedRayTubesOnCPU(p_face, incidentRayTubes, tempRayTubes, faceCountPerTime, identifier, tempDistance);
	//	}
	//	else
	//	{
	//		CalculateTempTriangleReflectedRayTubesOnCPU(p_face, incidentRayTubes, tempRayTubes, remainder, identifier, tempDistance);
	//	}
	//	for(int j = 0; j < rayTubeCount; j++)
	//	{
	//		//int distanceNumber = 3 * j;
	//		if(identifier[j] == 1 )
	//		{				
	//			ReflectedRayTubes[j] = tempRayTubes[j];
	//			//distance[distanceNumber] = tempDistance[distanceNumber];
	//			isReflectedRayTubesExist[j] = 1;					
	//		}
	//		//if(identifier[j] == 1 && tempDistance[++distanceNumber] < distance[distanceNumber])
	//		//{				
	//		//	ReflectedRayTubes[j] = tempRayTubes[j];
	//		//	distance[distanceNumber] = tempDistance[distanceNumber];
	//		//	isReflectedRayTubesExist[j] = 1;					
	//		//}
	//		//if(identifier[j] == 1 && tempDistance[++distanceNumber] < distance[distanceNumber])
	//		//{				
	//		//	ReflectedRayTubes[j] = tempRayTubes[j];
	//		//	distance[distanceNumber] = tempDistance[distanceNumber];
	//		//	isReflectedRayTubesExist[j] = 1;					
	//		//}
	//	}
	//	p_face += faceCountPerTime;	
	//}
	return 0;
}


//四边形反射，未用到
int CPUCalculateQuadrangleReflectedRayTubes(const unsigned int rayTubeCount, 
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
			CPUCalculateTempQuadrangleReflectedRayTubes(p_face, incidentRayTubes, tempRayTubes, faceCountPerTime, identifier, tempDistance);
		}
		else
		{
			CPUCalculateTempQuadrangleReflectedRayTubes(p_face, incidentRayTubes, tempRayTubes, remainder, identifier, tempDistance);
		}
		for(int j = 0; j < rayTubeCount; j++)
		{
			int distanceNumber = 4 * j;
			if(identifier[j] == 1)
			{				
				ReflectedRayTubes[j] = tempRayTubes[j];
				distance[distanceNumber] = tempDistance[distanceNumber];
				isReflectedRayTubesExist[j] = 1;					
			}
		}
		//	if(identifier[j] == 1 && tempDistance[++distanceNumber] < distance[distanceNumber])
		//	{				
		//		ReflectedRayTubes[j] = tempRayTubes[j];
		//		distance[distanceNumber] = tempDistance[distanceNumber];
		//		isReflectedRayTubesExist[j] = 1;					
		//	}
		//	if(identifier[j] == 1 && tempDistance[++distanceNumber] < distance[distanceNumber])
		//	{				
		//		ReflectedRayTubes[j] = tempRayTubes[j];
		//		distance[distanceNumber] = tempDistance[distanceNumber];
		//		isReflectedRayTubesExist[j] = 1;					
		//	}

		//}
		p_face += faceCountPerTime;		
	}
	return 0;
}

void CPUCalculateTempQuadrangleReflectedRayTubes(Face *face, 
	QuadrangleRayTube *incidentrayTubes, QuadrangleRayTube *tempReflectedRayTubes, 
	int faceCountPerBlock, int *identifier, float *tempDistance)
{	
	int i;
	int identifier1, identifier2, identifier3,identifier4;
	for(i=0; i<20480; i++)
	{
		int distanceNumber = 3 * i;
		identifier[i] = identifier1 * identifier2 * identifier3 * identifier4;
	}
}
