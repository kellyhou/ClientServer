#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// setvbuf.c
int main(int argc, char **argv)
{
//	setvbuf( stdout, NULL, _IONBF, 0 ); 

	int i = 5;
	while( 1 ) {
		putchar( 65 );
		sleep( 1 );
		if( i-- == 0 ){
			fflush( NULL );
			i = 5;
		}
	}

}
