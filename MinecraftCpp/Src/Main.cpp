#include "core/Engine.h"
#include "stdio.h"
#include "stdlib.h"
#include <iostream>
#include "json.hpp"
#include <fstream>

extern "C" {
	__declspec(dllexport) unsigned long long NvOptimusEnablement = 0x00000001;
}

int main()
{
	const char* t = "000000000000000000000";
	setlocale(LC_ALL, "PL");
	Engine e;
	return 0;
}