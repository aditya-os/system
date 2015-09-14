#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<unistd.h>
#include<errno.h>

char buff[4096];
void server(int port){
	int sd,err,rd,cli_len =0, nbytes= 0 ;
	struct sockaddr_in server,rmt_client;
	bzero(&server,sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	sd = socket(AF_INET,SOCK_STREAM,0);
	if(sd < 0 ){
		perror("socket");
		exit(2);
	}
	err = bind(sd,(struct sockaddr *)&server,sizeof(server));
	if(err < 0 ){
		perror("bind");
		exit(2);
	}
	err = listen(sd,1);
	if(err < 0 ){
		perror("listen");
		exit(2);
	}
	
	while(1){
		rd = accept(sd,(struct sockaddr *)&rmt_client,&cli_len);
		if(rd < 0){
			if(errno != EINTR ){
				perror("accept");
				exit(2);
			}
			continue;
		}
		nbytes = read(rd,buff,sizeof(buff));
		if(nbytes < 0 ){
			perror("read");
			exit(0);
		}
		if(nbytes == 0 ){
			close(rd);
		}
		write((int)1,buff,nbytes);	
		close(rd);

	}

}
