#include <stdio.h>
#include "decode.h"
#include "types.h"
#include <string.h>
#include "common.h"
#include <unistd.h>

Status Open_files(DecodeInfo *decInfo)
{
    // Stego Image file
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    // Do Error handling
    if (decInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);

    	return e_failure;
    }

    /*Secret file
    decInfo->fptr_secret = fopen(decInfo->secret_fname, "w");
    // Do Error handling
    if (decInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->secret_fname);

    	return e_failure;
    }

    // No failure return e_success*/
    return e_success;
}

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if(strstr(argv[2], ".bmp") != NULL)
    {
        decInfo->stego_image_fname = argv[2];
    }
    else
    {
        printf("Please enter .bmp extension\n");
        return e_failure;
    }
    
    if(argv[3] != NULL)
    {
        strcpy(decInfo->secret_fname, argv[3]);
    }
    else{
        strcpy(decInfo->secret_fname, "decode");
    }
    return e_success;
}

Status decode_magic_string(char *magic_string, DecodeInfo *decInfo)
{
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);
    char data[strlen(magic_string)];
    decode_data_to_image(data, strlen(magic_string), decInfo->fptr_stego_image,decInfo);
    if(strcmp(magic_string, data) == 0)
    {
        return e_success;
    }
    else
    {
        printf("Failed to decode magic string\n");
        return e_failure;
    }
}

Status decode_data_to_image(char *data, int size, FILE *fptr_stego_image, DecodeInfo *decInfo)
{
    int i;
    //fread(decInfo->fptr_stego_image, 8, 1, decInfo->fptr_stego_image);
    char image_buffer[8];
    for(i = 0; i< size; i++)
    {
        fread(image_buffer, 8, 1, fptr_stego_image);
        data[i] = (decode_lsb_to_byte(data[i], image_buffer));
    }
    data[i] = '\0';
    return e_success;
}

Status decode_lsb_to_byte(char data, char *image_buffer)
{
    char ch=0;
    for(int i=0;i<8;i++)
    {
        ch = ((image_buffer[i] & 1) << i) | ch;
        
    }
    return ch;
}

Status decode_size_to_lsb(char *buffer, int *data)
{
    *data = 0;
    for(int i=0;i<32;i++)
    {
        *data = ((buffer[i] & 1) << i) | *data;
    }
    return e_success;
}

Status decode_secret_file_extn_size(int *size, FILE *fptr_stego_image)
{
    char buffer[32];
    fread(buffer, 32, 1, fptr_stego_image);
    decode_size_to_lsb(buffer, size);
    return e_success;
}

Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char extn[decInfo->size_secret_file_extn];
    if(decode_data_to_image(extn, decInfo->size_secret_file_extn, decInfo->fptr_stego_image, decInfo) == e_success)
    {
        strcat(decInfo->secret_fname, extn);
        decInfo->fptr_secret = fopen(decInfo->secret_fname, "w");
        return e_success;
    }
}

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    decode_secret_file_extn_size(&(decInfo->decode_size_secret_file), decInfo->fptr_stego_image);
    return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char data[decInfo->decode_size_secret_file];
    decode_data_to_image(data, decInfo->decode_size_secret_file, decInfo->fptr_stego_image, decInfo);
    fprintf(decInfo->fptr_secret, "%s", data);
    return e_success;
    
}

Status do_decoding(DecodeInfo *decInfo)
{
    if(Open_files(decInfo) == e_success)
    {
        printf("Open files is a success\n");
    }
    else
    {
        printf("Failed to open files successfully\n");
        return e_failure;
    }
    if(decode_magic_string(MAGIC_STRING, decInfo) == e_success)
    {
        printf("Decoded magic string successfully\n");
    }
    else
    {
        printf("Failed to encode magic string\n");
        return e_failure;
    }
    //strcpy(decInfo->extn_secret_file, strchr(decInfo->secret_fname, '.'));
    if(decode_secret_file_extn_size(&(decInfo->size_secret_file_extn), decInfo->fptr_stego_image) == e_success)
    {
        printf("Decoded secret file extn size successfuly\n");
    }
    else
    {
        printf("Failed to decode secret file extn size\n");
        return e_failure;
    }
    if(decode_secret_file_extn(decInfo) == e_success)
    {
        printf("Decoded secret file extension successfully\n");
    }
    else
    {
        printf("Failed to decode secret file extension\n");
        return e_failure;
    }
    if(decode_secret_file_size(decInfo) == e_success)
    {
        printf("Decoded secret file size successfully\n");
    }
    else
    {
        printf("Failed to decode secret file size");
        return e_failure;
    }
    if(decode_secret_file_data(decInfo) == e_success)
    {
        printf("Decoded secret file data successfully\n");
    }
    else{
        printf("Failed to decode secret file data\n");
        return e_failure;
    }

    return e_success;
}
