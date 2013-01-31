#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// conf.c

struct pair {
	char * name;
	char * val;
};

int load_conf_file( char * filename, struct pair search[] )
{
	FILE * fp = fopen( filename, "rb" );
	if( fp == NULL ) {
		return -1;
	}

	char line[1024];
	while( NULL != fgets( line, sizeof(line), fp ) ){
		int i = 0, j = 0;
		while( line[i] != '\0' ) {
			if( line[i] != ' ' && line[i] != '\t' && line[i] != '\n' )
				line[j++] = line[i];
			i++;
		}
		line[j] = 0;

		if( line[0] != '#' ) {
			char * pe = strchr( line, '=' );				
			if( pe != NULL ) {
				*pe = '\0';
				i = 0;
				while( search[i].name != NULL ) {
					if( ! strcasecmp( search[i].name, line ) ) {
						asprintf( &search[i].val, "%s", pe+1 );
						break;
					}
					i ++;
				}
			}
		}
	}
	
}

int main(int argc, char **argv)
{
	struct pair search[] = {
		{ "ONBOOT", NULL },
		{ "IPADDR", NULL },
		{ "NETMASK", NULL },
		{ "DEVICE", NULL },
		{ NULL, NULL }
	};

	load_conf_file( argv[1] , search );

	int i = 0;
	while( search[i].name != NULL ) {
		printf("%s  =  %s\n", search[i].name, search[i].val );
		i ++;
	}

}


