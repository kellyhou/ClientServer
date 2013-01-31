#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "mul.h"

int main( int argc, char ** argv )
{

	int sock_fd = socket( AF_INET, SOCK_DGRAM, 0 );
	if( sock_fd == -1 ) {
		perror("socket");
		exit( 1 );
	}

	struct sockaddr_in broad_addr;
	broad_addr.sin_family = AF_INET;
	broad_addr.sin_port = htons( PORT_NUM );
	broad_addr.sin_addr.s_addr = inet_addr( MUL_ADDR );

	char buf[1400] = "hello";
	int count = 0;
	while( 1 ) {
		int ret = sendto( sock_fd, buf, sizeof(buf), 0, (SA*)&broad_addr, ADDR_LEN );
		printf("ret = %d\n", ret );
		printf("have sent %d packet.\n", ++count );
		sleep( 1 );
	}

	return 0;
}










