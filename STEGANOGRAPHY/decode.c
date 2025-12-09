#include <stdio.h>
#include<string.h>
#include "decode.h"
#include "types.h"
#include"common.h"


/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
char decode_byte_from_lsb(char *image_buffer)
{
    //collect the data from lsb side
    char ch = 0;
    for (int i = 0; i < 8; i++)
    {
        ch = (ch << 1) | (image_buffer[i] & 1);
    }
    return ch;
}

int decode_int_from_lsb(char *buffer)
{
    //collect the data from lsb side
    int value = 0;
    for (int i = 0; i < 32; i++)
    {
        value = (value << 1) | (buffer[i] & 1);
    }
    return value;
}

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    //check the argv[2] .bmp file or not
    if (argv[2] == NULL || strstr(argv[2], ".bmp") == NULL)
        return e_failure;
    decInfo->stego_image_fname = argv[2];
    //its check argv[3] present or not
    if (argv[3] == NULL)
    {
        // not present means default create file output
        decInfo->secret_fname = strdup("output");
    }
    //its present means check and store
    else
    {
        //check upto . 
        char *dot = strrchr(argv[3], '.');
        // terminate string at dot
        if (dot)
        {
            *dot = '\0';  
        }
        //store the file name
        decInfo->secret_fname = strdup(argv[3]);
    }
    //its passed all return success
    printf("-> Read and validation complete successfully\n");
    return e_success;
}

Status open_decode_files(DecodeInfo *decInfo)
{
    //open the file stego.bmp in read mode
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    //check the file its open or not
    if (decInfo->fptr_stego_image == NULL)
    {
        return e_failure;
    }
    //its all passed correct print and return successs
    printf("-> open decode files successfully\n");
    return e_success;
}

Status skip_bmp_header(FILE *fptr_image)
{
    // Move file pointer by 54 bytes (standard BMP header size)
    if (fseek(fptr_image, 54, SEEK_SET) != 0)
    {
        //its fseek != 0 its return failure
        return e_failure;
    }
    //its all passed correct print and return successs
    printf("-> skip bmp header successfully\n");
    return e_success;
}

Status decode_magic_string(DecodeInfo *decInfo)
{
    //create the buffer and magic string
     char buffer[8], magic[3];
     //run the loop until the magic string
    for (int i = 0; i < 2; i++)
    {
        //read the magic string from stego_image 
        fread(buffer, 1, 8, decInfo->fptr_stego_image);
        //strore magic dtring in magic buffer
        magic[i] = decode_byte_from_lsb(buffer);  
    }
    magic[2] = '\0';
    //compare the now read magic string and orginal magic string
    if (strcmp(magic, MAGIC_STRING) == 0)
    //its match means print and return success
    {
    printf("-> Magic string mached successfully\n");
        return e_success;
    }
    else
    //its mismached return failure
        return e_failure;
}
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    //create the buffer
    char buffer[32];
    //read the file extern of secret file size
    fread(buffer, 1, 32, decInfo->fptr_stego_image);
    decInfo->extn_size = decode_int_from_lsb(buffer);
    //its read the file extension size print and return sucessfully
    printf("-> secrect file extension size find successfully\n");
    return e_success;
}

Status decode_secret_file_extn(DecodeInfo *decInfo, char *file_extn, int extn_size)
{
    //create the buffer
    char buffer[8];
    //loop run the until file extn_size
    for (int i = 0; i < extn_size; i++)
    {
        //read the file extension
        fread(buffer, 1, 8, decInfo->fptr_stego_image);
        file_extn[i] = decode_byte_from_lsb(buffer);
    }
    //add NULL in the end file
    file_extn[extn_size] = '\0';
    //its read all in correct means print and return success
    printf("-> secrete file Extension read successfully\n");
    return e_success;
}
Status open_output_file(DecodeInfo *decInfo, char *file_extn)
{
    //create the string to store the secrete data
    char final_name[50];
   //its sreach . in the string
    if (strstr(decInfo->output_fname, ".") != NULL)
    {
        //copy the file name
        strcpy(final_name, decInfo->output_fname);
    }
    else
    {
       //copy the file name and concertetion
        strcpy(final_name, decInfo->output_fname);
        strcat(final_name, file_extn);
    }
    //open the  final_name file in write mode
    decInfo->fptr_output = fopen(final_name, "w");
    if (decInfo->fptr_output == NULL)
    {
        //check the file is open adderess point the null means return failure
        return e_failure;
    }
    //file save in final_name and return success
    printf("-> Decoded secret will be saved as: %s\n", final_name);
    return e_success;
}

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    //create the buffer
    char buffer[32];
    //read the secret file size
    fread(buffer, 1, 32, decInfo->fptr_stego_image);
    //store the buffer and call the function decode_int_from_lsb(buffer)
    decInfo->secret_file_size = decode_int_from_lsb(buffer);
    printf("-> secrete size found successfully\n");
    return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    //create the buffer
    char buffer[8];
    //loop run until size
    for (int i = 0; i < decInfo->secret_file_size; i++)
    {
        //read the from stego_image to store buffer
        fread(buffer, 1, 8, decInfo->fptr_stego_image);
        //call the opertion to in function call
        char ch = decode_byte_from_lsb(buffer);
        //print the data in character by character
        fputc(ch, decInfo->fptr_output);
    }
    //close the file
    fclose(decInfo->fptr_output);
    printf("-> secret file data print output file successfully\n");
    return e_success;
}

Status do_decode(DecodeInfo *decInfo)
{
    // Step 1: Open stego image
    if (open_decode_files(decInfo) == e_failure)
    {
        printf("Failed to open stego image.\n");
        return e_failure;
    }

    // Step 2: Skip BMP header
    if (skip_bmp_header(decInfo->fptr_stego_image) == e_failure)
    {
        printf("Failed to skip BMP header.\n");
        return e_failure;
    }

    // Step 3: Decode magic string
    if (decode_magic_string(decInfo) == e_failure)
    {
        printf("-> Magic string not found. This is not a valid encoded image.\n");
        return e_failure;
    }

    // Step 4: Decode secret file extension size
    if (decode_secret_file_extn_size(decInfo) == e_failure)
    {
        printf("Failed to decode secret file extension size.\n");
        return e_failure;
    }

    // Step 5: Decode secret file extension
    char file_extn[10];
    if (decode_secret_file_extn(decInfo, file_extn, decInfo->extn_size) == e_failure)
    {
        printf("Failed to decode secret file extension.\n");
        return e_failure;
    }

    // Step 6: Prepare output file name
    strcpy(decInfo->output_fname, decInfo->secret_fname);

    // Step 7: Open output file
    if (open_output_file(decInfo, file_extn) == e_failure)
    {
        printf("Failed to create output file.\n");
        return e_failure;
    }

    // Step 8: Decode secret file size
    if (decode_secret_file_size(decInfo) == e_failure)
    {
        printf("Failed to decode secret file size.\n");
        return e_failure;
    }

    // Step 9: Decode secret file data
    if (decode_secret_file_data(decInfo) == e_failure)
    {
        printf("Failed to decode secret file data.\n");
        return e_failure;
    }

    // Step 10: Close stego image file
    fclose(decInfo->fptr_stego_image);

    //printf("Decoding successful!\n");
    return e_success;
}

