#ifndef __CHAIN_QUEUE_H
#define __CHAIN_QUEUE_H

typedef struct cq_node cq_node_t;
struct cq_node {
	cq_node_t * next;	
	char data[0];
};

typedef struct chain_queue chain_queue_t;
struct chain_queue {
	int per_data_len;	
	cq_node_t * tail;
	cq_node_t * head;
	int exist_ele_num;
};



int init_chain_queue( chain_queue_t*cq, int per_data_len );
int full_chain_queue( chain_queue_t * cq );
int empty_chain_queue( chain_queue_t * cq );
int put_queue( chain_queue_t * cq, void * pdata );
int get_queue( chain_queue_t * cq, void * poutdata );


#endif /* __CHAIN_QUEUE_H */
