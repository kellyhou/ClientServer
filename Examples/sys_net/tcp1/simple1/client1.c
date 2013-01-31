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
	char * ip;
	if( argc == 1 ) {
		ip = "127.0.0.1";	
	} else if( argc == 2 ) {
		ip = argv[1];
	} else {
		fprintf( stderr, "Usage: %s ipaddress.\n", argv[0] );
		exit( 1 );
	}


	int sock_connect_fd = socket( AF_INET, SOCK_STREAM, 0 );
		
	struct sockaddr_in server_addr;
	init_addr( &server_addr, PORT_NUM, ip ); 	

	int ret = connect( sock_connect_fd, ( sockaddr_t * ) &server_addr, ADDR_SIZE ); 
	if( ret == -1 ) {
		perror("connect");
		exit( 1 );
	}
	
	send( sock_connect_fd, "hello world.", 13, 0 );
	
	close( sock_connect_fd );
		
	return 0;
}
	
