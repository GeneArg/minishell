
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main()
{
	char *test = "HELLO=WORLD";
	int a = strlen(test) - (strchr(test, '=') - test);

	printf("%d\n", a);
	return 0;
}