#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
//#include<fcntl.h>
#include<errno.h>
#include<dlfcn.h>
#include<unistd.h>
#include"remoteop.h"
#include"remote_client_conn_setup.h"
#include"marshal_remote_call.h"
#include"unmarshal_remote_res.h"
void * get_res_buff(){
	void *buff;
	int sz = 0 ;
	sz = sizeof(rem_res_t);
	buff = malloc(sz);
	if(!buff)
		NULL;
	return buff;
}
int get_res_size(int op){
	int sz = 0 ; 
	sz = sizeof(int); // sizeof of response code i.e first field in rem_res_t 
	switch (op){
		case REMOP_OPEN:
		sz += sizeof(open_return_t);
		break;
	}
	return sz;
}
int open(char *fname, int flags, mode_t m ){
	int fd,serv_conn_fd,msg_sz,ret,res_sz;
	void *rem_req,*rem_res;
	rem_file_t *rf;
	open_fn orig_open;
	rem_req_t req;
	rem_res_t res;
	printf("This is passthrough Layer: open\n");
	orig_open = dlsym(RTLD_NEXT,"open");
	fd = orig_open(fname,flags,m);
	serv_conn_fd = setup_client_connection();
	if(serv_conn_fd < 0 ){
		return -1;
	}
	printf("Connection Established\n");
	printf("File name %s \n",fname);
	printf("Flags %d \n",flags);
	printf("mode %d \n",m);
	/*Connect with remote service and send request*/
	remote_open_req(&req,fname,flags,m);
	rem_req = marshal_open_params(&req,&msg_sz);
	if(!rem_req){
		return -1;
	}
	ret = write(serv_conn_fd,rem_req,msg_sz);
	if(ret < msg_sz )
		return -1;
	/* Wait for reply */
	rem_res = get_res_buff();
	ret = read(serv_conn_fd,rem_res,sizeof(rem_res_t));
	if(ret< 0 || ret < res_sz){
		return -1;
	}
	unmarshal_open_res_msg(rem_res,ret,&res);
	printf("Open:Remote Operation executed \n");
	printf("Open:Rem_op %d \n",res.rem_op);
	printf("Open:ret_val %d  \n",res.u.open_res.ret_val);
	printf("Open:err_no %d \n",res.u.open_res.err_no);
	
//	errno = EINVAL;
	return fd;
}
/*
int read(int fd,char *buff, int n){
	int res;
	read_fn orig_read;
	printf("This is passthrough Layer: read\n");
	orig_read = dlsym(RTLD_NEXT,"read");
	res = orig_read(fd,buff,n);
//	errno = EINVAL;
	return res;

}
int write(int fd,char *buff, int n){
	int res;
	write_fn orig_write;
	printf("This is passthrough Layer: write\n");
	orig_write = dlsym(RTLD_NEXT,"write");
	res = orig_write(fd,buff,n);
//	errno = EINVAL;
	return res;

}
int close(int fd){
	int res;
	close_fn orig_close;
	printf("This is passthough layer: close\n");
	orig_close = dlsym(RTLD_NEXT,"close");
	res = orig_close(fd);
	return res ; 
}

*/
