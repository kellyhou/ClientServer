#ifndef _PLAY_WAV_H
#define _PLAY_WAV_H

#define AUDIO_DEV_FILE "/dev/audio"
#define MAGIC "RIFF"

typedef struct {
	unsigned int  riff_flag;   /* RIFF flag                         */
	unsigned int  file_long;   /* then wav file long                */
	unsigned int  wav_flag;    /* wave flag                         */
	unsigned int  fnt_flag;    /* fnt flag                          */
	unsigned int  pass_byte;   /* ususal be 16 indicate head 44 byte*/
	short         format;      /* 01 is pcm , or not surport        */
	short         channels;    /* the number of sound channel       */
	int   	      rate;        /* the frequency of get sample       */
	unsigned int  tran_rate;   /* translate rate                    */
	short         date_block;  /* date block adjust                 */
	short         bits;        /* the bit number of numeric         */
	unsigned int  data_flag;   /* data flag                         */
	unsigned int  sound_long;  /* wav data long                     */
} wav_head_t;

int set_wav_arg( wav_head_t head, int dev_fd );
int wait_play_end( int dev_fd );

#endif /* _PLAY_WAV_H */

