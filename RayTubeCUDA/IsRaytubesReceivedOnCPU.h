#include "BasicTool.cuh"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"



//三角形射线管是否打在接收机上（三角形射线管判收入口)
void IsTriangleRayTubesReceivedOnCPU(TriangleRayTube *triangRayTube, Point receiver, int rayTubeCount, bool *isexist);


//四边形射线管是否打在接收机上（四边形射线管判断接收入口）
void IsQuadrangleRayTubesReceivedOnCPU(QuadrangleRayTube *quadrangleRayTube, Point receiver, int rayTubeCount, bool *isexist);

//判断射线管内的射线是否有交点并返回交点个数
int JudgeIntersectionOfThreeRayOnCPU(Ray *ray1, Ray *ray2, Ray *ray3);


//判断绕射射线管是否打在打在接收机上（改）
bool ChooseReceiveMethodByIntersectionOnCPU(Ray ray1, Ray ray2, Ray ray3, Point receiver);


//得到新的射线管（改）
void GetNewRayTubeOnCPU(Ray *temRay1, Ray *temRay2, Ray *temRay3, Ray *temRay4, int flag);


//计算绕射射线管是否接收到(改)
bool IsRayTubesWithoutSameStartPointReceivedOnCPU(Ray temRay1, Ray temRay2, Ray temRay3, Point receiver);


//判断直线与面是否有交点并返回
bool JudgeIntersectionOfStraightAndFaceOnCPU(float A, float B, float C, float D, Ray ray, Point *intersection);

//判断射线管是否打在接收机上（最终判断）
bool IsRayTubesWithSameStartPointReceivedOnCPU(Ray ray1, Ray ray2, Ray ray3, Point receiver, int flag);




