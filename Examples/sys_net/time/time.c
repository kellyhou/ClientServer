#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// time.c

int main(int argc, char **argv)
{
	time_t t = time( NULL );
	printf("从1970,1,1到现在 %d秒.\n", t );
}
