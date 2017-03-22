#ifndef REMOP_H
#define REMOP_H

#include<stdio.h>
#include<sys/types.h>
#define FILENAME_LEN 255
typedef int (*open_fn)(char *fname,int flags,mode_t m);
typedef int (*read_fn)(int fd,char *buff, int size);
typedef int (*write_fn)(int fd,char *buff, int size);
typedef int (*close_fn)(int fd);
/*typedef enum remote_op{
	REMOP_OPEN,
	REMOP_READ,
	REMOP_WRITE,
	REMOP_CLOSE
}remote_op_t;
*/
#define REMOP_OPEN 	1
#define REMOP_READ 	2
#define REMOP_WRITE 	3
#define REMOP_CLOSE	4
typedef struct open_params{
	char filename[FILENAME_LEN];
	int flags;
	int mode;
}open_params_t;
typedef struct rem_req{
	int rem_op; 
	union op_parms{
		open_params_t open_param;
	}u;
}rem_req_t;


#endif // REMOP_H
