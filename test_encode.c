#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "decode.h"
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    if(argc >= 3)
    {
        if(check_operation_type(argv) == e_encode)
        {
            printf("Selected encoding\n");
            EncodeInfo encode;
            if(read_and_validate_encode_args(argv, &encode) == e_success)
            {
                printf("Read and validate encode argument is a success\n");
                printf("<--------Started encoding-------->\n");
                if(do_encoding(&encode) == e_success)
                {
                    printf("Encoding is successful\n");
                }
                else
                {
                    printf("Failed to perform encoding\n");
                    return -2;
                }
            }
            else
            {
                printf("Read and validate encode argument is a failure\n");
                return -1;
            }
        }
    else if(check_operation_type(argv) == e_decode)
    {
            printf("Selected decoding\n");
            DecodeInfo decInfo;
            if(read_and_validate_decode_args(argv, &decInfo) == e_success)
            {
                printf("Read and validate decode argument is a success\n");
                printf("<--------Started decoding-------->\n");
                if(do_decoding(&decInfo) == e_success)
                {
                    printf("Decoding is successful\n");
                }
                else
                {
                    printf("Failed to perform decoding\n");
                    return -3;
                }
            }
            else
            {
                printf("Read and validate decode argument is a failure\n");
                return -4;
            }
    }
        else
        {
            printf("Invalid Input\n");
            printf("For encoding : ./a.out -e beautiful.bmp secret.txt [stego.bmp]\n");
            printf("For decoding : ./a.out -d stego.bmp [decode.txt]\n");
        }
    }

    /*EncodeInfo encInfo;
    uint img_size;

    // Fill with sample filenames
    encInfo.src_image_fname = "beautiful.bmp";
    encInfo.secret_fname = "secret.txt";
    encInfo.stego_image_fname = "stego_img.bmp";

    // Test open_files
    if (open_files(&encInfo) == e_failure)
    {
    	printf("ERROR: %s function failed\n", "open_files" );
    	return 1;
    }
    else
    {
    	printf("SUCCESS: %s function completed\n", "open_files" );
    }

    // Test get_image_size_for_bmp
    img_size = get_image_size_for_bmp(encInfo.fptr_src_image);
    printf("INFO: Image size = %u\n", img_size);*/

    return 0;
}

OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1], "-e") == 0)
    {
        return e_encode;
    }
    else if(strcmp(argv[1], "-d") == 0)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}
