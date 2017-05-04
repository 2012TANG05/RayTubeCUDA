#include "BasicTool.cuh"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"



//���������߹��Ƿ���ڽ��ջ���
void IsTriangleRayTubesReceived(TriangleRayTube triangRayTube, Point reciever, int rayTubeCount, bool *isexist);


//�ı������߹��Ƿ���ڽ��ջ���
void IsQuadrangleRayTubesReceived(QuadrangleRayTube quadrangleRayTube, Point reciever, int rayTubeCount, bool *isexist);


//�ж����߹��Ƿ���ڽ��ջ���
bool IsRayTubesReceived(Ray ray1, Ray ray2, Ray ray3, Point reciever);


