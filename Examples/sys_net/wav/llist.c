#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "llist.h"

int insert_llnode( llnode_t ** first, void * pdata, int data_size )
{
	llnode_t * temp = malloc( sizeof(llnode_t) );
	temp->pdata = malloc( data_size );
	memcpy( temp->pdata, pdata, data_size );
	if(*first == NULL) {
	    *first = temp;
	    temp->next = NULL;
	} else {
	    temp->next = *first;
	    *first = temp;
	}
	return 0;
}

int sort_llnode( llnode_t * first, int ( * compare )(  ) )
{
   llnode_t * temp; 
   llnode_t * cur = first;

   while( cur != NULL ) { 
      temp = cur->next;    
      while( temp != NULL ) { 
         if( compare( cur->pdata, temp->pdata ) ) { 
            cur->pdata = ( void * )((int)cur->pdata ^ (int)temp->pdata);
            temp->pdata = ( void * )((int)cur->pdata ^ (int)temp->pdata);
            cur->pdata = ( void * )((int)cur->pdata ^ (int)temp->pdata);
         }    
         temp = temp->next;
      }   
      cur = cur->next;
   }   
   return 0;
}

int do_each_node( llnode_t * first )
{
	while( first != NULL ) {
		first = first->next;
	}	
	return 0;
}

int del_llnode_condition( llnode_t **first, int (*del_condition)(void *))
{
    llnode_t *temp = *first;
    llnode_t *pre;
    llnode_t *save;
    while( temp != NULL) {
	if(del_condition(temp->pdata)) {
	    if( temp == *first) {
		*first = temp->next;
	    } else {
		pre->next = temp->next;
	    }
	    save = temp->next;
	    free(temp->pdata);
	    free(temp);
	    temp = save;
	} else {
	    pre = temp;
	    temp = temp->next;
	}
    }
    return 0;
}

int del_llnode( llnode_t **first, llnode_t **del_node )
{
    llnode_t *temp = *del_node;
    llnode_t *loop = *first;
    if(temp == *first) {
	*first = temp->next;
    } else {
    while(loop->next != temp) {
	loop = loop->next;
    }
	loop->next = temp->next;
    }
    *del_node = (*del_node)->next;
    free(temp->pdata);
    free(temp);
}

int update_pdata( llnode_t *temp, void *pdata, int len )
{
    memcpy(temp->pdata, pdata, len);
}

