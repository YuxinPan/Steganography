/*
 * steg.c
 *
 *  Created on: 7 Oct 2015
 *      Author: Yuxin Pan
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include "bitmap.h"
#include "steg.h"
#define SIZE 255
#define DATAFILESIZE 32


int main(int argc, char **argv){

    int decode;
    long maxrow=0,filesize=0;
    if ((argc!=3)&&(argc!=4)){  // Wrong number of arguments, display error message and quit
        DispErr(1,NULL,0);
        exit(0);
    }
    if (argc==3){
    	decode=1;
    }
    else{
    	decode=0;
    }  // Tell if it is encoding or decoding from the number of arguments
    
    
    /* Switch to encoding or decoding module */
    switch (decode){
            
        case 0: {        /* Start of encoding module */

            long datasize,bmpsize,sum=0;
            int endatasize[DATAFILESIZE],temp[DATAFILESIZE];
            FILE *fbmp = fopen(argv[1], "rb");
            FILE *fbmp2 = fopen(argv[1], "rb");
            FILE *fdata1 = fopen(argv[2], "rb");
            
            
            /* Start detecting if the output file already exists */
            FILE *file_exist = fopen(argv[3], "r");
            if (file_exist){
                DispErr(5,argv[3],0);
                fflush(stdout);
                char user_input[SIZE];  // Read the command from user
                fgets(user_input,sizeof(user_input),stdin);
                if (!((strlen(user_input)==2)&&(user_input[0]=='y'))){
                	exit(0);
                }
            }
            /* Start detecting if the output file already exists */
            
            
            if (fbmp == NULL) {  // If unable to open the bitmap, then display the error message and quit
                DispErr(6,argv[1],0);
                return 0;
            }
            if (fdata1 == NULL) {// If unable to open the data file, then display the error message and quit
                DispErr(6,argv[2],0);
                return 0;
            }
            FILE *fout = fopen(argv[3], "wb");
            fseek(fdata1, 0L, SEEK_END);  // Find the size of the data file
            datasize = ftell(fdata1);
            fseek(fbmp2, 0L, SEEK_END);
            bmpsize = ftell(fbmp2);
            if (datasize>bmpsize){
            	DispErr(8,NULL,0);
            	exit(0);// There will be a more accurate way to find if the data file is too large to encode. This is just for safety and also can terminate the program earlier in some cases.
            }
            FILE *fdata = fopen(argv[2], "rb");
            long i;
            for (i=0;i<DATAFILESIZE;i++) endatasize[i]=2;
            Dec2Binaryarray(&endatasize[0],datasize);
            BmpData bdat;
            bdat = check_bitmap(fbmp);
            while (bdat.headersize--) {  // Write header block to output verbatim
                int c = fgetc(fbmp);
                fputc(c, fout);
            }
            
            
            /*  This section is just to format the array from left aligned to right aligned. */
            long j=DATAFILESIZE+1;
            int k;
            for (i=0;i<DATAFILESIZE;i++){
                if ((endatasize[i]==2)&&(j==(DATAFILESIZE+1))){
                    j=i-1;
                }
                temp[i]=endatasize[i];
            }
            if (j==DATAFILESIZE+1){
            	for (i=DATAFILESIZE-1;i>=0;i--){
            		endatasize[i]=temp[i];
            	}
            }
            else{
            for (i=DATAFILESIZE-1;i>=0;i--){
            	if(j>=0){
            		endatasize[i]=temp[j];
            		j--;
            	}
            	else
            		endatasize[i]=0;
            }
            }
            /*  End formatting array section. */

            
            /* Start Reading pixel data from original bitmap */
            i=1;
            while (bdat.numpixelbytes--) {
                int c = fgetc(fbmp);
                int e = c;
                if (i<=DATAFILESIZE){ // Read the first 32 bytes
                    c=c-(c%2);
                    if (endatasize[i-1]==1) {
                    	if ((c%2)==0){
                    		c++;sum++;
                    	}
                    }
                    else {
                    	if ((c%2)==1){
                    		c--;sum++;
                    	}
                    }
                    fputc(c, fout);   // Write the first 32 bytes to the output
                }
                if (i>DATAFILESIZE){
                	output[i-DATAFILESIZE-1]=c;
                	Dec2Binary(e,i,data);
                	Dec2Binary(e,i,data2);
                } // Store the data in output[] for corruption level calculation
                i++;
            }
            /* End Reading pixel data from original bitmap */

            
            i--;
            maxrow=i-DATAFILESIZE;
            k=floor(8*datasize/maxrow)+1;
            int ks;
            if (((8*datasize)%maxrow)==0){
            	k--;
            }
            ks=k;
            if (ks>8){DispErr(8,NULL,0);exit(0);};  // The bitmap is not big enough to store the entire data-file, so display the error and quit.
            DispErr(3,NULL,ks);  // Display the maximum number of bits modified
            j=8*datasize;
            int buffer[9],count=7,count2,i2=-1;
            
            
            /* Start putting the data file into the bitmap */
            while(j>0){
                if ((j%8)==0){
                    int c1=fgetc(fdata),j2;    // Read the data file
                    for (j2=0;j2<=8;j2++)
                    	buffer[j2]=2;
                    Dec2Binaryarray(&buffer[0],c1);  // Use buffer to temporarily store the data from the data file
                    count2=-1;
                    {
                        int i3,j1=DATAFILESIZE;
                        for (i3=0;i3<8;i3++)   // Remove data from buffer
                        {
                            if ((buffer[i3]==2)&&(j1==DATAFILESIZE)){
                                j1=i3-1;
                            }
                            temp[i3]=buffer[i3];
                        }
                        if (j1==DATAFILESIZE){
                        	for (i3=8-1;i3>=0;i3--){
                        		buffer[i3]=temp[i3];
                        	}
                        }
                        else
                        {  // Store the data in the buffer
                            for (i3=8-1;i3>=0;i3--){
                                if(j1>=0){
                                    buffer[i3]=temp[j1];j1--;
                                }
                                else buffer[i3]=0;
                            }
                        }
                    }
                }
                i2++;count2++;
                if (i2==(maxrow)){
                	i2=0;
                	count--;
                }   // Wrap around and start from 0
                data[i2][count]=buffer[count2];
                j--;
            }
            /* End putting the data file into the bitmap */


            /* Start calculating the overall level of file corruption*/
            for (j=0;j<maxrow;j++){
            	for (k=0;k<8;k++){
            		sum=sum+abs(data[j][k]-data2[j][k])*pow(2,(7-k));
            	}
            }
            j=sum/(SIZE*(maxrow+32)/10000);
            DispErr(7,NULL,j);
            /* End calculating the overall level of file corruption */
            
            
            /* Start writing to the output file */
            for (j=DATAFILESIZE+1;j<=i;j++){
                for (k=0;k<=7;k++){
                	output[k]=data[j-DATAFILESIZE-1][k];
                }
                k=Binary2Dec(&output[0],8);
                fputc(k,fout);
            }
            /* End writing to the output file */


            /* End of encoding module */
        }
        break;
  
            
        case 1: {
        	/* Start of decoding module */

            FILE *fbmp = fopen(argv[1], "rb");
            
            
            /* Start detecting if the output file already exists */
            FILE *file_exist = fopen(argv[2], "r");
            if (file_exist){
                DispErr(5,argv[2],0);
                fflush(stdout);
                char user_input[SIZE];  // Read the command from user
                fgets(user_input,sizeof(user_input),stdin);
                if (!((strlen(user_input)==2)&&(user_input[0]=='y'))){
                	exit(0);
                }
            }
            /* End detecting if the output file already exists */

            
            if (fbmp == NULL){   // If unable to open a file, then display the error message and quit
                DispErr(6,argv[1],0);
                return 0;
            }
            
            BmpData bdat;
            bdat = check_bitmap(fbmp);
            while (bdat.headersize--) { // Skip the header of the bitmap
                fgetc(fbmp);
            }
            {
                long i=1;
                int DataSize[DATAFILESIZE];
                
                
                /* Start Reading pixel data from the bitmap */
                while (bdat.numpixelbytes--) {
                    int c = fgetc(fbmp);
                    int d=c,e=c;
                    if (i<=DATAFILESIZE){  // read the first 32 bytes
                        DataSize[i-1]=d;
                        i++;
                        if (i==(DATAFILESIZE+1)) continue;
                    }
                    if (i==(DATAFILESIZE+1)){
                    	if (DataSize[0]==1){
                    		DispErr(2,NULL,0);
                    		return 0;
                    	}
                        filesize=ComputeDataSize(&DataSize[0]);
                    }
                    if (i>(DATAFILESIZE)){ // read rest of the data
                        Dec2Binary(e,i,data);
                        if (maxrow<(i-DATAFILESIZE))
                        	maxrow=i-DATAFILESIZE;
                        i++;
                    }
                }
                /* End Reading pixel data from the bitmap */


                long j;
                if (filesize>maxrow){
                	DispErr(2,NULL,0);
                	return 0;
                }    // The 32-bit value that denotes the size of the hidden data is larger than the available space in the bitmap
                filesize=filesize*8;
                long temp;
                temp=filesize;
                j=7;i=0;
                int buffer[8],count=0,count2=0;
                
                
                /* Start collecting the output data */
                while (temp>0){
                    temp--;
                    buffer[count]=data[i][j];  // Use buffer to temporarily store the data from data matrix
                    if ((count%8)==7){
                    	count=-1;
                    	output[count2]=Binary2Dec(&buffer[0],8);
                    	count2++;
                    }
                    i++;
                    count++;
                    if (i==(maxrow)){
                    	i=0;
                    	j--;
                    } // Wrap around and start from 0
                }
                /* End collecting the output data */
                

                /* Write to the output file */
                FILE *fout = fopen(argv[2], "wb");
                for (i=0;i<(filesize/8);i++){
                    fputc(output[i],fout);
                }
            }
        /* End of decoding module */
        }
        break;

            
        default:{
        	/* Should be impossible to reach here, but leave it for safety. */
        	DispErr(4,NULL,0);
        }

    }
    
    fflush(stdout);

    return 0;
}




