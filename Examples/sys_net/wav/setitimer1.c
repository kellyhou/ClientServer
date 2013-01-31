#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>

// setitimer.c

void sig_alrm( int sig )
{
	printf("已经过了1秒.\n");
}

int main(int argc, char **argv)
{
	signal( SIGALRM, sig_alrm );

	struct itimerval itv;
	itv.it_interval.tv_sec = 1;
	itv.it_interval.tv_usec = 0;

	itv.it_value.tv_sec = 3;
	itv.it_value.tv_usec = 0;

	setitimer( ITIMER_REAL, &itv, NULL );	

	int i = 100000000;
	while( 1 ) {
		while( i -- );
		printf("main\n");
		i = 100000000;
	};

}













