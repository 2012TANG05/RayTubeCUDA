#include "BasicTool.cuh"
#include "BasicToolOnCPU.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"



//���������߹��Ƿ���ڽ��ջ���
void IsTriangleRayTubesReceived(TriangleRayTube *triangRayTube, Point reciever, int rayTubeCount, bool *isexist);


//�ı������߹��Ƿ���ڽ��ջ���
void IsQuadrangleRayTubesReceived(QuadrangleRayTube *quadrangleRayTube, Point reciever, int rayTubeCount, bool *isexist);


//�ж����߹��Ƿ���ڽ��ջ���
bool IsRayTubesReceived(Ray ray1, Ray ray2, Ray ray3, Point reciever, int flag);


//�ж��������߹��Ƿ���ڴ��ڽ��ջ���
bool IsDiffractionRayTubesReceivedOnCPU(Ray ray1, Ray ray2, Ray ray3, Point reciever);


//�õ��µ����߹�
void GetNewRayOnCPU(Ray *temRay1, Ray *temRay2, Ray *temRay3, Ray *temRay4, Ray ray1, Ray ray2, Ray ray3);


//�����������߹��Ƿ���յ�
bool CalculateIsDiffractionRecievedOnCPU(Ray temRay1, Ray temRay2, Ray temRay3, Point reciever,Point originalPoint);


//�ж�ֱ�������Ƿ��н��㲢����
bool JudgeStraightAndFaceOnCPU(float A, float B, float C, float D, Ray ray, Point originalPoint, Point *intersection);