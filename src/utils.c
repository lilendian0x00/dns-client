#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

void checkError(int i, char* msg) {
    if (i < 0) {
        fprintf(stderr, "%s\n", msg);
        exit(1);
    }
}


// Convert a domain name to DNS label format
void convertStringToDnsFormat(unsigned char *dns, unsigned char *host) {
    int lock = 0;
    strcat((char *)host,".");

    for(int i = 0; i < strlen((char *)host); i++) {
        if(host[i] == '.') {
            *dns++ = i - lock;
            for(; lock < i; lock++) {
                *dns++ = host[lock];
            }
            lock++;
        }
    }

    *dns++='\0';
}