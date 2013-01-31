#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
// getpass.c

char * my_getpass( char * prompt )
{
	static char buf[100];
	
	struct termios term, save_term;
	tcgetattr( STDIN_FILENO, &term );
	save_term = term;

	term.c_lflag &= ~ECHO;
	tcsetattr( STDIN_FILENO, TCSANOW, &term );

	printf("%s", prompt );
	fgets( buf, sizeof(buf), stdin );

	tcsetattr( STDIN_FILENO, TCSANOW, &save_term );
	return buf;	
}

int main(int argc, char **argv)
{
	char * pass = my_getpass( "Passwd: " );
	printf("Passwd is : %s\n", pass );
}
