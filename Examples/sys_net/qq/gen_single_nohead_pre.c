#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "gen_single_nohead_pre.h"

void insert_node( node_t ** head, void * data, int len )
{
	node_t * temp = malloc( sizeof(node_t) );	
	temp->data = malloc( len );
	memcpy( temp->data, data, len );
	
	temp->next = *head;
	*head = temp;
}

int del_node( node_t ** head, void * data, int len )
{
	node_t * temp = *head;
	node_t * pre;

	if( temp == NULL )
			return -1;

	while( temp != NULL ) {
		if( ! memcmp( data, temp->data, len ) ) {
			if( temp == *head )  {
				*head = temp->next;
				free( temp->data );
				free( temp );
				return 0; 
			} else {
				pre->next = temp->next;
				free( temp->data );
				free( temp );
				return 0;
			}
		}
		pre = temp;
		temp = temp->next;
	}
	return 1;	
}

void print_list( node_t * head, void ( *print )( void * ) )
{
	node_t * temp = head;
	while( temp != NULL ) {
		print( temp->data );
		temp = temp->next;
	}
}

node_t * search( node_t * head, int (*search_cond)(void *) )
{
	node_t * temp = head;
	while( temp != NULL ) {
		int ret = search_cond( temp->data );
		if( ret == 1 ) 
			return temp;

		temp = temp->next;
	}

	return NULL;
}

void free_list( node_t * head )
{
	node_t * temp = head;
	while( temp != NULL ) {
		temp = temp->next;
		free( head );
		head = temp;
	}
}

int length( node_t * head )
{
	int len = 0;
	while( head != NULL ) {
		len ++;
		head = head->next;
	}

	return len;
}

void sort( node_t * head, int ( *compare)( void *, void * ) )
{
	int len = length( head );		
	int i, j;
	int flag;
	node_t * temp;

	for( i=1; i<len; i++ ) {
		temp = head;
		for( j=0; j<len-i; j++ ) {
			flag = compare( temp->data, temp->next->data );
			if( flag == 1 ) {
				void * data = temp->data;
				temp->data = temp->next->data;
				temp->next->data = data;
			}
			temp = temp->next;
		}
	}
}






