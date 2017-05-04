#include "PathNodeOnCPU.h"

//�������������߹�
//*rayTubesΪ�������߹� *receivedRayTubesΪ���յ������߹� receiver���ջ����� rayTubeCount�������߹ܸ���
void ReceiveTriangleRayTubesOnCPU(TriangleRayTube *rayTubes, TriangleRayTube *&receivedRayTubes, Point receiver, 
	const int rayTubeCount, int &receivedRayTubeCnt)
{
	if(rayTubeCount==0)
	{
		return;
	}
	bool *isReceived = new bool[rayTubeCount];
	IsTriangleRayTubesReceivedOnCPU(rayTubes, receiver, rayTubeCount, isReceived);//CPU����
	receivedRayTubeCnt = GetReceivedRayTubeCount(isReceived, rayTubeCount);
	receivedRayTubes = new TriangleRayTube[receivedRayTubeCnt];
	FilterReceivedRayTubes_Tri(rayTubes, receivedRayTubes, rayTubeCount, receivedRayTubeCnt, isReceived);
	delete[] isReceived;
}

//�����ı������߹�
//*rayTubesΪ�������߹� *receivedRayTubesΪ���յ������߹� receiver���ջ����� rayTubeCount�������߹ܸ���
void ReceiveQuadrangleRayTubesOnCPU(QuadrangleRayTube *rayTubes, QuadrangleRayTube *&receivedRayTubes, Point receiver, 
	const int rayTubeCount1, int &receivedRayTubeCnt)
{
	int rayTubeCount = rayTubeCount1 * 11;
	if(rayTubeCount1==0)
	{
		return;
	}
	bool *isReceived = new bool[rayTubeCount];
	IsQuadrangleRayTubesReceivedOnCPU(rayTubes, receiver, rayTubeCount, isReceived);//CPU����
	receivedRayTubeCnt = GetReceivedRayTubeCount(isReceived, rayTubeCount);
	receivedRayTubes = new QuadrangleRayTube[receivedRayTubeCnt];
	FilterReceivedRayTubes_Qua(rayTubes, receivedRayTubes, rayTubeCount, receivedRayTubeCnt, isReceived);
	delete[] isReceived;
}

//�˺����õ������ɵķ������߹ܣ�ͬʱ����֪����Щ���߹ܷ��������䣬�Լ������������������Ϣ
//*incidentRayTubesΪ�������߹� *reflectedRayTubesΪ��Ч�������߹� incidentRayTubeCntΪ�������߹����� reflectedRayTubeCntΪ��Ч�������߹�����
//buildingFaceCntΪ���������������� *buildingFacesΪ������������ *diffractionEdgeΪ������ *face1Ϊ��������1 *face2Ϊ��������2
//*incidentDiffractingRayTubesΪ�������߹��У�������������߹�
void BuildingReflectionAndJudgeDiffractionOnCPU(TriangleRayTube *incidentRayTubes, TriangleRayTube *&reflectedRayTubes, 
	const int incidentRayTubeCnt, int &reflectedRayTubeCnt, int &diffractingRayTubeCnt, const int buildingFaceCnt, Face *buildingFaces, 
	Edge *&diffractionEdge, Face *&face1, Face *&face2, TriangleRayTube *&incidentDiffractingRayTubes)
{
	if(incidentRayTubeCnt==0)
	{
		return;
	}
	TriangleRayTube *tmp_reflectedRayTubes = new TriangleRayTube[incidentRayTubeCnt*2];//���ڴ�ż�����������
	Edge *tmp_diffractionEdge = new Edge[incidentRayTubeCnt];//���ڴ��������
	int *tmp_faceNumber1 = new int[incidentRayTubeCnt];
	int *tmp_faceNumber2 = new int[incidentRayTubeCnt];
	int *faceNumber1;
	int *faceNumber2;
	int *isReflectionExist = new int[incidentRayTubeCnt*2];
	int *isDiffractionExist = new int[incidentRayTubeCnt];
	CalculateReflectionAndDiffractionWithBuildingFaceOnCPU(incidentRayTubes, buildingFaceCnt, incidentRayTubeCnt, 
		tmp_reflectedRayTubes, buildingFaces, tmp_diffractionEdge, tmp_faceNumber1, tmp_faceNumber2, 
		isReflectionExist, isDiffractionExist);//���㽨���ﷴ�����߹ܣ���¼������������߹�GPU����

	FILE *fp=fopen("C:\\Users\\EMC\\Desktop\\CPUtest.txt","w");
	int i=0;
	while (i<incidentRayTubeCnt)
	{
		if (isReflectionExist[i]==1)
		{
			fprintf(fp,"%d ������1:(%f, %f, %f) \n",i,tmp_reflectedRayTubes[2*i].path.node1.point1.x,tmp_reflectedRayTubes[2*i].path.node1.point1.y,tmp_reflectedRayTubes[2*i].path.node1.point1.z);
			fprintf(fp,"     ������2:(%f, %f, %f) \n",tmp_reflectedRayTubes[2*i+1].path.node1.point1.x,tmp_reflectedRayTubes[2*i+1].path.node1.point1.y,tmp_reflectedRayTubes[2*i+1].path.node1.point1.z);
			fprintf(fp,"     �����1ray1:(%f, %f, %f) \n",tmp_reflectedRayTubes[2*i].ray1.originalPoint.x,tmp_reflectedRayTubes[2*i].ray1.originalPoint.y,tmp_reflectedRayTubes[2*i].ray1.originalPoint.z);
			fprintf(fp,"     �����1ray2:(%f, %f, %f) \n",tmp_reflectedRayTubes[2*i].ray2.originalPoint.x,tmp_reflectedRayTubes[2*i].ray2.originalPoint.y,tmp_reflectedRayTubes[2*i].ray2.originalPoint.z);
			fprintf(fp,"     �����1ray3:(%f, %f, %f) \n",tmp_reflectedRayTubes[2*i].ray3.originalPoint.x,tmp_reflectedRayTubes[2*i].ray3.originalPoint.y,tmp_reflectedRayTubes[2*i].ray3.originalPoint.z);
			fprintf(fp,"     �����2ray1:(%f, %f, %f) \n",tmp_reflectedRayTubes[2*i+1].ray1.originalPoint.x,tmp_reflectedRayTubes[2*i+1].ray1.originalPoint.y,tmp_reflectedRayTubes[2*i+1].ray1.originalPoint.z);
			fprintf(fp,"     �����2ray2:(%f, %f, %f) \n",tmp_reflectedRayTubes[2*i+1].ray2.originalPoint.x,tmp_reflectedRayTubes[2*i+1].ray2.originalPoint.y,tmp_reflectedRayTubes[2*i+1].ray2.originalPoint.z);
			fprintf(fp,"     �����2ray3:(%f, %f, %f) \n",tmp_reflectedRayTubes[2*i+1].ray3.originalPoint.x,tmp_reflectedRayTubes[2*i+1].ray3.originalPoint.y,tmp_reflectedRayTubes[2*i+1].ray3.originalPoint.z);
			fprintf(fp,"     ��������1ray1:(%f, %f, %f) \n",tmp_reflectedRayTubes[2*i].ray1.direction.x,tmp_reflectedRayTubes[2*i].ray1.direction.y,tmp_reflectedRayTubes[2*i].ray1.direction.z);
			fprintf(fp,"     ��������1ray2:(%f, %f, %f) \n",tmp_reflectedRayTubes[2*i].ray2.direction.x,tmp_reflectedRayTubes[2*i].ray2.direction.y,tmp_reflectedRayTubes[2*i].ray2.direction.z);
			fprintf(fp,"     ��������1ray3:(%f, %f, %f) \n",tmp_reflectedRayTubes[2*i].ray3.direction.x,tmp_reflectedRayTubes[2*i].ray3.direction.y,tmp_reflectedRayTubes[2*i].ray3.direction.z);
			fprintf(fp,"     ��������2ray1:(%f, %f, %f) \n",tmp_reflectedRayTubes[2*i+1].ray1.direction.x,tmp_reflectedRayTubes[2*i+1].ray1.direction.y,tmp_reflectedRayTubes[2*i+1].ray1.direction.z);
			fprintf(fp,"     ��������2ray2:(%f, %f, %f) \n",tmp_reflectedRayTubes[2*i+1].ray2.direction.x,tmp_reflectedRayTubes[2*i+1].ray2.direction.y,tmp_reflectedRayTubes[2*i+1].ray2.direction.z);
			fprintf(fp,"     ��������2ray3:(%f, %f, %f) \n\n",tmp_reflectedRayTubes[2*i+1].ray3.direction.x,tmp_reflectedRayTubes[2*i+1].ray3.direction.y,tmp_reflectedRayTubes[2*i+1].ray3.direction.z);
		}
		i++;
	}
	fclose(fp);

	reflectedRayTubeCnt = GetNewReflectedRayTubesCount(isReflectionExist, incidentRayTubeCnt*2);//�õ���Ч�������߹ܵĸ���
	reflectedRayTubes = new TriangleRayTube[reflectedRayTubeCnt];//������Ч�������߹ܵĶ�̬����
	FilterReflectedRayTubes(tmp_reflectedRayTubes, reflectedRayTubes, 
		reflectedRayTubeCnt, incidentRayTubeCnt*2, isReflectionExist);//�����Ч�������߹�

	diffractingRayTubeCnt = GetDiffractionEdgeCount(isDiffractionExist, incidentRayTubeCnt,tmp_faceNumber1,tmp_faceNumber2);//�����Ч�ķ���������������߹ܸ���
	incidentDiffractingRayTubes = new TriangleRayTube[diffractingRayTubeCnt];//������������������߹ܷ���ռ�
	faceNumber1 = new int[diffractingRayTubeCnt];
	faceNumber2 = new int[diffractingRayTubeCnt];
	diffractionEdge = new Edge[diffractingRayTubeCnt];
	FilterDiffractionExistIncidentRayTubes(incidentRayTubes, incidentDiffractingRayTubes, tmp_diffractionEdge, diffractionEdge,
		tmp_faceNumber1, faceNumber1, tmp_faceNumber2, faceNumber2, incidentRayTubeCnt, isDiffractionExist);//��÷���������������߹���Ϣ

	FILE *fp1=fopen("C:\\Users\\EMC\\Desktop\\CPUtest.txt","w");
	int ii=0;
	while (ii<diffractingRayTubeCnt)
	{
		fprintf(fp1,"%d edge:((%f, %f, %f),(%f, %f, %f), face1:%d, face2:%d \n",ii,diffractionEdge[ii].p1.x,diffractionEdge[ii].p1.y,diffractionEdge[ii].p1.z,diffractionEdge[ii].p2.x,diffractionEdge[ii].p2.y,diffractionEdge[ii].p2.z,faceNumber1[ii],faceNumber2[ii]);	
		ii++;
	}
	fclose(fp1);


	face1 = new Face[diffractingRayTubeCnt];//Ϊ��������1����ռ�
	face2 = new Face[diffractingRayTubeCnt];//Ϊ��������2����ռ�

	FilterDiffractionFaces(buildingFaces, face1, face2, faceNumber1, faceNumber2, diffractingRayTubeCnt);//�������������Ϣ
	delete[] tmp_reflectedRayTubes;
	delete[] tmp_diffractionEdge;
	delete[] tmp_faceNumber1;
	delete[] tmp_faceNumber2;
	delete[] faceNumber1;
	delete[] faceNumber2;
	delete[] isReflectionExist;
	delete[] isDiffractionExist;
}

//������������
void BuildingDiffractionOnCPU(TriangleRayTube *incidentRayTubes, Edge *&diffractionEdge, Face *&diffractionFace1, Face *&diffractionFace2,
	QuadrangleRayTube *&diffractionRayTubes, unsigned const int diffractionRayCnt, unsigned const int RaysToBeDiffractedCnt)
{
	if(diffractionRayCnt == 0)
	{
		printf("����������Ϊ0\n");
		return;
	}
	Ray *diffractionRays1 = new Ray[diffractionRayCnt * RaysToBeDiffractedCnt];
	Ray *diffractionRays2 = new Ray[diffractionRayCnt * RaysToBeDiffractedCnt];
	diffractionRayTubes = new QuadrangleRayTube[(diffractionRayCnt - 1) * RaysToBeDiffractedCnt];
	GenerateDiffractionRaysOnCPU(incidentRayTubes, diffractionEdge, diffractionFace1, diffractionFace2,
		diffractionRayTubes, diffractionRayCnt, RaysToBeDiffractedCnt);//������������

	FILE *fp=fopen("C:\\Users\\EMC\\Desktop\\һ���������߹�CPU.txt","w");
	for(int i=0;i<(diffractionRayCnt - 1) * RaysToBeDiffractedCnt;i++)
	{
		fprintf(fp,"��%d�����߹�%d�ĵ�1�����ߣ�(%f, %f, %f)",i/11,i%11+1, diffractionRayTubes[i].ray1.originalPoint.x, diffractionRayTubes[i].ray1.originalPoint.y, diffractionRayTubes[i].ray1.originalPoint.z);
		fprintf(fp,"(%f, %f, %f)\n",diffractionRayTubes[i].ray1.direction.x, diffractionRayTubes[i].ray1.direction.y, diffractionRayTubes[i].ray1.direction.z);
		fprintf(fp,"��%d�����߹�%d�ĵ�2�����ߣ�(%f, %f, %f)",i/11,i%11+1, diffractionRayTubes[i].ray2.originalPoint.x, diffractionRayTubes[i].ray2.originalPoint.y, diffractionRayTubes[i].ray2.originalPoint.z);
		fprintf(fp,"(%f, %f, %f)\n",diffractionRayTubes[i].ray2.direction.x, diffractionRayTubes[i].ray2.direction.y, diffractionRayTubes[i].ray2.direction.z);
		fprintf(fp,"��%d�����߹�%d�ĵ�3�����ߣ�(%f, %f, %f)",i/11,i%11+1, diffractionRayTubes[i].ray3.originalPoint.x, diffractionRayTubes[i].ray3.originalPoint.y, diffractionRayTubes[i].ray3.originalPoint.z);
		fprintf(fp,"(%f, %f, %f)\n",diffractionRayTubes[i].ray3.direction.x, diffractionRayTubes[i].ray3.direction.y, diffractionRayTubes[i].ray3.direction.z);
		fprintf(fp,"��%d�����߹�%d�ĵ�4�����ߣ�(%f, %f, %f)",i/11,i%11+1, diffractionRayTubes[i].ray4.originalPoint.x, diffractionRayTubes[i].ray4.originalPoint.y, diffractionRayTubes[i].ray4.originalPoint.z);
		fprintf(fp,"(%f, %f, %f)\n",diffractionRayTubes[i].ray4.direction.x, diffractionRayTubes[i].ray4.direction.y, diffractionRayTubes[i].ray4.direction.z);
		fprintf(fp,"--------------------------------------------------------------\n");
	}
	fclose(fp);

	delete[] diffractionRays1;
	delete[] diffractionRays2;
}

//�ı������߹��ڽ�����ķ���
void BuildingReflection_QuaOnCPU(QuadrangleRayTube *incidentRayTubes, const int buildingFaceCnt, const int RayTubeCnt, int &reflectedRayTubeCnt,
	Face *buildingFaces, QuadrangleRayTube *&reflectedRayTubes)
{
	QuadrangleRayTube *tmp_reflectedRayTubes = new QuadrangleRayTube[RayTubeCnt*2];
	int *isReflectionExist = new int[RayTubeCnt*2];
	CalculateReflectionWithBuildingFaceOnCPU(incidentRayTubes, buildingFaceCnt, RayTubeCnt, tmp_reflectedRayTubes, buildingFaces, isReflectionExist);
	reflectedRayTubeCnt = GetNewReflectedRayTubesCount(isReflectionExist, RayTubeCnt*2);
	reflectedRayTubes = new QuadrangleRayTube[reflectedRayTubeCnt];
	FilterReflectedRayTubes_Qua(tmp_reflectedRayTubes, reflectedRayTubes, reflectedRayTubeCnt, RayTubeCnt*2, isReflectionExist);
	delete[] tmp_reflectedRayTubes;
	delete[] isReflectionExist;
}

//���������߹��ڵ��εķ���
//*incidentRayTubesΪ�������߹� *reflectedRayTubesΪ��Ч�������߹� incidentRayTubeCntΪ�������߹����� reflectedRayTubeCntΪ��Ч�������߹�����
void TerReflection_TriOnCPU(TriangleRayTube *incidentRayTubes, TriangleRayTube *&reflectedRayTubes, Face *faces,
	const int incidentRayTubeCnt, const int faceCnt, int &reflectedRayTubeCnt)
{
	if(incidentRayTubeCnt==0)
	{
		return;
	}
	TriangleRayTube *tmp_reflectedRayTubes = new TriangleRayTube[incidentRayTubeCnt];//���ڴ�ż�����������
	int *isReflectedTubeExist = new int[incidentRayTubeCnt];//���ڴ��ɸѡ��־λ
	float *tmp_distance = new float[incidentRayTubeCnt*3];//����㵽����ľ���
	CalculateTriangleReflectedRayTubesOnCPU(incidentRayTubeCnt, faceCnt, faces, incidentRayTubes, tmp_reflectedRayTubes, isReflectedTubeExist, tmp_distance);//�˴����������������߹�����淴��GPU����~~~~~~~~~~~
	//int i,j = 0;
	//for(i = 0; i<20480;i++)
	//{
	//	if(isReflectedTubeExist[i]==1)
	//	{
	//		if(j==2075)
	//			printf("��һ��ĵ�1762�����߹��ǹ�һ֮ǰ��%d\n", i);
	//		j++;
	//	}
	//}
	reflectedRayTubeCnt = GetNewReflectedRayTubesCount(isReflectedTubeExist, incidentRayTubeCnt);//�õ���Ч�������߹ܵĸ���
	reflectedRayTubes = new TriangleRayTube[reflectedRayTubeCnt];//������Ч�������߹ܵĶ�̬����
	FilterReflectedRayTubes(tmp_reflectedRayTubes, reflectedRayTubes, reflectedRayTubeCnt, incidentRayTubeCnt, isReflectedTubeExist);//���������߹ܽ���ɸѡ�ĺ����������Ч�������߹ܵĶ�̬����
	delete[] tmp_reflectedRayTubes;
	delete[] isReflectedTubeExist;
	delete[] tmp_distance;
}

//�ı������߹��ڵ��εķ���
void TerReflection_QuaOnCPU(QuadrangleRayTube *incidentRayTubes, QuadrangleRayTube *&reflectedRayTubes, Face *faces,
	const int incidentRayTubeCnt, const int faceCnt, int &reflectedRayTubeCnt)
{
	QuadrangleRayTube *tmp_reflectedRayTubes = new QuadrangleRayTube[incidentRayTubeCnt];//���ڴ�ż�����������
	int *isReflectedTubeExist = new int[incidentRayTubeCnt];//���ڴ��ɸѡ��־λ
	float *tmp_distance = new float[incidentRayTubeCnt*4];//����㵽����ľ���
	CPUCalculateQuadrangleReflectedRayTubes(incidentRayTubeCnt, faceCnt, faces, incidentRayTubes, tmp_reflectedRayTubes, isReflectedTubeExist, tmp_distance);//�˴��������ı������߹�����淴��GPU����~~~~~~~~~~~
	reflectedRayTubeCnt = GetNewReflectedRayTubesCount(isReflectedTubeExist, incidentRayTubeCnt);//�õ���Ч�������߹ܵĸ���
	reflectedRayTubes = new QuadrangleRayTube[reflectedRayTubeCnt];//������Ч�������߹ܵĶ�̬����
	FilterReflectedRayTubes_Qua(tmp_reflectedRayTubes, reflectedRayTubes, reflectedRayTubeCnt, incidentRayTubeCnt, isReflectedTubeExist);//���������߹ܽ���ɸѡ�ĺ����������Ч�������߹ܵĶ�̬����
	delete[] tmp_reflectedRayTubes;
	delete[] isReflectedTubeExist;
	delete[] tmp_distance;
}