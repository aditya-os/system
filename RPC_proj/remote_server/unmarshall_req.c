#include<stdio.h>
#include<string.h>
#include"../remote_client/remoteop.h"
#include<unistd.h>
#include<arpa/inet.h>
#include"unmarshall_req.h"
void unmarshall_open_params(void *recv_msg, rem_req_t *req){
	int processed=0,flag,mode;
	
	memcpy(req->u.open_param.filename,recv_msg+processed,sizeof(req->u.open_param.filename));
	processed+=sizeof(req->u.open_param.filename);

	memcpy(&flag,recv_msg+processed,sizeof(flag));
	processed += sizeof(flag);
	req->u.open_param.flags = ntohl(flag) ; 
	
	memcpy(&mode,recv_msg+processed,sizeof(mode));
	processed += sizeof(mode);
	req->u.open_param.mode = ntohl(mode);
	
}
void unmarshall_req_msg(void *recv_msg,rem_req_t * req, int rem_op){
	int m_rem_op; 
	int processed = 0 ; 
	memcpy(&m_rem_op,recv_msg,sizeof(rem_op));
	processed += sizeof(m_rem_op);
	m_rem_op = ntohl(rem_op);
	req->rem_op = m_rem_op;
	switch(rem_op){
		case REMOP_OPEN:
			unmarshall_open_params(recv_msg+processed,req);
			printf("********************Unmarshalled OPEN req*****************\n");
			printf("File name:  %s \n", req->u.open_param.filename);
			printf("Flag:  %d \n", req->u.open_param.flags);
			printf("Mode:  %d \n", req->u.open_param.mode);
		break;
		case REMOP_READ:
		break;
		case REMOP_WRITE:
		break;
		case REMOP_CLOSE:
		break;
	}
}
