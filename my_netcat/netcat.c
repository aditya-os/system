#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>

void print_host_entry(struct hostent *h){
	int i;
	struct in_addr **addr_list;
	addr_list = (struct in_addr **) h->h_addr_list;
	for(i=0;addr_list[i];i++){
		printf(" IP address %s\n",(char *)inet_ntoa(*addr_list[i]));
	}

}

int main(int argc, char **argv){
	struct hostent *he;
	struct in_addr *addr;
	if(argc < 1){
		exit(2);
	}	
	he = gethostbyname(argv[1]);
//	print_host_entry(he);
	server(6767);
	return 0;
}
