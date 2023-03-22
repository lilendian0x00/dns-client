#include <stdio.h>
#include <stdint.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#include "utils.h"
#include "dns.h"

#define UDP 0
#define TCP 1

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "./%s <dns_server> <domain> <TCP, UDP>\n", argv[0]);
        exit(1);
    }
    char DNS_SERVER[15];
    uint16_t DNS_SERVER_PORT = 0;

    memset(DNS_SERVER, '\x00', sizeof(DNS_SERVER));

    char* p = strtok(argv[1], ":");
    strcpy(DNS_SERVER, p);
    p = strtok(NULL, ":");
    if (p) {
        DNS_SERVER_PORT = atoi(p);
        if (DNS_SERVER_PORT > 65535) {
            fprintf(stderr, "Entered DNS SERVER PORT must be: 0 < X < 65535\n");
            exit(1);
        }
    } else {
        fprintf(stderr, "Entered DNS SERVER PORT: [%s] is not a integer! format: <IP:PORT> Ex: 1.1.1.1:53\n", argv[2]);
        exit(1);
    }

    char* domain = argv[2];

    if (strcmp(argv[3], "UDP") !=0 && strcmp(argv[3], "TCP") != 0) {
        fprintf(stderr, "[%s] protocol is wrong! must be UDP or TCP\n", argv[3]);
        exit(1);
    }

    uint8_t protocol = 0;

    if (!strcmp(argv[3], "UDP")) {
        protocol = UDP;
    } else {
        protocol = TCP;
    }

    srand(time(NULL));

    char buffer[65535], *qname;
    memset(buffer, 0, sizeof(char));

    dns_header* header = (dns_header *)&buffer;

    int id = rand() % 65535;

    header->id = htons(id);
    header->QuestionNumber = htons(1);
    header->AnswerNumber = htons(0);
    header->AuthorityNumber = htons(0);
    header->AdditionalNumber = htons(0);

    qname = (char*)&buffer[sizeof(dns_header)];
    convertStringToDnsFormat(qname, domain);


    question* q = (question*)&buffer[sizeof(dns_header) + strlen(qname) + 1];

    q->qtype = htons(1);
    q->qclass = htons(1);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(DNS_SERVER);
    addr.sin_port = htons(DNS_SERVER_PORT);


    int len = sizeof(dns_header) + strlen(qname)+1 + sizeof(question);

    switch (protocol) {
        case UDP:
            SEND_UDP_DNS(buffer, len, &addr);
            break;
        case TCP:
            SEND_TCP_DNS(buffer, len, &addr);
            break;
    }
}