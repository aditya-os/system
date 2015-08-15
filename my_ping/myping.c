#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<string.h>
#include<sys/time.h>
#include<time.h>
#include<sys/socket.h>
#include<netinet/ip.h>
#include<netdb.h>
static int loop = 0;
static int end = 4;
static int icmp_send_count = 4;
static start_time = 0;
void sig_handler(int sig){
	switch(sig){
		case SIGALRM:
		{
			loop = 0;
			icmp_send_count--;
			end--;
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
	int fd,i;
	struct hostent *h;
	char src_ip[16],dst_ip[16];
	bzero(&sig_act,sizeof(struct sigaction));
	sig_act.sa_handler = sig_handler;
	if(argc>1){
		h = gethostbyname(argv[1]);
		if(h == NULL)
		{
			perror("gethostbyname");
			exit(2);
		}
//		print_host_entry(h);
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
	//gettimeofday(&start_time);
	time(&start_time);
	// Send first packet
	strcpy(dst_ip,get_host_ip_addr(h));
	send_icmp_echo_req(fd,src_ip,dst_ip);
	while(end){
		// read socket for response
		while(loop);
		printf("Turing was right\n");
		loop = 1;
		alarm(1);
	}
	//gettimeofday(&end_time);
	time(&end_time);
/*	
	printf(" Time required by program is %lld \n",end_time/1ll-start_time/1ll);
	printf(" end time %lld start time %lld\n",end_time/1ll,start_time/1ll);
*/
	return 0;
}
