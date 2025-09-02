#ifndef MP3EDIT_H
#define MP3EDIT_H

#include "mp3types.h"

// Structure to hold MP3 file and edit info
typedef struct _Mp3EditInfo
{
    char *src_audio_fname;        
    FILE *fptr_src_audio;                    
    char temp_audio_fname[30];    
    FILE *fptr_temp_audio;                   
    int data_length;             
    char *modify_data;           
    uint size;                    
    char *frame;              
} Mp3EditInfo;

// Function declarations

/* Check the operation type (-v, -e, etc.) */
OperationType check_operation_type(char *argv);

/* Open source MP3 and temp file */
Status open_file(Mp3EditInfo *editInfo);

/* Validate command-line arguments */
Status read_and_validate_edit_args(char *argv[], Mp3EditInfo *editInfo);

/* Perform MP3 file editing */
Status do_edit(Mp3EditInfo *editInfo);

/* Modify a specific ID3 frame  */
Status do_change(Mp3EditInfo *editInfo, char str[], char frame[], int *flag);

/* Check if the frame exists in the MP3 file */
Status check_frame(Mp3EditInfo *editInfo, char str[]);

/* Convert byte order (endianness) */
void convert_endianess(char *ptr, uint size);

/* Modify the data inside the frame */
Status data_modify(Mp3EditInfo *editInfo);

/* Copy remaining data from source to temp file */
Status copy_remaining(FILE *fptr_dest, FILE *fptr_src);

/* Copy unchanged data from source to temp file */
Status copy_same(Mp3EditInfo *editInfo);

/* Copy MP3 header to temp file */
Status copy_header(FILE *fptr_dest, FILE *fptr_src);

/* Copy the temp file back to the original MP3 */
Status file_copy(Mp3EditInfo *editInfo);

#endif 
