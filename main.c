/* Name: Rajab J Sunkad
   Project: MP3 Tag reader/editor
   Description: An MP3 tag reader is a software tool that allows you to view and 
                edit the metadata associated with MP3 files. 
                This metadata is stored in the form of "tags" within the MP3 file
*/
#include <stdio.h>
#include <string.h>
#include "mp3view.h"
#include "mp3types.h"
#include "mp3edit.h"

int argcnt;
int main(int argc, char *argv[])
{
    Mp3ViewInfo mp3viewInfo;  // Declare variable for viewing MP3 file info
    Mp3EditInfo editInfo;      // Declare variable for editing MP3 file info

    argcnt = argc;
    // Validate argument count first
    if (argc < 2) 
    {
        printf("---------------------------------------------------------------------------------------\n");
        printf("ERROR: ./a.out : INVALID ARGUMENTS \n");
        printf("USAGE :\n");
        printf("To view please pass: ./a.out -v mp3filename\n");
        printf("To edit please pass: ./a.out -e <Tag>(-t/-a/-A/-m/-y/-c) editing_text mp3filename\n");
        printf("To get help pass like: ./a.out --help \n");
        printf("---------------------------------------------------------------------------------------\n");
        return e_failure;
    }

    // Check the operation type passed in the command line argument (view or edit)
    if (check_operation_type(argv[1]) == e_view)
    {
        // For view operation, ensure we have exactly 3 arguments
        if (argc < 3)
        {
            printf("------------------------------------------------\n");
            printf("ERROR: ./a.out : INVALID ARGUMENTS \n");
            printf("USAGE :\n");
            printf("To view please pass like: ./a.out -v mp3filename\n");
            printf("-------------------------------------------------\n");
            return e_failure;  // Exit if arguments are incorrect
        }

        // Assign the MP3 filename passed in the command line argument
        mp3viewInfo.src_audio_fname = argv[2];

        // Try to open the MP3 file in read mode
        mp3viewInfo.fptr_src_audio = fopen(mp3viewInfo.src_audio_fname, "r");

        // Error handling: If file can't be opened, print error and exit
        if (mp3viewInfo.fptr_src_audio == NULL)
        {
            fprintf(stderr, "ERROR: Invalid Extension or Unable to open file %s\n", mp3viewInfo.src_audio_fname);
            return e_failure;
        }

        // Read and validate view-specific arguments (such as what to view in the MP3 file)
        if (read_and_validate_view_args(argv, &mp3viewInfo) == e_failure)
        {
            fclose(mp3viewInfo.fptr_src_audio);  // Close file before returning
            return e_failure;  // Exit if arguments are invalid for viewing
        }

        // Print header for view details
        printf("---------------------------------------------------------\n");
        printf("            MP3 TAG READER AND EDITOR FOR ID3v2         \n");
        printf("---------------------------------------------------------\n");
        printf("---------------> SELECTED VIEW DETAILS <-----------------\n");

        // Perform the view operation (display MP3 file details)
        if (do_view(&mp3viewInfo) == e_success)
        {
            printf("--------------DETAILS DISPLAYED SUCCESSFULLY--------------\n");
        }

        // Close the MP3 file after viewing
        fclose(mp3viewInfo.fptr_src_audio);
    }
    // If the operation type is "edit"
    else if (check_operation_type(argv[1]) == e_edit)
    {
        // If there are not enough arguments for editing, print usage error and exit
        if (argc < 5)
        {
            printf("---------------------------------------------------------------------------------------------\n");
            printf("ERROR: ./a.out : INVALID ARGUMENTS\n");
            printf("USAGE :To edit please pass like: ./a.out -e <Tag>(-t/-a/-A/-m/-y/-c) editing_text mp3filename\n");
            printf("---------------------------------------------------------------------------------------------\n");
            return e_failure;
        }

        // Read and validate editing arguments (for different editing options like title, artist, etc.)
        if (read_and_validate_edit_args(argv, &editInfo) == e_failure)
        {
            return e_failure;  // Exit if edit arguments are invalid
        }
    }
    // If the operation type is "help" (display help menu)
    else if (check_operation_type(argv[1]) == e_help)
    {
        // Print the help menu with available options for viewing and editing
        printf("---------------------------------HELP MENU---------------------------------\n");
        printf("---------------------------------------------------------------------------\n");
        printf("Prefix       Tags              Functions\n");
        printf("---------------------------------------------------------------------------\n");
        printf(" -v     ->    --     ->      To view mp3 file contents\n");
        printf(" -e     ->    --     ->      To edit mp3 file contents\n");
        printf("        ->    -t     ->      To edit song title\n");
        printf("        ->    -a     ->      To edit artist name\n");
        printf("        ->    -A     ->      To edit album name\n");
        printf("        ->    -y     ->      To edit year\n");
        printf("        ->    -m     ->      To edit content(Music)\n");
        printf("        ->    -c     ->      To edit comment\n");
        printf("---------------------------------------------------------------------------\n\n");
    }
    else
    {
        // If an unsupported operation is provided, print an error message
        // printf("Invalid operation\n");
        printf("------------------------------------------------\n");
        printf("ERROR: ./a.out : INVALID ARGUMENTS \n");
        printf("USAGE :\n");
        printf("To view please pass like: ./a.out -v mp3filename\n");
        printf("To edit please pass: ./a.out -e <Tag>(-t/-a/-A/-m/-y/-c) editing_text mp3filename\n");
        printf("To get help pass like: ./a.out --help \n");
        printf("-------------------------------------------------\n");
        return e_failure;
    }

    return e_success;
}

/* Function to check the operation type based on user input */
OperationType check_operation_type(char* argv)
{
    // If the user passed "-v", return "view" operation type
    if (strcmp(argv, "-v") == 0)
    {
        if(argcnt == 3)
        return e_view;
    }
    // If the user passed "-e", return "edit" operation type
    else if (strcmp(argv, "-e") == 0)
    {
        return e_edit;
    }
    // If the user passed anything else, return "help" operation type
    else
    {
        return e_help;
    }
}
