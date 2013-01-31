typedef struct node node_t;
struct node {
	void * data;
	node_t * next;
};


void insert_node( node_t ** head, void * data, int len );
int del_node( node_t ** head, void * data, int len );
void print_list( node_t * head, void ( * print )( void * ) );
node_t * search( node_t * head, int ( *search_cond)( void * ) );
void free_list( node_t * head );
