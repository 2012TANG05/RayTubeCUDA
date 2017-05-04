#include "BasicTool.cuh"




//三角形射线管是否打在接收机上
__device__ bool IRR_IsTriangleRayTubesReceived(TriangleRayTube triangRayTube, Point Reciever);


//四边形射线管是否打在接收机上
__device__ bool IRR_IsTQuadrangleRayTubesReceived(QuadrangleRayTube QuadrangleRayTube, Point Reciever);


//判断射线管是否打在接收机上
__device__ bool IRR_IsRayTubesReceived(Ray ray1, Ray ray2, Ray ray3, Point recieve, int flag);


__global__ void CalculateIsTriangleRayTubesReceivedOnGPU(TriangleRayTube *rayTubes, 
	Point receive, bool *identifier, int rayTubeCount);
__global__ void CalculateIsQuadrangleRayTubesReceivedOnGPU(QuadrangleRayTube *rayTubes, 
	Point receive, bool *identifier, int rayTubeCount);


//int CalculateReflectedRayTubes(const unsigned int rayTubeCount,
//	unsigned int faceCount, Face *p_face, TriangleRayTube *incidentRayTubes,
//	TriangleRayTube *ReflectedRayTubes,int *isReflectedRaysExist,float *distance);

//将CPU中的数据传到GPU中
cudaError_t GetTriangleRayTubesReceived(TriangleRayTube *rayTubes,bool *identifier, Point receive, int rayTubeCount);

cudaError_t GetQuadrangleRayTubesReceived(QuadrangleRayTube *rayTubes, bool *identifier, Point receive, int rayTubeCount);





//判断绕射射线管是否打在打在接收机上
__device__ bool IsDiffractionRayTubesReceivedOnGPU(Ray ray1, Ray ray2, Ray ray3, Point reciever);



//得到新的射线管
__device__ void GetNewRayOnGPU(Ray *temRay1, Ray *temRay2, Ray *temRay3, Ray *temRay4, Ray ray1, Ray ray2, Ray ray3);


//计算绕射射线管是否接收到
__device__ bool CalculateIsDiffractionRecievedOnGPU(Ray temRay1, Ray temRay2, Ray temRay3, Point reciever,Point originalPoint);


//判断直线与面是否有交点并返回
__device__ bool JudgeStraightAndFaceOnGPU(float A, float B, float C, float D, Ray ray, Point originalPoint, Point *intersection);


__device__ Point CGetIntersectionOfTwoRaysOnGPU(Ray ray1,Ray ray2);