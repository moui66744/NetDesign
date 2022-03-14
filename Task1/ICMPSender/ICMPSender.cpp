#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <time.h>
#include "pcap.h"
#include "packet_header.h"
#pragma comment (lib, "wpcap.lib")
using namespace std;

/*
使用winpcap发送ICMP echo requset报文
注意：
	计算机中一般采用小端模式存储数据
	网络中规定高位字节优先存储的网络字节顺序，即大端模式
	因此需要将大端模式转换成小端模式
	可以通过函数：
	htons(u_short)、htonl(u_long)
	实现大小端之间的转换
*/


//二进制反码校验和计算
//buf: 待计算序列
//size: 序列占用空间大小
unsigned short CheckSum(unsigned short* buf, int size) {
	unsigned int checksum = 0;
	unsigned short* t_buf = buf;
	int t_size = size;
	while (t_size > 1) {
		checksum += *t_buf++;
		t_size -= sizeof(unsigned short);
	}
	if (t_size > 0) checksum += *(unsigned char*)t_buf;
	checksum = (checksum >> 16) + (checksum & 0xFFFF); //将高16bit与低16bit相加
	checksum += (checksum >> 16); //将进位到高位的16bit与低16bit 再相加
	return (unsigned short)(~checksum);
}

//生成ICMP首部
void fill_icmp(icmp_header* ich) {
	unsigned char icmp_buf[100];
	//填充ich
	ich->type_code = htons(0x0800);		//request类型为0x0800
	ich->checksum = htons(0);			//校验和先置0
	ich->id = htons(0x0001);
	ich->serial = htons(0x0022);
	memcpy(icmp_buf, ich, sizeof(icmp_header));
	memcpy(icmp_buf + sizeof(icmp_header), icmp_data, 32);
	ich->checksum = CheckSum((unsigned short*)icmp_buf, sizeof(icmp_header) + 32);
}

//生成IP首部
void fill_ip(ip_header* ih) {
	unsigned char ip_header_buf[100];
	// 填充ih
	ih->version_head_len = 0x45;		//IPv4，首部长20B
	ih->tos = 0;						//服务类型
	ih->len = htons(60);				//IP报文总长
	ih->identification = htons(0x1f78);	//标识
	ih->flag_offset = htons(0x0000);	//标志_片偏移
	ih->ttl = 128;						//生存实践
	ih->proto = 1;						//封装ICMP，因此协议字段为1
	ih->checksum = htons(0);			//校验和先置0
	ih->saddr = SRC_IP;					//源IP
	ih->daddr = DST_IP;					//目的IP
	// 计算IP首部校验和
	memcpy(ip_header_buf, ih, sizeof(ip_header));
	ih->checksum = CheckSum((unsigned short*)ip_header_buf, sizeof(ip_header));
}

//生成以太网帧首部
void fill_ethernet(ethernet_header* eh) {
	// 填充eh
	eh->smac = SRC_MAC;					//源主机MAC
	eh->dmac = DST_MAC;					//网关路由MAC
	eh->type = htons(0x0800);//IP协议
}

void ping(pcap_t* adhandle) {
	// 发送 14B(MAC首尾) + 20B(IP首部) + 8B(ICMP首部) + 32B(数据) = 74B
	unsigned char buffer[100];
	icmp_header ich;
	ip_header ih;
	ethernet_header eh;
	// 生成报文
	fill_icmp(&ich);
	fill_ip(&ih);
	fill_ethernet(&eh);
	// 填充buffer
	memcpy(buffer, &eh, 14);
	memcpy(buffer + 14, &ih, sizeof(ih));
	memcpy(buffer + 14 + sizeof(ih), &ich, sizeof(ich));
	memcpy(buffer + 14 + sizeof(ih) + sizeof(ich), icmp_data, 32);
	// 发送
	pcap_sendpacket(adhandle, (const u_char*)buffer, 74);
}

int main() {
	pcap_if_t* alldevs, * d;
	int inum, i = 0;
	pcap_t* adhandle;
	char errbuf[PCAP_ERRBUF_SIZE];
	u_int netmask;
	struct bpf_program fcode;

	//获取设备列表
	pcap_findalldevs_ex((char*)PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf);
	//打开指定适配器
	adhandle = pcap_open(APATER, 65536, PCAP_OPENFLAG_PROMISCUOUS, 1000, NULL, errbuf);
	//发送一次ICMP request报文
	ping(adhandle);
	return 0;
}
