#include "FinalExecutionOnGPU.h"

void ExecuteOnGPU()
{
	clock_t totalTime = 0, startTime = 0, endTime = 0;
	const int diffractionRayCount = 12;//细分程度，一条射线产生12条绕射射线
	const string desktopPath = "C:\\Users\\EMC\\Desktop\\";
	//Point receiver;
	//receiver.x = -50;
	//receiver.y = 0;
	//receiver.z = 50;

	
	printf("开始GPU计算\n");

	//加载三角面begin
	BuildingReader br=BuildingReader(desktopPath + "building.info");//读取建筑物文件
	vector<Building> Buildings=br.GetBuildings();//将读取到的建筑物存入vector容器
	BuildingTriangulation buildingTr=BuildingTriangulation(Buildings);//将建筑物三角化
	vector<CFace> tmpBuildingFaces=buildingTr.GetBuildingFaces();//得到建筑物三角化后的三角面对象vector数组
	int buildingFaceCnt = tmpBuildingFaces.size();//获取建筑物三角面个数
	Face *buildingFaces = new Face[buildingFaceCnt];//创建建筑物三角面结构体数组
	buildingFaces = TransferFaceObjectToStruct(tmpBuildingFaces);//将建筑物三角面对象vector数组转换为结构体数组
	Terrain ter = Terrain(desktopPath + "Plane54.ter");//读取地形文件
	vector<CFace> tmpTerFaces = ter.GetFaces();//得到地形的三角面对象vector数组
	int terFaceCnt = tmpTerFaces.size();//获取地形三角面个数
	Face *terFaces = new Face[terFaceCnt];//创建地形三角面结构体数组
	terFaces = TransferFaceObjectToStruct(tmpTerFaces);//将地形三角面对象vector数组转换为结构体数组
	//测试是否三角面对象vector数组是否转换为结构体数组
	/*for(int i=0;i<10;i++)
	{
		printf("三角面%d：",i+1);
		printf("(%f, %f, %f) ", terFaces[i].A.x, terFaces[i].A.y, terFaces[i].A.z);
		printf("(%f, %f, %f) ", terFaces[i].B.x, terFaces[i].B.y, terFaces[i].B.z);
		printf("(%f, %f, %f)\n", terFaces[i].C.x, terFaces[i].C.y, terFaces[i].C.z);
	}*/
	//加载三角面end

	const int defaultRayTubeCnt = 20480;//初始三角形射线管默认个数
	TriangleRayTube *oriTriRayTubes = new TriangleRayTube[defaultRayTubeCnt];//用于存放初始射线
	//生成初始射线管的函数
	Transmitter tx=Transmitter("C:\\Users\\EMC\\Desktop\\txh160330.tx");//因为是char数组，所以只能全部用双引号引起来
	GenerateOriginalRays(tx.GetPosition(),oriTriRayTubes,32,tx.GetHeight());
	printf("\n初始射线管生成完毕...\n");


	Receiver rx=Receiver("C:\\Users\\EMC\\Desktop\\txh160330.rx");//因为是char数组，所以只能全部用双引号引起来
	Point receiver = GenerateReceiver(rx.GetPosition(),rx.GetHeight());

	startTime = clock();
	TriangleRayTube *Tx_Rx = 0;
	int Cnt_Tx_Rx = 0;
	ReceiveTriangleRayTubes(oriTriRayTubes, Tx_Rx, receiver, defaultRayTubeCnt, Cnt_Tx_Rx);//判断初始射线到Rx的GPU函数
	printf("直射完毕...\n");


//goto Test;
	TriangleRayTube *Tx_terR = 0;
	int Cnt_Tx_terR = 0;
	clock_t start1, end1, time;
	start1 = clock();
	TerReflection_Tri(oriTriRayTubes, Tx_terR, terFaces, defaultRayTubeCnt, terFaceCnt, Cnt_Tx_terR);//计算与地形反射
	end1 = clock();
	time = end1 - start1;
	printf("当前与地形反射的用时为%dms\n", time);	
	TriangleRayTube *Tx_terR_Rx = 0;
	int Cnt_Tx_terR_Rx = 0;
	ReceiveTriangleRayTubes(Tx_terR, Tx_terR_Rx, receiver, Cnt_Tx_terR, Cnt_Tx_terR_Rx);
	printf("与地形一次反射完毕...\n");


//Test:	
	TriangleRayTube *Tx_bdR = 0;
	int Cnt_Tx_bdR = 0;
	Edge *edge_Tx_bdD = 0;
	Face *face1_Tx_bdD = 0;
	Face *face2_Tx_bdD = 0;
	TriangleRayTube *incident_Tx_bdD = 0;
	int Cnt_Tx_bdD = 0;
	BuildingReflectionAndJudgeDiffraction(oriTriRayTubes, Tx_bdR, defaultRayTubeCnt, Cnt_Tx_bdR, Cnt_Tx_bdD, buildingFaceCnt, buildingFaces,
		edge_Tx_bdD, face1_Tx_bdD, face2_Tx_bdD, incident_Tx_bdD); //计算与建筑物反射并判断哪些入射射线管发生绕射
	TriangleRayTube *Tx_bdR_Rx = 0;
	int Cnt_Tx_bdR_Rx = 0;
	ReceiveTriangleRayTubesOnCPU(Tx_bdR, Tx_bdR_Rx, receiver, Cnt_Tx_bdR, Cnt_Tx_bdR_Rx);//判断与建筑物反射的射线是否打到接收机
	printf("与建筑物一次反射完毕...\n");

//
	QuadrangleRayTube *Tx_bdD = 0;//申请绕射射线管的动态数组
	BuildingDiffraction(incident_Tx_bdD, edge_Tx_bdD, face1_Tx_bdD, face2_Tx_bdD, Tx_bdD, diffractionRayCount, Cnt_Tx_bdD);//计算绕射射线管
	QuadrangleRayTube *Tx_bdD_Rx = 0;
	int Cnt_Tx_bdD_Rx = 0;
	ReceiveQuadrangleRayTubesOnCPU(Tx_bdD, Tx_bdD_Rx, receiver, Cnt_Tx_bdD, Cnt_Tx_bdD_Rx);//判断建筑物绕射射线是否打到接收机
	printf("建筑物产生一次绕射完毕...\n");
	endTime = clock();
	totalTime = endTime - startTime;
	cout<<"GPU用时"<<totalTime<<" ms"<<endl;
	system("pause");
	return;
	
	//--------一次路径完毕---------
	//地形反射所得射线作为入射射线
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
	ReceiveTriangleRayTubes(Tx_terR_bdR, Tx_terR_bdR_Rx, receiver, Cnt_Tx_terR_bdR, Cnt_Tx_terR_bdR_Rx);//判断与建筑物反射的射线是否打到接收机

	QuadrangleRayTube *Tx_terR_bdD = 0;
	BuildingDiffraction(incident_Tx_terR_bdD, edge_Tx_terR_bdD, face1_Tx_terR_bdD, face2_Tx_terR_bdD, 
		Tx_terR_bdD, Cnt_Tx_terR_bdD,diffractionRayCount);
	QuadrangleRayTube *Tx_terR_bdD_Rx = 0;
	int Cnt_Tx_terR_bdD_Rx = 0;
	ReceiveQuadrangleRayTubes(Tx_terR_bdD, Tx_terR_bdD_Rx, receiver, Cnt_Tx_terR_bdD, Cnt_Tx_terR_bdD_Rx);//判断与建筑物绕射的射线是否打到接收机

	//建筑物反射所得射线作为入射射线
	TriangleRayTube *Tx_bdR_terR = 0;
	int Cnt_Tx_bdR_terR = 0;
	TerReflection_Tri(Tx_bdR, Tx_bdR_terR, terFaces, Cnt_Tx_bdR, terFaceCnt, Cnt_Tx_bdR_terR);//计算与地形反射
	TriangleRayTube *Tx_bdR_terR_Rx = 0;
	int Cnt_Tx_bdR_terR_Rx = 0;
	ReceiveTriangleRayTubes(Tx_bdR_terR, Tx_bdR_terR_Rx, receiver, Cnt_Tx_bdR_terR, Cnt_Tx_bdR_terR_Rx);//判断与地形反射的射线是否打到接收机

	TriangleRayTube *Tx_bdR_bdR = 0;
	int Cnt_Tx_bdR_bdR = 0;
	Edge *edge_Tx_bdR_bdD = 0;
	Face *face1_Tx_bdR_bdD = 0;
	Face *face2_Tx_bdR_bdD = 0;
	TriangleRayTube *incident_Tx_bdR_bdD = 0;
	int Cnt_Tx_bdR_bdD = 0;
	BuildingReflectionAndJudgeDiffraction(Tx_bdR, Tx_bdR_bdR, Cnt_Tx_bdR, Cnt_Tx_bdR_bdR, Cnt_Tx_bdR_bdD, buildingFaceCnt, buildingFaces,
		edge_Tx_bdR_bdD, face1_Tx_bdR_bdD, face2_Tx_bdR_bdD, incident_Tx_bdR_bdD); //计算与建筑物反射并判断哪些入射射线管发生绕射
	TriangleRayTube *Tx_bdR_bdR_Rx = 0;
	int Cnt_Tx_bdR_bdR_Rx = 0;
	ReceiveTriangleRayTubes(Tx_bdR_bdR, Tx_bdR_bdR_Rx, receiver, Cnt_Tx_bdR_bdR, Cnt_Tx_bdR_bdR_Rx);//判断与建筑物反射的射线是否打到接收机

	QuadrangleRayTube *Tx_bdR_bdD = 0;//申请绕射射线管的动态数组
	BuildingDiffraction(incident_Tx_bdR_bdD, edge_Tx_bdR_bdD, face1_Tx_bdR_bdD, face2_Tx_bdR_bdD, Tx_bdR_bdD, diffractionRayCount, Cnt_Tx_bdR_bdD);//计算绕射射线管
	QuadrangleRayTube *Tx_bdR_bdD_Rx = 0;
	int Cnt_Tx_bdR_bdD_Rx = 0;
	ReceiveQuadrangleRayTubes(Tx_bdR_bdD, Tx_bdR_bdD_Rx, receiver, Cnt_Tx_bdR_bdD, Cnt_Tx_bdR_bdD_Rx);//判断建筑物发生绕射的射线是否打到接收机

	
	//建筑物绕射射线作为入射射线
	QuadrangleRayTube *Tx_bdD_terR = 0;
	int Cnt_Tx_bdD_terR = 0;
	TerReflection_Qua(Tx_bdD, Tx_bdD_terR, terFaces, Cnt_Tx_bdD, terFaceCnt, Cnt_Tx_bdD_terR);//四边形射线管地面反射
	QuadrangleRayTube *Tx_bdD_terR_Rx = 0;
	int Cnt_Tx_bdD_terR_Rx = 0;
	ReceiveQuadrangleRayTubes(Tx_bdD_terR, Tx_bdD_terR_Rx, receiver, Cnt_Tx_bdD_terR, Cnt_Tx_bdD_terR_Rx);//判断与地形反射的射线是否打到接收机

	QuadrangleRayTube *Tx_bdD_bdR = 0;
	int Cnt_Tx_bdD_bdR = 0;
	BuildingReflection_Qua(Tx_bdD, buildingFaceCnt, Cnt_Tx_bdD, Cnt_Tx_bdD_bdR, buildingFaces, Tx_bdD_bdR);//四边形射线管建筑物反射
	QuadrangleRayTube *Tx_bdD_bdR_Rx = 0;
	int Cnt_Tx_bdD_bdR_Rx = 0;
	ReceiveQuadrangleRayTubes(Tx_bdD_bdR, Tx_bdD_bdR_Rx, receiver, Cnt_Tx_bdD_bdR, Cnt_Tx_bdD_bdR_Rx);
}
