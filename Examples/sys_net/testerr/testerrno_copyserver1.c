#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char **argv)
{
    int socket_fd;
    int server_port = 5858;
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0 );

    struct sockaddr_in server_addr, client_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    char buf[1024] = "";
    int client_addr_len = sizeof(struct sockaddr_in);
    
    for(;;){
	recvfrom(socket_fd, buf, sizeof(buf), 0, 
			(struct sockaddr*)&client_addr, &client_addr_len);
	printf("client_addr_len = %d\n", client_addr_len );
	printf("The msg: %s from:%s\n", buf, inet_ntoa(client_addr.sin_addr));
	int fd = 0, ret_read = 0;
	//printf("fd = %p, client_addr_len = %p\n", &fd, &client_addr_len );
	//getchar();
	char sendmsg[10] = "";
	//getchar();
	fd = open(buf, O_RDONLY);
	printf("fd = %d\n", fd);
	//read(fd, sendmsg, sizeof(sendmsg));
	//printf("the file context: %s\n", sendmsg);
	struct sockaddr_in client_addr_save = client_addr;

	    int k;
	    char * p = sendmsg;


	    
	    for( k=0; k<18; k ++ )
		    printf("index = %d %d\n",k,  p[k] );

	    char * dp;
	asprintf(&dp, "sendmsg = %p ret_read = %p fd = %p\n",  sendmsg, &ret_read, &fd );

	    for( k=0; k<18; k ++ )
		    printf("index = %d %d\n",k,  p[k] );


	getchar();
	while( 1 ) {
	    printf("fd = %d\n", fd );
	    ret_read = read( fd, sendmsg, sizeof(sendmsg) );
	    if( ret_read == 0 ) {
		printf("读取完毕 。\n");
		break; 
	    }

	    printf("pfd = %p\n", &fd );
	    printf("fd = %d socket_fd = %d ret_read = %d client_addr_len = %d\n", 
			    fd, socket_fd, ret_read, client_addr_len );
	    if( memcmp( &client_addr, &client_addr_save, sizeof(client_addr) ) ) {
		printf("地址不同.\n");
	    }

	    int ret = sendto(socket_fd, sendmsg, ret_read, 0, 
			    (struct sockaddr *)&client_addr, client_addr_len);

	    if( ret == -1 ) {
		perror("sendto zheli");
		exit( 1 );
	    }
	    
	    int len = strlen( sendmsg );
	    printf("strlen = %d\n", len );
	    //memset(sendmsg, 0, strlen(sendmsg));
	
	    for( k=0; k<len+8; k ++ )
		    printf("index = %d %d\n",k,  p[k] );

	    for( k=0; k<len; k++ )
		    p[k] = '\0';

	    for( k=0; k<len+5; k ++ )
		    printf("index = %d %d\n",k,  p[k] );

	    printf("fd = %d\n", fd );
	    getchar();
	    fflush( NULL );
	    //memset(sendmsg, 0, sizeof(sendmsg));
	    usleep(10000);
	     
	}
	printf("while exit.\n");
	if(ret_read == 0){
	    sendto(socket_fd, "ok!!!", 5, 0, (struct sockaddr*)&client_addr, client_addr_len);
	}
	if(!strcmp(buf, "exit")){
	    break;
	}
    }
    

	
    return 0;
}
