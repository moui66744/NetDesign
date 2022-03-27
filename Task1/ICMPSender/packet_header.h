#define SRC_IP { 192,168,165,164 }	//源主机IP
#define DST_IP { 192,168,165,176 }		//目的主机IP
#define SRC_MAC { 0x80, 0x30, 0x49, 0xAD, 0x49, 0xB5 }	//源主机MAC
#define DST_MAC { 0xd4, 0x25, 0x8b, 0x9b, 0x16, 0x7b }
#define APATER "//Device//NPF_{ADDAD27F-0AFB-4AE1-B166-5C704DF51D50}"	// 源主机适配器名称

//ICMP报文数据
unsigned char* icmp_data = (unsigned char*)"abcdefghijklmnopqrstuvwabcdefghi";

//IP地址
typedef struct ip_address {
	unsigned char byte1, byte2, byte3, byte4;
} ip_address;

//IP报文首部
typedef struct ip_header {
	unsigned int version_head_len : 8;	//版本_首部长(4B为单位)
	unsigned int tos : 8;				//服务
	unsigned int len : 16;				//IP报文总长(1B为单位)
	unsigned int identification : 16;	//标识
	unsigned int flag_offset : 16;		//标志_片偏移
	unsigned int ttl : 8;				//生存时间
	unsigned int proto : 8;				//协议，ICMP为1
	unsigned int checksum : 16;			//首部校验和
	ip_address saddr;					//源IP
	ip_address daddr;					//目的IP
} ip_header;

//ICMP报文首部
typedef struct icmp_header {
	unsigned int type_code : 16;		//类型，request为8
	unsigned int checksum : 16;			//首部校验和
	unsigned int id : 16;				//
	unsigned int serial : 16;			//
} icmp_header;

//MAC地址
typedef struct mac_addr {
	unsigned char byte1, byte2, byte3, byte4, byte5, byte6;
} mac_addr;

//以太网帧头
typedef struct ethernet_header {
	mac_addr dmac;						//目的MAC
	mac_addr smac;						//源MAC
	unsigned int type : 16;				//封装协议类型，IP为0x0800
} ethernet_header;
