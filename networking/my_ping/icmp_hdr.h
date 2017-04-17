#ifndef ICMP_HDR_H
#define ICMP_HDR_H

#include"../ip_hdr.h"

#define ICMP_ECHO 0x08;
#define ICMP_ECHO_HDR_LEN 8
typedef struct icmp_hdr{
	char type;
	char code;
	short chksm;
	char rest[];
}ICMP_HDR;
typedef struct icmp_echo_req{
	ICMP_HDR base;
	short id;
	short seq_no;
	char payload[];
}ICMP_ECHO_REQ;
typedef struct icmp_echo_reply{
	ICMP_HDR base;
	short id;
	short seq_no;
	char payload[];
}ICMP_ECHO_REPLY;
void init_icmp_echo_req_pkt(ICMP_ECHO_REQ *icmp_pkt);
void print_icmp_hdr(void *b);
void compose_ping_ip_hdr(IP_HDR *ip_pkt,char *src_ip,char * dst_ip, int icmp_payload_len);
void compose_ping_icmp_hdr(ICMP_ECHO_REQ *icmp_pkt,short seq_no,int icmp_payload_len);
void send_icmp_echo_req(int fd, char *src_ip, char *dst_ip,int seqno);
int recv_icmp_echo_reply(int fd,int seqno);


#endif 
