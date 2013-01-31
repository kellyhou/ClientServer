#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main( int argc, char ** argv )
{
	unsigned int val = 0x12345678;

	unsigned int after = htons( val );
	printf("after = %x\n", after );

	return 0;
}

