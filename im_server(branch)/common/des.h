#ifndef _DES_H
#define _DES_H

void DesEnc(unsigned char *outdata, unsigned char *indata, int datalen, const unsigned char *key);
void DesDec(unsigned char *outdata, unsigned char *indata, int datalen, const unsigned char *key);


#endif

