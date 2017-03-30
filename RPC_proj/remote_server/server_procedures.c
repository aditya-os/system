#include<stdio.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include"../remote_client/remoteop.h"
#include"server_procedures.h"
#include<errno.h>
int execute_open_procedure(rem_req_t *req,rem_res_t *res){
	int fd;
	printf("Executing open procedure locally\n");
	fd = open(req->u.open_param.filename,req->u.open_param.flags,req->u.open_param.mode);
	res->rem_op = REMOP_OPEN;
	res->u.open_res.ret_val = fd;
	if(fd<0){
		res->u.open_res.err_no = errno;
	}
	else{
		res->u.open_res.err_no = 0;
	}
	close (fd);
	return 1;
}
