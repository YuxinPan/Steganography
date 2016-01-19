#ifndef __Steganography__steg__
#define __Steganography__steg__

long Binary2Dec(int *p,int length);
void Dec2Binary(int e,long i,short data[50000000][8]);
long ComputeDataSize(int *p);
void Dec2Binaryarray(int array[],long input);
void DispErr(int num,char *name,int ks);
int output[50000000];
short data[50000000][8];
short data2[50000000][8];

#endif /* defined(__Steganography__steg__) */
