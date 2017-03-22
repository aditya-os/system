#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<assert.h>
#include"server.h"
#include"../remote_client/remoteop.h"
#include"unmarshall_req.h"
char * get_serv_IP_addr(){
        char *env="REMOP_SERVER_IP";
        char *serv_ip;
        serv_ip = getenv(env);
        printf("Remote_OP server IP : %s \n",serv_ip);
        if(!serv_ip){
                return NULL;
        }
        return serv_ip;
}
short get_serv_port(){
        char *env="REMOP_PORT";
        char *serv_port_s;
        short serv_port =0 ;
        serv_port_s = getenv(env);
        if(!serv_port_s)
                return 0;
        serv_port = atoi(serv_port_s);
        printf("Remote_OP server port : %u \n",serv_port);
        return serv_port;
}

void process_client_connection(int session_fd){
	rem_req_t req;
	int buf_sz,ret,rem_op,processed=0;
	void *buffer;
	buffer= malloc(sizeof(req));
	if(!buffer)
		return ;
	buf_sz = sizeof(req);
	ret = read(session_fd,buffer,buf_sz);
	if(ret<=0)
		return ;
	memcpy(&rem_op,buffer,sizeof(rem_op));
	processed +=sizeof(rem_op);
	rem_op = ntohl(rem_op);
	switch(rem_op){
		case REMOP_OPEN:
			printf("Client requested OPEN\n");
			unmarshall_req_msg(buffer,&req,REMOP_OPEN);
		break;
		case REMOP_READ:
			printf("Client requested READ\n");
		break;
		case REMOP_WRITE:
			printf("Client requested WRITE\n");
		break;
		case REMOP_CLOSE:
			printf("Client requested CLOSE\n");
		break; 
	}
	
}

int main(){
	int serv_fd, client_session,sa_sz;
	struct sockaddr_in cln;
	serv_fd = init_serv_conn();
	if(serv_fd < 0)
		return 0;
	sa_sz = sizeof(struct sockaddr);
	while(1){
		printf("Waiting for Client Connection\n");
		client_session = accept(serv_fd,(struct sockaddr *)&cln,&sa_sz);
		printf("Clinet connection received \n");
		process_client_connection(client_session);
		close(client_session);
	}
	return 0; 
}
int init_serv_conn(){
	int fd ,ret ;
	char *serv_ip; 
	struct sockaddr_in srv;
	fd = socket(AF_INET,SOCK_STREAM,0);
	if(fd < 0 ){
		return -1; 
	}
	memset(&srv,0,sizeof(srv));
	srv.sin_family = AF_INET;
	serv_ip = get_serv_IP_addr();
	assert(serv_ip!=NULL);
	if(serv_ip!=NULL)
		srv.sin_addr.s_addr = inet_addr(serv_ip);
	else	
		srv.sin_addr.s_addr = htonl(INADDR_ANY);
		
	srv.sin_port = htons(get_serv_port());
	ret = bind(fd,(struct sockaddr *)&srv,sizeof(struct sockaddr));

	if(ret < 0 )
		return -1;
	listen(fd,1);

	return fd;	
}
