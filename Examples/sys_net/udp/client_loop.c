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
	
	int sock_fd = socket( AF_INET, SOCK_DGRAM, 0 );
	if( sock_fd == -1 ) {
		perror("socket");
		return 1;
	}

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons( PORT_NUM ); 
	server_addr.sin_addr.s_addr = inet_addr( "192.168.20.177" );

	char * buf = "hello world.";
	int count = 0;

	while( 1 ) {
	int ret = sendto( sock_fd, buf, strlen(buf)+1, 0, ( struct sockaddr * )&server_addr, sizeof(struct sockaddr_in ) );
	
	if( ret == -1 ) {
		perror("sendto");
		close( sock_fd );
		exit( 3 );
	}
		printf("client have send %d packet.\n", ++ count );
	}
	printf("send finish.\n");
	close( sock_fd );
	return 0;
}

