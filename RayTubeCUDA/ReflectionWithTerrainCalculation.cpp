#include "ReflectionWithTerrainCalculation.h"
#include<time.h>


//单位化向量
void CPURWTC_UnitizeVector(float *a, float *b, float *c)
{
	float length = sqrt((*a) * (*a) + (*b) * (*b) + (*c) * (*c));
	*a = *a / length;
	*b = *b / length;
	*c = *c / length;
}

 
//计算两点间距离
float CPURWTC_CalculateDistanceOfBipartite(Point a,Point b)
{
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
}


//计算反射射线方向向量
Point CPURWTC_GetReflectedVector(Point d,Point n)
{
	Point reflectedVector;
	float temp = 2 * (d.x * n.x + d.y * n.y + d.z * n.z);
	reflectedVector.x = d.x - temp * n.x;
	reflectedVector.y = d.y - temp * n.y;
	reflectedVector.z = d.z - temp * n.z;
	return reflectedVector;
}

//计算反射
void CPURWTC_CalculateTempReflectedRays(Face *face, Ray incidentray, 
	Ray *tempReflectedRay, int faceCountPerBlock, int *identifier, float *tempDistance, 
	int distanceNumber)
{
//face面    数据类型Face *
//incidentray入射向量    数据类型Ray
//tempReflectedRay反射向量    数据类型Ray *
//faceCountPerBlock每次测得面的数目    数据类型int
//identifier 用于确定是否村在反射射线    数据类型int *
//tempDistance 距离短则为实际的反射射线    数据类型float *
//distanceNumber距离编号    用于改变distanceNumber
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
			if(u > 0 && u < 1 && (u + v) > 0 && (u + v) < 1 && t_numerator / denominator > 0)
			{//有交点且交点在平面内，且距离近(需要考虑因为计算误差导致一次反射点在平面下的问题，会很快打到平面上)
				t = t_numerator / denominator;
				(*tempReflectedRay).originalPoint.x =	incidentray.originalPoint.x + incidentray.direction.x * t;
				(*tempReflectedRay).originalPoint.y = incidentray.originalPoint.y + incidentray.direction.y * t;
				(*tempReflectedRay).originalPoint.z = incidentray.originalPoint.z + incidentray.direction.z * t;
				Point n,d;
				n.x = b2 * c3 - b3 * c2; n.y = b3 * c1 - b1 * c3; n.z = b1 * c2 - c1 * b2;
				d.x = incidentray.direction.x; d.y = incidentray.direction.y; d.z = incidentray.direction.z;
				CPURWTC_UnitizeVector(&n.x, &n.y, &n.z);
				Point reflectedVector = CPURWTC_GetReflectedVector(d, n);
				(*tempReflectedRay).direction.x = reflectedVector.x;
				(*tempReflectedRay).direction.y = reflectedVector.y;
				(*tempReflectedRay).direction.z = reflectedVector.z;
				//*identifier = 1;
				//tempDistance[distanceNumber] = t;
				//break;
				if((u>0)&&(u<1)&&((u+v)>0)&&((u+v)<1)&&v>0&&v<1)
				{
					*identifier = 1;
//					faceNumber = i;
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

//GPU中计算反射射线管
//face面    变量类型Face *
//incidentrayTubes入射射线管    变量类型TriangleRayTube *
//ReflectedRayTubes反射射线管    变量类型TriangleRayTube *
//faceCountPerBlock每个block面的数目，数据过大会出错    变量类型int
//identifier 判断是否有反射射线管只有1和0    变量类型int *
//temDistance 判断距离大小用于找到最近的反射点（远的不会被打到）   变量类型float*
void CPUCalculateTempTriangleReflectedRayTubes(Face *face, 
	TriangleRayTube *incidentrayTubes, TriangleRayTube *tempReflectedRayTubes, 
	int faceCountPerBlock, int *identifier, float *tempDistance)
{	
	int i;
	FILE *fp;
	fp = fopen("cpu时间.txt","w+");

	int reflectedFace1 = -1, reflectedFace2 = -1, reflectedFace3 = -1;
	for(i=0; i<20480; i++)
	{
		clock_t startTime, endTime;

		startTime = clock();
		fprintf(fp, "第%d条射线管起始时间1：%dms\n", i, startTime);
		int identifier1 = 0, identifier2 = 0, identifier3 = 0;
		int distanceNumber = 3 * i;
		tempReflectedRayTubes[i].ray1 = CalculateReflectedRayWithTerrainOnCPU(incidentrayTubes[i].ray1, face, faceCountPerBlock, &reflectedFace1, &identifier1);
		tempReflectedRayTubes[i].ray2 = CalculateReflectedRayWithTerrainOnCPU(incidentrayTubes[i].ray2, face, faceCountPerBlock, &reflectedFace2, &identifier2);
		tempReflectedRayTubes[i].ray3 = CalculateReflectedRayWithTerrainOnCPU(incidentrayTubes[i].ray3, face, faceCountPerBlock, &reflectedFace3, &identifier3);
		/*CPURWTC_CalculateTempReflectedRays(face, incidentrayTubes[i].ray1, &tempReflectedRayTubes[i].ray1 , faceCountPerBlock, &identifier1, tempDistance, distanceNumber);
		CPURWTC_CalculateTempReflectedRays(face, incidentrayTubes[i].ray2, &tempReflectedRayTubes[i].ray2, faceCountPerBlock, &identifier2, tempDistance, distanceNumber+1);
		CPURWTC_CalculateTempReflectedRays(face, incidentrayTubes[i].ray3, &tempReflectedRayTubes[i].ray3, faceCountPerBlock, &identifier3, tempDistance, distanceNumber+2);	*/
		identifier[distanceNumber] = identifier1;
		identifier[distanceNumber+1] = identifier2;
		identifier[distanceNumber+2] = identifier3;

		endTime = clock();
		fprintf(fp,"第%d条射线管终止时间1：%dms\n", i, endTime);
	}
	fclose(fp);

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
		CPURWTC_CalculateTempReflectedRays(face, incidentrayTubes[i].ray1, &tempReflectedRayTubes[i].ray1 , faceCountPerBlock, &identifier1, tempDistance, distanceNumber);
		CPURWTC_CalculateTempReflectedRays(face, incidentrayTubes[i].ray2, &tempReflectedRayTubes[i].ray2, faceCountPerBlock, &identifier2, tempDistance, distanceNumber+1);
		CPURWTC_CalculateTempReflectedRays(face, incidentrayTubes[i].ray3, &tempReflectedRayTubes[i].ray3, faceCountPerBlock, &identifier3, tempDistance, distanceNumber+2);
		CPURWTC_CalculateTempReflectedRays(face, incidentrayTubes[i].ray4, &tempReflectedRayTubes[i].ray4, faceCountPerBlock, &identifier4, tempDistance, distanceNumber+3);
		identifier[i] = identifier1 * identifier2 * identifier3 * identifier4;
	}
}

int CPUCalculateTriangleReflectedRayTubes(const unsigned int rayTubeCount, 
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
//	static int identifier[20480];
	static int identifier[61440];
	static float tempDistance[61440];
	//TriangleRayTube tempRayTubes[20480];
	//int identifier[20480];
	//float tempDistance[61440];


	const int faceCountPerTime = 3000;

	//int time = faceCount / faceCountPerTime;
	//int remainder = faceCount % faceCountPerTime;
	CPUCalculateTempTriangleReflectedRayTubes(p_face, incidentRayTubes, tempRayTubes, faceCount, identifier, tempDistance);
	for (int i=0;i<rayTubeCount;i++)
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
	//		CPUCalculateTempTriangleReflectedRayTubes(p_face, incidentRayTubes, tempRayTubes, faceCountPerTime, identifier, tempDistance);
	//	}
	//	else
	//	{
	//		CPUCalculateTempTriangleReflectedRayTubes(p_face, incidentRayTubes, tempRayTubes, remainder, identifier, tempDistance);
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