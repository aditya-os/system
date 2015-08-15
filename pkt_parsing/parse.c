#include<stdio.h>
#include<pcap.h>
#include"parser.h"
#include"ip_hdr.h"

void print_struct_sizes(){
	printf("Sizeof MAC_ADDR %d\n",sizeof(MAC_ADDR));
	printf("Sizeof ETHER_TYPE %d\n",sizeof(ETHER_TYPE));
	printf("Sizeof ETHER_FRAME %d\n",sizeof(ETHERNET_FRAME));
}
/* The input should be in host format 
 * i.e ntohs conversion should be aleady done 
 */
ETHER_PROTO get_ether_proto(unsigned short type){
	
	if ( type <= ETHER_SIZE )
		return ETHER_SIZE;
	switch (type){
		case 0x0800:
			return ETHER_IP;
		case 0x0806:
			return ETHER_ARP;
		case 0x8847:
			return ETHER_MPLS_UC;
		case 0x8848:
			return ETHER_MPLS_MC;

	}
	return ETHER_NONE;
};
void print_ethernet_frame(ETHERNET_FRAME *f){
	//ETHER_PROTO is_ip = ETHER_NONE;
	printf(" DST ADDR\n   ");
	print_mem(&(f->dst),sizeof(MAC_ADDR));
	printf(" SRC ADDR\n   ");
	print_mem(&(f->src),sizeof(MAC_ADDR));
	printf(" ETHER_TYPE\n   ");
	print_mem(&(f->protocol.type),sizeof(ETHER_TYPE));
	//is_ip = get_ether_proto(ntohs(f->protocol.type));
}
int main()
{
	pcap_t *handle;
	char *pkt;
	struct pcap_pkthdr header;
	char errbuf[PCAP_ERRBUF_SIZE];
	ETHERNET_FRAME *frame;
	ETHER_PROTO is_ip = ETHER_NONE;
	IP_HDR *ip_hdr;
	print_struct_sizes();
	handle = pcap_open_offline("sample.pcap",errbuf);
	if(handle == NULL)
	{
		printf("Error opening file/dev");
		return 1;
	}
	printf("header %p\n",&header);
	while(pkt=(char *)pcap_next(handle,&header))
	{
		printf("Packet %p %d\n",pkt,header.len);
		frame=(ETHERNET_FRAME *)pkt;
//		print_ethernet_frame(frame);
		is_ip = get_ether_proto(ntohs(frame->protocol.type));
		if ( is_ip == ETHER_IP )
		{
			ip_hdr = (IP_HDR *) (pkt + ETHERNET_FRAME_LEN) ;
			printf("IP hdr %p\n",ip_hdr);
			print_ip_hdr(ip_hdr);
		}
		
	}
	pcap_close(handle);
	return 0;
}
