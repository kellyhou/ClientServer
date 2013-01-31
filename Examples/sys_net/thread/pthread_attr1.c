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
	sleep( 1000000 );
	//start( NULL );	
}


int main(int argc, char **argv)
{
	pthread_attr_t attr;
	pthread_attr_init( &attr );

	int stack_size;
	pthread_attr_getstacksize( &attr, &stack_size );
	printf("默认栈大小: %d\n", stack_size / 1024 );

	pthread_attr_setstacksize( &attr, 1024 * 100 );	

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
	//pthread_join( tid, NULL );
}

