#ifndef DNS_TEST_DNS_H
#define DNS_TEST_DNS_H

#include <stdint.h>

typedef struct __attribute__((__packed__)) {
    uint8_t qr: 1;
    uint8_t opcode: 4;
    uint8_t aa: 1;
    uint8_t tc: 1;
    uint8_t rd: 1;
    uint8_t ra: 1;
    uint8_t zero: 3;
    uint8_t rCode: 4;
} dns_flags;

typedef struct __attribute__((__packed__)) {
    uint16_t id;
    dns_flags flags;
    uint16_t QuestionNumber;
    uint16_t AnswerNumber;
    uint16_t AuthorityNumber;
    uint16_t AdditionalNumber;
} dns_header;

// DNS query structure
typedef struct __attribute__((__packed__)) {
    uint16_t qtype;
    uint16_t qclass;
} question;

void SEND_UDP_DNS(char* pkt, size_t len, struct sockaddr_in *addr);
void SEND_TCP_DNS(char* pkt, size_t len, struct sockaddr_in *addr);

#endif
