#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int hanoi(int n, char A, char B, char C)
{
	if ( 1 == n ){
		printf( " %c ---> %c   %d \n", A, C, n);
		return ;
	}
	hanoi( n-1, A, C, B );
	printf( " %c ---> %c   %d \n", A, C, n);
	hanoi( n-1, B, A, C); 
}



int main(int argc, char **argv)
{
//	int steps = atoi( argv[1] );
	int steps = 3;
	hanoi(steps, 'A', 'B', 'C');
}
