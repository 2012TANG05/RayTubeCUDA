#include "FinalExecutionOnGPU.h"

void ExecuteOnGPU()
{
	clock_t totalTime = 0, startTime = 0, endTime = 0;
	const int diffractionRayCount = 12;//ϸ�̶ֳȣ�һ�����߲���12����������
	const string desktopPath = "C:\\Users\\EMC\\Desktop\\";
	//Point receiver;
	//receiver.x = -50;
	//receiver.y = 0;
	//receiver.z = 50;

	
	printf("��ʼGPU����\n");

	//����������begin
	BuildingReader br=BuildingReader(desktopPath + "building.info");//��ȡ�������ļ�
	vector<Building> Buildings=br.GetBuildings();//����ȡ���Ľ��������vector����
	BuildingTriangulation buildingTr=BuildingTriangulation(Buildings);//�����������ǻ�
	vector<CFace> tmpBuildingFaces=buildingTr.GetBuildingFaces();//�õ����������ǻ�������������vector����
	int buildingFaceCnt = tmpBuildingFaces.size();//��ȡ���������������
	Face *buildingFaces = new Face[buildingFaceCnt];//����������������ṹ������
	buildingFaces = TransferFaceObjectToStruct(tmpBuildingFaces);//�����������������vector����ת��Ϊ�ṹ������
	Terrain ter = Terrain(desktopPath + "Plane54.ter");//��ȡ�����ļ�
	vector<CFace> tmpTerFaces = ter.GetFaces();//�õ����ε����������vector����
	int terFaceCnt = tmpTerFaces.size();//��ȡ�������������
	Face *terFaces = new Face[terFaceCnt];//��������������ṹ������
	terFaces = TransferFaceObjectToStruct(tmpTerFaces);//���������������vector����ת��Ϊ�ṹ������
	//�����Ƿ����������vector�����Ƿ�ת��Ϊ�ṹ������
	/*for(int i=0;i<10;i++)
	{
		printf("������%d��",i+1);
		printf("(%f, %f, %f) ", terFaces[i].A.x, terFaces[i].A.y, terFaces[i].A.z);
		printf("(%f, %f, %f) ", terFaces[i].B.x, terFaces[i].B.y, terFaces[i].B.z);
		printf("(%f, %f, %f)\n", terFaces[i].C.x, terFaces[i].C.y, terFaces[i].C.z);
	}*/
	//����������end

	const int defaultRayTubeCnt = 20480;//��ʼ���������߹�Ĭ�ϸ���
	TriangleRayTube *oriTriRayTubes = new TriangleRayTube[defaultRayTubeCnt];//���ڴ�ų�ʼ����
	//���ɳ�ʼ���߹ܵĺ���
	Transmitter tx=Transmitter("C:\\Users\\EMC\\Desktop\\txh160330.tx");//��Ϊ��char���飬����ֻ��ȫ����˫����������
	GenerateOriginalRays(tx.GetPosition(),oriTriRayTubes,32,tx.GetHeight());
	printf("\n��ʼ���߹��������...\n");


	Receiver rx=Receiver("C:\\Users\\EMC\\Desktop\\txh160330.rx");//��Ϊ��char���飬����ֻ��ȫ����˫����������
	Point receiver = GenerateReceiver(rx.GetPosition(),rx.GetHeight());

	startTime = clock();
	TriangleRayTube *Tx_Rx = 0;
	int Cnt_Tx_Rx = 0;
	ReceiveTriangleRayTubes(oriTriRayTubes, Tx_Rx, receiver, defaultRayTubeCnt, Cnt_Tx_Rx);//�жϳ�ʼ���ߵ�Rx��GPU����
	printf("ֱ�����...\n");


//goto Test;
	TriangleRayTube *Tx_terR = 0;
	int Cnt_Tx_terR = 0;
	clock_t start1, end1, time;
	start1 = clock();
	TerReflection_Tri(oriTriRayTubes, Tx_terR, terFaces, defaultRayTubeCnt, terFaceCnt, Cnt_Tx_terR);//��������η���
	end1 = clock();
	time = end1 - start1;
	printf("��ǰ����η������ʱΪ%dms\n", time);	
	TriangleRayTube *Tx_terR_Rx = 0;
	int Cnt_Tx_terR_Rx = 0;
	ReceiveTriangleRayTubes(Tx_terR, Tx_terR_Rx, receiver, Cnt_Tx_terR, Cnt_Tx_terR_Rx);
	printf("�����һ�η������...\n");


//Test:	
	TriangleRayTube *Tx_bdR = 0;
	int Cnt_Tx_bdR = 0;
	Edge *edge_Tx_bdD = 0;
	Face *face1_Tx_bdD = 0;
	Face *face2_Tx_bdD = 0;
	TriangleRayTube *incident_Tx_bdD = 0;
	int Cnt_Tx_bdD = 0;
	BuildingReflectionAndJudgeDiffraction(oriTriRayTubes, Tx_bdR, defaultRayTubeCnt, Cnt_Tx_bdR, Cnt_Tx_bdD, buildingFaceCnt, buildingFaces,
		edge_Tx_bdD, face1_Tx_bdD, face2_Tx_bdD, incident_Tx_bdD); //�����뽨���ﷴ�䲢�ж���Щ�������߹ܷ�������
	TriangleRayTube *Tx_bdR_Rx = 0;
	int Cnt_Tx_bdR_Rx = 0;
	ReceiveTriangleRayTubesOnCPU(Tx_bdR, Tx_bdR_Rx, receiver, Cnt_Tx_bdR, Cnt_Tx_bdR_Rx);//�ж��뽨���ﷴ��������Ƿ�򵽽��ջ�
	printf("�뽨����һ�η������...\n");

//
	QuadrangleRayTube *Tx_bdD = 0;//�����������߹ܵĶ�̬����
	BuildingDiffraction(incident_Tx_bdD, edge_Tx_bdD, face1_Tx_bdD, face2_Tx_bdD, Tx_bdD, diffractionRayCount, Cnt_Tx_bdD);//�����������߹�
	QuadrangleRayTube *Tx_bdD_Rx = 0;
	int Cnt_Tx_bdD_Rx = 0;
	ReceiveQuadrangleRayTubesOnCPU(Tx_bdD, Tx_bdD_Rx, receiver, Cnt_Tx_bdD, Cnt_Tx_bdD_Rx);//�жϽ��������������Ƿ�򵽽��ջ�
	printf("���������һ���������...\n");
	endTime = clock();
	totalTime = endTime - startTime;
	cout<<"GPU��ʱ"<<totalTime<<" ms"<<endl;
	system("pause");
	return;
	
	//--------һ��·�����---------
	//���η�������������Ϊ��������
	TriangleRayTube *Tx_terR_bdR = 0;
	int Cnt_Tx_terR_bdR = 0;
	Edge *edge_Tx_terR_bdD = 0;
	Face *face1_Tx_terR_bdD = 0;
	Face *face2_Tx_terR_bdD = 0;
	TriangleRayTube *incident_Tx_terR_bdD = 0;
	int Cnt_Tx_terR_bdD = 0;
	BuildingReflectionAndJudgeDiffraction(Tx_terR, Tx_terR_bdR, Cnt_Tx_terR, Cnt_Tx_terR_bdR, Cnt_Tx_terR_bdD, 
		buildingFaceCnt, buildingFaces, edge_Tx_terR_bdD, face1_Tx_terR_bdD, face2_Tx_terR_bdD, incident_Tx_terR_bdD);
	TriangleRayTube *Tx_terR_bdR_Rx = 0;
	int Cnt_Tx_terR_bdR_Rx = 0;
	ReceiveTriangleRayTubes(Tx_terR_bdR, Tx_terR_bdR_Rx, receiver, Cnt_Tx_terR_bdR, Cnt_Tx_terR_bdR_Rx);//�ж��뽨���ﷴ��������Ƿ�򵽽��ջ�

	QuadrangleRayTube *Tx_terR_bdD = 0;
	BuildingDiffraction(incident_Tx_terR_bdD, edge_Tx_terR_bdD, face1_Tx_terR_bdD, face2_Tx_terR_bdD, 
		Tx_terR_bdD, Cnt_Tx_terR_bdD,diffractionRayCount);
	QuadrangleRayTube *Tx_terR_bdD_Rx = 0;
	int Cnt_Tx_terR_bdD_Rx = 0;
	ReceiveQuadrangleRayTubes(Tx_terR_bdD, Tx_terR_bdD_Rx, receiver, Cnt_Tx_terR_bdD, Cnt_Tx_terR_bdD_Rx);//�ж��뽨��������������Ƿ�򵽽��ջ�

	//�����ﷴ������������Ϊ��������
	TriangleRayTube *Tx_bdR_terR = 0;
	int Cnt_Tx_bdR_terR = 0;
	TerReflection_Tri(Tx_bdR, Tx_bdR_terR, terFaces, Cnt_Tx_bdR, terFaceCnt, Cnt_Tx_bdR_terR);//��������η���
	TriangleRayTube *Tx_bdR_terR_Rx = 0;
	int Cnt_Tx_bdR_terR_Rx = 0;
	ReceiveTriangleRayTubes(Tx_bdR_terR, Tx_bdR_terR_Rx, receiver, Cnt_Tx_bdR_terR, Cnt_Tx_bdR_terR_Rx);//�ж�����η���������Ƿ�򵽽��ջ�

	TriangleRayTube *Tx_bdR_bdR = 0;
	int Cnt_Tx_bdR_bdR = 0;
	Edge *edge_Tx_bdR_bdD = 0;
	Face *face1_Tx_bdR_bdD = 0;
	Face *face2_Tx_bdR_bdD = 0;
	TriangleRayTube *incident_Tx_bdR_bdD = 0;
	int Cnt_Tx_bdR_bdD = 0;
	BuildingReflectionAndJudgeDiffraction(Tx_bdR, Tx_bdR_bdR, Cnt_Tx_bdR, Cnt_Tx_bdR_bdR, Cnt_Tx_bdR_bdD, buildingFaceCnt, buildingFaces,
		edge_Tx_bdR_bdD, face1_Tx_bdR_bdD, face2_Tx_bdR_bdD, incident_Tx_bdR_bdD); //�����뽨���ﷴ�䲢�ж���Щ�������߹ܷ�������
	TriangleRayTube *Tx_bdR_bdR_Rx = 0;
	int Cnt_Tx_bdR_bdR_Rx = 0;
	ReceiveTriangleRayTubes(Tx_bdR_bdR, Tx_bdR_bdR_Rx, receiver, Cnt_Tx_bdR_bdR, Cnt_Tx_bdR_bdR_Rx);//�ж��뽨���ﷴ��������Ƿ�򵽽��ջ�

	QuadrangleRayTube *Tx_bdR_bdD = 0;//�����������߹ܵĶ�̬����
	BuildingDiffraction(incident_Tx_bdR_bdD, edge_Tx_bdR_bdD, face1_Tx_bdR_bdD, face2_Tx_bdR_bdD, Tx_bdR_bdD, diffractionRayCount, Cnt_Tx_bdR_bdD);//�����������߹�
	QuadrangleRayTube *Tx_bdR_bdD_Rx = 0;
	int Cnt_Tx_bdR_bdD_Rx = 0;
	ReceiveQuadrangleRayTubes(Tx_bdR_bdD, Tx_bdR_bdD_Rx, receiver, Cnt_Tx_bdR_bdD, Cnt_Tx_bdR_bdD_Rx);//�жϽ����﷢������������Ƿ�򵽽��ջ�

	
	//����������������Ϊ��������
	QuadrangleRayTube *Tx_bdD_terR = 0;
	int Cnt_Tx_bdD_terR = 0;
	TerReflection_Qua(Tx_bdD, Tx_bdD_terR, terFaces, Cnt_Tx_bdD, terFaceCnt, Cnt_Tx_bdD_terR);//�ı������߹ܵ��淴��
	QuadrangleRayTube *Tx_bdD_terR_Rx = 0;
	int Cnt_Tx_bdD_terR_Rx = 0;
	ReceiveQuadrangleRayTubes(Tx_bdD_terR, Tx_bdD_terR_Rx, receiver, Cnt_Tx_bdD_terR, Cnt_Tx_bdD_terR_Rx);//�ж�����η���������Ƿ�򵽽��ջ�

	QuadrangleRayTube *Tx_bdD_bdR = 0;
	int Cnt_Tx_bdD_bdR = 0;
	BuildingReflection_Qua(Tx_bdD, buildingFaceCnt, Cnt_Tx_bdD, Cnt_Tx_bdD_bdR, buildingFaces, Tx_bdD_bdR);//�ı������߹ܽ����ﷴ��
	QuadrangleRayTube *Tx_bdD_bdR_Rx = 0;
	int Cnt_Tx_bdD_bdR_Rx = 0;
	ReceiveQuadrangleRayTubes(Tx_bdD_bdR, Tx_bdD_bdR_Rx, receiver, Cnt_Tx_bdD_bdR, Cnt_Tx_bdD_bdR_Rx);
}
