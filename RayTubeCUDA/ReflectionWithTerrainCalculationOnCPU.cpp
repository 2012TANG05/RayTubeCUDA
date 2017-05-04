#include "ReflectionWithTerrainCalculationOnCPU.h"


//GPU�м��㷴�����߹�
//face��    ��������Face *
//incidentrayTubes�������߹�    ��������TriangleRayTube *
//ReflectedRayTubes�������߹�    ��������TriangleRayTube *
//faceCountPerBlockÿ��block�����Ŀ�����ݹ�������    ��������int
//identifier �ж��Ƿ��з������߹�ֻ��1��0    ��������int *
//temDistance �жϾ����С�����ҵ�����ķ���㣨Զ�Ĳ��ᱻ�򵽣�   ��������float*
void CalculateTempTriangleReflectedRayTubesOnCPU(Face *face, 
	TriangleRayTube *incidentrayTubes, TriangleRayTube *tempReflectedRayTubes, 
	int faceCountPerBlock, int *identifier, float *tempDistance)
{	
	int i;
	int reflectedFace1 = -1, reflectedFace2 = -1, reflectedFace3 = -1;//�����棬��ʱδ�õ�
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


//rayTubeCount���߹���Ŀ    ��������const unsigned int
//faceCount�����Ŀ    ��������unsigned int
//p_face������ָ��    ��������Face *
//incidentRayTubes�������߹�    ��������RayTube *
//ReflectedRayTubes�������߹�    ��������RayTube *
//isReflectedRayTubesExist�������߹��Ƿ����    ��������int *
//distance��������Ĳ��Ƿ�������    ��������float *
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


//�ı��η��䣬δ�õ�
int CPUCalculateQuadrangleReflectedRayTubes(const unsigned int rayTubeCount, 
	unsigned int faceCount, Face *p_face, QuadrangleRayTube *incidentRayTubes, 
	QuadrangleRayTube *ReflectedRayTubes, int *isReflectedRayTubesExist, float *distance)
{
	//rayTubeCount���߹���Ŀ    ��������const unsigned int
	//faceCount�����Ŀ    ��������unsigned int
	//p_face������ָ��    ��������Face *
	//incidentRayTubes�������߹�    ��������RayTube *
	//ReflectedRayTubes�������߹�    ��������RayTube *
	//isReflectedRayTubesExist�������߹��Ƿ����    ��������int *
	//distance��������Ĳ��Ƿ�������    ��������float *
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
