#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<string.h>
#include<sys/time.h>
#include<time.h>
static int loop = 0;
static int end = 4;
static start_time = 0;
void sig_handler(int sig){
	loop = 0;
	switch(sig){
		case SIGALRM :
		{
			loop = 0;
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
int main(){
	struct sigaction sig_act;
	time_t start_time,end_time;
	int fd;
	bzero(&sig_act,sizeof(struct sigaction));
	sig_act.sa_handler = sig_handler;
	if(sigaction(SIGALRM,&sig_act,NULL) < 0){
		perror("Signal alarm");
		exit(1);
	}
	if(sigaction(SIGINT,&sig_act,NULL) < 0){
		perror("Signal int");
		exit(1);
	}
	//gettimeofday(&start_time);
	time(&start_time);
	while(end){
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
