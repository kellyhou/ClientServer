#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <fcntl.h>

// semget.c

#define FILENAME "file"

int lock( int semid )
{
	struct sembuf sb;

	sb.sem_num = 0;
	sb.sem_op = -1;
	sb.sem_flg = SEM_UNDO;

	semop( semid, &sb, 1 );
}

int unlock( int semid )
{
	struct sembuf sb;

	sb.sem_num = 0;
	sb.sem_op = 1;
	sb.sem_flg = SEM_UNDO;

	semop( semid, &sb, 1 );
}

int write_op( int fd )
{
	int i;
	char ch = 'B';
	for( i=0; i<50; i++ ) {
		write( fd, &ch, 1 );
	}

	ch = '\n';
	write( fd, &ch, 1 );
}

int main(int argc, char **argv)
{
	key_t key = ftok( "/home/play.c", 567 );

	int sem_id = semget( key, 1, IPC_CREAT | 0666 ) ;
	if( sem_id == -1 ) {
		perror("semget");
		exit( 1 );
	}

	/*semctl( sem_id, 1, IPC_SET, 1 );*/

	int fd = open( FILENAME, O_CREAT | O_WRONLY | O_APPEND, 0666 );	
	if( fd == -1 ) {
		perror("open");
		exit( 2 );
	}
			
	
	int i = 1000000;

	while( i-- ) {
		lock( sem_id );
		write_op( fd );
		unlock( sem_id );
	}
	
	close( fd );

}


