#define _GNU_SOURCE
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
//#include<fcntl.h>
#include<errno.h>
#include<dlfcn.h>
#include<unistd.h>
#include"remoteop.h"
#include"remote_client_conn_setup.h"
#include"marshal_remote_call.h"
int open(char *fname, int flags, mode_t m ){
	int res,serv_conn_fd,msg_sz,ret;
	void *rem_req;
	open_fn orig_open;
	rem_req_t req;
	printf("This is passthrough Layer: open\n");
	orig_open = dlsym(RTLD_NEXT,"open");
	res = orig_open(fname,flags,m);
	serv_conn_fd = setup_client_connection();
	if(serv_conn_fd < 0 ){
		return -1;
	}
	printf("Connection Established\n");
	printf("File name %s \n",fname);
	printf("Flags %d \n",flags);
	printf("mode %d \n",m);
	remote_open_req(&req,fname,flags,m);
	rem_req = marshal_open_params(&req,&msg_sz);
	if(!rem_req){
		return -1;
	}
	ret = write(serv_conn_fd,rem_req,msg_sz);
	if(ret < msg_sz )
		return -1;	
//	errno = EINVAL;
	return res;
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
