#ifndef __TCP__H__
#define __TCP__H__

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define PORT_NUM 7777
#define ADDR_LEN sizeof(struct sockaddr_in)
#define BACK_LOG 1
typedef struct sockaddr SA;

#endif /* __TCP__H__ */
