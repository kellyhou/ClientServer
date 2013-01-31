#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

// tree.c

int tree_print( char * dirname, int level )
{
	DIR * dir = opendir( dirname );	
	if( dir == NULL ) {
		printf("path = %s ", dirname );
		perror("opendir");
		exit( 1 );
	}

	char save_dir[100];
	getcwd( save_dir, sizeof(save_dir) );

	chdir( dirname );

	struct dirent * dirent = NULL;
	struct stat st;

	while( ( dirent = readdir( dir ) ) != NULL ) {
		if( strcmp( dirent->d_name, "." ) && 
				strcmp( dirent->d_name, ".." )) {
			stat( dirent->d_name,  &st );
			int i;
			for( i=0; i<level; i++ )
				putchar( '\t' );
			printf("%s\n", dirent->d_name );
			if( S_ISDIR( st.st_mode ) )
				tree_print( dirent->d_name, level + 1 );
			}
	}

	closedir( dir );
	chdir( save_dir );
}

int main(int argc, char **argv)
{
	if( argc == 2 ) {
		tree_print( argv[1], 0 );
	} else {
		fprintf( stderr, "Usage: %s dirname\n", argv[0] );
		return 1;
	}
	return 0;
}
