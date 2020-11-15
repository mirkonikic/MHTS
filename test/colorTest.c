#include "main.h"

int main()
{
	char hostname[1024];
	gethostname(hostname, 1024);
	printf("%s\n", hostname);

	return 0;
}

