#define SRC_IP { 192,168,1,106 }	//Դ����IP
#define DST_IP { 192,168,1,1 }		//Ŀ������IP
#define SRC_MAC { 0x80, 0x30, 0x49, 0xAD, 0x49, 0xB5 }	//Դ����MAC
#define DST_MAC { 0xf4, 0x83, 0xcd, 0x3f, 0x4e, 0xb8 }	//����·��MAC
#define APATER "//Device//NPF_{ADDAD27F-0AFB-4AE1-B166-5C704DF51D50}"	// Դ��������������

//ICMP��������
unsigned char* icmp_data = (unsigned char*)"abcdefghijklmnopqrstuvwabcdefghi";

//IP��ַ
typedef struct ip_address {
	unsigned char byte1, byte2, byte3, byte4;
} ip_address;

//IP�����ײ�
typedef struct ip_header {
	unsigned int version_head_len : 8;	//�汾_�ײ���(4BΪ��λ)
	unsigned int tos : 8;				//����
	unsigned int len : 16;				//IP�����ܳ�(1BΪ��λ)
	unsigned int identification : 16;	//��ʶ
	unsigned int flag_offset : 16;		//��־_Ƭƫ��
	unsigned int ttl : 8;				//����ʱ��
	unsigned int proto : 8;				//Э�飬ICMPΪ1
	unsigned int checksum : 16;			//�ײ�У���
	ip_address saddr;					//ԴIP
	ip_address daddr;					//Ŀ��IP
} ip_header;

//ICMP�����ײ�
typedef struct icmp_header {
	unsigned int type_code : 16;		//���ͣ�requestΪ8
	unsigned int checksum : 16;			//�ײ�У���
	unsigned int id : 16;				//
	unsigned int serial : 16;			//
} icmp_header;

//MAC��ַ
typedef struct mac_addr {
	unsigned char byte1, byte2, byte3, byte4, byte5, byte6;
} mac_addr;

//��̫��֡ͷ
typedef struct ethernet_header {
	mac_addr dmac;						//Ŀ��MAC
	mac_addr smac;						//ԴMAC
	unsigned int type : 16;				//��װЭ�����ͣ�IPΪ0x0800
} ethernet_header;