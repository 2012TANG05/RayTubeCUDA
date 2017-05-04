#include "BasicTool.cuh"




//���������߹��Ƿ���ڽ��ջ���
__device__ bool IRR_IsTriangleRayTubesReceived(TriangleRayTube triangRayTube, Point Reciever);


//�ı������߹��Ƿ���ڽ��ջ���
__device__ bool IRR_IsTQuadrangleRayTubesReceived(QuadrangleRayTube QuadrangleRayTube, Point Reciever);


//�ж����߹��Ƿ���ڽ��ջ���
__device__ bool IRR_IsRayTubesReceived(Ray ray1, Ray ray2, Ray ray3, Point recieve, int flag);


__global__ void CalculateIsTriangleRayTubesReceivedOnGPU(TriangleRayTube *rayTubes, 
	Point receive, bool *identifier, int rayTubeCount);
__global__ void CalculateIsQuadrangleRayTubesReceivedOnGPU(QuadrangleRayTube *rayTubes, 
	Point receive, bool *identifier, int rayTubeCount);


//int CalculateReflectedRayTubes(const unsigned int rayTubeCount,
//	unsigned int faceCount, Face *p_face, TriangleRayTube *incidentRayTubes,
//	TriangleRayTube *ReflectedRayTubes,int *isReflectedRaysExist,float *distance);

//��CPU�е����ݴ���GPU��
cudaError_t GetTriangleRayTubesReceived(TriangleRayTube *rayTubes,bool *identifier, Point receive, int rayTubeCount);

cudaError_t GetQuadrangleRayTubesReceived(QuadrangleRayTube *rayTubes, bool *identifier, Point receive, int rayTubeCount);





//�ж��������߹��Ƿ���ڴ��ڽ��ջ���
__device__ bool IsDiffractionRayTubesReceivedOnGPU(Ray ray1, Ray ray2, Ray ray3, Point reciever);



//�õ��µ����߹�
__device__ void GetNewRayOnGPU(Ray *temRay1, Ray *temRay2, Ray *temRay3, Ray *temRay4, Ray ray1, Ray ray2, Ray ray3);


//�����������߹��Ƿ���յ�
__device__ bool CalculateIsDiffractionRecievedOnGPU(Ray temRay1, Ray temRay2, Ray temRay3, Point reciever,Point originalPoint);


//�ж�ֱ�������Ƿ��н��㲢����
__device__ bool JudgeStraightAndFaceOnGPU(float A, float B, float C, float D, Ray ray, Point originalPoint, Point *intersection);


__device__ Point CGetIntersectionOfTwoRaysOnGPU(Ray ray1,Ray ray2);