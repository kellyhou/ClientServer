#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>


int main( int argc, char ** argv )
{
	if( argc != 2 ) {
		return 1;
	}	

	struct hostent * he = gethostbyname( argv[1] );
	if( he == NULL ) {
		perror("gethostbyname");
		return 2;
	}

	char ** temp = he->h_aliases;	
	printf("office name: %s\n", he->h_name );
	printf("alias list: \n");
	while( *temp != NULL ) {
		printf("\t%s\n", *temp );
		temp ++;
	}	
	
	printf("IP地址列表:\n");
	temp = he->h_addr_list;
	while( *temp != NULL ) {
		printf("\t%s\n", inet_ntoa( *( struct in_addr * )(*temp) ) );
		temp ++;
	}	
	

	return 0;
}

