#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "chain_queue.h"

struct disk {
	char n;
	char a;
	char b;
	char c;
};

void set_disk( struct disk * temp, char n, char a, char b, char c )
{
	temp->n = n;
	temp->a = a;
	temp->b = b;
	temp->c = c;
}

int hanoi(int n, char a, char b, char c)
{
	struct disk temp;
	set_disk( &temp, n, a, b, c );

	chain_queue_t cq;
	init_chain_queue( &cq, sizeof(struct disk) );
	put_queue( &cq, &temp );

	int i;
	int steps = 0;
	for( i=1; i<=n-1; i++ ) {
		steps += 1 << i;
	}
	steps -= ( n - 1 );

	printf("steps = %d\n", steps );

	struct disk get;
	for( i=0; i<steps; i++ ) {
		get_queue( &cq, &get );	
		if( get.n == 1 ) {
			put_queue( &cq, &get );
		} else {
			set_disk( &temp, get.n-1, get.a, get.c, get.b );
			put_queue( &cq, &temp );
			set_disk( &temp, 1, get.a, get.b, get.c );
			put_queue( &cq, &temp );
			set_disk( &temp, get.n-1, get.b, get.a, get.c );
			put_queue( &cq, &temp );
		}
	}

	while( !empty_chain_queue( &cq ) ) {
		get_queue( &cq, &get );
		printf("%c ===>> %c\n", get.a, get.c );
	}
	return 0;
}

int main(int argc, char **argv)
{
	printf("sizeof = %d\n", sizeof(struct disk) );
	int disk_num = atoi( argv[1] );
	hanoi( disk_num, 'A', 'B', 'C');
}










