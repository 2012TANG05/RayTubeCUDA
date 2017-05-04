#include "ObjectToArray.h"

//�����������ת�������飬������CPU��
float *  TransferFaceObjectToArray(vector<CFace> faces)
{
	int facesCnt = (int)faces.size();
	unsigned int mem_size_faces = (facesCnt * 3 * 3) * sizeof(float);//3�����㣬ÿ������3������
	float *h_faces = (float *)malloc(mem_size_faces);
    if (h_faces == NULL)
    {
        fprintf(stderr, "Failed to allocate host vector h_faces ! \n");
        exit(EXIT_FAILURE);
    }
	int i = 0;
    for (vector<CFace>::iterator itFace = faces.begin(); itFace < faces.end(); itFace ++)
    {
        h_faces[i]   = itFace->GetA().GetX();
        h_faces[i+1] = itFace->GetA().GetY();
        h_faces[i+2] = itFace->GetA().GetZ();

        h_faces[i+3] = itFace->GetB().GetX();
        h_faces[i+4] = itFace->GetB().GetY();
        h_faces[i+5] = itFace->GetB().GetZ();

        h_faces[i+6] = itFace->GetC().GetX();
        h_faces[i+7] = itFace->GetC().GetY();
        h_faces[i+8] = itFace->GetC().GetZ();

        //printf("%f %f %f %f %f %f %f %f %f \n\n", h_faces[i] , h_faces[i+1] , h_faces[i+2] , h_faces[i+3] , h_faces[i+4] , h_faces[i+5] , h_faces[i+6] , h_faces[i+7] , h_faces[i+8]);

        i += 9;
	}
	return h_faces;
	
	//float* d_faces;
	//cudaError_t error;
	//error = cudaMalloc((void **) &d_faces, mem_size_faces);
 //   if (error != cudaSuccess)
 //   {
 //       printf("cudaMalloc d_faces returned error code %d, line(%d)\n", error, __LINE__);
	//	exit(EXIT_FAILURE);
 //   }
	//error = cudaMemcpy(d_faces, h_faces, mem_size_faces, cudaMemcpyHostToDevice);
 //   if (error != cudaSuccess)
 //   {
 //       printf("cudaMemcpy (d_faces, h_faces) returned error code %d, line(%d)\n", error, __LINE__);
	//	exit(EXIT_FAILURE);
 //   }
	//return d_faces;
}

//�õ���������ռ�ڴ�ռ�	
unsigned int GetMemorySizeOfFaces(vector<CFace> faces)
{
	int faceCnt = (int)faces.size();
	return faceCnt * sizeof(Face);
}

//�����������ת����������ṹ������
Face * TransferFaceObjectToStruct(vector<CFace> faces)
{
	int faceCnt = (int)faces.size();
	unsigned int mem_size_faces = faceCnt * sizeof(Face);
	Face *h_faces = (Face*)malloc(mem_size_faces);
	if (h_faces == NULL)
    {
        fprintf(stderr, "Failed to allocate host vector h_faces ! \n");
        exit(EXIT_FAILURE);
    }
	int i = 0;
    for (vector<CFace>::iterator itFace = faces.begin(); itFace < faces.end(); itFace ++)
    {
		h_faces[i].A.x = itFace->GetA().GetX();
		h_faces[i].A.y = itFace->GetA().GetY();
		h_faces[i].A.z = itFace->GetA().GetZ();

		h_faces[i].B.x = itFace->GetB().GetX();
		h_faces[i].B.y = itFace->GetB().GetY();
		h_faces[i].B.z = itFace->GetB().GetZ();

		h_faces[i].C.x = itFace->GetC().GetX();
		h_faces[i].C.y = itFace->GetC().GetY();
		h_faces[i].C.z = itFace->GetC().GetZ();
        i++;
	}
	return h_faces;
}