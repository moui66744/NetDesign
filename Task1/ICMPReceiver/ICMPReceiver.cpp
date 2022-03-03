#include <iostream>
#include "pcap.h"
#include "packet_header.h"
#pragma comment (lib, "wpcap.lib")
#pragma warning (disable:4996)
using namespace std;

// ץ���ص���������ʾ������Ϣ
void packet_handle(u_char* param, const struct pcap_pkthdr* header, const u_char* pkt_data) {
	struct tm ltime;
	char timestr[16], file_name[20];
	time_t local_tv_sec;
	ethernet_header* eh;
	ip_header* ih;
	icmp_header* ich;
	char* icmp_data;

	local_tv_sec = header->ts.tv_sec;
	localtime_s(&ltime, &local_tv_sec);
	strftime(timestr, sizeof(timestr), "%H:%M:%S", &ltime);
	strftime(file_name, sizeof(file_name), "%HH%MM%SS", &ltime);

	// ��pkt_data�н�ȡ����
	eh = (ethernet_header*)pkt_data;
	ih = (ip_header*)(pkt_data + 14);
	ich = (icmp_header*)(pkt_data + 14 + sizeof(ip_header));
	icmp_data = (char*)(pkt_data + 14 + sizeof(ip_header) + sizeof(icmp_header));

	FILE* fp = fopen(file_name, "a+");
	fprintf(fp, "==================================================\n");
	fprintf(fp, "time: %s.%.6d len: %d \n", timestr, header->ts.tv_usec, header->len);
	// ��̫��֡ͷ��Ϣ
	fprintf(fp, "---------------------------------\n");
	fprintf(fp, "Ethernet Header:\n");
	fprintf(fp, "Destination: %02x:%02x:%02x:%02x:%02x:%02x\n", eh->dmac.byte1, eh->dmac.byte2, eh->dmac.byte3, eh->dmac.byte4, eh->dmac.byte5, eh->dmac.byte6);
	fprintf(fp, "Source: %02x:%02x:%02x:%02x:%02x:%02x\n", eh->smac.byte1, eh->smac.byte2, eh->smac.byte3, eh->smac.byte4, eh->smac.byte5, eh->smac.byte6);
	fprintf(fp, "Type: %s(0x%04x)\n", (htons(eh->type) == 0x0800 ? "IPv4" : "null"), htons(eh->type));
	// IP�ײ���Ϣ
	fprintf(fp, "---------------------------------\n");
	fprintf(fp, "IP Header:\n");
	fprintf(fp, "Version: %d\n", (ih->version_head_len & 0xf0) >> 4);
	fprintf(fp, "Header Length: %d bytes (%d)\n", (ih->version_head_len & 0xf) * 4, ih->version_head_len & 0xf);
	fprintf(fp, "Totoal Length: %d\n", htons(ih->len));
	fprintf(fp, "Identification: 0x%x (%d)\n", htons(ih->identification), htons(ih->identification));
	fprintf(fp, "Flags: 0x%02x\n", htons(ih->flag_offset) & 0xe000);
	fprintf(fp, "Fragment Offset: %d\n", htons(ih->flag_offset) & 0x1fff);
	fprintf(fp, "Time to Live: %d\n", ih->ttl);
	fprintf(fp, "Protocol: %s(%d)\n", (ih->proto == 1 ? "ICMP" : "null"), ih->proto);
	fprintf(fp, "Header Checksum: 0x%04x\n", htons(ih->checksum));
	fprintf(fp, "Source Address: %d.%d.%d.%d\n", ih->saddr.byte1, ih->saddr.byte2, ih->saddr.byte3, ih->saddr.byte4);
	fprintf(fp, "Destinction Address: %d.%d.%d.%d\n", ih->daddr.byte1, ih->daddr.byte2, ih->daddr.byte3, ih->daddr.byte4);
	// ICMP�ײ���Ϣ
	fprintf(fp, "---------------------------------\n");
	fprintf(fp, "ICMP Header:\n");
	fprintf(fp, "Type: %d (%s)\n", htons(ich->type_code) & 0xf0, ((htons(ich->type_code) & 0xf0) == 0 ? "Echo (ping) reply" : "null"));
	fprintf(fp, "Code: %d\n", htons(ich->type_code) & 0xf);
	fprintf(fp, "Checksum: 0x%04x\n", htons(ich->checksum));
	// ICMP������Ϣ
	fprintf(fp, "---------------------------------\n");
	fprintf(fp, "Data:\n");
	fprintf(fp, "%s\n", icmp_data);
	fprintf(fp, "==================================================\n\n");
	fclose(fp);
	//exit(1);
}

int main() {
	pcap_if_t* alldevs, * d;
	int inum, i = 0;
	pcap_t* adhandle;
	char errbuf[PCAP_ERRBUF_SIZE];
	u_int netmask;
	char packet_filter[] = "ip and icmp and dst host 192.168.1.106";//��������
	struct bpf_program fcode;

	////��ȡ�豸�б�
	//pcap_findalldevs_ex((char*)PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf);
	//for (d = alldevs; d; d = d->next) {
	//	printf("%d. %s", ++i, d->name);
	//	if (d->description) printf("%s", d->description);
	//	puts("");
	//}
	////�����豸���
	//printf("\nEnter the interface number (1-%d):", i);
	//scanf_s("%d", &inum);
	////��ָ��������
	//for (d = alldevs, i = 0; i < inum - 1; d = d->next, i++);

	//��������
	adhandle = pcap_open((char*)"//Device//NPF_{ADDAD27F-0AFB-4AE1-B166-5C704DF51D50}", 65536, PCAP_OPENFLAG_PROMISCUOUS, 1000, NULL, errbuf);
	netmask = 0xffffff;
	//���ù�����
	if (pcap_compile(adhandle, &fcode, packet_filter, 1, netmask) < 0) {
		printf("\nUnable to compile the packet filter\n");
		return 0;
	}
	if (pcap_setfilter(adhandle, &fcode) < 0) {
		printf("\nError setting the filter\n");
		return 0;
	}
	//printf("\nListening on %s...\n", d->description);
	//pcap_freealldevs(alldevs);
	//����
	pcap_loop(adhandle, 0, packet_handle, NULL);
	return 0;
}
