#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "chain_queue.h"

int main( int argc, char ** argv )
{
	chain_queue_t cq;
	init_chain_queue( &cq, sizeof(int) );

	int put = 1;
	int get;

	int op;
	int ret;

	int loop = 20000;
	while( loop -- ) {
		op = random() % 50;	
		if( op ) {
			ret = put_queue( &cq, &put );
			if( ret != -1 )
				put ++;
			else
				printf("full.\n");
		} else {
			ret = get_queue( &cq, &get );
			if( ret != -1 )
				printf("get = %d\n", get );
		}
		printf("exist_ele_num = %d\n", cq.exist_ele_num );
	}		

	while( !empty_chain_queue( &cq ) ) {
		get_queue( &cq, &get );
		printf("get = %d\n", get );
		printf("exist_ele_num = %d\n", cq.exist_ele_num );
	}
	
	return 0;
}

