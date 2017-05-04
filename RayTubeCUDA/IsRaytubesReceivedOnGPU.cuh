#include "BasicTool.cuh"


//三角形射线管是否打在接收机上（三角形射线管判收入口)
__device__ bool IsTriangleRayTubesReceivedOnGPU(TriangleRayTube triangRayTube, Point receiver);


//四边形射线管是否打在接收机上（四边形射线管判断接收入口）
__device__ bool IsQuadrangleRayTubesReceivedOnGPU(QuadrangleRayTube QuadrangleRayTube, Point receiver);



__device__ bool ChooseReceiveMethodByIntersectionOnGPU(Ray ray1, Ray ray2, Ray ray3, Point receiver);


//判断射线管内的射线是否有交点并返回交点个数
__device__ int JudgeIntersectionOfThreeRayOnGPU(Ray *ray1, Ray *ray2, Ray *ray3);


//得到新的射线管
__device__ void GetNewRayTubeOnGPU(Ray *temRay1, Ray *temRay2, Ray *temRay3, Ray *temRay4, int flag);


//计算没有共同起点的射线管是否接收
__device__ bool IsRayTubesWithoutSameStartPointReceivedOnGPU(Ray temRay1, Ray temRay2, Ray temRay3, Point receiver);


//判断直线与面是否有交点并返回
__device__ bool JudgeIntersectionOfStraightAndFaceOnGPU(float A, float B, float C, float D, Ray ray, Point *intersection);



//判断射线管是否打在接收机上（最终判断）
__device__ bool IsRayTubesWithSameStartPointReceivedOnGPU(Ray ray1, Ray ray2, Ray ray3, Point receiver, int flag);



__global__ void CalculateIsTriangleRayTubesReceivedOnGPU(TriangleRayTube *rayTubes, 
	Point receiver, bool *identifier, int rayTubeCount);
__global__ void CalculateIsQuadrangleRayTubesReceivedOnGPU(QuadrangleRayTube *rayTubes, 
	Point receiver, bool *identifier, int rayTubeCount);


//将CPU中的数据传到GPU中
cudaError_t GetTriangleRayTubesReceived(TriangleRayTube *rayTubes,bool *identifier, Point receiver, int rayTubeCount);

cudaError_t GetQuadrangleRayTubesReceived(QuadrangleRayTube *rayTubes, bool *identifier, Point receiver, int rayTubeCount);


//判断两个点是否相等
__device__ bool JudgePointEqualOnGPU(Point a, Point b);

//交换两条射线的位置
__device__ void ExchangeTwoRayOnGPU(Ray *ray1, Ray *ray2);