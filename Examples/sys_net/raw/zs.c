#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/time.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <netinet/ip_icmp.h>

#define ICMP_PACKET_SIZE 16
#define TIME_OUT_SECONDS 1

unsigned short cal_chk_sum( unsigned short * buf, int len )
{	
	unsigned int sum = 0;
	
	while( len > 1 ) {
		sum += ( * buf ++ );	
		len -= 2;
	}		
	
	if( len == 1 ) {
		sum += ( *( unsigned char * ) buf );	
	}
	
	sum = ( sum >> 16 ) + ( sum & 0xffff );
	sum = ( sum >> 16 ) + sum;
	
	unsigned short ret = ~sum;

	return ret;
}


int main( void )
{
	unsigned short buf[8];
	buf[1] = 0;

	unsigned short result = cal_chk_sum( buf, sizeof(buf) );	
	buf[1] = result;

	result = cal_chk_sum( buf, 16 );
	printf("result = %d\n", result );

}
