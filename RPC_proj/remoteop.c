#define _GNU_SOURCE
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
//#include<fcntl.h>
#include<errno.h>
#include<dlfcn.h>
typedef int (*open_fn)(char *fname,int flags,mode_t m);
typedef int (*read_fn)(int fd,char *buff, int size);
typedef int (*write_fn)(int fd,char *buff, int size);
typedef int (*close_fn)(int fd);
int open(char *fname, int flags, mode_t m ){
	int res;
	open_fn orig_open;
	printf("This is passthrough Layer: open\n");
	orig_open = dlsym(RTLD_NEXT,"open");
	res = orig_open(fname,flags,m);
//	errno = EINVAL;
	return res;
}

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
