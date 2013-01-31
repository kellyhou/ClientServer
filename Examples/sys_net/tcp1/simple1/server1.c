#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include <sys/socket.h>
#include <netinet/in.h>

#define PORT_NUM 6000
#define ADDR_SIZE ( sizeof( struct sockaddr_in ) )
typedef struct sockaddr sockaddr_t;

void init_addr( struct sockaddr_in * addr, unsigned short port, char * ip )
{
	addr->sin_family = AF_INET;
	addr->sin_port = htons( port );	
	addr->sin_addr.s_addr = inet_addr( ip );
}



int main( int argc, char ** argv )
{
	
	int sock_listen_fd = socket( AF_INET, SOCK_STREAM, 0 );
	
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons( PORT_NUM );
	server_addr.sin_addr.s_addr = htonl( INADDR_ANY );
	
	int re_use_addr = 1;
	setsockopt( sock_listen_fd, SOL_SOCKET, SO_REUSEADDR, &re_use_addr, sizeof(int) );
		
	int ret = bind( sock_listen_fd, ( sockaddr_t * )&server_addr, ADDR_SIZE );
	if( ret == -1 ) {
		perror("bind");
		exit( 1 );	
	}
	 
	listen( sock_listen_fd, 20 );
	
	int sock_connect_fd = accept( sock_listen_fd, NULL, NULL );

	char buf[1024];
	recv( sock_connect_fd, buf, 0, 0 );
	//recv( sock_connect_fd, buf, 1024, 0 );

	printf("buf = %s\n", buf );

	sleep( 1 );
		
	close( sock_connect_fd );

	
	close( sock_listen_fd );	
	
	return 0;
}











