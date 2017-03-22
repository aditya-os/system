#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include"remote_client_conn_setup.h"
char * get_RPC_serv_IP_addr(){
	char *env="REMOP_SERVER_IP";
	char *serv_ip;
	serv_ip = getenv(env);
	printf("Remote_OP server IP : %s \n",serv_ip);
	if(!serv_ip){
		return NULL;
	}
	return serv_ip;
}
short get_RPC_serv_port(){
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
/*

This function will set up connection with remote server 
offering remote operation to client. It will get server 
IP address and remote port no from env variable 

*/
int setup_client_connection(){
	char *serv_ip ;
	short port_no; 
	int sock_conn,ret;
	struct sockaddr_in srv;
	serv_ip = get_RPC_serv_IP_addr();
	port_no = get_RPC_serv_port(); 
	sock_conn = socket(AF_INET,SOCK_STREAM,0);
	if(sock_conn < 0 ){
		return -1;
	}
	memset(&srv,0,sizeof(srv));
	srv.sin_family = AF_INET;
	srv.sin_addr.s_addr = inet_addr(serv_ip);
	srv.sin_port = htons(port_no);
	ret = connect(sock_conn,(struct sockaddr *)&srv, sizeof(struct sockaddr));
	if(ret < 0 )
		return -1;
	return sock_conn;


}
