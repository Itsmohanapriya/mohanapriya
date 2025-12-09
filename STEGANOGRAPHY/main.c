/*
Name : Mohanapriya K
Date : 02.09.2025
Description :  LSB image Steganography
*/
#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"
#include "decode.h"

OperationType check_operation_type(char *);

/* Main Function */
int main(int argc, char *argv[])
{
    if (argc < 2)  // must have at least operation flag
    {
        printf("ERROR: Insufficient arguments\n");
        printf("USAGE:\n");
        printf("  For Encoding: ./a.out -e <.bmp file> <secret.txt> [output.bmp]\n");
        printf("  For Decoding: ./a.out -d <.bmp file> [output file]\n");
        return e_failure;
    }

    // Step 2 : Check operation type
    OperationType op_type = check_operation_type(argv[1]);

    if (op_type == e_encode)
    {
        printf("----------------------Start encode------------------\n");
        if (argc < 4)
        {
            printf("ERROR: Insufficient arguments for ENCODING\n");
            printf("USAGE: ./a.out -e <.bmp file> <secret.txt> [output.bmp]\n");
            return e_failure;
        }

        // Step 3 : Declare EncodeInfo
        EncodeInfo encInfo;

        // Step 4 : Validate encoding args
        if (read_and_validate_encode_args(argv, &encInfo) == e_success)
        {
            // Step 5 : Do encoding
            if (do_encoding(&encInfo) == e_success)
            {
                printf("Encoding completed successfully\n");
            }
            else
            {
                printf("ERROR: Encoding failed\n");
            }
        }
        else
        {
            printf("ERROR: Validation failed\n");
        }
    }
    else if (op_type == e_decode)
{
    printf("----------------------Start decode------------------\n");
    if (argc < 3)
    {
        printf("ERROR: Insufficient arguments for DECODING\n");
        printf("USAGE: ./a.out -d <.bmp file> [output file]\n");
        return e_failure;
    }

    DecodeInfo decodeInfo;
                 //  Validate decoding args
    if (read_and_validate_decode_args(argv, &decodeInfo) == e_success)
    {
        if (do_decode(&decodeInfo) == e_success)
            printf("Decoding completed successfully\n");
        else
            printf("ERROR: Decoding failed\n");
    }
    else
    {
        printf("ERROR: Validation failed for decoding\n");
    }
}
}
//check operation type encode or decode
OperationType check_operation_type(char *symbol)
{
    if (strcmp(symbol, "-e") == 0)
        return e_encode;

    if (strcmp(symbol, "-d") == 0)
        return e_decode;
    else
    {   
    printf("Invaild extension");
    return e_unsupported;
    }
}


