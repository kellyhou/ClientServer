#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "chain/chain_queue.h"

// cond.c

chain_queue_t head;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void * execute( void * data )
{
	while( 1 ) {
		pthread_mutex_lock( &mutex );
		if( ! empty_chain_queue( &head ) ) {
			char buf[100];
			get_queue( &head, buf );
			printf("buf = %s\n", buf );
		}
		pthread_mutex_unlock( &mutex );
	}		
}

int main(int argc, char **argv)
{
	init_chain_queue( &head, 100 );

	pthread_t tid;
	pthread_create( &tid, NULL, execute, NULL );

	char buf[100];
	while( 1 ) {
		gets( buf );
		pthread_mutex_lock( &mutex );
		put_queue( &head, buf );
		pthread_mutex_unlock( &mutex );
	}		

}


