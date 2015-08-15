#define IP_ADDR_LEN 4
#define IP_HDR_WO_OPT_LEN 20
typedef struct ip_addr{
	char addr[IP_ADDR_LEN];
}IP_ADDR;
typedef struct ip_hdr{
	char hdr_len:4;
	char ver:4;
	char ecn:2;
	char dscp:6;
	short tot_len;
	short id;
	short frg_offset:13;
	short flags:3;
	char ttl;
	char protocol;
	short hdr_chksm;
	IP_ADDR src;
	IP_ADDR dst;
	char option[];
}IP_HDR;
