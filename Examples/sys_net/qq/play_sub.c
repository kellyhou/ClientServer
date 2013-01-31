#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "playwav.h"

void print_wav_info( wav_head_t *head , char *s)
{
	int second_long = head -> rate * head -> bits * head -> channels / 8;
	int time = (int )head -> sound_long / second_long / 60;
	int second = (int )head -> sound_long / second_long % 60;
	char *s1 = 0;
	asprintf(&s1, "%d:%d", time, second );
	printf("================================\n");
	printf("歌曲名字: %s\n", s);
	printf("采样率: %d\n", head -> rate );
	printf("采样位数: %d\n", head -> bits );
	printf("声道数:%d\n", head -> channels );
	printf("时间: %s\n", s1);
	printf("================================\n");
	printf("Playing...........\n");

}

int play_wav_file( char * name )
{
	int fd_wav = open( name, O_RDONLY );
	if( fd_wav == -1 ){
		perror("open wavfile");
		return 2;
	}

	wav_head_t head;
	read( fd_wav, &head, sizeof(head) );
	int fd_dev = open( AUDIO_DEV_FILE, O_WRONLY );
	if( fd_dev == -1 ){
		perror("open dev");
		return 3;
	}
	set_wav_arg( head, fd_dev );
	print_wav_info( &head, name );


	int second_long = head.rate * head.bits * head.channels / 8;
	char *buf = malloc( second_long );

	int flag = 1;
	fd_set readfs;
	int ret;

	while( 1 ){
			int read_len = read( fd_wav, buf, second_long );
			if( read_len == 0 )
				break;
			write( fd_dev, buf, read_len );
	}
	
	close( fd_wav );
	close( fd_dev );
}

