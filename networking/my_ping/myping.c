#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<string.h>
#include<sys/time.h>
#include<time.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include"icmp_hdr.h"
static int loop = 1;
static int end = 5;
static int icmp_send_count = 4;
static int start_time = 0;
void sig_handler(int sig){
	switch(sig){
		case SIGALRM:
		{
			loop = 0;
			icmp_send_count--;
	//		end--;
		}
		break;
		case SIGINT:
		{
			loop = 0;
			end = 0;
		}
		break;
	}

}
char * get_host_ip_addr(struct hostent *h){
	int i;
	struct in_addr **addr_list;
	addr_list = (struct in_addr **) h->h_addr_list;
	for(i=0;addr_list[i];i++){
//		printf(" IP address %s\n",inet_ntoa(*addr_list[i]));
		return (char *)inet_ntoa(*addr_list[i]);
	}
	return "";
}
void print_host_entry(struct hostent *h){
	int i;
	struct in_addr **addr_list;
	addr_list = (struct in_addr **) h->h_addr_list;
	for(i=0;addr_list[i];i++){
		printf(" IP address %s\n",(char *)inet_ntoa(*addr_list[i]));
	}

}
int main( int argc, char *argv[]){
	struct sigaction sig_act;
	time_t start_time,end_time;
	int fd,i,err,seq_no=0;
	struct hostent *h;
	struct sockaddr_in dest={0,},src={0,};
	char src_ip[16],dst_ip[16];
	char buff[4096];
	bzero(&sig_act,sizeof(struct sigaction));
	sig_act.sa_handler = sig_handler;
	if(argc>1){
		h = gethostbyname(argv[1]);
		if(h == NULL)
		{
			perror("gethostbyname");
			exit(2);
		}
		strcpy(dst_ip,get_host_ip_addr(h));
		if(argv[2]){
			strcpy(src_ip,argv[2]);
		}
		else{
			strcpy(src_ip,"");
		}
	}
	if(sigaction(SIGALRM,&sig_act,NULL) < 0){
		perror("Signal alarm");
		exit(1);
	}
	if(sigaction(SIGINT,&sig_act,NULL) < 0){
		perror("Signal int");
		exit(1);
	}
	fd = socket(AF_INET, SOCK_RAW , IPPROTO_ICMP );
	if(fd == -1)
	{
		perror("socket");
		exit(1);
	}
	dest.sin_family = AF_INET;
	dest.sin_port = 0 ;
	inet_aton(dst_ip,&dest.sin_addr);
	err = connect(fd,(struct sockaddr *)&dest,sizeof(dest));
	if(err < 0){
		perror("connect");
		exit(2);
	}
	while(end){
		send_icmp_echo_req(fd,src_ip,dst_ip,seq_no+1);
		//printf("Sent ICMP req\n");
		alarm(1);
		err = recv_icmp_echo_reply(fd,seq_no+1);
		if(err){
			printf("Received reply\n");
			end--;
		}
		while(loop);
		loop = 1;
		seq_no++;
	}
	return 0;
}
