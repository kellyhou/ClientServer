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

int pack_icmp( char * buf, int seq )
{
	struct icmp * icmp = ( struct icmp * ) buf;

	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_cksum = 0;
	
	icmp->icmp_id = getpid();
	icmp->icmp_seq = seq;
	
	struct timeval tv;
	gettimeofday( &tv, NULL );	
	
	memcpy( buf+8, &tv, sizeof( tv ) );
	
	icmp->icmp_cksum = cal_chk_sum( (unsigned short * ) buf, ICMP_PACKET_SIZE );
}

int parse_ip_icmp_info( char * buf, struct sockaddr_in answer )
{
	struct ip * ip = ( struct ip * ) buf;
	struct icmp * icmp = ( struct icmp * ) (buf + ( ip->ip_hl << 2 )); 
	
	if( icmp->icmp_id != getpid() )
		return -1;	
	
	if( icmp->icmp_type != ICMP_ECHOREPLY )
		return -1;
	
	unsigned short chk_sum = icmp->icmp_cksum;
	icmp->icmp_cksum = 0;

	if( chk_sum != cal_chk_sum( ( unsigned short * )icmp, ICMP_PACKET_SIZE ) )
		return -2;	

	unsigned char ttl = ip->ip_ttl;
	unsigned short seq = icmp->icmp_seq;
	
	struct timeval * tv_send = ( struct timeval * )( (char *)icmp + 8 );
	struct timeval tv_now;		
	gettimeofday( &tv_now, NULL );
	
	unsigned int m_seconds = ( tv_now.tv_sec - tv_send->tv_sec ) * 1000000 +
				 ( tv_now.tv_usec - tv_send->tv_usec );
	
	printf("%d bytes from %s: icmp_seq=%d ttl=%d times=%.3f ms\n",
		(ip->ip_hl<<2)+ICMP_PACKET_SIZE+14, 
		inet_ntoa( answer.sin_addr ),
		seq, ttl, m_seconds / 1000.0 );
	return 0;				
}

int main( int argc, char ** argv )
{
	if( argc != 2 ) {
		printf("Usage: %s ipaddr/hostname\n", argv[0] );
		exit( 0 );	
	}
	
	struct hostent * host = gethostbyname( argv[1] );
	if( host == NULL ) {
		printf("ping: unknow host %s \n", argv[1] );
		exit( 2 );
	}
	 
	struct sockaddr_in dest;
	dest.sin_family = AF_INET;
	memcpy( &dest.sin_addr, host->h_addr, sizeof(int) );
	
	int sock_raw_fd = socket( AF_INET, SOCK_RAW, IPPROTO_ICMP );
	if( sock_raw_fd == -1 ) {
		perror("socket");
		exit( 1 );
	}	
	
	char buf[ICMP_PACKET_SIZE];

	int seq = 0;
	
	char recv_buf[ 50 ];
	struct sockaddr_in answer;
	int answer_len = sizeof(answer);

	while( 1 ) {
		seq ++;
		pack_icmp( buf, seq );
		sendto( sock_raw_fd, buf, ICMP_PACKET_SIZE, 0, 
			( struct sockaddr * )&dest, sizeof( dest ) );

		
		while( 1 ) {
			fd_set readset;
			FD_ZERO( &readset );
			FD_SET( sock_raw_fd, &readset );
		
			struct timeval tv;
			tv.tv_sec = TIME_OUT_SECONDS;
			tv.tv_usec = 0;

			int ret = select( sock_raw_fd+1, &readset, NULL, NULL, &tv );
			if( ret == -1 ) {
				perror("select");
				exit( 3 );
			} else if( ret == 0 ) {
				printf("time out.\n");
				break;
			} else {
				int resolve = 0;
				int ret = recvfrom( sock_raw_fd, recv_buf, 36, 0, 
				( struct sockaddr * )&answer, &answer_len );
	
				if( ret > 0 )	
					resolve = parse_ip_icmp_info( recv_buf, answer );
				if( resolve == 0 )
					break;
			}
		}
		sleep( 1 );
	}

	
	return 0;
}













