#include<stdio.h>
#include"ip_hdr.h"
#define INT_HI(a) (a >> 16)
#define INT_LO(a) (a & 0x0000FFFF )
/*
 * Calculates IPv4 checksum for packet
 * */
unsigned short ipv4_chksm(unsigned short *p, int len){
	unsigned int sum = 0, i, carry ;
	unsigned short val,chksm;
//	printf("chksm: len %d\n",len);
	for(i=0;i<len;i++)
	{
//		printf("Byte group %d %p\n",i,p);
		sum += *p;
//		printf(" chksm : sum(dec) %d data ",sum);
//		print_mem((char *)p,2);
//		print_mem((char *)&sum,4);
		if(sum & 0x80000000)
			sum = INT_LO(sum) + INT_HI(sum);
		//printf(" chksm : len ",print_mem((char *)p,2));
//		print_mem((char *)&sum,4);
		p++;
	}
	while(INT_HI(sum)){
	//	carry = INT_HI(sum);
		sum = INT_LO(sum) + INT_HI(sum);
//		printf("Removing carry\n");
//		print_mem((char *)&sum,4);

	}
	chksm = sum;
	chksm = ~chksm;
//	print_mem((char *)&chksm,2);
//	printf("\n");
	return (chksm);
	
}
/*
 * The function verifies ipv4 check sum.
 * If correct returns true else retrn false
 */
int verify_ipv4_chksm(unsigned short *p,int len){
	int res = 0;
	if(!ipv4_chksm(p,len))
		res = 1;
	return res;
}
void print_ip_hdr(IP_HDR *ip_pkt){
	unsigned char c = 0;
	printf("IP pkt %p IP pkt +1 %p\n",ip_pkt,(void *)ip_pkt+1);
	printf("Version and header lenght\n");
	print_mem(ip_pkt,sizeof(char));	
	printf(" ver(dec) %u hdr_len(dec) %u\n",ip_pkt->ver,ip_pkt->hdr_len);
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

	printf("verification %u\n",ipv4_chksm((short *)ip_pkt,ip_pkt->hdr_len*2));
	ip_pkt->hdr_chksm=0;
//	printf("Chksm via calculation %x\n",ipv4_chksm((short *)ip_pkt,ip_pkt->hdr_len*2));
	//print_mem
}
