#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// gets.c
int main(int argc, char **argv)
{
	printf("Input: ");
	char buf[100];
	gets( buf );
	puts( buf );
}
