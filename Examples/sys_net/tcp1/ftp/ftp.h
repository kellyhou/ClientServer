#ifndef __FTP__H
#define __FTP__H

#define FILE_NAMESIZE 128

enum {
	PUT,
	GET,
	LS,
	CD,
	DEL,
	QUIT,

	DATA,
	DATA_END,
};

#endif
