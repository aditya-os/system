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
