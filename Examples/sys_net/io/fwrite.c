#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// fwrite.c
int main(int argc, char **argv)
{
	FILE * fp = fopen( argv[1], "wb" );
	
	int i = 100000;
	while( i--  ) {
		fwrite( "a", 1, 1, fp );
		fwrite( "\n", 1, 1, fp );
		usleep( 10000 );
	}
	fclose( fp );
}
