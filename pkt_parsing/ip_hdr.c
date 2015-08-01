#include<stdio.h>
#include"ip_hdr.h"
void print_ip_hdr(IP_HDR *ip_pkt){
	unsigned char c = 0;
	printf("IP pkt %p IP pkt +1 %p\n",ip_pkt,(void *)ip_pkt+1);
	printf("Version and header lenght\n");
	print_mem(ip_pkt,sizeof(char));	
	printf("ToS and ECN\n");
	print_mem((void *)ip_pkt+sizeof(char),sizeof(char));
	printf("  TOS %u ECN %u\n",ip_pkt->dscp,ip_pkt->ecn);
	printf("Total lenght\n");
	print_mem(&(ip_pkt->tot_len),sizeof(short));
	printf(" Integer Len  %u\n",ntohs(ip_pkt->tot_len));
	printf("Packet Id \n");
	print_mem(&(ip_pkt->id),sizeof(short));
	printf(" Packet ID(dec)  %u\n",ntohs(ip_pkt->id));
	printf("Flag and fragment\n");
	print_mem((void *)&(ip_pkt->id)+sizeof(short),sizeof(short));
	printf("TTL and Protocol\n");
	print_mem(&(ip_pkt->ttl),sizeof(short));
	printf(" TTL(dec) %u Protocol(dec) %u\n",ip_pkt->ttl,ip_pkt->protocol);
	printf("Header Checksum\n");
	print_mem(&(ip_pkt->hdr_chksm),sizeof(short));
	printf(" Header Checksum(dec) %u\n",ip_pkt->hdr_chksm);
	printf("Source IP\n");
	print_mem(&(ip_pkt->src.addr),4);
	printf("Destination IP\n");
	print_mem(&(ip_pkt->dst.addr),4);
	
}
