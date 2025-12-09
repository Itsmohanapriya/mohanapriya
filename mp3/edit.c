#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mp3.h"

union 
{
    int size;
    char arr[4];

}u;
//convert big endian to little endian
void Converstion(char arr[4])
{
    for(int i = 0 ; i < 2 ; i++)
    {
        char temp = arr[i];
        arr[i] = arr[3 - i];
        arr[3 - i] = temp;
    }
}

// Function to edit mp3
int edit_mp3(const char *option, const char *new_content, const char *filename)
{
    FILE *src, *tmp;
    char buf[5]; // 4-byte frame ID + null terminator
    unsigned char size_bytes[4];
    unsigned int old_size, new_size;

    // Corresponding frame IDs (ID3v2.3)
    char *TAGS[6]    = {"TIT2", "TPE1", "TALB", "TYER", "TCON", "COMM"};
    // Corresponding options
    char *OPTIONS[6] = {"-t", "-A", "-a", "-y", "-m", "-c"};

    // Step 1: Open files
    src = fopen(filename, "rb");
    if (src == NULL)
    {
        printf("Error: Cannot open file '%s'\n", filename);
        return 1;
    }
    tmp = fopen("temp.mp3", "wb");

    if (tmp == NULL)
    {
        printf("Error: Cannot create temp file\n");
        fclose(src);
        return 1;
    }

    // Step 2: Copy first 10 bytes (ID3 header)
    unsigned char header[10];
    if (fread(header, 1, 10, src) != 10)
    {
        printf("Error: Invalid MP3 header\n");
        fclose(src);
        fclose(tmp);
        return 1;
    }
    fwrite(header, 1, 10, tmp);

    int updated = 0; // check if tag matched


    while (fread(buf, 1, 4, src) == 4)
    {
        buf[4] = '\0'; // frame ID as string


        if (buf[0]==0 && buf[1]==0 && buf[2]==0 && buf[3]==0)
            break;

        // Read frame size (4 bytes little endian → int)
        //if (fread(size_bytes, 1, 4, src) != 4)

        if(fread(&u.size , 1 , 4 , src) != 4)
            break;

        Converstion(u.arr);
        old_size = u.size;

        // Read frame flags (2 bytes)
        unsigned char flags[2];
        if (fread(flags, 1, 2, src) != 2)
            break;

        // Find matching tag index
        int tag_index = -1;
        for (int i = 0; i < 6; i++)
        {
            if (strcmp(option, OPTIONS[i]) == 0 && strcmp(buf, TAGS[i]) == 0)
            {
                tag_index = i;
                break;
            }
        }

        if (tag_index != -1)
        {
            // Write frame ID
            fwrite(TAGS[tag_index], 1, 4, tmp);

            // New size = content length + 1 encoding byte
            new_size = strlen(new_content) + 1;
            //be_to_le_bytes(new_size, size_bytes);
            u.size = new_size;
            Converstion(u.arr);
            //fwrite(size_bytes, 1, 4, tmp);
            fwrite(&u.size , 1 , 4 , tmp);
            // Write flags
            fwrite(flags, 1, 2, tmp);

            // Write encoding byte (0 = ISO-8859-1) + content
            fputc(0, tmp);
            fwrite(new_content, 1, strlen(new_content), tmp);

            // Skip old frame data
            fseek(src, old_size, SEEK_CUR);

            updated = 1;
            break;
        }
        
        else
        {
            // copy frame
            fwrite(buf, 1, 4, tmp);

            u.size = old_size;
            Converstion(u.arr);
            //fwrite(size_bytes, 1, 4, tmp);
            fwrite(&u.size , 1 , 4 , tmp);
            fwrite(flags, 1, 2, tmp);

            unsigned char *old_data = malloc(old_size);
            if (!old_data)
            {
                printf("Memory allocation failed\n");
                fclose(src);
                fclose(tmp);
                return 1;
            }
            fread(old_data, 1, old_size, src);
            fwrite(old_data, 1, old_size, tmp);
            free(old_data);
        }
    }

    int ch;
    while((ch = fgetc(src)) != EOF)
        fputc(ch , tmp);

    fclose(src);
    fclose(tmp);

    // Replace original file with updated temp
    remove(filename);
    rename("temp.mp3", filename);

    if (updated)
        printf("Edit successful! Updated %s in %s\n", option, filename);
    else
        printf("Warning: Tag for %s not found in %s\n", option, filename);

    return 0;
}
