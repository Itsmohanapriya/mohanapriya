#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "types.h"
#include"common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

uint get_file_size(FILE *fptr)
{
    fseek(fptr,0,SEEK_END);
    long int size = ftell(fptr);
    fseek(fptr,0,SEEK_SET);
    return size;
}

/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    //  Step 1 : Check the argv[2] is having the .bmp extension or not
    // true - > Store into the structure member step 3 , false - > return e_failure
    if (strstr(argv[2], ".bmp") != NULL)
        encInfo->src_image_fname = argv[2];
    else
        return e_failure;
    // Step 2 : Check the argv[3] is having the .txt extension or not
    // true - > Store into the structure member step 3 , false - > return e_failure
    if (strstr(argv[3], ".txt") != NULL)
        encInfo->secret_fname = argv[3];
        else if (strstr(argv[3], ".c") != NULL)
        encInfo->secret_fname = argv[3];
        else if (strstr(argv[3], ".sh") != NULL)
        encInfo->secret_fname = argv[3];
    else
        return e_failure;
     // Step 3 : Check argv[4] != NULL or not true - > Step 4, false -> Step 5
    if (argv[4] != NULL)
    {
        // Step 4 : Check the argv[4] is having the .bmp extension or not
    // true - > Store into the structure member step 6 , false - > return e_failure
        if (strstr(argv[4], ".bmp") != NULL)
            encInfo->stego_image_fname = argv[4];
        else
            return e_failure;
    }
    // Step 5 : Store the default.bmp into the structure member - > Step 6
    else
    {
        encInfo->stego_image_fname = "stego.bmp";
    }
    // Step 6 : return e_success;
    printf("-> read and vaildate file is successful\n");
    return e_success;
}

Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {

        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

        return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

        return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

        return e_failure;
    }

    // No failure return e_success
    printf("-> File open successfully\n");
    return e_success;
}

Status check_capacity(EncodeInfo *encInfo)
{
    // Step 1 :  Call get_image_size_for_bmp(encinfo->fptr_image) store into the encinfo->image_capacity
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    // Step 2 : Call get_file_size(encinfo->fptr_secret) store into the encinfo-> size_secret_file;
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
    // Step 3 : encinfo->image_capacity > 16 + 32 + 32 + 32 + (encinfo-> size_secret_file * 8)
    //   true - return e_ succes , false - > e _ failure
    if (encInfo->image_capacity > (54 + 16 + 32 + 32 + 32 + (encInfo->size_secret_file * 8)))
    {
        printf("-> Check capacity Done\n");
        return e_success;
    }  
    else
    {
        return e_failure;
    }
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    //copy the header file 54 bytes
    char image_buffer[54];
    fseek(fptr_src_image, 0, SEEK_SET);
    fread(image_buffer, 54, 1, fptr_src_image);
    fwrite(image_buffer, 54, 1, fptr_dest_image);
    printf("-> copy header file successful\n");
    return e_success;
}
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
     char buffer[8];
     // Step 1 : Generate the loop upto the magic_string size
    for (int i = 0; magic_string[i] != '\0'; i++)
    {
        // Step 2 : Read the 8 bytes from src store into image buffer
        fread(buffer, 8, 1, encInfo->fptr_src_image);
         // Step 3 : Call the encode_byte_to_lsb(magic_string[i],imagebuffer)
        encode_byte_to_lsb(magic_string[i], buffer);
        // Step 4 : Write the imageBuffer into destination file
        fwrite(buffer, 8, 1, encInfo->fptr_stego_image);
    }
    // Return e_success
    printf("-> Encode the magic string is done\n");
    return e_success;
}
Status encode_secret_file_extn_size(int size, EncodeInfo *encInfo)
{
     char buffer[32];
     // Step 1 : Read the 32 bytes from src store into image buffer
    fread(buffer, 32, 1, encInfo->fptr_src_image);
    // Step 2 : Call the encode_size_to_lsb(size,imagebuffer)
    encode_size_to_lsb(size, buffer);
    // Step 3 :  Write the imageBuffer into destination file
    fwrite(buffer, 32, 1, encInfo->fptr_stego_image);
    // Return e_success
    return e_success;
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    int size = strlen(file_extn);
   // Step 1 : Generate the loop upto the file_extn size
    for (int i = 0; i < size; i++)
    {
        char imageBuffer[8];

        // Step 2: Read 8 bytes from src into imageBuffer
        if (fread(imageBuffer, 1, 8, encInfo->fptr_src_image) != 8)
            return e_failure;

        // Step 3: Encode one byte of file extension into imageBuffer
        if (encode_byte_to_lsb(file_extn[i], imageBuffer) == e_failure)
            return e_failure;

        // Step 4: Write the modified buffer into destination file
        if (fwrite(imageBuffer, 1, 8, encInfo->fptr_stego_image) != 8)
            return e_failure;
    }
   // Return e_success
    return e_success;
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char imageBuffer[32];

    // Step 1: Read 32 bytes from src into imageBuffer
    if (fread(imageBuffer, 1, 32, encInfo->fptr_src_image) != 32)
        return e_failure;

    // Step 2: Encode file size into LSBs
    if (encode_size_to_lsb((int)file_size, imageBuffer) == e_failure)
        return e_failure;

    // Step 3: Write buffer into destination file
    if (fwrite(imageBuffer, 1, 32, encInfo->fptr_stego_image) != 32)
        return e_failure;
    // Return e_success
    return e_success;

}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    // Step 1: Rewind secret file pointer
    rewind(encInfo->fptr_secret);

    // Step 2: Allocate buffer for secret file data
    char secret_file_data[encInfo->size_secret_file];

    // Step 3: Read the entire secret file into buffer
    if (fread(secret_file_data, 1, encInfo->size_secret_file, encInfo->fptr_secret) != encInfo->size_secret_file)
        return e_failure;

    // Step 4 & 5: Loop through each byte of secret file
    for (long i = 0; i < encInfo->size_secret_file; i++)
    {
        char imageBuffer[8];

        // Step 6: Read 8 bytes from src image into buffer
        if (fread(imageBuffer, 1, 8, encInfo->fptr_src_image) != 8)
            return e_failure;

        // Step 7: Encode one byte of secret file into LSBs
        if (encode_byte_to_lsb(secret_file_data[i], imageBuffer) == e_failure)
        //return e_failure
            return e_failure;

        // Step 8: Write the buffer to stego image
        if (fwrite(imageBuffer, 1, 8, encInfo->fptr_stego_image) != 8)
            return e_failure;
    }
  // Return e_success
  printf("-> secret data write in image sucessfully\n");
    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char buffer[1024];
    size_t bytesRead;

    // Copy remaining bytes until EOF
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), fptr_src)) > 0)
    {
        if (fwrite(buffer, 1, bytesRead, fptr_dest) != bytesRead)
            return e_failure;
    }
    // Return e_success
    printf("-> Remaining data copy successfully\n");
    return e_success;
}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for (int i = 0; i < 8; i++)
    {
        // Step 2: Clear LSB of image_buffer[i]
        image_buffer[i] &= 0xFE;

        // Step 3: Get i-th bit from data (from MSB to LSB)
        char bit = (data >> (7 - i)) & 1;

        // Step 4: Insert bit into image_buffer[i]
        image_buffer[i] |= bit;
    }
    // Return e_success
    return e_success;
}

Status encode_size_to_lsb(int size, char *image_buffer)
{
    for (int i = 0; i < 32; i++)
    {
        // Step 2: Clear LSB
        image_buffer[i] &= 0xFE;

        // Step 3: Get i-th bit of size
        int bit = (size >> (31 - i)) & 1;

        // Step 4: Replace into LSB
        image_buffer[i] |= bit;
    }
    // Return e_success
    return e_success;
}

Status do_encoding(EncodeInfo *encInfo)
{
    //step 1: open fun call
     if (open_files(encInfo) == e_failure)
    {
        printf("ERROR: Opening files failed\n");
        return e_failure;
    }
    //step 2: check_capacity fun call
    if (check_capacity(encInfo) == e_failure)
    {
        printf("ERROR: Insufficient image capacity\n");
        return e_failure;
    }
    // Step 3: Copy BMP header
    if (copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        printf("ERROR: Copying BMP header failed\n");
        return e_failure;
    }

    // Step 4: Encode magic string
    if (encode_magic_string(MAGIC_STRING,encInfo) == e_failure)
    {
        printf("ERROR: Encoding magic string failed\n");
        return e_failure;
    }

    // Step 5: Extract extension from secret file name
    char *dot = strchr(encInfo->secret_fname, '.');
    if (dot != NULL)
    {
        strcpy(encInfo->extn_secret_file, dot);
    }
    else
    {
        printf("ERROR: Secret file has no extension\n");
        return e_failure;
    }
    // Step 6: Find extension size
    int extn_size = strlen(encInfo->extn_secret_file);
    if (encode_secret_file_extn_size(extn_size, encInfo) == e_failure)
    {
        return e_failure;
    }
     // Step 8: Encode secret file extension
    if (encode_secret_file_extn(dot, encInfo) == e_failure)
    {
        printf("ERROR: Encoding secret file extension failed\n");
        return e_failure;
    }

    // Step 9: Encode secret file size
    if (encode_secret_file_size(get_file_size(encInfo->fptr_secret), encInfo) == e_failure)
    {
        printf("ERROR: Encoding secret file size failed\n");
        return e_failure;

    }

    // Step 10: Encode secret file data
    if (encode_secret_file_data(encInfo) == e_failure)
    {
        printf("ERROR: Encoding secret file data failed\n");
        return e_failure;
    }

    // Step 11: Copy remaining image data
    if (copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        printf("ERROR: Copying remaining image data failed\n");
        return e_failure;
    }
    // Return e_success
    return e_success;
}


    

