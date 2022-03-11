#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <time.h>
#include "pcap.h"
#include "packet_header.h"
#pragma comment (lib, "wpcap.lib")
using namespace std;

/*
ʹ��winpcap����ICMP echo requset����
ע�⣺
	�������һ�����С��ģʽ�洢����
	������ʹ�ô��ģʽ��������
	�����Ҫ�����ģʽת����С��ģʽ
	����ͨ��������
	htons(u_short)��htonl(u_long)
	ʵ�ִ�С��֮���ת��
*/


//�����Ʒ���У��ͼ���
//buf: ����������
//size: ����ռ�ÿռ��С
unsigned short CheckSum(unsigned short* buf, int size) {
	unsigned int checksum = 0;
	unsigned short* t_buf = buf;
	int t_size = size;
	while (t_size > 1) {
		checksum += *t_buf++;
		t_size -= sizeof(unsigned short);
	}
	if (t_size > 0) checksum += *(unsigned char*)t_buf;
	checksum = (checksum >> 16) + (checksum & 0xFFFF); //����16bit���16bit���
	checksum += (checksum >> 16); //����λ����λ��16bit���16bit �����
	return (unsigned short)(~checksum);
}

//����ICMP�ײ�
void fill_icmp(icmp_header* ich) {
	unsigned char icmp_buf[100];
	//���ich
	ich->type_code = htons(0x0800);		//request����Ϊ0x0800
	ich->checksum = htons(0);			//У�������0
	ich->id = htons(0x0001);
	ich->serial = htons(0x0022);
	memcpy(icmp_buf, ich, sizeof(icmp_header));
	memcpy(icmp_buf + sizeof(icmp_header), icmp_data, 32);
	ich->checksum = CheckSum((unsigned short*)icmp_buf, sizeof(icmp_header) + 32);
}

//����IP�ײ�
void fill_ip(ip_header* ih) {
	unsigned char ip_header_buf[100];
	// ���ih
	ih->version_head_len = 0x45;		//IPv4���ײ���20B
	ih->tos = 0;						//��������
	ih->len = htons(60);				//IP�����ܳ�
	ih->identification = htons(0x1f78);	//��ʶ
	ih->flag_offset = htons(0x0000);	//��־_Ƭƫ��
	ih->ttl = 128;						//����ʵ��
	ih->proto = 1;						//��װICMP�����Э���ֶ�Ϊ1
	ih->checksum = htons(0);			//У�������0
	ih->saddr = SRC_IP;					//ԴIP
	ih->daddr = DST_IP;					//Ŀ��IP
	// ����IP�ײ�У���
	memcpy(ip_header_buf, ih, sizeof(ip_header));
	ih->checksum = CheckSum((unsigned short*)ip_header_buf, sizeof(ip_header));
}

//������̫��֡�ײ�
void fill_ethernet(ethernet_header* eh) {
	// ���eh
	eh->smac = SRC_MAC;					//Դ����MAC
	eh->dmac = DST_MAC;					//����·��MAC
	eh->type = htons(0x0800);//IPЭ��
}

void ping(pcap_t* adhandle) {
	// ���� 14B(MAC��β) + 20B(IP�ײ�) + 8B(ICMP�ײ�) + 32B(����) = 74B
	unsigned char buffer[100];
	icmp_header ich;
	ip_header ih;
	ethernet_header eh;
	// ���ɱ���
	fill_icmp(&ich);
	fill_ip(&ih);
	fill_ethernet(&eh);
	// ���buffer
	memcpy(buffer, &eh, 14);
	memcpy(buffer + 14, &ih, sizeof(ih));
	memcpy(buffer + 14 + sizeof(ih), &ich, sizeof(ich));
	memcpy(buffer + 14 + sizeof(ih) + sizeof(ich), icmp_data, 32);
	// ����
	pcap_sendpacket(adhandle, (const u_char*)buffer, 74);
}

int main() {
	pcap_if_t* alldevs, * d;
	int inum, i = 0;
	pcap_t* adhandle;
	char errbuf[PCAP_ERRBUF_SIZE];
	u_int netmask;
	struct bpf_program fcode;

	//��ȡ�豸�б�
	pcap_findalldevs_ex((char*)PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf);
	//��ָ��������
	adhandle = pcap_open(APATER, 65536, PCAP_OPENFLAG_PROMISCUOUS, 1000, NULL, errbuf);
	//����һ��ICMP request����
	ping(adhandle);
	return 0;
}
