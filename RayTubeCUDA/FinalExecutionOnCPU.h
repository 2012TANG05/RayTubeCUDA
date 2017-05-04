#pragma once
#include <stdio.h>
#include <iostream>
#include "Terrain.h"
#include "BuildingReader.h"
#include "BuildingTriangulation.h"
#include "ObjectToArray.h"
#include "ReflectionAndDiffractionWithBuildingCalculationOnGPU.cuh"
#include "BasicTool.cuh"
#include "Transmitter.h"
#include <string>
#include <fstream>
#include <math.h>
#include <time.h>
#include "Transition.h"
#include "DiffractionOnCPU.h"
#include "PathNodeOnCPU.h"
#include "Receiver.h"

using namespace std;

void ExecuteOnCPU();