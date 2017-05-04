#include "BasicTool.cuh"


//���������߹��Ƿ���ڽ��ջ��ϣ����������߹��������)
__device__ bool IsTriangleRayTubesReceivedOnGPU(TriangleRayTube triangRayTube, Point receiver);


//�ı������߹��Ƿ���ڽ��ջ��ϣ��ı������߹��жϽ�����ڣ�
__device__ bool IsQuadrangleRayTubesReceivedOnGPU(QuadrangleRayTube QuadrangleRayTube, Point receiver);



__device__ bool ChooseReceiveMethodByIntersectionOnGPU(Ray ray1, Ray ray2, Ray ray3, Point receiver);


//�ж����߹��ڵ������Ƿ��н��㲢���ؽ������
__device__ int JudgeIntersectionOfThreeRayOnGPU(Ray *ray1, Ray *ray2, Ray *ray3);


//�õ��µ����߹�
__device__ void GetNewRayTubeOnGPU(Ray *temRay1, Ray *temRay2, Ray *temRay3, Ray *temRay4, int flag);


//����û�й�ͬ�������߹��Ƿ����
__device__ bool IsRayTubesWithoutSameStartPointReceivedOnGPU(Ray temRay1, Ray temRay2, Ray temRay3, Point receiver);


//�ж�ֱ�������Ƿ��н��㲢����
__device__ bool JudgeIntersectionOfStraightAndFaceOnGPU(float A, float B, float C, float D, Ray ray, Point *intersection);



//�ж����߹��Ƿ���ڽ��ջ��ϣ������жϣ�
__device__ bool IsRayTubesWithSameStartPointReceivedOnGPU(Ray ray1, Ray ray2, Ray ray3, Point receiver, int flag);



__global__ void CalculateIsTriangleRayTubesReceivedOnGPU(TriangleRayTube *rayTubes, 
	Point receiver, bool *identifier, int rayTubeCount);
__global__ void CalculateIsQuadrangleRayTubesReceivedOnGPU(QuadrangleRayTube *rayTubes, 
	Point receiver, bool *identifier, int rayTubeCount);


//��CPU�е����ݴ���GPU��
cudaError_t GetTriangleRayTubesReceived(TriangleRayTube *rayTubes,bool *identifier, Point receiver, int rayTubeCount);

cudaError_t GetQuadrangleRayTubesReceived(QuadrangleRayTube *rayTubes, bool *identifier, Point receiver, int rayTubeCount);


//�ж��������Ƿ����
__device__ bool JudgePointEqualOnGPU(Point a, Point b);

//�����������ߵ�λ��
__device__ void ExchangeTwoRayOnGPU(Ray *ray1, Ray *ray2);