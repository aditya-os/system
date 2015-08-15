#include"icmp_hdr.h"
#include"../pkt_parsing/ip_hdr.h"
#include<stdio.h>
#include<string.h>
char sendbuff[1500];
char recvbuff[1500];
void init_icmp_echo_req_pkt(ICMP_ECHO_REQ *icmp_pkt)
{
	icmp_pkt->base.type = ICMP_ECHO;
	icmp_pkt->base.code = 0 ;
	icmp_pkt->base.chksm = 0;
	icmp_pkt->id = 0;
	icmp_pkt->seq_no = 0; 
}
void send_icmp_echo_req(int fd, char *src_ip, char *dst_ip){
	IP_HDR *ip_pkt;
	bzero(&sendbuff,sizeof(sendbuff));
	ip_pkt = (IP_HDR *)sendbuff;
	init_ipv4_hdr_wo_option(ip_pkt);
	
//	printf(" Src IP %s Dst IP %s",src_ip,dst_ip);
//	fill ip packet
//	fill ICMP echo packet
	
}
