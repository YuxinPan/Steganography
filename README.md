# Steganography
This program could hide the contents of a data file inside a bitmap file. The original data file may subsequently be recovered from the modified bitmap file.


The program uses a command-line interface. 


The interface for encoding a data file inside a bitmap file takes three filenames as arguments:

steg <bmpfile> <datafile> <outputfile>

In this case, <bmpfile> is a Windows bitmap file, <datafile> is the data file, and <outputfile> is the modified bitmap that contains the hidden message. 

The interface for decoding a modified bitmap and recovering the data file takes only two arguments:

steg <bmpfile> <outputfile>

Here <bmpfile> is a modified bitmap and <outputfile> is the recovered data file.


Steg.c takes the input from file, and sends it to bitmap module for analysis including checking if the bitmap type is correct and separating the header block. Then program will switch to either encoding or decoding module according to the number of arguments. Encoding and decoding modules process the file and call converting for bitwise operation and conversion and call displaying for printing out the message when needed. 


Appropriate feedback will be provided after the image is processed.

