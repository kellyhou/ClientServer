#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "ansi_lib.h"

typedef struct sockaddr sockaddr_t;
#define ADDR_SIZE ( sizeof( struct sockaddr_in ) )
#define PORT_NUM 80
#define DATA_BUF_SIZE  ( 1024 * 1024 )

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
	char url[ 200 ];
	struct sockaddr_in addr;
	char hostname[ 200 ];
	char file_path[ 200 ];
	char * file_name;
} url_t;
url_t url;

typedef struct {
	unsigned int start_position;
	unsigned int data_len;
	unsigned int thread_seq;
		
}seed_t;

int thread_num = 0;
int fd;
int win_x, win_y;
	
int parse_path( void )
{
	char * path = url.url;
	char * p_host = path + 7;

	char * p_host_end = strchr( p_host, '/' );
	strcpy( url.file_path, p_host_end );
	url.file_name = strrchr( p_host_end, '/' ) + 1;
	*p_host_end = 0;	
	strcpy( url.hostname, p_host );
	
	struct hostent * host = gethostbyname( p_host );
	if( host == NULL ) {
		printf("Domain name invaliad.\n");
		return -1;
	}
	
	memcpy( &url.addr.sin_addr, host->h_addr, host->h_length );
	url.addr.sin_family = AF_INET;
	url.addr.sin_port = htons( PORT_NUM );
	
	return 0;		
}

int do_head( int sock_fd, char * data_buf, int * data_length )
{
	int ret;
	char buf[1400];
	int read_num = 0;	
	char * p;

	bzero( buf, sizeof(buf) );
	while( 1 ) {
		ret = recv( sock_fd, buf+read_num, sizeof(buf), 0 );
		if(  ( p = strstr( buf, "\r\n\r\n" ) ) != NULL ) {
			break;			
		}
		read_num += ret;
	}

	read_num += ret;	

	int head_len = p + 4 - buf;
	memcpy( data_buf, p+4, read_num - head_len );
	
	if( ( p = ( char * )strcasestr( buf, "Content-Length: " ) ) != NULL ) {
		*data_length = atoi( p+16 ); 
	}

	return read_num - head_len;	
}

int print_url_info( void )
{
	printf("Hostname: %s\n", url.hostname );
	printf("IP address: %s\n", inet_ntoa( url.addr.sin_addr ) );
	printf("File path: %s\n", url.file_path );
}

void * thread_load( void * position_data )
{
	seed_t * seed = ( seed_t * ) position_data;
	int sock_fd = socket( AF_INET, SOCK_STREAM, 0 );
	if( sock_fd == -1 ) {
		perror("socket");
		exit( 1 );	
	}

	int ret = connect( sock_fd, ( sockaddr_t * )&url.addr, ADDR_SIZE );
	if( ret == -1 ) {
		perror("connect");	
		exit( 1 );
	}
	
	unsigned int start_position = seed->start_position;
	unsigned int data_len = seed->data_len;
	unsigned int thread_seq = seed->thread_seq;

	char * request = malloc( 1400 );
	sprintf( request, "GET %s HTTP/1.1\r\nHost: %s:%d\r\nRange: bytes=%d-\r\n\r\n",
		 url.file_path, url.hostname,PORT_NUM, start_position );

	send( sock_fd, request, strlen(request)+1, 0 );

	char * data_buf = malloc( DATA_BUF_SIZE );
	if( data_buf == NULL ) {
		perror("malloc");
		exit( 1 );
	}	
	
	pthread_mutex_lock( &mutex );
	move_xy( 1, thread_seq );
	printf("Thread %3d: [", thread_seq );
	pthread_mutex_unlock( &mutex );

	unsigned read_num = 0;
	unsigned write_position = start_position;
	unsigned total_read_size = 0;

	char * guage = ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
	guage += strlen( guage );

	int data_length;
	ret = do_head( sock_fd, data_buf, &data_length );
	if( ret > 0 ) {
		pthread_mutex_lock( &mutex );
		if( ret >= data_len ) {
			pwrite( fd, data_buf, data_len, write_position );
			if( thread_seq < win_y ) {
				move_xy( 14, thread_seq );
				printf("%s", guage );
			}
			pthread_mutex_unlock( &mutex );
			pthread_exit( 0 );
		}else
			pwrite( fd, data_buf, ret, write_position );
		write_position += ret;
		total_read_size += ret;
		pthread_mutex_unlock( &mutex );
	}

	while( 1 ) {
		ret = recv( sock_fd, data_buf, data_len - total_read_size, 0 );
		if( ret < 0 ) {
			perror("recv");
			break;
		} else if( ret == 0 ) {
			break;
		}

		total_read_size += ret;
	
		if( thread_seq < win_y ) {
			pthread_mutex_lock( &mutex );
	
			move_xy( 14, thread_seq );
			printf("%s", guage - ( ( int )(total_read_size * 1.0 / data_len * 50 )) );
			move_xy( 64, thread_seq );
			printf("]%3d%%", total_read_size  * 100 / data_len );		
			pwrite( fd, data_buf, ret, write_position );
	
			pthread_mutex_unlock( &mutex );
		}

		write_position += ret;	
		if( total_read_size == data_len )
			break;
	}

	close( sock_fd );
	pthread_exit( 0 );		
}

int main( int argc, char ** argv )
{
	
	if( argc != 2 ) {
		printf( "Usage: %s thread_num.\n", argv[0] );
		exit( 1 );
	}
	
	thread_num = atoi( argv[1] );

	int sock_fd = socket( AF_INET, SOCK_STREAM, 0 );
	if( sock_fd == -1 ) {
		perror("socket");
		exit( 1 );
	}
	
	setvbuf( stdout, NULL, _IONBF, 0 );

	int ret;
	printf("Input URL: ");
	ret = read( STDIN_FILENO, url.url, sizeof(url.url) );
	url.url[ret-1] = 0;
	ret = parse_path();			
	if( ret == -1 ) {
		printf("parse path error.\n");
		exit( 1 );
	}
	
	int addr_len = ADDR_SIZE;
	ret = connect( sock_fd, ( sockaddr_t * )( &url.addr), addr_len );
	if( ret == -1 ) {
		perror("connect error");
		exit( 1 );
	}
	
	print_url_info();
	
	char * request = malloc( 1000 );
	sprintf( request, "GET %s HTTP/1.1\r\nHost: %s:%d\r\n\r\n", 
		url.file_path, url.hostname, PORT_NUM );
	

	printf("request: %s\n", request );
	
	send( sock_fd, request, strlen(request)+1, 0 );
	
	fd = open( url.file_name, O_CREAT | O_TRUNC | O_WRONLY, 0666 );
	if( fd == -1 ) {
		perror( "open");
		exit( 1 );
	}
	
	char recv_buf[1400];
	int data_length;	

	printf("Receive data from %s ... \n", url.hostname ); 
	ret = do_head( sock_fd, recv_buf, &data_length );
	if( ret > 0 ) {
		write( fd, recv_buf, ret );
	}
	
	printf("File size: %d\n\n", data_length );
	char end = 0;
	lseek( fd, data_length -1 , SEEK_SET );
	write( fd, &end, 1 );

	clear_screen();
	close( sock_fd );	

	int i;
	seed_t seed[ thread_num ];
	pthread_t thread[ thread_num ];
	
	
	get_winsz( &win_x, &win_y );
	struct timeval tv_start;
	gettimeofday( &tv_start, NULL );
	
	if( !strcasecmp( url.file_name + strlen(url.file_name)-5, ".html" ) )
		thread_num = 1;
	
	int per_thread_load_size = data_length / thread_num;

	for( i=0; i<thread_num; ++i ) {
		seed[i].start_position = per_thread_load_size * i;
		if( i == ( thread_num - 1 ) )
			seed[i].data_len = per_thread_load_size + 
					   data_length % thread_num ;
		else 
			seed[i].data_len = per_thread_load_size;
		seed[i].thread_seq = i + 1;
		pthread_create( &thread[i], NULL, thread_load, 
					     &seed[i] );
	}	
		
	for( i=0; i<thread_num; ++i )
		pthread_join( thread[i], NULL );
	
	struct timeval tv_end;
	gettimeofday( &tv_end, NULL );
	
	float seconds = (( tv_end.tv_sec - tv_start.tv_sec ) * 1000000 + 
			( tv_end.tv_usec - tv_start.tv_usec )) /1000000.0;
	move_xy( 2, i+1 );
	printf("========== infomation ===========\n");
	printf("Total spent times: %3.1f seconds.\n", seconds );
	printf("average load speead: %4.4fk /s.\n\n", data_length /1024.0 /seconds ); 
	
	close( fd );
	return 0;
}
