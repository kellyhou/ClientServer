#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// pthread_create.c

void * thread1_start( void * arg )
{
	int count = ( int )arg;	

	int i;
	for( i=0; i<count/10; i++ ) {
		printf("thread1 i = %d\n", i );	
		usleep( 100000 );
	}
}

int main(int argc, char **argv)
{
	pthread_t thread1;

	int count = 100;
	pthread_create( &thread1, NULL, thread1_start, ( int * )100 ) ;

	int i;
	for( i=0; i<100; i++ ) {
		printf("main thread i = %d\n", i );	
		usleep( 100000 );
	}
}



