#include "ProcessStatParser.h"
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* agrv[])
{
	ProcessStatParser Parser;
	Parser.AddPid(2029);

	while(1)
	{
		Parser.Reflush();
		sleep(1);
	}

	return 0;
}

