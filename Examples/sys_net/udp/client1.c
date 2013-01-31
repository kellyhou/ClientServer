#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8888

int main(int argc, char **argv)
{
	int sfd = socket( AF_INET, SOCK_DGRAM, 0 ) ;
	
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons( PORT );
	server_addr.sin_addr.s_addr = inet_addr( argv[1] );

	char buf[100000];
	memset( buf, 'A', sizeof(buf) );

	int i = 0;
	while( 1 ) {
		int ret = sendto( sfd, buf, ++ i, 0, 
			( struct sockaddr * )&server_addr, sizeof(server_addr) );
		printf("i = %d\n", i );
		if( ret <= 0 ) {
			perror("sendto");
			break;
		}
	}

	close( sfd );
}








