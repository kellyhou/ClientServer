#ifndef __GLIST_DICT_H
#define __GLIST_DICT_H

typedef struct gnode gnode_t;
struct gnode {
	gnode_t * next;	
	gnode_t * sub_level;
	char * explain;
	char ch;
};

typedef struct g_dict_list g_dict_list_t;
struct g_dict_list {
	gnode_t * first_level;	
	int node_num;
};



int init_g_dict_list( g_dict_list_t * gdl );
gnode_t * search( gnode_t * phead, char ch );
gnode_t * insert_single_gnode( gnode_t ** p, char ch, char * explain );
int insert_dict_glist( g_dict_list_t * gdl, char *words,char * explain);
int traversal_glist( gnode_t * p );


#endif /* __GLIST_DICT_H */

