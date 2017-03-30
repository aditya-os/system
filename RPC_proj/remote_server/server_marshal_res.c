#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<string.h>
#include"server_marshal_res.h"
#include"../remote_client/remoteop.h"
void * marshal_open_res_msg(rem_res_t *res,int *buff_sz){
	int pos = 0, rem_op = 0 ,ret_val,err_no ; 
	void *buff; 
	*buff_sz=0;
	buff = malloc(sizeof(rem_res_t));
	if(!buff)
		return NULL;
	rem_op = htonl(res->rem_op);
	memcpy(buff+pos,&rem_op,sizeof(rem_op));
	pos += sizeof(rem_op);
	
	ret_val = htonl(res->u.open_res.ret_val);
	memcpy(buff+pos,&ret_val,sizeof(ret_val));
	pos += sizeof(ret_val);

	err_no = htonl(res->u.open_res.err_no);
	memcpy(buff+pos,&err_no,sizeof(err_no));
	pos += sizeof(err_no);
	*buff_sz = pos;
	return buff;	
}
