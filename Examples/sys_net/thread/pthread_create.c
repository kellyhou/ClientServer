#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// pthread_create.c

struct range {
	int start;
	int end;
};

void * thread1_start( void * arg )
{
	struct range * r1 = ( struct range * )arg;	

	int i;
	for( i=r1->start; i<r1->end; i++ ) {
		printf("thread1 i = %d\n", i );	
		usleep( 1000 );
	}

	return ( void * )100;
}

int main(int argc, char **argv)
{
	pthread_t thread1;

	struct range range1 = {
		.start = 1000,
		.end = 10000
	};

	
	pthread_create( &thread1, NULL, thread1_start, &range1 ) ;

	int i;
	for( i=0; i<100; i++ ) {
		printf("main thread i = %d\n", i );	
		usleep( 100000 );
	}
	
	void * retval;
	pthread_join( thread1, &retval );

	printf("retval = %d\n", ( int )retval );
}



