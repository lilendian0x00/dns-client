#ifndef DNS_TEST_UTILS_H
#define DNS_TEST_UTILS_H

void checkError(int i, char* msg);
void convertStringToDnsFormat(unsigned char *dns, unsigned char *host);

#endif
