#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h> 
#include "playwav.h"
#include <sys/select.h>
#include <time.h>
#define AUDIO_FILE "/dev/audio"

// main.c
int main( int argc, char **argv )
{
	/*char *audio_file = "bill.wav";*/
	int fd = open( argv[1], O_RDONLY );
	if( fd < 1){
		perror( "openfile error!\n");
		return 1;
	}
	printf("open file ok!\n");
	int audio_fd = open ( AUDIO_FILE, O_RDWR );
	wav_head_t head;
	int retval ;
	
	retval = read( fd, &head, sizeof(head));
	if (retval < 1){
		perror( "cannot read this file\n");
		return 2;
	}
	retval = set_wav_arg( head, audio_fd);
	if (retval != 0){
		perror( "set_wav_arg error\n");
		return 3; 
	}

	int second_long = head.rate * head.channels * head.bits /8 ;
	char *buf = malloc( second_long );
	printf("second_long %d \n", second_long); 

	while (1){
		retval = read( fd, buf, second_long);
		if( retval < 1 ) break;
		write( audio_fd, buf, retval );
	}

	if (0 != wait_play_end( audio_fd)){ 
		return 5;
	}
	close (audio_fd);
	close (fd);
	return 0;	
}
