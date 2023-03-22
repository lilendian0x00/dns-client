#include <stdio.h>
#include <stdint.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "utils.h"
#include "dns.h"



void SEND_UDP_DNS(char* pkt, size_t len, struct sockaddr_in *addr) {
    int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    checkError(fd, "Socket didn't open!");

    printf("SENDING UDP DNS PACKET....\n");
    int send_res = sendto(fd, pkt, len, 0, (const struct sockaddr *)addr, sizeof(*addr));
    checkError(send_res, "Couldn't send the udp dns packet!");


    char recv_buffer[1024];
    memset(recv_buffer, '\x00', sizeof(recv_buffer));

    socklen_t addr_size = sizeof(*addr);
    ssize_t res_recv = recvfrom(fd, recv_buffer, 1024, 0, (struct sockaddr *)addr, &addr_size);
    close(fd);
    checkError(res_recv, "Couldn't recv the udp dns packet!");


    dns_header* header = (dns_header*)&recv_buffer;
    printf("ID: %d\n", ntohs(header->id));
    printf("ANSWERS: %d\n", ntohs(header->AnswerNumber));

    char* qname = (char*)&recv_buffer[sizeof(dns_header)];
    printf("Qname: %s\n", qname);

    question* ques = (question*)&recv_buffer[sizeof(dns_header)+strlen(qname)+1];
    printf("q-type: %d\n", ntohs(ques->qtype));
    printf("q-class: %d\n",  ntohs(ques->qclass));

    char *p = (char *)&recv_buffer[sizeof(dns_header) + strlen(qname) + 1 + sizeof(question)];
    char ip_address[5][INET_ADDRSTRLEN];

    for (int i=0; i < ntohs(header->AnswerNumber); i++) {
        p += 6; // name: c0 0c
        p += 4; // type: 00 01 & class: 00 01
        p += 2; // TTL:
        inet_ntop(AF_INET, p, ip_address[i], INET_ADDRSTRLEN);;
        printf("IP: %s\n", ip_address[i]);
        p += 4; // + current IP (4 * 8-bit = 32bit)
    }
    close(fd);
}

void SEND_TCP_DNS(char* pkt, size_t len, struct sockaddr_in *addr) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    checkError(fd, "Socket didn't open!");

    socklen_t addr_size = sizeof(*addr);
    int res = connect(fd, (const struct sockaddr *)addr, addr_size);
    checkError(res, "Couldn't connect!");

    // Calculate total length of DNS query
    uint16_t total_length = htons(len);

    char finalBuff[ len + sizeof(total_length)];
    memset(finalBuff, '\x00', len+sizeof(total_length));

    memcpy(finalBuff, &total_length, sizeof(total_length));
    memcpy(&finalBuff[sizeof(total_length)], pkt, len);

    printf("SENDING TCP DNS PACKET....\n");
    int res_send = send(fd, finalBuff, len + sizeof(total_length), 0);
    checkError(res_send, "Couldn't send the tcp dns packet!");

    char recv_buffer[1024];
    memset(recv_buffer, '\x00', sizeof(recv_buffer));

    ssize_t res_recv = recv(fd, recv_buffer, sizeof(recv_buffer), 0);
    close(fd);
    checkError(res_recv, "Couldn't recv the udp dns packet!");

    uint16_t* length = (uint16_t*)&recv_buffer;
    printf("LEN: %d\n", ntohs(*length));

    dns_header* header = (dns_header*)&recv_buffer[sizeof(uint16_t)];
    printf("ID: %d\n", ntohs(header->id));
    printf("ANSWERS: %d\n", ntohs(header->AnswerNumber));

    char* qname = (char*)&recv_buffer[sizeof(uint16_t) + sizeof(dns_header)];
    printf("Qname: %s\n", qname);

    question* ques = (question*)&recv_buffer[ sizeof(uint16_t) + sizeof(dns_header) + strlen(qname)+1 ];

    printf("q-type: %d\n", ntohs(ques->qtype));
    printf("q-class: %d\n",  ntohs(ques->qclass));

    char *p = (char *)&recv_buffer[ sizeof(uint16_t) + sizeof(dns_header) + strlen(qname) + 1 + sizeof(question)];
    char ip_address[5][INET_ADDRSTRLEN];

    for (int i=0; i < ntohs(header->AnswerNumber); i++) {
        p += 6; // name: c0 0c
        p += 4; // type: 00 01 & class: 00 01
        p += 2; // TTL:
        inet_ntop(AF_INET, p, ip_address[i], INET_ADDRSTRLEN);;
        printf("IP: %s\n", ip_address[i]);
        p += 4; // + current IP (4 * 8-bit = 32bit)
    }

}