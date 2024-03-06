#include "core/Engine.h"
#define SzymekMode

#ifdef SzymekMode
int main()
{
	Engine* e = new Engine();
	e->start();

	delete e;
	return 0;
}
#endif