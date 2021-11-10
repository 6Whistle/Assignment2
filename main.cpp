#include "Manager.h"
#include <iostream>

int main()
{
	Manager* k = new Manager(3);
	k->run("command.txt");
	delete k;
	_CrtDumpMemoryLeaks();
    return 0;
}

