#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <errno.h>
#define UDP_PORT 9999 
#include "head.h"

// upd_server.c
int main(int argc, char **argv)
{
	if (argc != 2){
		printf("Uage: use ip address\n");
		return -1;
	}
	int client= socket( PF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in server;

	server.sin_family = AF_INET;
	server.sin_port = UDP_PORT;
	server.sin_addr.s_addr = inet_addr(argv[1]);

	char buf[ BUF_LEN ];
	bzero( buf, BUF_LEN);
	socklen_t socklen = sizeof(server);
	while (1){
		bzero( buf, BUF_LEN);
		read(0, buf, BUF_LEN-1);
		if (!strncmp(buf, "file", 4)){
			read_file( client, &server);	
		}else {
			sendto( client , buf, sizeof(buf), 0,
				(struct sockaddr *)&server, socklen);
		}
	}
	close(client);
}
int read_file( int client, struct sockaddr_in *server)
{
	char buf [BUF_LEN];
	socklen_t socklen = sizeof(struct sockaddr_in);

	printf("输入你要传的文件:\n");
	bzero(buf, BUF_LEN);
	sprintf(buf, "%s", "file");
	read(0, buf + 4, BUF_LEN-1);

	buf[strlen(buf)-1]=0;	
	int fd = 0;
	fd = open( buf + 4 , O_RDONLY);
	if( fd == -1){
		printf("read file error\n");
		perror("");
		return -1;	
	}
	struct stat file_st;
	if ( -1 == fstat(fd, &file_st)){ 
		printf("file cannot access\n");
		return -2;
	}
	sprintf(buf + 64, "%d", file_st.st_size );
	sendto( client , buf, sizeof(buf), 0,
			(struct sockaddr *)server, socklen);

	int retval = 0;
	int read_size =0;
	char size[16] ; 
	short percent = 0.0;
	while (1){
		bzero(buf, BUF_LEN);
		bzero(size, 16);
		sprintf(buf,"%s", "zhu ");
		retval = read ( fd, buf + 20, BUF_LEN - 20); 
		sprintf(size, "%d", retval);	
		memcpy( buf + 4, size, 16);
		sendto( client , buf, sizeof(buf), 0,
			(struct sockaddr *)server, socklen);
		
		read_size += retval;
		/*percent = ( short ) file_st.st_size / read_size;*/
		printf(" 已发送了 %d 个字节, 还剩 %d  ... \n", read_size, percent );

		if (retval < 1){
			break;
		} 
	}	
	return 0;	
}

