/*
    Name : Mohanapriya K
    Date : 17.9.2025
    Description : An MP3 metadata management project that allows viewing, editing song information.
*/

#include <stdio.h>      
#include <string.h>     
#include <stdlib.h>     
#include"mp3.h"

// Main function starts
int main(int argc, char *argv[])
{
    // Step 1: Validate minimum arguments
    if (argc < 3)
    {
        printf("Invalid input\n");
        printf("USAGE for viewing: ./a.out -v <something.mp3>\n");
        printf("USAGE for editing: ./a.out -e <edit_tag> <edit_name> <something.mp3>\n");
        return 0;  
    }

    // Step 2: Check if option is "-v" for View mode
    if (strcmp(argv[1], "-v") == 0)
    {
        // Ensure exactly 3 arguments for view mode
        if (argc != 3)
        {
            printf("USAGE: ./a.out -v <something.mp3>\n");
            return 0;  
        }

        // Call view function and return its status
        return view_mp3(argv[2]);
    }

    // Step 3: Check if option is "-e" → Edit mode
    else if (strcmp(argv[1], "-e") == 0)
    {
        // Ensure exactly 5 arguments for edit mode
        if (argc != 5)
        {
            printf("USAGE: ./a.out -e <edit_tag> <edit_name> <something.mp3>\n");
            return 0;   
        }

        // Call edit function and return its status
        return edit_mp3(argv[2], argv[3], argv[4]);
    }

    // Handle invalid options
    else
    {
        printf("Invalid option! Use -v (view) or -e (edit)\n");
        return 0;  
    }
    return 0;
}