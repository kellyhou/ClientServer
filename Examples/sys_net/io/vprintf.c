#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// vprintf.c
int main(int argc, char **argv)
{
	char a[12];
	*( double * )a = 12.34;
	*( int * )( a + 8 ) = 22;

	vprintf("%f\n%d\n", a );

}
