#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void * ping( void * arg )
{
	int off = ( int )arg;

	char buf[100];
	sprintf( buf, "ping 10.1.1.%d -c 1 >& /dev/null", off );
	int ret = system( buf );

	if( ret == 0 ) {
		printf("ping 10.1.1.%d is success.\n", off );
	}

	return ( void * )( ret > 0 );
}

int main(int argc, char **argv)
{
	pthread_t thread[255];

	int i;
	for( i=1; i<255; i++ ) {
		pthread_create( thread+i, NULL, ping, ( void * )i ) ;
	}

	void * retval;
	int total = 0;

	for( i=1; i<255; i++ ) {
		pthread_join( thread[i], &retval );
		if( ( int )retval == 0 )
			total ++;
	}

	printf("总共能连通 %d 台机器.\n", total );
	return 0;
}



