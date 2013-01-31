#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

int count = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void * thread_start( void * arg )
{
	int i = 100000000;
	while( i -- ) {
		pthread_mutex_lock( &mutex );
		count ++;
		pthread_mutex_unlock( &mutex );
	}
}

int main(int argc, char **argv)
{
	pthread_t thread1, thread2;

	pthread_create( &thread1, NULL, thread_start, NULL ) ;
	pthread_create( &thread2, NULL, thread_start, NULL ) ;

	void * retval;
	pthread_join( thread1, &retval );
	pthread_join( thread2, &retval );

	printf("count = %d\n", count );
	return 0;
}



