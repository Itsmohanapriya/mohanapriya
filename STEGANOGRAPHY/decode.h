#ifndef DNCODE_H
#define DNCODE_H
#include <stdio.h>

#include "types.h" // Contains user defined types

/*
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

typedef struct _DecodeInfo
{
    char *stego_image_fname; // Stego image name
    FILE *fptr_stego_image;

    char *secret_fname;      // Base secret file name (without ext)
    char output_fname[50];   // Final output name
    FILE *fptr_output;

    int extn_size;           // Extension size
    char extn_secret_file[10]; // File extension

    int secret_file_size;    // Secret file size
} DecodeInfo;

/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);
/* Perform the encoding */
Status do_decode(DecodeInfo *decInfo);
/* Get File pointers for i/p files */
Status open_decode_files(DecodeInfo *decInfo);
/* Get File pointers for o/p files */
Status open_output_file(DecodeInfo *decInfo, char *file_extn);
/* skip the header file*/
Status skip_bmp_header(FILE *fptr_image);
/* decode magic string*/
Status decode_magic_string(DecodeInfo *decInfo);
/* decode extension size*/
Status decode_secret_file_extn_size(DecodeInfo *decInfo);
/* decode extension */
Status decode_secret_file_extn(DecodeInfo *decInfo, char *file_extn, int extn_size);
/* decode secrete file size*/
Status decode_secret_file_size(DecodeInfo *decInfo);
/* decode secrete file  data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

// Helpers
char decode_byte_from_lsb(char *buffer);
int decode_int_from_lsb(char *buffer);
#endif