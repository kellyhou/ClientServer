#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

// pthread_attr.c
int count = 0;

void * start( void * data )
{
	printf("data = %d\n", ( int * ) data );
}


int main(int argc, char **argv)
{
	pthread_attr_t attr;
	pthread_attr_init( &attr );

	pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_DETACHED );

	pthread_t tid;
	int pthread_num = 0;

	int i = 10000;
	while( i -- ) { 
		int ret = pthread_create( &tid, &attr, start, ( void * ) i );
		if( ret != 0 )
			break;
		printf("ret = %d pthread_num = %d\n", ret, ++ pthread_num );
	}

	sleep( 10 );
}

