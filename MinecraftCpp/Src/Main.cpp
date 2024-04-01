#include "core/Engine.h"
#include "stdio.h"
#include "stdlib.h"
#include <iostream>

int main()
{
	setlocale(LC_ALL, "PL");
	Engine* e = new Engine();
	e->start();

	delete e;
	return 0;
}