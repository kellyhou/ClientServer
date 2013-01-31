#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

int flag = 0;

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

void * thread_start( void * arg )
{
	int i = 100000;
	while( i -- ) {
		pthread_rwlock_rdlock( &rwlock );
		printf("%s flag = %d\n", arg, flag );	
		pthread_rwlock_unlock( &rwlock );
	}
}

int main(int argc, char **argv)
{
	pthread_t thread1, thread2;

	pthread_create( &thread1, NULL, thread_start, "Thread a" ) ;
	pthread_create( &thread2, NULL, thread_start, "Thread b" ) ;

	int i;
	for( i=0; i<10000; i++ ) {
		pthread_rwlock_wrlock( &rwlock );
		flag ++;
		printf("Main thread flag = %d\n", flag );
		pthread_rwlock_unlock( &rwlock );
		usleep( 1 );
	}

	pthread_join( thread1, NULL );
	pthread_join( thread2, NULL );

	return 0;
}







