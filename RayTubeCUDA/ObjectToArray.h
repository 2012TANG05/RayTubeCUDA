#include <vector>
#include "Face.h"
#include "BasicTool.cuh"

using namespace std;

unsigned int GetMemorySizeOfFaces(vector<CFace> faces);

float * TransferFaceObjectToArray(vector<CFace> faces);

Face * TransferFaceObjectToStruct(vector<CFace> faces);