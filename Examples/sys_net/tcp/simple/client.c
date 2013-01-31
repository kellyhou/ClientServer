#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "tcp.h"

int main( int argc, char ** argv )
{
	if( argc != 3 ) {
		printf("Usage: %s ipaddr str\n", argv[0] );
		exit( 1 );
	}

	int sock_fd = socket( AF_INET, SOCK_STREAM, 0 );
	int ret;

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons( PORT_NUM );
	server_addr.sin_addr.s_addr = inet_addr( argv[1] );
	ret = connect( sock_fd, (SA*)&server_addr, ADDR_LEN );
	if( ret == -1 ) {
		perror("connect");
		exit( 2 );
	}
	
	send( sock_fd, argv[2], strlen(argv[2])+1, 0 );	

	close( sock_fd );
	
	return 0;
}

