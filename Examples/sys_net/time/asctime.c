#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

// asctime.c

int main(int argc, char **argv)
{
	time_t t = time( NULL );
	struct tm * tm = localtime( &t );
	printf("时间: %s\n", asctime( tm ) );
}
