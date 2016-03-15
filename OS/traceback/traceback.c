#define __USE_GNU
#include<stdio.h>
#include<link.h>
#define _GNU_SOURCE
#include<dlfcn.h>
struct base_ptr{
	struct base_ptr *next;
	void *rtn_addr;
};
int val_add(int a,int b);
int val_div(int a,int b);
int val_sub(int a,int b);
int val_mod(int a,int b);
const char * get_func_name(void *fnp){
//	struct Dl_info info;
	Dl_info info;
//	void *info ;
	int ret;
	ret = dladdr(fnp,&info);
	if(ret < 0)
		return NULL;
//	return info.dli_fname;
}
void traceback(){
//	register struct base_ptr *b asm("ebp");
	struct base_ptr *base ;
	int n,i;
	void *fp[10];
	asm(
		"movl %%ebp,%0":"=r"(base)
	);
	while(base){
		printf("base %p rtn %p\n",base,base->rtn_addr);
		printf("Function name %s\n",get_func_name(base->rtn_addr));
		base = base->next;
	}
	//printf("base %p rtn %p\n",base,base->rtn_addr);
	n = backtrace(fp,10);
	for(i=0;i<n;i++)
		printf("stack pointer%p\n",fp[i]);

}

int val_div(int a,int b){
	int temp;
	temp = a/b;
	val_sub(a,b);
	return temp;
}
int val_sub(int a,int b){
	int temp;
	temp=a-b;
	val_mod(a,b);
	return temp;
}
int val_mod(int a,int b){
	int temp;
	temp=a%b;
	val_add(a,b);
	return temp;
}
int val_add(int a,int b){
	int temp;
	temp= a+b;
	traceback();
	return temp;
}
int main(){
	int a=20,b=5;
	val_div(a,b);
	return 0;
}
