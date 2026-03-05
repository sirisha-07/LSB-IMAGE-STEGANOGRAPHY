# LSB-IMAGE-STEGANOGRAPHY
This project implements LSB(Least Significant Bit) based image steganography in C to hide and extract secret messages inside BMP image files without noticeable visual distortion.

Technologies Used
C Programming
File Handling
Bitwise Operations
BMP File Format Handling

Features
Encode secret message into a BMP image
Decode hidden message from encoded image
File validation checks
Error handling for invalid inputs

How it Works
The project uses LSB substitution technique:
Each bit of secret message is stored in least bit of RGB data.
Since LSB changes minimally affect pixel value, visual distortion is negligible.
Compilation : gcc test_encode.c encode.c encode.h 
Usage Encoding ./a.out -e beautiful.bmp secret.txt stego.bmp
Decoding ./a.out -d stego.bmp
