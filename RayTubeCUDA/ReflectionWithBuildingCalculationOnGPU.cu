#include "ReflectionWithBuildingCalculationOnGPU.cuh"

__device__ void RWBC_UitizeVectorOnGPU(float *a,float *b,float *c)//单位化向量
{
	float length=sqrt((*a)*(*a)+(*b)*(*b)+(*c)*(*c));
	*a=*a/length;
	*b=*b/length;
	*c=*c/length; 
}

__device__ Point RWBC_GetReflectedVectorOnGPU(Point d,Point n)//计算反射射线方向向量
{
	Point reflectedPoint;
	float temp=2*(d.x*n.x+d.y*n.y+d.z*n.z);
	reflectedPoint.x=d.x-temp*n.x;
	reflectedPoint.y=d.y-temp*n.y;
	reflectedPoint.z=d.z-temp*n.z;
	return reflectedPoint;
}
__device__ Ray RWBC_CalculateReflectedRayOnGPU(Ray incidentRay,Face *face,int faceCount,int *reflectedFace,int *flag)
{
	Ray reflectedRay;
	float t=50000;
	for (int i=0;i<faceCount;i++)
	{
		float a1=-incidentRay.direction.x,a2=-incidentRay.direction.y,a3=-incidentRay.direction.z;
		float b1=face[i]. B.x-face[i].A.x,b2=face[i].B.y-face[i].A.y,b3=face[i].B.z-face[i].A.z;
		float c1=face[i]. C.x-face[i].A.x,c2=face[i].C.y-face[i].A.y,c3=face[i].C.z-face[i].A.z;
		float x1=incidentRay.originalPoint.x-face[i].A.x,x2=incidentRay.originalPoint.y-face[i].A.y,x3=incidentRay.originalPoint.z-face[i].A.z;
		float denominator=a1*(b2*c3-b3*c2)-b1*(a2*c3-a3*c2)+c1*(a2*b3-a3*b2);
		float t_numerator=x1*(b2*c3-b3*c2)-b1*(x2*c3-x3*c2)+c1*(x2*b3-x3*b2);
		float u_numerator=a1*(x2*c3-x3*c2)-x1*(a2*c3-a3*c2)+c1*(a2*c3-a3*c2);
		float v_numerator=a1*(b2*x3-b3*x2)-b1*(a2*x3-a3*x2)+x1*(a2*b3-a3*b2);
		if (abs(denominator)>0.000001)
		{
			float u=u_numerator/denominator;
			float v=v_numerator/denominator;
			if(t_numerator/denominator<t&&t_numerator/denominator>1)
			{
				t=t_numerator/denominator;
				reflectedRay.originalPoint.x=incidentRay.originalPoint.x+incidentRay.direction.x*t;
				reflectedRay.originalPoint.y=incidentRay.originalPoint.y+incidentRay.direction.y*t;
				reflectedRay.originalPoint.z=incidentRay.originalPoint.z+incidentRay.direction.z*t;
				Point n;
				n.x=b2*c3-b3*c2;n.y=b3*c1-b1*c3;n.z=b1*c2-c1*b2;
				RWBC_UitizeVectorOnGPU(&n.x,&n.y,&n.z);
				reflectedRay.direction=RWBC_GetReflectedVectorOnGPU(incidentRay.direction,n);
				if ((u>0)&&(u<1)&&((u+v)>0)&&((u+v)<1))
				{
					*flag=1;
					*reflectedFace=i;
				}
			}
		}
	}
	return reflectedRay;
}

__device__ bool RWBC_JudgeTwoTriangleFacesOnTheSameQuadrangle(int faceNumber1,int faceNumber2)
{
	int a=faceNumber1%12;
	int b=faceNumber2%12;
	if (faceNumber1==faceNumber2)
	{
		return true;
	} 
	if (a%2==0)
	{
		if (b==a+1)
		{
			return  true;
		}
	}
	if (a%2==1)
	{
		if (b==a-1)
		{
			return true;
		}
	}
	return false;
}

__device__ bool RWBC_JudgeTwoTriangleFacesOnTheAdjacentQuadrangle(int faceNumber1,int faceNumber2)
{
	int a=faceNumber1%12;
	int b=faceNumber2%12;
	if (a%2==0&&a!=0)
	{
		if (b==a-1)
		{
			return  true;
		}
	}
	if (a%2==1&&a!=7)
	{
		if (b==a+1)
		{
			return true;
		}
	}
	if ((a==0&&b==7)||(a==7&&b==0))
	{
		return true;
	}
	return false;
}


__device__ void RWBC_GetThreeRemainingNumbersOnGPU(int number,int *remainingNumber1,int *remainingNumber2,int *remainingNumber3)
{
	if (number==0)
	{
		*remainingNumber1=1;
		*remainingNumber2=2;
		*remainingNumber3=3;
	}
	if (number==1)
	{
		*remainingNumber1=0;
		*remainingNumber2=2;
		*remainingNumber3=3;
	}
	if (number==2)
	{
		*remainingNumber1=0;
		*remainingNumber2=1;
		*remainingNumber3=3;
	}
	if (number==3)
	{
		*remainingNumber1=0;
		*remainingNumber2=1;
		*remainingNumber3=2;
	}
}
__device__ void RWBC_GetOneRemainingNumberOnGPU(int number1,int number2,int number3,int *remainingNumber)
{
	*remainingNumber=6-number1-number2-number3;
}


__device__ void GetReflectedRayTubesWhenTwoFlagsEqualOneOnGPU(int flagNumber1,int flagNumber2,int *reflectedFace,Ray *reflectedRays,Ray *incidentRays,QuadrangleRayTube *reflectedRayTubes,Face *face,int *reflectionExist,int i)
{
	if (RWBC_JudgeTwoTriangleFacesOnTheSameQuadrangle(reflectedFace[flagNumber1],reflectedFace[flagNumber2]))
	{
		reflectionExist[i]=1;
		reflectedRayTubes[2*i].ray1=reflectedRays[0];
		reflectedRayTubes[2*i].ray2=reflectedRays[1];
		reflectedRayTubes[2*i].ray3=reflectedRays[2];
		reflectedRayTubes[2*i].ray4=reflectedRays[3];
		reflectedRayTubes[2*i+1].ray1=reflectedRays[0];
		reflectedRayTubes[2*i+1].ray2=reflectedRays[1];
		reflectedRayTubes[2*i+1].ray3=reflectedRays[2];
		reflectedRayTubes[2*i+1].ray4=reflectedRays[3];

	}
	if (RWBC_JudgeTwoTriangleFacesOnTheAdjacentQuadrangle(reflectedFace[flagNumber1],reflectedFace[flagNumber2]))
	{
		reflectionExist[i]=1;
		int remainingNumber1,remainingNumber2,remainingNumber3;
		RWBC_GetThreeRemainingNumbersOnGPU(flagNumber1,&remainingNumber1,&remainingNumber2,&remainingNumber3);
		reflectedRayTubes[2*i].ray1=reflectedRays[flagNumber1];
		reflectedRayTubes[2*i].ray2=reflectedRays[remainingNumber1];
		reflectedRayTubes[2*i].ray3=reflectedRays[remainingNumber2];
		reflectedRayTubes[2*i].ray4=reflectedRays[remainingNumber2];
		RWBC_GetThreeRemainingNumbersOnGPU(flagNumber2,&remainingNumber1,&remainingNumber2,&remainingNumber3);
		reflectedRayTubes[2*i+1].ray1=reflectedRays[flagNumber2];
		reflectedRayTubes[2*i+1].ray2=reflectedRays[remainingNumber1];
		reflectedRayTubes[2*i+1].ray3=reflectedRays[remainingNumber2];
		reflectedRayTubes[2*i+1].ray4=reflectedRays[remainingNumber3];

	}	
}

__device__ void GetReflectedRayTubesWhenThreeFlagsEqualOneOnGPU(int flagNumber1,int flagNumber2,int flagNumber3,int *reflectedFace,Ray *reflectedRays,Ray *incidentRays,QuadrangleRayTube *reflectedRayTubes,QuadrangleRayTube *incidentRayTubes,Face *face,int *reflectionExist,int i)
{
	//int remainingNumber;
	//RWBC_GetOneRemainingNumberOnGPU(flagNumber1,flagNumber2,flagNumber3,&remainingNumber);
	if (RWBC_JudgeTwoTriangleFacesOnTheSameQuadrangle(reflectedFace[flagNumber1],reflectedFace[flagNumber2]))
	{
		RWBC_AddPathNodeToReflectedRayTubeOnGPU(incidentRayTubes,reflectedRayTubes,i,face[reflectedFace[flagNumber1]],face[reflectedFace[flagNumber3]]);
		GetReflectedRayTubesWhenTwoFlagsEqualOneOnGPU(flagNumber1,flagNumber3,reflectedFace,reflectedRays,incidentRays,reflectedRayTubes,face,reflectionExist,i);
	}
	if (RWBC_JudgeTwoTriangleFacesOnTheSameQuadrangle(reflectedFace[flagNumber1],reflectedFace[flagNumber3]))
	{
		RWBC_AddPathNodeToReflectedRayTubeOnGPU(incidentRayTubes,reflectedRayTubes,i,face[reflectedFace[flagNumber2]],face[reflectedFace[flagNumber3]]);
		GetReflectedRayTubesWhenTwoFlagsEqualOneOnGPU(flagNumber2,flagNumber3,reflectedFace,reflectedRays,incidentRays,reflectedRayTubes,face,reflectionExist,i);
	}
	if (RWBC_JudgeTwoTriangleFacesOnTheSameQuadrangle(reflectedFace[flagNumber2],reflectedFace[flagNumber3]))
	{
		RWBC_AddPathNodeToReflectedRayTubeOnGPU(incidentRayTubes,reflectedRayTubes,i,face[reflectedFace[flagNumber1]],face[reflectedFace[flagNumber2]]);
		GetReflectedRayTubesWhenTwoFlagsEqualOneOnGPU(flagNumber1,flagNumber2,reflectedFace,reflectedRays,incidentRays,reflectedRayTubes,face,reflectionExist,i);
	}
}

__device__ void RWBC_AddPathNodeToReflectedRayTubeOnGPU(QuadrangleRayTube *incidentRayTubes,QuadrangleRayTube *reflectedRayTubes,int i,Face face1,Face face2)
{
	if(incidentRayTubes[i].path.nodeLevel=0)
	{
		reflectedRayTubes[2*i].path.nodeLevel=1;
		reflectedRayTubes[2*i].path.node1.point1=face1.A;
		reflectedRayTubes[2*i].path.node1.point2=face1.B;
		reflectedRayTubes[2*i].path.node1.point3=face1.C;
		reflectedRayTubes[2*i+1].path.nodeLevel=1;
		reflectedRayTubes[2*i+1].path.node1.point1=face2.A;
		reflectedRayTubes[2*i+1].path.node1.point2=face2.B;
		reflectedRayTubes[2*i+1].path.node1.point3=face2.C;
	}
	if(incidentRayTubes[i].path.nodeLevel=1)
	{
		reflectedRayTubes[2*i].path.node1=incidentRayTubes[i].path.node1;
		reflectedRayTubes[2*i].path.nodeLevel=2;
		reflectedRayTubes[2*i].path.node2.point1=face1.A;
		reflectedRayTubes[2*i].path.node2.point2=face1.B;
		reflectedRayTubes[2*i].path.node2.point3=face1.C;
		reflectedRayTubes[2*i+1].path.nodeLevel=2;
		reflectedRayTubes[2*i+1].path.node1=incidentRayTubes[i].path.node1;
		reflectedRayTubes[2*i+1].path.node2.point1=face2.A;
		reflectedRayTubes[2*i+1].path.node2.point2=face2.B;
		reflectedRayTubes[2*i+1].path.node2.point3=face2.C;
	}
	if(incidentRayTubes[i].path.nodeLevel=2)
	{
		reflectedRayTubes[2*i].path.node1=incidentRayTubes[i].path.node1;
		reflectedRayTubes[2*i].path.node2=incidentRayTubes[i].path.node2;
		reflectedRayTubes[2*i].path.nodeLevel=3;
		reflectedRayTubes[2*i].path.node3.point1=face1.A;
		reflectedRayTubes[2*i].path.node3.point2=face1.B;
		reflectedRayTubes[2*i].path.node3.point3=face1.C;
		reflectedRayTubes[2*i+1].path.nodeLevel=3;
		reflectedRayTubes[2*i+1].path.node1=incidentRayTubes[i].path.node1;
		reflectedRayTubes[2*i+1].path.node2=incidentRayTubes[i].path.node2;
		reflectedRayTubes[2*i+1].path.node3.point1=face2.A;
		reflectedRayTubes[2*i+1].path.node3.point2=face2.B;
		reflectedRayTubes[2*i+1].path.node3.point3=face2.C;
	}
}

__global__ void CalculateReflectionWithBuildingFaceOnGPU(QuadrangleRayTube *incidentRayTubes,int faceCount,int rayTubeCount,QuadrangleRayTube *reflectedRayTubes,Face *face,int *reflectionExist)
{
	int i=blockIdx.x * blockDim.x+threadIdx.x;
	int reflectedFace[4];
	int flag[4]={0,0,0,0};
	Ray reflectedRays[4],incidentRays[4];
	incidentRays[0]=incidentRayTubes[i].ray1;
	incidentRays[1]=incidentRayTubes[i].ray2;
	incidentRays[2]=incidentRayTubes[i].ray3;
	incidentRays[3]=incidentRayTubes[i].ray4;
	reflectedRays[0]=RWBC_CalculateReflectedRayOnGPU(incidentRays[0],face,faceCount,&reflectedFace[0],&flag[0]);
	reflectedRays[1]=RWBC_CalculateReflectedRayOnGPU(incidentRays[1],face,faceCount,&reflectedFace[1],&flag[1]);
	reflectedRays[2]=RWBC_CalculateReflectedRayOnGPU(incidentRays[2],face,faceCount,&reflectedFace[2],&flag[2]);
	reflectedRays[3]=RWBC_CalculateReflectedRayOnGPU(incidentRays[3],face,faceCount,&reflectedFace[3],&flag[3]);
	if (flag[0]+flag[1]+flag[2]+flag[3]==0)
	{
		reflectionExist[i]=0;
	}
	if (flag[0]+flag[1]+flag[2]+flag[3]==1)
	{
		reflectionExist[i]=1;
		//reflectedRayTubes[2*i].path.nodeLevel=1;
		reflectedRayTubes[2*i].ray1=reflectedRays[0];
		reflectedRayTubes[2*i].ray2=reflectedRays[1];
		reflectedRayTubes[2*i].ray3=reflectedRays[2];
		reflectedRayTubes[2*i].ray4=reflectedRays[3];
		//reflectedRayTubes[2*i+1].path.nodeLevel=1;
		reflectedRayTubes[2*i+1].ray1=reflectedRays[0];
		reflectedRayTubes[2*i+1].ray2=reflectedRays[1];
		reflectedRayTubes[2*i+1].ray3=reflectedRays[2];
		reflectedRayTubes[2*i+1].ray4=reflectedRays[3];
		for (int j=0;j<4;j++)
		{
			if (flag[j]==1)
			{
				RWBC_AddPathNodeToReflectedRayTubeOnGPU(incidentRayTubes,reflectedRayTubes,i,face[reflectedFace[j]],face[reflectedFace[j]]);
			}
		}
	}
	if (flag[0]+flag[1]+flag[2]+flag[3]==2)
	{
		int m,n;
		for (int j=0;j<4;j++)
		{
			if (flag[j]==1)
			{
				m=j;
				for (int t=j+1;t<4;t++)
				{
					if(flag[t]==1)
					{
						n=t;
					}
				}
			}
		}
		GetReflectedRayTubesWhenTwoFlagsEqualOneOnGPU(m,n,reflectedFace,reflectedRays,incidentRays,reflectedRayTubes,face,reflectionExist,i);
		RWBC_AddPathNodeToReflectedRayTubeOnGPU(incidentRayTubes,reflectedRayTubes,i,face[reflectedFace[m]],face[reflectedFace[n]]);
	}
	if (flag[0]+flag[1]+flag[2]+flag[3]==3)
	{
		int j=0;
		int remainingNumber1,remainingNumber2,remainingNumber3;
		for (;j<4;j++)
		{
			if (flag[j]==0)
			{
				break;
			}
		}
		RWBC_GetThreeRemainingNumbersOnGPU(j,&remainingNumber1,&remainingNumber2,&remainingNumber3);
		GetReflectedRayTubesWhenThreeFlagsEqualOneOnGPU(remainingNumber1,remainingNumber2,remainingNumber3,reflectedFace,reflectedRays,incidentRays,reflectedRayTubes,incidentRayTubes,face,reflectionExist,i);
		
	}
	if (flag[0]+flag[1]+flag[2]+flag[3]==4)
	{
		if (RWBC_JudgeTwoTriangleFacesOnTheSameQuadrangle(reflectedFace[0],reflectedFace[1]))
		{
			if (RWBC_JudgeTwoTriangleFacesOnTheAdjacentQuadrangle(reflectedFace[0],reflectedFace[2]))
			{
				RWBC_AddPathNodeToReflectedRayTubeOnGPU(incidentRayTubes,reflectedRayTubes,i,face[reflectedFace[0]],face[reflectedFace[2]]);
				GetReflectedRayTubesWhenTwoFlagsEqualOneOnGPU(0,2,reflectedFace,reflectedRays,incidentRays,reflectedRayTubes,face,reflectionExist,i);
			}
			else
			{
				RWBC_AddPathNodeToReflectedRayTubeOnGPU(incidentRayTubes,reflectedRayTubes,i,face[reflectedFace[1]],face[reflectedFace[3]]);
				GetReflectedRayTubesWhenTwoFlagsEqualOneOnGPU(1,3,reflectedFace,reflectedRays,incidentRays,reflectedRayTubes,face,reflectionExist,i);
			}
		}
		if (RWBC_JudgeTwoTriangleFacesOnTheSameQuadrangle(reflectedFace[0],reflectedFace[3]))
		{
			if (RWBC_JudgeTwoTriangleFacesOnTheAdjacentQuadrangle(reflectedFace[0],reflectedFace[1]))
			{
				RWBC_AddPathNodeToReflectedRayTubeOnGPU(incidentRayTubes,reflectedRayTubes,i,face[reflectedFace[0]],face[reflectedFace[1]]);
				GetReflectedRayTubesWhenTwoFlagsEqualOneOnGPU(0,1,reflectedFace,reflectedRays,incidentRays,reflectedRayTubes,face,reflectionExist,i);
			}
			else
			{
				RWBC_AddPathNodeToReflectedRayTubeOnGPU(incidentRayTubes,reflectedRayTubes,i,face[reflectedFace[2]],face[reflectedFace[3]]);
				GetReflectedRayTubesWhenTwoFlagsEqualOneOnGPU(2,3,reflectedFace,reflectedRays,incidentRays,reflectedRayTubes,face,reflectionExist,i);
			}
		}
	}
}


cudaError_t  GetReflectionWithBuildingFaceOnGPU(QuadrangleRayTube *incidentRayTubes,int faceCount,int rayTubeCount,Face *buildingFace,QuadrangleRayTube *reflectedRayTubes,int *reflectionExist)
{
	const int pointPerFace=9;
	cudaError_t cudaStatus;

	QuadrangleRayTube *device_incidentRayTubes=0;
	QuadrangleRayTube *device_reflectedRayTubes=0;

	Face *device_face=0;
	//float *device_distance=0;
	//int *device_faceNumber1=0;
	//int *device_faceNumber2=0;

	int *device_reflectionExist=0;
	//float *device_a=0;
	cudaStatus=cudaSetDevice(0);
	if (cudaStatus!=cudaSuccess)
	{
		fprintf(stderr,"CUDA capable GPU is not available!");
		goto Error;
	}

	cudaStatus=cudaMalloc((void**)&device_incidentRayTubes,rayTubeCount*sizeof(QuadrangleRayTube));
	if (cudaStatus!=cudaSuccess)
	{
		fprintf(stderr,"device_incidentRayTubes cudaMalloc error!");
		goto Error;
	}
	cudaStatus=cudaMalloc((void**)&device_reflectedRayTubes,rayTubeCount*2*sizeof(QuadrangleRayTube));
	if (cudaStatus!=cudaSuccess)
	{
		fprintf(stderr,"device_reflectedRayTubes cudaMalloc error!");

		goto Error;
	}

	cudaStatus=cudaMalloc((void**)&device_face,faceCount*sizeof(Face));
	if (cudaStatus!=cudaSuccess)
	{
		fprintf(stderr,"device_face cudaMalloc error!");
		goto Error;
	}
	/*cudaStatus=cudaMalloc((void**)&device_faceNumber1,rayTubeCount*sizeof(int));
	if (cudaStatus!=cudaSuccess)
	{
		fprintf(stderr,"device_faceNumber1 cudaMalloc error!");
		goto Error;
	}
	cudaStatus=cudaMalloc((void**)&device_faceNumber2,rayTubeCount*sizeof(int));
	if (cudaStatus!=cudaSuccess)
	{
		fprintf(stderr,"device_faceNumber2 cudaMalloc error!");
		goto Error;
	}*/

	cudaStatus=cudaMalloc((void**)&device_reflectionExist,rayTubeCount*sizeof(int));
	if (cudaStatus!=cudaSuccess)
	{
		fprintf(stderr,"device_reflectionExis cudaMalloc error!");
		goto Error;
	}
	//cudaStatus=cudaMalloc((void**)&device_a,rayTubeCount*sizeof(float));
	//if (cudaStatus!=cudaSuccess)
	//{
	//	fprintf(stderr,"device_reflectionExis cudaMalloc error!");
	//	goto Error;
	//}

	cudaStatus=cudaMemcpy(device_incidentRayTubes,incidentRayTubes,rayTubeCount*sizeof(QuadrangleRayTube),cudaMemcpyHostToDevice);
	if (cudaStatus!=cudaSuccess)
	{
		fprintf(stderr,"incidentRayTubes Memcpy failed!");
	}
	cudaStatus=cudaMemcpy(device_face,buildingFace,faceCount*sizeof(Face),cudaMemcpyHostToDevice);
	if (cudaStatus!=cudaSuccess)
	{
		fprintf(stderr,"face Memcpy failed!");
	}

	const int num_blocks=32;
	const int num_threads=640;

	//CalculateReflectionAndDiffractionWithBuildingFace<<<num_blocks,num_threads>>>(device_incidentRayTubes,faceCount,rayTubeCount,device_reflectedRayTubes,device_face,device_edge,device_faceNumber1,device_faceNumber2,device_reflectionExist,device_diffractionExist);

	cudaStatus=cudaMemcpy(reflectedRayTubes,device_reflectedRayTubes,rayTubeCount*2*sizeof(QuadrangleRayTube),cudaMemcpyDeviceToHost);
	if (cudaStatus!=cudaSuccess)
	{
		fprintf(stderr,"reflectedRayTubes cudaMemcpy failed!");
	}
	//cudaStatus=cudaMemcpy(faceNumber1,device_faceNumber1,rayTubeCount*sizeof(int),cudaMemcpyDeviceToHost);
	//if (cudaStatus!=cudaSuccess)
	//{
	//	fprintf(stderr,"faceNumber1 cudaMemcpy failed!");
	//}
	//cudaStatus=cudaMemcpy(faceNumber2,device_faceNumber2,rayTubeCount*sizeof(int),cudaMemcpyDeviceToHost);
	//if (cudaStatus!=cudaSuccess)
	//{
	//	fprintf(stderr,"faceNumber2 cudaMemcpy failed!");
	//}

	cudaStatus=cudaMemcpy(reflectionExist,device_reflectionExist,rayTubeCount*sizeof(int),cudaMemcpyDeviceToHost);
	if (cudaStatus!=cudaSuccess)
	{
		fprintf(stderr,"reflectionExist cudaMemcpy failed!");
	}

	return cudaStatus;
Error:
	cudaFree(device_incidentRayTubes);
	cudaFree(device_reflectedRayTubes);
	cudaFree(device_face);
	//cudaFree(device_faceNumber1);
	//cudaFree(device_faceNumber2);
	cudaFree(device_reflectionExist);
}