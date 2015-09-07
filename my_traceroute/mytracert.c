#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<string.h>
#include<sys/time.h>
#include<time.h>
#include<sys/socket.h>
#include<netinet/ip.h>
#include<netdb.h>
#include<unistd.h>
static int loop = 1;
static int end = 5;
static int icmp_send_count = 4;
static start_time = 0;
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
int init_raw_icmp_socket(){
	int fd;
	 
	fd = socket(AF_INET, SOCK_RAW , IPPROTO_ICMP );
	if(fd == -1)
	{
		perror("socket");
		return -1;
	}
	return fd;
}
/* This function calls connect on raw socket, so we can call write function */
int connect_raw_icmp_socket(int fd,char *dst_ip){
	int err;	
	struct sockaddr_in dest={0,},src={0,};
	dest.sin_family = AF_INET;
	dest.sin_port = 0 ;
//	dest.sin_addr.s_addr = INADDR_ANY;
	inet_aton(dst_ip,&dest.sin_addr);
	err = connect(fd,(struct sockaddr *)&dest,sizeof(dest));
	if(err < 0){
		perror("connect");
		return 0;
	}
	return 1;
}
int main( int argc, char *argv[]){
	struct sigaction sig_act;
	time_t start_time,end_time;
	int sndfd,i,err=0,seq_no=0, ttl= 0,on =1,rcvfd;
	struct hostent *h;
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

	/* set up socket to send ICMP message */
	sndfd = init_raw_icmp_socket();
	if(sndfd<0){
		exit(2);
	}
	err = connect_raw_icmp_socket(sndfd,dst_ip);
	if(err < 0 ){
		exit(2);
	}
        err = setsockopt(sndfd,IPPROTO_IP,IP_HDRINCL,&on,sizeof(on));
	if(err == -1){
	        perror("setsockopt,IP_HDRINCL,sndfd");
	        exit(2);
	}

	/* Set up socket to receive ICMP message */
	rcvfd = init_raw_icmp_socket();
	if(rcvfd < 0 ){
		exit(2);
	}
        err = setsockopt(rcvfd,IPPROTO_IP,IP_HDRINCL,&on,sizeof(on));
	if(err == -1){
	        perror("setsockopt,IP_HDRINCL,rcvfd");
	        exit(2);
	}
	/* Start main loop of sending ICMP echo req  with TTL one and further  */
	while(end){
		send_icmp_echo_req(sndfd,src_ip,dst_ip,seq_no+1,ttl+1);
		alarm(1);
		err = recv_icmp_echo_reply(rcvfd,seq_no+1);
		if(err){
			printf("Destination reached\n");
			return 0;
		}
		//while(loop);
		//loop = 1;
		seq_no++;
		ttl++;
	}
	return 0;
}
