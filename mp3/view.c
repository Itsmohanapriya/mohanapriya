#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mp3.h"

// Convert Big Endian (4 bytes) to integer
unsigned int be_to_int(unsigned char b[4])
{
    return (b[0] << 24) | (b[1] << 16) | (b[2] << 8) | b[3];
}

// Print text-based frames (TIT2, TPE1, TALB, TYER, TCON)
// Skip the first byte (encoding information)
void print_text_frame(const char *tag, unsigned char *data, int size)
{
    if (size <= 1) 
        return; // No valid text data

    printf("%s : ", tag);   // Print tag name
    for (int i = 1; i < size; i++) // Skip first encoding byte
        putchar(data[i]);   
    printf("\n");
}

// Print COMM frame: [encoding][3-byte language][description][\0][actual comment]
void print_comm_frame(unsigned char *data, int size)
{
    if (size <= 4) 
        return; // Not enough data for comment

    // Print "COMM :" and language code
    printf("COMM : ");
    for (int i = 1; i <= 3 && i < size; i++)
        putchar(data[i]);  // Print 3-byte language
    putchar('.');          // Add dot after language

    // Skip description text until null terminator
    int i = 4;
    while (i < size && data[i] != '\0')
        i++;
    i++; // Skip null byte

    // Print the actual comment
    for (; i < size; i++)
        putchar(data[i]);
    printf("\n");
}

// Main function to view MP3 ID3v2 tags
int view_mp3(const char *filename)
{
    // Step 1: Open source file in binary read mode
    FILE *fp = fopen(filename, "rb");
    if (!fp)
    {
        perror("File open error"); // Print error if file not found
        return 0;
    }

    // Step 2: Read 10-byte ID3 header
    unsigned char header[10];
    if (fread(header, 1, 10, fp) < 10) // Check header read
    {
        printf("Not a valid MP3 file\n");
        fclose(fp);
        return 0;
    }

    // Step 3: Check if file starts with "ID3"
    if (strncmp((char *)header, "ID3", 3) != 0)
    {
        printf("No ID3 tag found\n");
        fclose(fp);
        return 0;
    }

    // Step 4: Calculate total tag size using synchsafe integer
    unsigned int tag_size = ((header[6] & 0x7F) << 21) |((header[7] & 0x7F) << 14) |((header[8] & 0x7F) << 7)  |(header[9] & 0x7F);

    // Tags to extract from MP3 metadata
    const char *TAGS[6] = {"TIT2", "TPE1", "TALB", "TYER", "TCON", "COMM"};

    unsigned int pos = 10; // Start after header
    while (pos < tag_size + 10)
    {
        // Step 5: Read 10-byte frame header
        unsigned char frame_header[10];
        if (fread(frame_header, 1, 10, fp) < 10)
            break;

        // Extract frame ID (4 bytes + null terminator)
        char frame_id[5];
        for (int i = 0; i < 4; i++) 
            frame_id[i] = frame_header[i];
        frame_id[4] = '\0';

        // Extract frame size (4-byte Big Endian integer)
        unsigned int frame_size = be_to_int(frame_header + 4);
        if (frame_size == 0) 
            break; // No data, stop parsing

        // Step 6: Allocate memory for frame data
        unsigned char *data = malloc(frame_size);
        if (!data) // Memory allocation failed
            break;

        // Step 7: Read frame data into buffer
        if (fread(data, 1, frame_size, fp) < frame_size)
        {
            free(data); // Free memory if read incomplete
            break;
        }

       //step 8: Loop through desired tags
       for (int i = 0; i < 6; i++)  
       {
        if (strcmp(frame_id, TAGS[i]) == 0)   // Check if frame ID matches a tag
        {
         if (strcmp(frame_id, "COMM") == 0)   // If it's a comment frame
            print_comm_frame(data, frame_size);  // Print comment frame
         else
            print_text_frame(TAGS[i], data, frame_size); // Print text frame
         break;   // Exit after processing matching tag
        }
       }

      free(data);            // Free frame data after use
      pos += 10 + frame_size; // Move position to next frame
    }

  fclose(fp);            // Close the source file
}