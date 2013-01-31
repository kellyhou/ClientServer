#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "input.h"
#include "ansi_code.h"
#include "glist.h"

#define CIKU_FILENAME	"ciku.dict"
#define MAX_WORDS_LEN 128

int winx, winy;
int left_words_num;
int have_search_words_num;

int debug( char * fmt, char * str )
{
	save_position();
	
	move_xy( 65, 15 );
	printf("            \n");
	move_xy( 65, 15 );
	printf( fmt, str );
	
	restore_position();
}

int search_store_left_words( gnode_t * temp, char * words_str, int cur_words_str_len, char left_words[][MAX_WORDS_LEN] )
{
	gnode_t * eachtemp = temp;

	cur_words_str_len ++;
	while( eachtemp != NULL ) {
   		if( have_search_words_num == left_words_num )
				return 0;
		words_str[cur_words_str_len-1] = eachtemp->ch; 
		if( eachtemp->explain != NULL ) {
			memcpy( left_words[have_search_words_num], 
				words_str, cur_words_str_len );
			have_search_words_num ++;
		} 
		if( temp->sub_level != NULL )
			search_store_left_words( eachtemp->sub_level, words_str, 
				cur_words_str_len, left_words );
		eachtemp = eachtemp->next;
	}
	return 0;
}

int search_words( g_dict_list_t * gdl, char * input_buf, 
	char left_words [][MAX_WORDS_LEN], char ** explain )
{
	char ch;
	gnode_t * temp = gdl->first_level;
	gnode_t * search_node;
	*explain = NULL;

	if( input_buf[0] == '\0' )
		return -1;

	have_search_words_num = 0;
	while( 1 ) {
		ch = *input_buf ++;	
		search_node = search( temp, ch );		

		if( search_node == NULL ) {
			*explain = NULL;
			return -1;
		} else {
			if( *input_buf == '\0' ) {
				*explain = search_node->explain;
				temp = search_node;
				if( *explain != NULL )
					have_search_words_num ++;
				break;
			} else {
				temp = search_node->sub_level;	
				if( temp == NULL ) 
					return -1;
			}
		}
	}

	char words_str[MAX_WORDS_LEN];

	if( temp->sub_level != NULL ) {
		search_store_left_words( temp->sub_level, words_str, 
			0, left_words );
	}

	return 0;
}

int show_window( void )
{
   clear_screen();
	get_win_size( &winx, &winy );	
   rectangle( 1, 1, winx, winy );

	move_xy( 3, 3 );
	printf("善哉词霸");

	rectangle( 13, 2, winx-15, 3 );
   line( 1, 5, winx, 0, '=' );  

	line( winx / 3, 6, winy-6, 1, '|' );
	move_xy( 10,10 );
	fflush( NULL );
}

int move_input_window( void )
{
	move_xy( 15, 3  );
}

int show_left_words( char * prefix, char left_words[][MAX_WORDS_LEN] )
{
	save_position();

	bar( 2, 6, winx / 3 - 3, left_words_num, ' ' );
	int i;
	for( i=0; i<have_search_words_num; i++ ) {
		move_xy( 3, 6+i );
		left_words[i][winx/3-5-strlen(prefix)] = '\0';
		printf("%s%s", prefix, left_words[i] );
	}
	fflush( NULL );
	restore_position();
}

int show_explain( char * words, char * explain )
{
	save_position();

	bar( winx/3 + 2, 7, winx/3*2 -2, winy - 8, ' ' );
	move_xy( winx / 3 + 2, 7 );
	if( explain == NULL ) {
		printf("sorry, 没有找到这个单词.\n" );
	} else {
		printf("------ <<善哉电子辞典>> -------");
		move_xy( winx / 3 + 2, 8 );
		printf("单词: ");
		move_xy( winx / 3 + 2, 9 );
		printf("   %s", words ); 
		move_xy( winx / 3 + 2, 10 );
		printf("意义: ");
		move_xy( winx / 3 + 2, 11 );
		printf( "  %s\n", explain );
	}
	restore_position();
}

void print_input_buf( char * str, int input_off )
{
	save_position();
	move_xy( 20, 10 );
	printf("                                   \n");
	move_xy( 20, 10 );
	printf("%s len = %d\n", str, strlen(str) );
	printf( "input_off = %d\n", input_off );
	fflush( NULL );
	restore_position();
}

int main( int argc, char ** argv )
{
	g_dict_list_t gdl;
	init_g_dict_list( &gdl );

	printf("正在加载词库...\n");	
	FILE * fp = fopen( CIKU_FILENAME, "rb" );
	if( fp == NULL ) {
		perror("fopen");
		return 1;
	}

	char words_line[1024];
	char explain_line[1024];
	char * ret;
	char * p;

	int len = 0;
	while( !feof( fp ) ) {
		ret = fgets( words_line, sizeof(words_line), fp );
		if( ret == NULL )
			break;

		p = strchr(words_line, '\n');
		*p = '\0';
		len += strlen( words_line );
		ret = fgets( explain_line, sizeof(explain_line), fp );
		if( ret == NULL )
			break;

		p = strchr(explain_line, '\n');
		*p = '\0';
		insert_dict_glist( &gdl, words_line, explain_line ); 	
	}
	printf("cha liao %d ci.\n", gdl.node_num );
	
	printf("加载完毕.\n");
	//traversal_glist( gdl.first_level );
	printf("len = %d\n", len );
	//return 0;

	show_window();	

	char input_buf[ winy ];
	move_input_window();
	int key;
	int input_off = 0;

	left_words_num = winy - 6;
	char left_words[ left_words_num ][MAX_WORDS_LEN];
	char * explain;

	insert_dict_glist( &gdl, "shit", "些特" );

	while( 1 ) {
		key = get_key( 0 );		
		switch( key ) {
			case BACKSPACE:
				if( input_off > 0 ) {
					move_left( 1 );
					putchar( ' ' );
					move_left( 1 );
					input_off --;

					input_buf[ input_off ] = '\0';
					bzero( left_words, sizeof(left_words) );

					search_words( &gdl, input_buf, left_words, &explain );

					show_left_words( input_buf, left_words );
					show_explain( input_buf, explain );
					fflush( NULL );
				}
				break;
			case ESC:
				clear_screen();
				move_xy( 1, 1 );
				exit( 0 );
				break;	
			default:
				if( input_off < winx - 20 ) {
					if( isprint( key ) ) 
						putchar( key );

					input_buf[ input_off ++ ] = key;

					input_buf[ input_off ] = '\0';
					bzero( left_words, sizeof(left_words) );
					search_words( &gdl, input_buf, left_words, &explain );

					show_left_words( input_buf, left_words );
					show_explain( input_buf, explain );
					fflush( NULL );
				}
		}
	}
		
	return 0;
}







