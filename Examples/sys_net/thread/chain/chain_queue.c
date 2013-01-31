#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "chain_queue.h"

int init_chain_queue( chain_queue_t * cq, int per_data_len )
{
	cq->per_data_len = per_data_len;
	cq->tail = NULL;
	cq->head = NULL;
	cq->exist_ele_num = 0;
}

int empty_chain_queue( chain_queue_t * cq )
{
	if( cq->exist_ele_num == 0 )
		return 1;
	return 0;
}

int put_queue( chain_queue_t * cq, void * pdata )
{
	cq_node_t * temp = malloc( sizeof(cq_node_t) + cq->per_data_len );
	temp->next = NULL;
	memcpy( temp->data, pdata, cq->per_data_len );

	if( empty_chain_queue( cq ) ) 
		cq->head = temp;
	else 
		cq->tail->next = temp;

	cq->tail = temp;
	cq->exist_ele_num ++;

	return 0;
}

int get_queue( chain_queue_t * cq, void * poutdata )
{
	if( empty_chain_queue( cq ) )
		return -1;
	memcpy( poutdata, cq->head->data, cq->per_data_len );

	cq_node_t * temp = cq->head;
	cq->head = cq->head->next;
	free( temp );
	cq->exist_ele_num --;

	return 0;
}














