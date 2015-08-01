#define MAC_ADDR_LEN		6
#define ETHERNET_FRAME_LEN	14
typedef struct mac_addr{
	char addr[MAC_ADDR_LEN];
}MAC_ADDR;
typedef struct ether_type{
	short type;
}ETHER_TYPE;
typedef struct ethernet_frame{
	MAC_ADDR dst;
	MAC_ADDR src;
	ETHER_TYPE protocol;
}ETHERNET_FRAME;
typedef enum ether_proto{
	ETHER_NONE	= 0x0000,
	ETHER_SIZE	= 0x05DC,
	ETHER_IP	= 0x0800,
	ETHER_ARP	= 0x0806,
	ETHER_MPLS_UC	= 0x8847,
	ETHER_MPLS_MC	= 0x8848
}ETHER_PROTO;
