#define IP_ADDR_LEN 4
#define IP_HDR_WO_OPT_LEN 20
typedef struct ip_addr{
	char addr[IP_ADDR_LEN];
}IP_ADDR;
typedef struct ip_hdr{
	char ver:4;
	char hdr_len:4;
	char dscp:6;
	char ecn:2;
	short tot_len;
	short id;
	short flags:3;
	short frg_offset:13;
	char ttl;
	char protocol;
	short hdr_chksm;
	IP_ADDR src;
	IP_ADDR dst;
	char option[];
}IP_HDR;
