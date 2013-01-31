#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include "llist.h"
#include "play_sub.h"

// lrc_display.c


struct line_lrc {
	int tsec;
	char lrc_data[100];
};

llnode_t * pcurnode;

void sig_alrm( int sig )
{
	struct line_lrc * ll = pcurnode->pdata;
	printf("%s", ll->lrc_data );

	int cur_tsec = ll->tsec;
	pcurnode = pcurnode->next;
	ll = pcurnode->pdata;

	struct itimerval itv;
	//itv.it_interval.tv_sec = 0;
	//itv.it_interval.tv_usec = ll->tsec * 10000;


	if( pcurnode == NULL ) {
		itv.it_value.tv_sec = 0;
		itv.it_value.tv_usec = 0;
	} else {
		itv.it_value.tv_sec = 0;
		itv.it_value.tv_usec = ( ll->tsec - cur_tsec ) * 10000;
	}

	setitimer( ITIMER_REAL, &itv, NULL );
}

int compare( int * tsec1, int * tsec2 )
{
	if( * tsec1 > *tsec2 )
		return 1;
	return 0;
}

int main(int argc, char **argv)
{
	llnode_t * head = NULL;	

	FILE * fp = fopen( argv[1], "rb" );
	if( fp == NULL ) {
		perror("fopen");
		exit( 1 );
	}	
	
	int ret;
	char * line = NULL;
	int len = 0;

	int i, j;
	char buf[1024] = { 0 };
	char * p = buf;

	for( i=0; i<4; i++ ) {
		fgets( p, 1024, fp );
		p += strlen( p );
	}
	struct line_lrc llrc;			
	llrc.tsec = 0;
	strcpy( llrc.lrc_data, buf );
	insert_llnode( &head, &llrc, 4 + strlen( buf ) + 1 );

	while( ( ret = getline( &line, &len, fp ) ) != -1 ) {
		i = 0;
		while( line[i] == '[' ) {
			i += 10;
		}
		for( j=0; j<i; j+=10 ) {
			int min, sec, usec;		
			sscanf( line+j, "[%d:%d.%d]", &min, &sec, &usec );
			llrc.tsec = min * 6000 + sec * 100 + usec;
			strcpy( llrc.lrc_data, line+i );	
			insert_llnode( &head, &llrc, 4 + strlen(line+i)+1);
		}	
	}

	printf("sort ....\n");
	sort_llnode( head, compare );

	struct line_lrc * ll;

	/*
	while( head != NULL ) {
		ll = head->pdata;
		puts( ll->lrc_data );	
		head = head->next;
	}*/

	pcurnode = head;
	ll = pcurnode->pdata;	
	puts( ll->lrc_data );
	pcurnode = pcurnode->next;
	ll = pcurnode->pdata;	

	struct itimerval itv;
	//itv.it_interval.tv_sec = 0;
	//itv.it_interval.tv_usec = ll->tsec * 10000;
	itv.it_value.tv_sec = 0;
	itv.it_value.tv_usec = ll->tsec * 10000;

	signal( SIGALRM, sig_alrm );
	setitimer( ITIMER_REAL, &itv, NULL );

	play_wav_file( argv[2] );

}





