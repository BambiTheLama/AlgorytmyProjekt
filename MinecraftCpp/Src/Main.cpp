#include "core/Engine.h"
#include "stdio.h"
#include "stdlib.h"
#include <iostream>

extern "C" {
	__declspec(dllexport) unsigned long long NvOptimusEnablement = 0x00000001;
}

int main()
{
	setlocale(LC_ALL, "PL");
	Engine* e = new Engine();
	e->start();

	delete e;
	return 0;
}