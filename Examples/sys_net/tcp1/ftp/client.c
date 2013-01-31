#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include <sys/socket.h>
#include <netinet/in.h>

#include "ftp.h"

#define PORT_NUM 6000
#define ADDR_SIZE ( sizeof( struct sockaddr_in ) )
typedef struct sockaddr sockaddr_t;

void init_addr( struct sockaddr_in * addr, unsigned short port, char * ip )
{
	addr->sin_family = AF_INET;
	addr->sin_port = htons( port );	
	addr->sin_addr.s_addr = inet_addr( ip );
}

int readn( int fd, void * buf, int len )
{
	char * p = buf;

	int num = 0;
	while( len - num ) {
		num += read( fd, p+num, len - num );	 
		
	}			

	return 0;	
}

int writen( int fd, void * buf, int len )
{
	char * p = buf;

	int num = 0;
	while( len - num ) {
		num += write( fd, p+num, len - num );	 
	}			

	return 0;	
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
	
	char cmd[128];
	char data_buf[1400];

	while( 1 ) {
		printf("lftp %s:~> ", argv[1] );
		fflush( NULL );
		
		gets( cmd );
		if( !strncmp( cmd, "ls", 2 ) ) {
			*(int*)data_buf = LS;
			strncpy( data_buf+4, cmd+3, sizeof(cmd)-3 );
			writen( sock_connect_fd, data_buf, 4+sizeof(cmd) );
			
			int code;
			while( 1 ) {
				readn( sock_connect_fd, &code, 4 );		
				if( code == DATA ) {
					char data[1500];
					bzero( data, sizeof(data) );
					int len;
					readn( sock_connect_fd, &len, 4 );	
					readn( sock_connect_fd, data, len ); 
					data[len] = 0;
					printf("%s", data );
					fflush( NULL );
				} else if( code == DATA_END ) {
					break;	
				} else {
					printf("error from server code.\n");
					break;
				}
			}	
		} else if( !strcmp( cmd, "quit" )) {
			close( sock_connect_fd );	
			break;
		}
	}
		
	return 0;
}
	







