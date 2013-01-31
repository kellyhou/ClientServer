#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT_NUM 6666

int main( int argc, char ** argv )
{
	if( argc != 3 ) {
		fprintf( stderr, "%s <ipaddr> <str>\n", argv[0] );
		exit( 1 );
	}	

	int sock_fd = socket( AF_INET, SOCK_DGRAM, 0 );
	if( sock_fd == -1 ) {
		perror("socket");
		return 1;
	}

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons( PORT_NUM ); 
	server_addr.sin_addr.s_addr = inet_addr( argv[1] );

	char * buf = argv[2];
	int ret = sendto( sock_fd, buf, strlen(buf)+1, 0, ( struct sockaddr * )&server_addr, sizeof(struct sockaddr_in ) );
	
	if( ret == -1 ) {
		perror("sendto");
		close( sock_fd );
		exit( 3 );
	}
	printf("send finish.\n");

	char recv_buf[128];
	bzero( recv_buf, sizeof(recv_buf) );
	recvfrom( sock_fd, recv_buf, sizeof(recv_buf), 0, NULL, NULL );
	printf("recv form server: %s\n", recv_buf );

	close( sock_fd );
	return 0;
}

