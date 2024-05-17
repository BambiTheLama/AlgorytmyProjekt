#include "core/Engine.h"
#include "stdio.h"
#include "stdlib.h"
#include <iostream>
#include "json.hpp"
#include <fstream>
#include "world/Blocks/WaveColapseFunction.h"

extern "C" {
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}

int main()
{
	setlocale(LC_ALL, "PL");
	Engine e;
	return 0;
}