#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "broad.h"

int main( int argc, char ** argv )
{

	int sock_fd = socket( AF_INET, SOCK_DGRAM, 0 );
	if( sock_fd == -1 ) {
		perror("socket");
		exit( 1 );
	}

	int broad_flag = 1;
	int ret = setsockopt( sock_fd, SOL_SOCKET, SO_BROADCAST, 
		&broad_flag, sizeof(int) );
	
	if( ret == -1 ) {
		perror("setsockopt");
		exit( 2 );
	}

	struct sockaddr_in broad_addr;
	broad_addr.sin_family = AF_INET;
	broad_addr.sin_port = htons( PORT_NUM );
	broad_addr.sin_addr.s_addr = inet_addr( "10.255.255.255" );

	char buf[1400] = "hello";
	int count = 0;
	while( 1 ) {
		sendto( sock_fd, buf, sizeof(buf), 0, (SA*)&broad_addr, ADDR_LEN );
		if( !( count % 1000 ) ) printf("have sent %d packet.\n", count );
		count ++;
	}

	return 0;
}










