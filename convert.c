#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "steg.h"
#define DATAFILESIZE 32



/* The function is used to compute the size of the data file. The input is a pointer to the array of size and the output is the size of the data file in integer. */

long ComputeDataSize(int *p){
    int i;
    for (i=0;i<DATAFILESIZE;i++){
        if ((p[i]%2)==1){p[i]=1;}else{p[i]=0;}
    }
    
    return Binary2Dec(&p[0],DATAFILESIZE);
}



/* The function is used to convert binary to decimal. The inputs are a pointer to the array of binary and the length of the binary number, the output is the decimal number in integer. */

long Binary2Dec(int *p,int length){
    long sum=0,i;
    for (i=0;i<length;i++){
        sum=sum+p[i]*pow(2,(length-i-1));
    }
    return sum;
}



/* The function is used to convert decimal in the data[][] to binary. The input is data[][] and two integers (e for the decimal number and i for the current position). Since the value has been stored in the array, no output is needed. */

void Dec2Binary(int e,long i,short data[50000000][8]){
    
    if (e>127) {data[i-DATAFILESIZE-1][0]=1;e=e-128;}   else data[i-DATAFILESIZE-1][0]=0;
    if (e>63)  {data[i-DATAFILESIZE-1][1]=1;e=e-64;}    else data[i-DATAFILESIZE-1][1]=0;
    if (e>31)  {data[i-DATAFILESIZE-1][2]=1;e=e-32;}    else data[i-DATAFILESIZE-1][2]=0;
    if (e>15)  {data[i-DATAFILESIZE-1][3]=1;e=e-16;}    else data[i-DATAFILESIZE-1][3]=0;
    if (e>7)   {data[i-DATAFILESIZE-1][4]=1;e=e-8;}     else data[i-DATAFILESIZE-1][4]=0;
    if (e>3)   {data[i-DATAFILESIZE-1][5]=1;e=e-4;}     else data[i-DATAFILESIZE-1][5]=0;
    if (e>1)   {data[i-DATAFILESIZE-1][6]=1;e=e-2;}     else data[i-DATAFILESIZE-1][6]=0;
    if ((e%2)!=0)   data[i-DATAFILESIZE-1][7]=1;        else data[i-DATAFILESIZE-1][7]=0;
    
}



/* The function is used to convert decimal to binary. The input is an array and an integer for the decimal number. Since the value has been stored in the array, no output is needed. */

void Dec2Binaryarray(int array[],long input){
    
    int temp[32];
    int i=0,j=0,reminder;
    do{  // Convert to binary, just like the way to do this by hand
        reminder=input%2; // Find the reminder
        temp[i]=reminder;
        input=input/2;
        i++;
    }while(input!=0);
    while(i>0){
        array[j]=temp[--i];j++; // Read the temp from end back to start to get binary
    }
    array[j]=2;
    
}
