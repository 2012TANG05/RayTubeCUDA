#pragma once
#include <stdio.h>
#include <iostream>
#include "FinalExecutionOnGPU.h"
#include "FinalExecutionOnCPU.h"
using namespace std;

int main()
{
	ExecuteOnGPU();
	ExecuteOnCPU();
}

