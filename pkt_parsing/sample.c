#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void print_basic_sizes(){
	printf("int	%d\n",sizeof(int));
	printf("short	%d\n",sizeof(short));
	printf("long	%d\n",sizeof(long));
	printf("longlong%d\n",sizeof(long long));
	printf("float	%d\n",sizeof(float));
	printf("double	%d\n",sizeof(double));
	printf("char	%d\n",sizeof(char));
	printf("void *	%d\n",sizeof(void *));
	return;
}
struct record{
	char	first_name[20];
	char	last_name[20];
	char	initials;
	long	acc_no;
	short	acc_type;
	double	acc_bal;
	short	ver:6;
	short	unused:10;
};
void print_address(struct record *r){
	printf("fname	%p\n",&(r->first_name));
	printf("lname	%p\n",&(r->last_name));
	printf("mname	%p\n",&(r->initials));
	printf("acc_no	%p\n",&(r->acc_no));
	printf("acc_typ %p\n",&(r->acc_type));
	printf("acc_bal	%p\n",&(r->acc_bal));
	//printf("ver	%p\n",&(r->ver));
	//printf("unused	%p\n",&(r->unused));
	return;
}
void print_record(struct record *r){
	printf("fname	%s\n",(r->first_name));
	printf("lname	%s\n",(r->last_name));
	printf("mname	%c\n",(r->initials));
	printf("acc_no	%ld\n",(r->acc_no));
	printf("acc_typ %d\n",(r->acc_type));
	printf("acc_bal	%g\n",(r->acc_bal));
	printf("ver	%d\n",(r->ver));
	printf("unused	%d\n",(r->unused));
	return;
}
void print_mem(char *start, int len){
	int i;
	for(i=0;i<len;i++){
		printf(" %.2x",(unsigned char)start[i]);
	}
	printf("\n");
	return;
}
void print_bits(char *t){
	int i;
	char c=*t;
	for(i=0;i<8;i++){
		if((c) & 0x01){
			printf("1");
		}
		else{
			printf("0");
		}
		c = c >> 1;
	}
	printf("\n");
}
typedef enum tcp_flags{
	fin = 0x0001,
	syc = 0x0002,
	rst = 0x0004,
	psh = 0x0008,
	ack = 0x0010,
	urg = 0x0020,
	ece = 0x0040,
	cwr = 0x0080,
	ns  = 0x0100,
	end = 0x0200
}TCP_FLAGS;
char * get_flag_str(TCP_FLAGS flag){
	switch(flag)
	{
		case fin:
		{
			return "FIN";	
		}
		break;
		case syc:
		{
			return "SYC";
		}
		break;
		case rst:
		{
			return "RST";
		}
		break;
		case psh:
		{
			return "PSH";
		}
		break;
		case ack:
		{
			return "ACK";
		}
		break;
		case urg:
		{
			return "URG";
		}
		break;
		case ece:
		{
			return "ECE";
		}
		break;
		case cwr:
		{
			return "CWR";
		}
		break;
		case ns:
		{
			return "NS";
		}
		break;
	}
	return "";
}
void print_tcp_flags(short flag){
	int i;
	TCP_FLAGS flags[]={fin,syc,rst,psh,ack,urg,ece,cwr,ns};
	for(i=0;i<sizeof(flags)/sizeof(flags[0]);i++){
		printf(" %s",get_flag_str( flag & flags[i]  ));
	}
	printf("\n");
}
void swap_bytes(unsigned short d){
	unsigned short t;
	printf("%u\n",d);
	t = d;
	d = d << 8;
        t = t >> 8;
	printf("%u\n",d|t);	
	return;
}
/*
int main(){
	struct	record *r,r2={0,};
	int	a=1;
	char	*b;
	void	*p;
//	print_basic_sizes();
//	printf("struct record %d\n",sizeof(struct record));
	print_address(&r2);
//	print_mem((char *)&a,sizeof(int));
	char data_record[]= "\x41\x64\x61\x6d\x00\x7f\x00\x00\x30"
		"\x57\xf1\x6c\xff\x7f\x00\x00\x00\x00"
		"\x00\x00\x41\x76\x69\x76\x00\x4b\xf1"
		"\x6c\xff\x7f\x00\x00\x85\xb8\x7e\xc5"
		"\xc3\x91\x98\xa0\x4a\x00\x00\x00\x00"
		"\x00\x00\x00\x32\x79\x06\x00\x00\x00"
		"\x00\x00\x0a\x00\x00\x00\x00\x00\x00"
		"\x00\x8d\x97\x6e\x12\x83\x00\xa0\x40\x08";
	r=(struct record *)data_record;
	r2.ver=1;
	r2.unused=0;
	print_record(&r2);
	p=( (void *) &(r2.acc_bal))+sizeof(double);
	printf("%p\n",p);
	print_mem(p,2);
	print_bits( ( char*) p);
	printf(" %u \n",htons(r2.ver));
//	print_tcp_flags(0x01A1);
//	swap_bytes(0x01);

	return 0;	
}
*/