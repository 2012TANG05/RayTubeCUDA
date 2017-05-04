#include "BasicTool.cuh"
#include "BasicToolOnCPU.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"



//三角形射线管是否打在接收机上
void IsTriangleRayTubesReceived(TriangleRayTube *triangRayTube, Point reciever, int rayTubeCount, bool *isexist);


//四边形射线管是否打在接收机上
void IsQuadrangleRayTubesReceived(QuadrangleRayTube *quadrangleRayTube, Point reciever, int rayTubeCount, bool *isexist);


//判断射线管是否打在接收机上
bool IsRayTubesReceived(Ray ray1, Ray ray2, Ray ray3, Point reciever, int flag);


//判断绕射射线管是否打在打在接收机上
bool IsDiffractionRayTubesReceivedOnCPU(Ray ray1, Ray ray2, Ray ray3, Point reciever);


//得到新的射线管
void GetNewRayOnCPU(Ray *temRay1, Ray *temRay2, Ray *temRay3, Ray *temRay4, Ray ray1, Ray ray2, Ray ray3);


//计算绕射射线管是否接收到
bool CalculateIsDiffractionRecievedOnCPU(Ray temRay1, Ray temRay2, Ray temRay3, Point reciever,Point originalPoint);


//判断直线与面是否有交点并返回
bool JudgeStraightAndFaceOnCPU(float A, float B, float C, float D, Ray ray, Point originalPoint, Point *intersection);