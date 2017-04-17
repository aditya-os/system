#include"icmp_hdr.h"
#include"../ip_hdr.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<sys/time.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<assert.h>
#include"../misc.h"
char sendbuff[1500];
char recvbuff[9000];
void init_icmp_echo_req_pkt(ICMP_ECHO_REQ *icmp_pkt)
{
	icmp_pkt->base.type = ICMP_ECHO;
	icmp_pkt->base.code = 0 ;
	icmp_pkt->base.chksm = 0;
	icmp_pkt->id = 0;
	icmp_pkt->seq_no = 0; 
}
void print_icmp_hdr(void *b){
	printf("ICMP msg type & code\n");
	print_mem((void*)b,2);
	b+=2;
	printf("Hdr checksum\n");
	print_mem( (void*)b,2);
	b+=2;
	printf("Identifier & Seq No\n");
	print_mem((void *)b,4);

}
void compose_ping_ip_hdr(IP_HDR *ip_pkt,char *src_ip,char * dst_ip, int icmp_payload_len){
	unsigned int src,dst;
	ip_pkt->ver=4;
	ip_pkt->hdr_len =5;
	ip_pkt->ecn =0;
	ip_pkt->dscp =0;
	ip_pkt->tot_len = htons(IP_HDR_WO_OPT_LEN + ICMP_ECHO_HDR_LEN + icmp_payload_len);
        ip_pkt->id = rand();
	ip_pkt->flags = htons(0);
	ip_pkt->frg_offset = htons(0);
	ip_pkt->ttl = 100;
	ip_pkt->protocol = IP_PROTO_ICMP ;
	src = inet_addr(src_ip);
	memcpy(&(ip_pkt->src),&src,IP_ADDR_LEN);
	dst = inet_addr(dst_ip);
	memcpy(&(ip_pkt->dst),&dst,IP_ADDR_LEN);
	ip_pkt->hdr_chksm =0;
	ip_pkt->hdr_chksm = ipv4_chksm((unsigned short *)ip_pkt,ip_pkt->hdr_len*2); /* This does not require htons conversion */
	
	/*Verify the IP header is correct*/
	if(!verify_ipv4_chksm((unsigned short *)ip_pkt,ip_pkt->hdr_len*2)){
		assert(0);
	}
}
void compose_ping_icmp_hdr(ICMP_ECHO_REQ *icmp_pkt,short seq_no,int icmp_payload_len){
	struct timeval start;

	/* ID for ICMP echo req is process ID */
	icmp_pkt->id = htons(getpid());
	icmp_pkt->seq_no = htons(seq_no);

	/*Insert time in payload field, so we can calculate RTT */
	gettimeofday(&start,NULL);
	memcpy(&(icmp_pkt->payload),&start,sizeof(start));
	icmp_pkt->base.chksm = 0;
	icmp_pkt->base.chksm = ipv4_chksm((unsigned short *)icmp_pkt, (ICMP_ECHO_HDR_LEN + icmp_payload_len)/2);

	/* Verify if ICMP packet checksum is correct */
	if(!verify_ipv4_chksm((unsigned short *)icmp_pkt,(ICMP_ECHO_HDR_LEN+icmp_payload_len)/2)){
		assert(0);
	}
}
void send_icmp_echo_req(int fd, char *src_ip, char *dst_ip,int seqno){
	int len =0, err=0,on =1, icmp_pkt_len = 50;
	IP_HDR *ip_pkt;
	ICMP_ECHO_REQ *icmp_pkt;
	bzero(&sendbuff,sizeof(sendbuff));

	/* Initializing and componsing IP header for ping  */
	ip_pkt = (IP_HDR *)sendbuff;
	init_ipv4_hdr_wo_option(ip_pkt);
	compose_ping_ip_hdr(ip_pkt,src_ip,dst_ip,icmp_pkt_len);

	/* Initializing and composing ICMP header for ping*/
	icmp_pkt = (void *)ip_pkt + IP_HDR_WO_OPT_LEN; 
	init_icmp_echo_req_pkt(icmp_pkt);
	compose_ping_icmp_hdr(icmp_pkt,seqno,icmp_pkt_len);

	/* Writing the data present in buffer to socket*/
	len= IP_HDR_WO_OPT_LEN + ICMP_ECHO_HDR_LEN + icmp_pkt_len ;
	err = setsockopt(fd,IPPROTO_IP,IP_HDRINCL,&on,sizeof(on));
	if(err == -1){
		perror("setsockopt");
		exit(2);
	}
	err =0;
	err = write(fd,sendbuff,len);
	if(err == -1){
		if (EINTR == errno){
			return;
		}
		perror("write");
		exit(2);
	}
	
}

int recv_icmp_echo_reply(int fd,int seqno){
	int err=0;
	unsigned long long diff;
	struct timeval start_time,end_time;
	IP_HDR *ip_pkt;
	ICMP_ECHO_REQ *icmp_echo_reply, *orignal_req;

	/* Read socket for ICMP echo reply data */
	err = read(fd,recvbuff,sizeof(recvbuff));
	if(err < 0)
	{
		if( errno == EINTR)
		{
			return 0;
		}
		perror("read");
		exit(2);
	}

	/* Received data will contain IP packet followed by ICMP reply */
	ip_pkt = (IP_HDR *)recvbuff;
	icmp_echo_reply = (void *)ip_pkt + IP_HDR_WO_OPT_LEN ;
	
	/* Pull payload field from the ICMP packet, which will contain send time */
	memcpy(&start_time,icmp_echo_reply->payload,sizeof(start_time));

	/* Get current time, to calculate RTT */
	gettimeofday(&end_time,NULL);
	diff = get_time_milli_sec(&end_time) - get_time_milli_sec(&start_time); 
	printf("MYPING: sent and received  ICMP ECHO REQ/REPLY icmp_seq_no %d RTT %llu\n",ntohs(icmp_echo_reply->seq_no),diff);
		
	
	return 1;
}
