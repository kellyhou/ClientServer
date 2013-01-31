#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "glist.h"

int init_g_dict_list( g_dict_list_t * gdl )
{
	gdl->first_level = NULL;
	gdl->node_num = 0;
}

gnode_t * search( gnode_t * phead, char ch )
{
	while( phead != NULL ) {
		if( phead->ch == ch ) 
			return phead;
		phead = phead->next;
	}
	return NULL;
}

gnode_t * insert_single_gnode( gnode_t ** p, char ch, char * explain )
{
	gnode_t * temp = malloc( sizeof(gnode_t) );

	if( explain == NULL )
		temp->explain = NULL;
	else {
		temp->explain = malloc( strlen(explain) + 1 );
		strcpy( temp->explain, explain );
	}
	temp->sub_level = NULL;
	temp->ch = ch;

	if( *p == NULL || (*p)->ch > ch ) {
		temp->next = *p;
		*p = temp;
		return *p;
	} else {
		gnode_t * each = *p;
		while( each->next != NULL ) {
			if( each->next->ch > ch ) {
				temp->next = each->next;
				each->next = temp;
				return temp;
			}	
			each = each->next;
		}
		temp->next = NULL;
		each->next = temp;
		return temp;
	}	
}

int insert_dict_glist( g_dict_list_t * gdl, char *words,char * explain)
{
	char ch;
	gnode_t ** temp = &gdl->first_level;
	gnode_t * search_node;
	gnode_t * insert_node;

	while( 1 ) {
		ch = *words ++;	
		search_node = search( *temp, ch );
		if( search_node == NULL ) {
			gdl->node_num ++;
			if( *words == '\0' ) {
				insert_node = insert_single_gnode( temp, ch, explain );	
				break;
			} else {
				insert_node = insert_single_gnode( temp, ch, NULL );	
			}
			temp = &insert_node->sub_level;
		} else {
			temp = &search_node->sub_level;	
		}
	}	
}

int traversal_glist( gnode_t * p )
{
	gnode_t * temp = p;
	while( temp != NULL ) {
		printf("%c\n", temp->ch );
		if( temp->sub_level == NULL )
			printf("explain = %s\n", temp->explain );
		traversal_glist( temp->sub_level );
		temp = temp->next;
	}
}


