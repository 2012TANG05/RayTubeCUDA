#include "BasicTool.cuh"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"



//���������߹��Ƿ���ڽ��ջ��ϣ����������߹��������)
void IsTriangleRayTubesReceivedOnCPU(TriangleRayTube *triangRayTube, Point receiver, int rayTubeCount, bool *isexist);


//�ı������߹��Ƿ���ڽ��ջ��ϣ��ı������߹��жϽ�����ڣ�
void IsQuadrangleRayTubesReceivedOnCPU(QuadrangleRayTube *quadrangleRayTube, Point receiver, int rayTubeCount, bool *isexist);

//�ж����߹��ڵ������Ƿ��н��㲢���ؽ������
int JudgeIntersectionOfThreeRayOnCPU(Ray *ray1, Ray *ray2, Ray *ray3);


//�ж��������߹��Ƿ���ڴ��ڽ��ջ��ϣ��ģ�
bool ChooseReceiveMethodByIntersectionOnCPU(Ray ray1, Ray ray2, Ray ray3, Point receiver);


//�õ��µ����߹ܣ��ģ�
void GetNewRayTubeOnCPU(Ray *temRay1, Ray *temRay2, Ray *temRay3, Ray *temRay4, int flag);


//�����������߹��Ƿ���յ�(��)
bool IsRayTubesWithoutSameStartPointReceivedOnCPU(Ray temRay1, Ray temRay2, Ray temRay3, Point receiver);


//�ж�ֱ�������Ƿ��н��㲢����
bool JudgeIntersectionOfStraightAndFaceOnCPU(float A, float B, float C, float D, Ray ray, Point *intersection);

//�ж����߹��Ƿ���ڽ��ջ��ϣ������жϣ�
bool IsRayTubesWithSameStartPointReceivedOnCPU(Ray ray1, Ray ray2, Ray ray3, Point receiver, int flag);




