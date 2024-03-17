#include "core/Engine.h"

#include <json.hpp>

int main()
{
	Engine* e = new Engine();
	e->start();

	delete e;
	return 0;
}