#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include"remoteop.h"
#include"marshal_remote_call.h"
void * marshal_open_params(rem_req_t *req, int *msg_sz){
	int pos =0 ; 
	void *req_msg;
	int rem_op,flag,mode;
	req_msg = malloc(sizeof(rem_req_t));
	if(!req_msg)
		return NULL;
	rem_op = htonl(req->rem_op);
	memcpy(req_msg+pos,&rem_op,sizeof(int));
	pos = pos+sizeof(int);
	memcpy(req_msg+pos,req->u.open_param.filename,sizeof(req->u.open_param.filename));
	pos = pos + sizeof(req->u.open_param.filename);

	flag = htonl(req->u.open_param.flags);
	memcpy(req_msg+pos,&flag, sizeof(flag) );
	pos = pos + sizeof(flag);

	mode = htonl(req->u.open_param.mode);
	memcpy(req_msg+pos,&mode,sizeof(mode));
	pos = pos + sizeof(mode);
	*msg_sz = pos;
	return req_msg;
}
void remote_open_req(rem_req_t *req, char *fname, int flags , mode_t m ){
	req->rem_op = REMOP_OPEN;
	strcpy((req->u.open_param.filename),fname);
	req->u.open_param.flags = flags;
	req->u.open_param.mode = (int)m ;
}
