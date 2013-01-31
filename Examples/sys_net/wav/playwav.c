#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/soundcard.h>
#include "playwav.h"

int set_wav_arg( wav_head_t head, int dev_fd )
{
 
 	/* set numeric bits of get sample */
	int arg;
	int retval;
	
	arg = head.bits;
	retval = ioctl( dev_fd, SOUND_PCM_WRITE_BITS, &arg );
	if( retval == -1 ){
		printf("sound_pcm_write_bits failed.\n");
		exit( 1 );
	}
	if( head.bits != arg )
		perror("Unable to set sample bits !");
 
	 /* set the number of channel at get sample */
	 arg = head.channels;
	 retval = ioctl( dev_fd, SOUND_PCM_WRITE_CHANNELS, &arg );
	 if( retval == -1 ){
	 	printf("SOUND_PCM_WRITE_CHANNELS failed.\n");
		exit( 1 );	
	 }
	 if( arg != head.channels )
	        perror("Unable to set number of channels !");
 
	 /* set the frequence of get samples */
	 arg = head.rate;
	 retval = ioctl(dev_fd,SOUND_PCM_WRITE_RATE,&arg);
	 if( retval == -1 ){
	 	printf("set SOUND_PCM_WRITE_RATE failed.\n");
		exit( 1 );
	 }
	 if( arg != head.rate )
		perror("Uable to set number of rate");
	 
	return 0;
}

int wait_play_end( int dev_fd )
{
   
	int retval ; 
    	retval = ioctl( dev_fd, SOUND_PCM_SYNC, 0 );
	if( retval == -1 ){
		printf("SOUND_PCM_SYNC failed.\n");
		exit( 1 );
	}
	return 0;
}













