#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "mul.h"

int main( int argc, char ** argv )
{
	
	int sock_fd = socket( AF_INET, SOCK_DGRAM, 0 );
	if( sock_fd == -1 ) {
		perror("socket");
		return 1;
	}

	struct sockaddr_in myaddr;
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons( PORT_NUM ); 
	myaddr.sin_addr.s_addr = htonl( INADDR_ANY );

	int ret = bind( sock_fd, ( SA * )&myaddr, ADDR_LEN );
	if( ret == -1 ) {
		perror("bind");
		exit( 2 );
	}

	struct ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = inet_addr( MUL_ADDR );
	mreq.imr_interface.s_addr = htonl( INADDR_ANY );
	ret = setsockopt( sock_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq,
			sizeof(struct ip_mreq) );
	if( ret == -1 ) {
		perror("setsockopt");
		exit( 3 );
	}
	
	char recv_buf[1400];
	int count = 0;
	while( 1 ) {
		recvfrom( sock_fd, recv_buf, sizeof(recv_buf), 0, NULL, NULL );	
		printf("count = %d recv_buf = %s\n", ++count, recv_buf );
	}

	close( sock_fd );
	return 0;
}

