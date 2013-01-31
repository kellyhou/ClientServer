#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>


int filter( const struct dirent * d )
{
	if( !strncmp( d->d_name, "key", 3 ) )
		return 1;
	return 0;
}
// scandir.c

int main(int argc, char **argv)
{
		struct dirent ** namelist = NULL;

		scandir( argv[1], &namelist, filter, NULL  );

		while( *namelist != NULL ) {
			puts( (*namelist)->d_name );
			namelist ++;
		}

}
