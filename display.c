#include <stdio.h>
#include <stdlib.h>
#include "steg.h"

/*  The function DispErr prints out messages on the screen. The inputs are the a number that indicates which message to be displayed, a pointer to the string of the file name (NULL if not applicable) and a number when there are figures needed. Since the message is to be printed out on screen, no output is needed. */

void DispErr(int num,char *name,int ks){
    if (num==1){printf("Usage: (encode or decode, respectively)\n    steg <bmpfile> <datafile> <outputfile>\n    steg <bmpfile> <outputfile>\n");}
    if (num==2){printf("Error: Expected data size is larger than available space in bitmap.\n");}
    if (num==3){printf("There was a maximum of %d bits modified per byte.\n",ks);}
    if (num==4){printf("Error at switch\n");}
    if (num==5){printf("Output file %s already exists. Overwrite (y/n)?\n",name);}
    if (num==6){printf("Error: Could not open file %s.\n",name);}
    if (num==7){float f=ks;f=f/100;printf("Steganographic process has created %.1f%% image corruption.\n",f);}
    if (num==8){printf("Error: Bitmap too small to store data file.\n");}
}
