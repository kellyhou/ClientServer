#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main( int argc, char ** argv )
{
	unsigned char * str = "中国,国足欢迎你.";
	unsigned char ch;

	while( ( ch = *str++ ) ) {
		printf("ch = %d\n", ch );
	}
	return 0;
}

