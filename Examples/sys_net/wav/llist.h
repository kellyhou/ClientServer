#ifndef __LLIST__H__
#define __LLIST__H__

typedef struct llnode llnode_t;
struct llnode {
	void * pdata;
	llnode_t * next;
};

int insert_llnode( llnode_t ** first, void * pdata, int data_size );
int sort_llnode( llnode_t * first, int ( * compare )(  ) );
int do_each_node( llnode_t * first );
int del_llnode_condition( llnode_t **first, int (*del_condition)( ));
int del_llnode( llnode_t **first, llnode_t **temp);
int update_pdata( llnode_t *temp, void *pdata, int len );

#endif  /* __LLIST__H__ */

