#include<stdio.h>
#include<string.h>
#include"mp3edit.h"
#include"mp3types.h"
#include "mp3view.h"

/* Open the source and temporary MP3 files */
Status open_file(Mp3EditInfo *editInfo)
{
    editInfo->fptr_src_audio = fopen(editInfo->src_audio_fname, "r"); // Open source MP3 file
    strcpy(editInfo->temp_audio_fname, "temp.mp3"); // Set temporary filename
    if (editInfo->fptr_src_audio == NULL) 
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", editInfo->src_audio_fname);
        return e_failure; // Return failure 
    }
    editInfo->fptr_temp_audio = fopen(editInfo->temp_audio_fname, "w"); // Open temp MP3 file for writing
    if (editInfo->fptr_temp_audio == NULL) 
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", editInfo->temp_audio_fname);
        return e_failure; // Return failure
    }
}

/* Validate the edit arguments and check MP3 file validity */
Status read_and_validate_edit_args(char *argv[], Mp3EditInfo *editInfo)
{
    char extn[10];
    // Check if the file has a valid .mp3 extension
    if (strchr(argv[4], '.') == NULL || strcmp(strchr(argv[4], '.'), ".mp3") != 0)
    {
        printf("ERROR: INVALID EXTENSION\n");
        return e_failure;
    }

    // Validate the edit frame argument (-t, -a, -A, -m, -y, or -c)
    if (strcmp(argv[2], "-t") != 0 && strcmp(argv[2], "-a") != 0 && strcmp(argv[2], "-A") != 0 && 
        strcmp(argv[2], "-m") != 0 && strcmp(argv[2], "-y") != 0 && strcmp(argv[2], "-c") != 0)
    {
        printf("ERROR: INVALID ARGUMENTS\n");
        printf("To edit please pass like: ./a.out -e <Tag>(-t/-a/-A/-m/-y/-c) editing_text mp3filename\n");
        return e_failure;
    }

    // Assign the source filename and other arguments
    editInfo->src_audio_fname = argv[4];
    editInfo->frame = argv[2];
    editInfo->modify_data = argv[3];
    editInfo->data_length = strlen(editInfo->modify_data) + 1;

    // Open the MP3 file
    if (open_file(editInfo) == e_failure)
    {
        return e_failure;
    }

    // Check if the file has the ID3 tag and version 3
    char tag[4];
    fread(tag, 3, 1, editInfo->fptr_src_audio);
    tag[3] = 0;
    if (strcmp(tag, "ID3") != 0)
    {
        printf("The given audio does not have ID3 tag\n");
        return e_failure;
    }

    short version;
    fread(&version, 2, 1, editInfo->fptr_src_audio);
    if (version != 3)
    {
        printf("The given audio is not of version 3\n");
        return e_failure;
    }

    // Proceed with editing if everything is valid
    if (do_edit(editInfo) == e_failure)
    {
        return e_failure;
    }
    return e_success;
}

Status do_edit(Mp3EditInfo *editInfo)
{
    // Copy header from the original file to the temporary file
    if (copy_header(editInfo->fptr_temp_audio, editInfo->fptr_src_audio) == e_failure)
    {
        return e_failure;
    }

    int flag = 0;
    // Perform the change on different frames based on the operation
    if (do_change(editInfo, "TIT2", "-t", &flag) == e_failure || flag)
    {
        if (flag)
        {
            fclose(editInfo->fptr_src_audio);
            fclose(editInfo->fptr_temp_audio);
            file_copy(editInfo); // Copy the modified content to the original file
            Mp3ViewInfo mp3viewInfo;
            printf("------------------SELECTED EDIT DETAILS---------------------\n");
            mp3viewInfo.src_audio_fname = editInfo->src_audio_fname;
            mp3viewInfo.fptr_src_audio = fopen(mp3viewInfo.src_audio_fname, "r");
            if (mp3viewInfo.fptr_src_audio == NULL)
            {
                perror("fopen");
                return e_failure;
            }
            do_view(&mp3viewInfo);
            fclose(mp3viewInfo.fptr_src_audio);
            printf("------------------------------------------------------------\n");
            printf("               TITLE CHANGED SUCCESSFULLY\n");
            printf("------------------------------------------------------------\n");
            return e_success;
        }
        return e_failure;
    }

    if (do_change(editInfo, "TPE1", "-a", &flag) == e_failure || flag)
    {
        if (flag)
        {
            fclose(editInfo->fptr_src_audio);
            fclose(editInfo->fptr_temp_audio);
            file_copy(editInfo);
            printf("------------------SELECTED EDIT DETAILS---------------------\n");
            Mp3ViewInfo mp3viewInfo;
            mp3viewInfo.src_audio_fname = editInfo->src_audio_fname;
            mp3viewInfo.fptr_src_audio = fopen(mp3viewInfo.src_audio_fname, "r");
            if (mp3viewInfo.fptr_src_audio == NULL)
            {
                perror("fopen");
                return e_failure;
            }
            do_view(&mp3viewInfo);
            fclose(mp3viewInfo.fptr_src_audio);
            printf("------------------------------------------------------------\n");
            printf("                ARTIST CHANGED SUCCESSFULLY\n");
            printf("------------------------------------------------------------\n");
            return e_success;
        }
        return e_failure;
    }

    if (do_change(editInfo, "TALB", "-A", &flag) == e_failure || flag)
    {
        if (flag)
        {
            fclose(editInfo->fptr_src_audio);
            fclose(editInfo->fptr_temp_audio);
            file_copy(editInfo);
            printf("------------------SELECTED EDIT DETAILS---------------------\n");
            Mp3ViewInfo mp3viewInfo;
            mp3viewInfo.src_audio_fname = editInfo->src_audio_fname;
            mp3viewInfo.fptr_src_audio = fopen(mp3viewInfo.src_audio_fname, "r");
            if (mp3viewInfo.fptr_src_audio == NULL)
            {
                perror("fopen");
                return e_failure;
            }
            do_view(&mp3viewInfo);
            fclose(mp3viewInfo.fptr_src_audio);
            printf("------------------------------------------------------------\n");
            printf("               ALBUM CHANGED SUCCESSFULLY\n");
            printf("------------------------------------------------------------\n");
            return e_success;
        }
        return e_failure;
    }

    if (do_change(editInfo, "TYER", "-y", &flag) == e_failure || flag)
    {
        if (flag)
        {
            fclose(editInfo->fptr_src_audio);
            fclose(editInfo->fptr_temp_audio);
            file_copy(editInfo);
            printf("------------------SELECTED EDIT DETAILS---------------------\n");
            Mp3ViewInfo mp3viewInfo;
            mp3viewInfo.src_audio_fname = editInfo->src_audio_fname;
            mp3viewInfo.fptr_src_audio = fopen(mp3viewInfo .src_audio_fname, "r");
            if (mp3viewInfo.fptr_src_audio == NULL)
            {
                perror("fopen");
                return e_failure;
            }
            do_view(&mp3viewInfo);
            fclose(mp3viewInfo.fptr_src_audio);
            printf("------------------------------------------------------------\n");
            printf("               YEAR CHANGED SUCCESSFULLY\n");
            printf("------------------------------------------------------------\n");
            return e_success;
        }
        return e_failure;
    }

    if (do_change(editInfo, "TCON", "-m", &flag) == e_failure || flag)
    {
        if (flag)
        {
            fclose(editInfo->fptr_src_audio);
            fclose(editInfo->fptr_temp_audio);
            file_copy(editInfo);
            printf("------------------SELECTED EDIT DETAILS---------------------\n");
            Mp3ViewInfo mp3viewInfo;
            mp3viewInfo.src_audio_fname = editInfo->src_audio_fname;
            mp3viewInfo.fptr_src_audio = fopen(mp3viewInfo.src_audio_fname, "r");
            if (mp3viewInfo.fptr_src_audio == NULL)
            {
                perror("fopen");
                return e_failure;
            }
            do_view(&mp3viewInfo);
            fclose(mp3viewInfo.fptr_src_audio);
            printf("------------------------------------------------------------\n");
            printf("              CONTENT CHANGED SUCCESSFULLY\n");
            printf("------------------------------------------------------------\n");
            return e_success;
        }
        return e_failure;
    }

    if (do_change(editInfo, "COMM", "-c", &flag) == e_failure || flag)
    {
        if (flag)
        {
            fclose(editInfo->fptr_src_audio);
            fclose(editInfo->fptr_temp_audio);
            file_copy(editInfo);
            printf("------------------SELECTED EDIT DETAILS---------------------\n");
            Mp3ViewInfo mp3viewInfo;
            mp3viewInfo.src_audio_fname = editInfo->src_audio_fname;
            mp3viewInfo.fptr_src_audio = fopen(mp3viewInfo.src_audio_fname, "r");
            if (mp3viewInfo.fptr_src_audio == NULL)
            {
                perror("fopen");
                return e_failure;
            }
            do_view(&mp3viewInfo);
            fclose(mp3viewInfo.fptr_src_audio);
            printf("------------------------------------------------------------\n");
            printf("            COMMENT CHANGED SUCCESSFULLY\n");
            printf("------------------------------------------------------------\n");
            return e_success;
        }
        return e_failure;
    }

    return e_success; // Return success if no changes were made
}
/* Perform the frame check and apply changes */
Status do_change(Mp3EditInfo *editInfo, char str[], char frame[], int *flag)
{
    // Check if the current frame matches
    if (check_frame(editInfo, str) == e_failure)
    {
        printf("Frame Matching Error\n");
        return e_failure;
    }

    // If the frame matches, modify data and copy it to the temp file
    if (strcmp(editInfo->frame, frame) == 0)
    {
        data_modify(editInfo);
        fseek(editInfo->fptr_src_audio, editInfo->size - 1, SEEK_CUR);
        copy_remaining(editInfo->fptr_temp_audio, editInfo->fptr_src_audio);
        *flag = 1;
        return e_success;
    }
    // Otherwise, copy the frame without modification
    if (copy_same(editInfo) == e_failure)
    {
        return e_failure;
    }
    return e_success;
}

/* Check if the frame matches in the MP3 file */
Status check_frame(Mp3EditInfo *editInfo, char str[])
{
    char buffer[4];
    fread(buffer, 4, 1, editInfo->fptr_src_audio); // Read the frame tag
    if (strcmp(buffer, str) != 0)
    {
        return e_failure;
    }
    fwrite(buffer, 4, 1, editInfo->fptr_temp_audio); // Write to temp file

    fread(&editInfo->size, 4, 1, editInfo->fptr_src_audio); // Read the frame size
    convert_endianess((char *)&editInfo->size, sizeof(int)); // Convert endianess
    return e_success;
}

/* Convert the byte order (endianess) of the data */
void convert_endianess(char *ptr, uint size)
{
    char temp;
    for (int i = 0; i < size / 2; i++)
    {
        temp = ptr[i];
        ptr[i] = ptr[size - i - 1];
        ptr[size - i - 1] = temp;
    }
}

/* Modify the data and write to the temp file */
Status data_modify(Mp3EditInfo *editInfo)
{
    convert_endianess((char *)&editInfo->data_length, sizeof(int));
    fwrite(&editInfo->data_length, 4, 1, editInfo->fptr_temp_audio);
    char flag_buffer[3];
    fread(flag_buffer, 3, 1, editInfo->fptr_src_audio);
    fwrite(flag_buffer, 3, 1, editInfo->fptr_temp_audio);
    fwrite(editInfo->modify_data, strlen(editInfo->modify_data), 1, editInfo->fptr_temp_audio);
    return e_success;
}

/* Copy remaining data from the source to the temp file */
Status copy_remaining(FILE *fptr_dest, FILE *fptr_src)
{
    char ch;
    while (fread(&ch, 1, 1, fptr_src) > 0)
    {
        fwrite(&ch, 1, 1, fptr_dest);
    }
    return e_success;
}

Status copy_same(Mp3EditInfo *editInfo)
{
    uint size=editInfo->size;
    //convert
    convert_endianess((char *)&size, sizeof(int));
    fwrite(&size,4,1,editInfo->fptr_temp_audio);
    char flag_buffer[3];
    fread(flag_buffer,3,1,editInfo->fptr_src_audio);
    fwrite(flag_buffer,3,1,editInfo->fptr_temp_audio);
    //convert size
    char buffer[editInfo->size];
    fread(buffer,editInfo->size-1,1,editInfo->fptr_src_audio);
    fwrite(buffer,editInfo->size-1,1,editInfo->fptr_temp_audio);
    return e_success;

}

Status copy_header(FILE *fptr_dest,FILE *fptr_src)
{
    rewind(fptr_dest);
    rewind(fptr_src);
    char header[10];
    fread(header,10,1,fptr_src);
    fwrite(header,10,1,fptr_dest);
    return e_success;
}

Status file_copy(Mp3EditInfo *editInfo)
{
    editInfo->fptr_src_audio = fopen(editInfo->src_audio_fname,"w");
    editInfo->fptr_temp_audio = fopen(editInfo->temp_audio_fname,"r");
    rewind(editInfo->fptr_src_audio);
    rewind(editInfo->fptr_temp_audio);
    char ch;
    while( fread(&ch,1,1,editInfo->fptr_temp_audio) > 0)
    {
        fwrite(&ch,1,1,editInfo->fptr_src_audio);
    }
    return e_success;
}
