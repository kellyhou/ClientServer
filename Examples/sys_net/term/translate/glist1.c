#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct gnode gnode_t;
struct gnode {
	gnode_t * next;	
	gnode_t * sub_level;
	char data[0];
};

gnode_t * search( gnode_t * phead, char * data )
{
	while( phead != NULL ) {
		if( !strcmp( phead->data, data ) )
			return phead;
		phead = phead->next;
	}

	return NULL;
}

int insert_single_gnode( gnode_t ** p, char * words )
{
	gnode_t * temp = malloc( sizeof(gnode_t) + strlen(words) );
	memcpy( temp->data, words, strlen(words) + 1 );
	temp->sub_level = NULL;
	temp->next = *p;
	*p = temp;
}

int insert_gnode( gnode_t ** p, char * str )
{
	char * words;
	gnode_t * temp;
	words = strsep( &str, " " );

	if( ( temp = search( *p, words )) == NULL ) {
		insert_single_gnode( p, words );
		temp = *p;
	}

	gnode_t * psub;
	while( 1 ) {
		words = strsep( &str, " " );	
		if( words == NULL ) 
			break;
		if( ( psub = search( temp->sub_level, words ) ) == NULL )	{
			insert_single_gnode( &temp->sub_level, words );
			temp = temp->sub_level;
		} else {
			temp = psub;
		}
	}	
	return 0;
}

int traversal_glist( gnode_t * p )
{
	gnode_t * temp = p;
	while( temp != NULL ) {
		printf("%s\n", temp->data );
		traversal_glist( temp->sub_level );
		temp = temp->next;
	}
}

int main( int argc, char ** argv )
{
	gnode_t * province = NULL;

	char info[1000];
	while( 1 ) {
		printf("Input a info: ");
		gets( info );
		if( !strcmp( info, "finish" ) )
			break;
		insert_gnode( &province, info );
	}
	
	traversal_glist( province );
	
	return 0;
}











