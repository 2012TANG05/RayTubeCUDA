#include "PathNodeOnGPU.h"

//接收三角形射线管
//*rayTubes为所有射线管 *receivedRayTubes为接收到的射线管 receiver接收机坐标 rayTubeCount所有射线管个数
void ReceiveTriangleRayTubes(TriangleRayTube *rayTubes, TriangleRayTube *&receivedRayTubes, Point receiver, 
	const int rayTubeCount, int &receivedRayTubeCnt)
{
	if(rayTubeCount==0)
	{
		return;
	}
	bool *isReceived = new bool[rayTubeCount];
//	static bool isReceived[20480];
	GetTriangleRayTubesReceived(rayTubes, isReceived, receiver, rayTubeCount);
	receivedRayTubeCnt = GetReceivedRayTubeCount(isReceived, rayTubeCount);
	receivedRayTubes = new TriangleRayTube[receivedRayTubeCnt];
	FilterReceivedRayTubes_Tri(rayTubes, receivedRayTubes, rayTubeCount, receivedRayTubeCnt, isReceived);
	delete[] isReceived;
}

//接收四边形射线管
//*rayTubes为所有射线管 *receivedRayTubes为接收到的射线管 receiver接收机坐标 rayTubeCount所有射线管个数
void ReceiveQuadrangleRayTubes(QuadrangleRayTube *rayTubes, QuadrangleRayTube *&receivedRayTubes, Point receiver, 
	const int rayTubeCount, int &receivedRayTubeCnt)
{
	if(rayTubeCount==0)
	{
		return;
	}
	bool *isReceived = new bool[rayTubeCount];
	GetQuadrangleRayTubesReceived(rayTubes, isReceived, receiver, rayTubeCount);
	receivedRayTubeCnt = GetReceivedRayTubeCount(isReceived, rayTubeCount);
	receivedRayTubes = new QuadrangleRayTube[receivedRayTubeCnt];
	FilterReceivedRayTubes_Qua(rayTubes, receivedRayTubes, rayTubeCount, receivedRayTubeCnt, isReceived);
	delete[] isReceived;
}

//此函数得到新生成的反射射线管，同时可以知道哪些射线管发生了绕射，以及绕射面与绕射棱的信息
//*incidentRayTubes为入射射线管 *reflectedRayTubes为有效反射射线管 incidentRayTubeCnt为入射射线管数量 reflectedRayTubeCnt为有效反射射线管数量
//buildingFaceCnt为建筑物三角面数量 *buildingFaces为建筑物三角面 *diffractionEdge为绕射棱 *face1为绕射劈面1 *face2为绕射劈面2
//*incidentDiffractingRayTubes为入射射线管中，发生绕射的射线管
void BuildingReflectionAndJudgeDiffraction(TriangleRayTube *incidentRayTubes, TriangleRayTube *&reflectedRayTubes, 
	const int incidentRayTubeCnt, int &reflectedRayTubeCnt, int &diffractingRayTubeCnt, const int buildingFaceCnt, Face *buildingFaces, 
	Edge *&diffractionEdge, Face *&face1, Face *&face2, TriangleRayTube *&incidentDiffractingRayTubes)
{
	if(incidentRayTubeCnt==0)
	{
		return;
	}
	TriangleRayTube *tmp_reflectedRayTubes = new TriangleRayTube[incidentRayTubeCnt*2];//用于存放计算所得射线
	Edge *tmp_diffractionEdge = new Edge[incidentRayTubeCnt];//用于存放绕射棱
	int *tmp_faceNumber1 = new int[incidentRayTubeCnt];
	int *tmp_faceNumber2 = new int[incidentRayTubeCnt];
	int *faceNumber1;
	int *faceNumber2;
	int *isReflectionExist = new int[incidentRayTubeCnt*2];
	int *isDiffractionExist = new int[incidentRayTubeCnt];
	GetReflectionAndDiffractionWithBuildingFace(incidentRayTubes, buildingFaceCnt, incidentRayTubeCnt, buildingFaces, 
		tmp_reflectedRayTubes, tmp_diffractionEdge, tmp_faceNumber1, tmp_faceNumber2, 
		isDiffractionExist,isReflectionExist);//计算建筑物反射射线管，记录发生绕射的射线管GPU函数

	//FILE *fp=fopen("C:\\Users\\EMC\\Desktop\\GPUtest.txt","w");
	//int i=0;
	//while (i<incidentRayTubeCnt)
	//{
	//	if (isReflectionExist[i]==1)
	//	{
	//		fprintf(fp,"%d 反射面1:(%f, %f, %f) \n",i,tmp_reflectedRayTubes[2*i].path.node1.point1.x,tmp_reflectedRayTubes[2*i].path.node1.point1.y,tmp_reflectedRayTubes[2*i].path.node1.point1.z);
	//		fprintf(fp,"     反射面2:(%f, %f, %f) \n",tmp_reflectedRayTubes[2*i+1].path.node1.point1.x,tmp_reflectedRayTubes[2*i+1].path.node1.point1.y,tmp_reflectedRayTubes[2*i+1].path.node1.point1.z);
	//		fprintf(fp,"     反射点1ray1:(%f, %f, %f) \n",tmp_reflectedRayTubes[2*i].ray1.originalPoint.x,tmp_reflectedRayTubes[2*i].ray1.originalPoint.y,tmp_reflectedRayTubes[2*i].ray1.originalPoint.z);
	//		fprintf(fp,"     反射点1ray2:(%f, %f, %f) \n",tmp_reflectedRayTubes[2*i].ray2.originalPoint.x,tmp_reflectedRayTubes[2*i].ray2.originalPoint.y,tmp_reflectedRayTubes[2*i].ray2.originalPoint.z);
	//		fprintf(fp,"     反射点1ray3:(%f, %f, %f) \n",tmp_reflectedRayTubes[2*i].ray3.originalPoint.x,tmp_reflectedRayTubes[2*i].ray3.originalPoint.y,tmp_reflectedRayTubes[2*i].ray3.originalPoint.z);
	//		fprintf(fp,"     反射点2ray1:(%f, %f, %f) \n",tmp_reflectedRayTubes[2*i+1].ray1.originalPoint.x,tmp_reflectedRayTubes[2*i+1].ray1.originalPoint.y,tmp_reflectedRayTubes[2*i+1].ray1.originalPoint.z);
	//		fprintf(fp,"     反射点2ray2:(%f, %f, %f) \n",tmp_reflectedRayTubes[2*i+1].ray2.originalPoint.x,tmp_reflectedRayTubes[2*i+1].ray2.originalPoint.y,tmp_reflectedRayTubes[2*i+1].ray2.originalPoint.z);
	//		fprintf(fp,"     反射点2ray3:(%f, %f, %f) \n",tmp_reflectedRayTubes[2*i+1].ray3.originalPoint.x,tmp_reflectedRayTubes[2*i+1].ray3.originalPoint.y,tmp_reflectedRayTubes[2*i+1].ray3.originalPoint.z);
	//		fprintf(fp,"     反射向量1ray1:(%f, %f, %f) \n",tmp_reflectedRayTubes[2*i].ray1.direction.x,tmp_reflectedRayTubes[2*i].ray1.direction.y,tmp_reflectedRayTubes[2*i].ray1.direction.z);
	//		fprintf(fp,"     反射向量1ray2:(%f, %f, %f) \n",tmp_reflectedRayTubes[2*i].ray2.direction.x,tmp_reflectedRayTubes[2*i].ray2.direction.y,tmp_reflectedRayTubes[2*i].ray2.direction.z);
	//		fprintf(fp,"     反射向量1ray3:(%f, %f, %f) \n",tmp_reflectedRayTubes[2*i].ray3.direction.x,tmp_reflectedRayTubes[2*i].ray3.direction.y,tmp_reflectedRayTubes[2*i].ray3.direction.z);
	//		fprintf(fp,"     反射向量2ray1:(%f, %f, %f) \n",tmp_reflectedRayTubes[2*i+1].ray1.direction.x,tmp_reflectedRayTubes[2*i+1].ray1.direction.y,tmp_reflectedRayTubes[2*i+1].ray1.direction.z);
	//		fprintf(fp,"     反射向量2ray2:(%f, %f, %f) \n",tmp_reflectedRayTubes[2*i+1].ray2.direction.x,tmp_reflectedRayTubes[2*i+1].ray2.direction.y,tmp_reflectedRayTubes[2*i+1].ray2.direction.z);
	//		fprintf(fp,"     反射向量2ray3:(%f, %f, %f) \n\n",tmp_reflectedRayTubes[2*i+1].ray3.direction.x,tmp_reflectedRayTubes[2*i+1].ray3.direction.y,tmp_reflectedRayTubes[2*i+1].ray3.direction.z);
	//	}
	//	i++;
	//}
	//fclose(fp);

	reflectedRayTubeCnt = GetNewReflectedRayTubesCount(isReflectionExist, incidentRayTubeCnt*2);//得到有效反射射线管的个数
	reflectedRayTubes = new TriangleRayTube[reflectedRayTubeCnt];//申请有效反射射线管的动态数组
	FilterReflectedRayTubes(tmp_reflectedRayTubes, reflectedRayTubes, 
		reflectedRayTubeCnt, incidentRayTubeCnt*2, isReflectionExist);//获得有效反射射线管
	
	diffractingRayTubeCnt = GetDiffractionEdgeCount(isDiffractionExist, incidentRayTubeCnt,tmp_faceNumber1,tmp_faceNumber2);//获得有效的发生绕射的入射射线管个数
	incidentDiffractingRayTubes = new TriangleRayTube[diffractingRayTubeCnt];//给发生绕射的入射射线管分配空间
	faceNumber1 = new int[diffractingRayTubeCnt];
	faceNumber2 = new int[diffractingRayTubeCnt];
	diffractionEdge = new Edge[diffractingRayTubeCnt];
	FilterDiffractionExistIncidentRayTubes(incidentRayTubes, incidentDiffractingRayTubes, tmp_diffractionEdge, diffractionEdge,
		tmp_faceNumber1, faceNumber1, tmp_faceNumber2, faceNumber2, incidentRayTubeCnt, isDiffractionExist);//获得发生绕射的入射射线管信息
	
	FILE *fp1=fopen("C:\\Users\\EMC\\Desktop\\GPUtest.txt","w");
	int i1=0;
	while (i1<diffractingRayTubeCnt)
	{
		fprintf(fp1,"%d edge:((%f, %f, %f),(%f, %f, %f), face1:%d, face2:%d \n",i1,diffractionEdge[i1].p1.x,diffractionEdge[i1].p1.y,diffractionEdge[i1].p1.z,diffractionEdge[i1].p2.x,diffractionEdge[i1].p2.y,diffractionEdge[i1].p2.z,faceNumber1[i1],faceNumber2[i1]);	
		i1++;
	}
	fclose(fp1);


	face1 = new Face[diffractingRayTubeCnt];//为绕射劈面1分配空间
	face2 = new Face[diffractingRayTubeCnt];//为绕射劈面2分配空间
	FilterDiffractionFaces(buildingFaces, face1, face2, faceNumber1, faceNumber2, diffractingRayTubeCnt);//获得绕射劈面信息
	delete[] tmp_reflectedRayTubes;
	delete[] tmp_diffractionEdge;
	delete[] tmp_faceNumber1;
	delete[] tmp_faceNumber2;
	delete[] faceNumber1;
	delete[] faceNumber2;
	delete[] isReflectionExist;
	delete[] isDiffractionExist;
}

//产生绕射射线
void BuildingDiffraction(TriangleRayTube *incidentRayTubes, Edge *&diffractionEdge, Face *&diffractionFace1, Face *&diffractionFace2,
	QuadrangleRayTube *&diffractionRayTubes, unsigned const int diffractionRayCnt, unsigned const int RaysToBeDiffractedCnt)
{
	if(RaysToBeDiffractedCnt == 0)
	{
		printf("绕射射线数为0\n");
		return;
	}
	Ray *diffractionRays1 = new Ray[diffractionRayCnt * RaysToBeDiffractedCnt];
	Ray *diffractionRays2 = new Ray[diffractionRayCnt * RaysToBeDiffractedCnt];
	diffractionRayTubes = new QuadrangleRayTube[(diffractionRayCnt - 1) * RaysToBeDiffractedCnt];
	GetDiffractionRaysInGPU(incidentRayTubes, diffractionEdge, diffractionFace1, diffractionFace2, diffractionRays1, diffractionRays2,
		diffractionRayTubes, diffractionRayCnt, RaysToBeDiffractedCnt);//生成绕射射线

	FILE *fp=fopen("C:\\Users\\EMC\\Desktop\\一次绕射射线管GPU.txt","w");
	for(int i=0;i<(diffractionRayCnt - 1) * RaysToBeDiffractedCnt;i++)
	{
		fprintf(fp,"棱%d的射线管%d的第1条射线：(%f, %f, %f)",i/11,i%11+1, diffractionRayTubes[i].ray1.originalPoint.x, diffractionRayTubes[i].ray1.originalPoint.y, diffractionRayTubes[i].ray1.originalPoint.z);
		fprintf(fp,"(%f, %f, %f)\n",diffractionRayTubes[i].ray1.direction.x, diffractionRayTubes[i].ray1.direction.y, diffractionRayTubes[i].ray1.direction.z);
		fprintf(fp,"棱%d的射线管%d的第2条射线：(%f, %f, %f)",i/11,i%11+1, diffractionRayTubes[i].ray2.originalPoint.x, diffractionRayTubes[i].ray2.originalPoint.y, diffractionRayTubes[i].ray2.originalPoint.z);
		fprintf(fp,"(%f, %f, %f)\n",diffractionRayTubes[i].ray2.direction.x, diffractionRayTubes[i].ray2.direction.y, diffractionRayTubes[i].ray2.direction.z);
		fprintf(fp,"棱%d的射线管%d的第3条射线：(%f, %f, %f)",i/11,i%11+1, diffractionRayTubes[i].ray3.originalPoint.x, diffractionRayTubes[i].ray3.originalPoint.y, diffractionRayTubes[i].ray3.originalPoint.z);
		fprintf(fp,"(%f, %f, %f)\n",diffractionRayTubes[i].ray3.direction.x, diffractionRayTubes[i].ray3.direction.y, diffractionRayTubes[i].ray3.direction.z);
		fprintf(fp,"棱%d的射线管%d的第4条射线：(%f, %f, %f)",i/11,i%11+1, diffractionRayTubes[i].ray4.originalPoint.x, diffractionRayTubes[i].ray4.originalPoint.y, diffractionRayTubes[i].ray4.originalPoint.z);
		fprintf(fp,"(%f, %f, %f)\n",diffractionRayTubes[i].ray4.direction.x, diffractionRayTubes[i].ray4.direction.y, diffractionRayTubes[i].ray4.direction.z);
		fprintf(fp,"--------------------------------------------------------------\n");
	}
	fclose(fp);
	delete[] diffractionRays1;
	delete[] diffractionRays2;
}

//四边形射线管在建筑物的反射
void BuildingReflection_Qua(QuadrangleRayTube *incidentRayTubes, const int buildingFaceCnt, const int RayTubeCnt, int &reflectedRayTubeCnt,
	Face *buildingFaces, QuadrangleRayTube *&reflectedRayTubes)
{
	QuadrangleRayTube *tmp_reflectedRayTubes = new QuadrangleRayTube[RayTubeCnt*2];
	int *isReflectionExist = new int[RayTubeCnt*2];
	GetReflectionWithBuildingFaceOnGPU(incidentRayTubes, buildingFaceCnt, RayTubeCnt, buildingFaces, tmp_reflectedRayTubes, isReflectionExist);
	reflectedRayTubeCnt = GetNewReflectedRayTubesCount(isReflectionExist, RayTubeCnt*2);
	reflectedRayTubes = new QuadrangleRayTube[reflectedRayTubeCnt];
	FilterReflectedRayTubes_Qua(tmp_reflectedRayTubes, reflectedRayTubes, reflectedRayTubeCnt, RayTubeCnt*2, isReflectionExist);
	delete[] tmp_reflectedRayTubes;
	delete[] isReflectionExist;
}

//三角形射线管在地形的反射
//*incidentRayTubes为入射射线管 *reflectedRayTubes为有效反射射线管 incidentRayTubeCnt为入射射线管数量 reflectedRayTubeCnt为有效反射射线管数量
void TerReflection_Tri(TriangleRayTube *incidentRayTubes, TriangleRayTube *&reflectedRayTubes, Face *faces,
	const int incidentRayTubeCnt, const int faceCnt, int &reflectedRayTubeCnt)
{
	if(incidentRayTubeCnt==0)
	{
		return;
	}
	TriangleRayTube *tmp_reflectedRayTubes = new TriangleRayTube[incidentRayTubeCnt];//用于存放计算所得射线
	int *isReflectedTubeExist = new int[incidentRayTubeCnt];//用于存放筛选标志位
	float *tmp_distance = new float[incidentRayTubeCnt*3];//发射点到交点的距离
	CalculateTriangleReflectedRayTubesWithTerrainOnGPU(incidentRayTubeCnt, faceCnt, faces, incidentRayTubes, tmp_reflectedRayTubes, isReflectedTubeExist, tmp_distance);//此处调用求三角形射线管与地面反射GPU函数~~~~~~~~~~~
	reflectedRayTubeCnt = GetNewReflectedRayTubesCount(isReflectedTubeExist, incidentRayTubeCnt);//得到有效反射射线管的个数
	reflectedRayTubes = new TriangleRayTube[reflectedRayTubeCnt];//申请有效反射射线管的动态数组
	FilterReflectedRayTubes(tmp_reflectedRayTubes, reflectedRayTubes, reflectedRayTubeCnt, incidentRayTubeCnt, isReflectedTubeExist);//对所得射线管进行筛选的函数，获得有效反射射线管的动态数组
	delete[] tmp_reflectedRayTubes;
	delete[] isReflectedTubeExist;
	delete[] tmp_distance;
}

//四边形射线管在地形的反射
void TerReflection_Qua(QuadrangleRayTube *incidentRayTubes, QuadrangleRayTube *&reflectedRayTubes, Face *faces,
	const int incidentRayTubeCnt, const int faceCnt, int &reflectedRayTubeCnt)
{
	QuadrangleRayTube *tmp_reflectedRayTubes = new QuadrangleRayTube[incidentRayTubeCnt];//用于存放计算所得射线
	int *isReflectedTubeExist = new int[incidentRayTubeCnt];//用于存放筛选标志位
	float *tmp_distance = new float[incidentRayTubeCnt*4];//发射点到交点的距离
	GetQuadrangleReflectedRayTubes(incidentRayTubes, tmp_reflectedRayTubes, faces, incidentRayTubeCnt, faceCnt, isReflectedTubeExist, tmp_distance);//此处调用求四边形射线管与地面反射GPU函数~~~~~~~~~~~
	reflectedRayTubeCnt = GetNewReflectedRayTubesCount(isReflectedTubeExist, incidentRayTubeCnt);//得到有效反射射线管的个数
	reflectedRayTubes = new QuadrangleRayTube[reflectedRayTubeCnt];//申请有效反射射线管的动态数组
	FilterReflectedRayTubes_Qua(tmp_reflectedRayTubes, reflectedRayTubes, reflectedRayTubeCnt, incidentRayTubeCnt, isReflectedTubeExist);//对所得射线管进行筛选的函数，获得有效反射射线管的动态数组
	delete[] tmp_reflectedRayTubes;
	delete[] isReflectedTubeExist;
	delete[] tmp_distance;
}