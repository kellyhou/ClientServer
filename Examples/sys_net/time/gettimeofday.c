#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// gettimeofday.c

int main(int argc, char **argv)
{
	struct timeval tv;
	gettimeofday( &tv, NULL );

	printf("tv.tv_sec = %d\ntv.tv_usec = %d\n", tv.tv_sec, tv.tv_usec );
}
