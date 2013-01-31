#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

// strftime.c

int main(int argc, char **argv)
{
	char buf[120];

	time_t t = time( NULL );
	struct tm * tm = localtime( &t );

	strftime( buf, sizeof(buf), "今天是 %G年%m月%d日 %H点%M分%S秒", tm );
	puts( buf );
}
