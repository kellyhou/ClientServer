#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>

// setitimer.c

struct itimerval itv;

void sig_alrm( int sig )
{
	printf("已经过了1秒.\n");
	itv.it_interval.tv_usec -= 100000;
	itv.it_value.tv_usec = itv.it_interval.tv_usec;
	setitimer( ITIMER_REAL, &itv, NULL );	
}

int main(int argc, char **argv)
{
	signal( SIGALRM, sig_alrm );

	itv.it_interval.tv_sec = 0;
	itv.it_interval.tv_usec = 1000000;

	itv.it_value.tv_sec = 0;
	itv.it_value.tv_usec = 1000000;

	setitimer( ITIMER_REAL, &itv, NULL );	

	int i = 100000000;
	while( 1 ) {
		while( i -- );
		printf("main\n");
		i = 100000000;
	};

}













