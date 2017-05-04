#include <math.h>
#include "Face.h"
#include "BasicTool.cuh"



bool JudgeTwoTriangleFacesOnTheSameQuadrangleOnCPU(int faceNumber1,int faceNumber2);
bool JudgeTwoTriangleFacesOnTheAdjacentQuadrangleOnCPU(int faceNumber1,int faceNumber2);
void GetThreeRemainingNumbersOnCPU(int number,int *remainingNumber1,int *remainingNumber2,int *remainingNumber3);
void GetOneRemainingNumberOnCPU(int number1,int number2,int number3,int *remainingNumber);
void GetReflectedRayTubesWhenTwoFlagsEqualOneOnGPU(int flagNumber1,int flagNumber2,int *reflectedFace,Ray *reflectedRays,Ray *incidentRays,QuadrangleRayTube *reflectedRayTubes,Face *face,int *reflectionExist,int i);
void GetReflectedRayTubesWhenThreeFlagsEqualOneOnCPU(int flagNumber1,int flagNumber2,int flagNumber3,int *reflectedFace,Ray *reflectedRays,Ray *incidentRays,QuadrangleRayTube *reflectedRayTubes,QuadrangleRayTube *incidentRayTubes,Face *face,int *reflectionExist,int i);
void AddPathNodeToReflectedRayTubeOnCPU(QuadrangleRayTube *incidentRayTubes,QuadrangleRayTube *reflectedRayTubes,int i,Face face1,Face face2);
void CalculateReflectionWithBuildingFaceOnCPU(QuadrangleRayTube *incidentRayTubes,int faceCount,int rayTubeCount,QuadrangleRayTube *reflectedRayTubes,Face *face,int *reflectionExist);