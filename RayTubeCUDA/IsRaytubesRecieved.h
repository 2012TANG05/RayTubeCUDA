#include "BasicTool.cuh"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"



//三角形射线管是否打在接收机上
void IsTriangleRayTubesReceived(TriangleRayTube triangRayTube, Point reciever, int rayTubeCount, bool *isexist);


//四边形射线管是否打在接收机上
void IsQuadrangleRayTubesReceived(QuadrangleRayTube quadrangleRayTube, Point reciever, int rayTubeCount, bool *isexist);


//判断射线管是否打在接收机上
bool IsRayTubesReceived(Ray ray1, Ray ray2, Ray ray3, Point reciever);


