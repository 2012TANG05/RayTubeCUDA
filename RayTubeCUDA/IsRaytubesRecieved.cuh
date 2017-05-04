#include "BasicTool.cuh"
#include <math.h>



//三角形射线管是否打在接收机上
bool IsTriangleRayTubesReceived(TriangleRayTube triangRayTube, Point Reciever);


//四边形射线管是否打在接收机上
bool IsTQuadrangleRayTubesReceived(QuadrangleRayTube QuadrangleRayTube, Point Reciever);


//判断射线管是否打在接收机上
bool IsRayTubesReceived(Ray ray1, Ray ray2, Ray ray3, Point recieve);
