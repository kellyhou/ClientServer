#include <stdio.h>
#include <asm/termios.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "ansi_code.h"
#include "tcp.h"

void get_winsz( int * x, int * y ) 
{
   struct winsize winsz;

   ioctl( STDIN_FILENO, TIOCGWINSZ, &winsz );
   *x = winsz.ws_col;
   *y = winsz.ws_row;
}

int x_len, y_len;

void show_window( void )
{
	clear_screen();
	rectangle( 1, 1, x_len, y_len );
	line( 1, y_len * 2 / 3, x_len, 0, '=' );
}

void update_message_window( void * buf, int show_line, int show_line_len )
{
	save_position();

	bar( 2, 2, x_len-2, y_len/3*2-2, ' ' );
	int i;
	char sbuf[ show_line ] [ show_line_len ];
	memcpy( sbuf, buf, show_line * show_line_len );
	for( i=2; i<2+show_line; i++ ) {
		move_xy( 2, i );
		printf("%s", sbuf[ i - 2 ] );
	}
	restore_position();
	fflush( NULL );
}

int main( int argc, char ** argv )
{
	if( argc != 3 ) {
		printf("Usage: %s ipaddr login_name\n", argv[0] );
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

	get_winsz( &x_len, &y_len );
	show_window();

	int show_line = y_len / 3 * 2 - 2;
	int show_line_len = x_len - 2;
	char show_buf[ show_line ][ show_line_len ]; 
	bzero( show_buf, show_line * show_line_len );

	move_xy( 2, y_len/3*2 + 2 );
	fflush( NULL );

	while( 1 ) {
		
		fd_set readset;
		FD_ZERO( &readset );
		FD_SET( STDIN_FILENO, &readset );
		FD_SET( sock_fd, &readset );
		
		int sel_ret;
		sel_ret = select( sock_fd + 1, &readset, NULL, NULL, NULL );
		if( sel_ret < 0 ) {
			perror("select");
			exit( 3 );
		} else if( sel_ret > 0 ) {
			char read_buf[1000];
			bzero( read_buf, sizeof(read_buf) );
			int read_count;

			if( FD_ISSET( STDIN_FILENO, &readset ) ) {
				read_count = read( STDIN_FILENO, read_buf, sizeof(read_buf) );	
				send( sock_fd, read_buf, read_count + 1 , 0 );
				move_xy( 2, y_len/3*2 + 2 );
				clear_to_line_end();
				move_xy( 2, y_len/3*2 + 2 );
				fflush( NULL );
			}

			char recv_buf[1500];
			if( FD_ISSET( sock_fd, &readset ) ) {
				int recv_count = recv( sock_fd, recv_buf, 
					sizeof(recv_buf), 0 );
				memmove( show_buf, show_buf+1, (show_line-1)*show_line_len ); 
				memcpy( show_buf[show_line-1], recv_buf, recv_count + 1 );
				update_message_window( show_buf, show_line, show_line_len );
			}
		}
	}

	close( sock_fd );
	
	return 0;
}

