#include<stdio.h>
#include<string.h>
#include"mp3view.h"
#include"mp3types.h"

// Open the MP3 file for reading
Status read_and_validate_view_args(char *argv[], Mp3ViewInfo *mp3viewInfo)
{
    char tag[3];
    if (fread(tag, 1, 3, mp3viewInfo->fptr_src_audio) < 0) 
    {
        perror("Unable to read file!");
        return e_failure;
    }

    // Check if it is an ID3 tag
    if (tag[0] != 'I' && tag[1] != 'D' && tag[2] != '3') 
    {
        return e_failure;
    } 
    
    // Check for version
    char version[2];
    if (fread(version, 2, 2, mp3viewInfo->fptr_src_audio) < 0) 
    {
        perror("Unable to read file!");
        return e_failure;
    }
    if (version[0] != 0x03 && version[1] != 0x00)
    {
        printf("Not version 3\n");
        return e_failure;
    }

    // Rewind to the beginning of the file
    rewind(mp3viewInfo->fptr_src_audio);
    return e_success;
}

/* Skip 10 bytes of audio header */
Status skip_audio_header(FILE *fptr_src_audio)
{
    /*setting offset at the 10th byte using fseek*/
    fseek(fptr_src_audio,10,SEEK_SET);
    /*return e_success*/
    return e_success;
}

Status do_view(Mp3ViewInfo *mp3viewInfo)
{
    /* Open file function call */
    if (open_files(mp3viewInfo) == e_failure)
    {
        return e_failure;
    }

    /* Skip audio header function call */
    if (skip_audio_header(mp3viewInfo->fptr_src_audio) == e_failure)
    {
        return e_failure;
    }

    if (title_tag(mp3viewInfo) == e_failure)
    {
        return e_failure;
    }

    if (artist_tag(mp3viewInfo) == e_failure)
    {
        return e_failure;
    }

    if (album_tag(mp3viewInfo) == e_failure)
    {
        return e_failure;
    }

    if (year_tag(mp3viewInfo) == e_failure)
    {
        return e_failure;
    }

    if (music_tag(mp3viewInfo) == e_failure)
    {
        return e_failure;
    }

    if (comment_tag(mp3viewInfo) == e_failure)
    {
        return e_failure;
    }
    return e_success;
}

Status open_files(Mp3ViewInfo *mp3viewInfo)
{
    mp3viewInfo->fptr_src_audio = fopen(mp3viewInfo->src_audio_fname, "r");
    if (mp3viewInfo->fptr_src_audio == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR:Invalid Extension or Unable to open file %s\n", mp3viewInfo->src_audio_fname);
        return e_failure;
    }
    return e_success;
}

Status title_tag(Mp3ViewInfo *mp3viewInfo)
{
    char title_buffer[4];
        
    if(fread(title_buffer,1,4,mp3viewInfo->fptr_src_audio)!=4)
    {
        printf("Unable to read the tag");
        return e_failure;
    }
    //printf("%s\n",title_buffer);
    if(title_size(mp3viewInfo)==e_failure)
    {
        return e_failure;
    }
    if(skip_flag(mp3viewInfo->fptr_src_audio)==e_failure)
    {
        return e_failure;
    }
    if(title(mp3viewInfo)==e_failure)
    {
        return e_failure;
    }
 return e_success;
}

Status title_size(Mp3ViewInfo *mp3viewInfo)
{
    unsigned char size_bytes[4];
    if (fread(size_bytes, 1, 4, mp3viewInfo->fptr_src_audio) != 4) 
    {
        perror("Unable to read tag size");
        return e_failure;
    }
    //convert to little endian
    int tag_size = (size_bytes[0] << 24) | (size_bytes[1] << 16) | (size_bytes[2] << 8) | size_bytes[3];
    mp3viewInfo->title_size=tag_size;
    return e_success;
}

Status skip_flag(FILE* fptr_src_audio)
{
    //skip 3 bytes
   fseek(fptr_src_audio, 3, SEEK_CUR);
    return e_success;
}
// Main function to view MP3 details
Status title(Mp3ViewInfo *mp3viewInfo)
{
    int i;
    char title[mp3viewInfo->title_size];
    for(i=0;i<(mp3viewInfo->title_size-1);i++)
    {
        fread(&title[i],1,1,mp3viewInfo->fptr_src_audio);
    }
    title[i]='\0';
    printf("TITLE   : %s\n",title);
}

Status artist_tag(Mp3ViewInfo *mp3viewInfo)
{
    char artist_buffer[4];  
    if(fread(artist_buffer,1,4,mp3viewInfo->fptr_src_audio)!=4)
    {
        printf("Unable to read the tag");
        return e_failure;
    }
    if(artist_size(mp3viewInfo)==e_failure)
    {
        return e_failure;
    }
    if(skip_flag(mp3viewInfo->fptr_src_audio)==e_failure)
    {
        return e_failure;
    }
    if(artist(mp3viewInfo)==e_failure)
    {
        return e_failure;
    }
 return e_success;
}
// Get and convert the artist tag size
Status artist_size(Mp3ViewInfo *mp3viewInfo)
{
    unsigned char size_bytes[4];
    if (fread(size_bytes, 1, 4, mp3viewInfo->fptr_src_audio) != 4) 
    {
        perror("Unable to read tag size");
        return e_failure;
    }
    //convert to big endian
    int tag_size = (size_bytes[0] << 24) | (size_bytes[1] << 16) | (size_bytes[2] << 8) | size_bytes[3];
    mp3viewInfo->artist_size=tag_size;
    return e_success;
}

// Read and print the artist string based on the size
Status artist(Mp3ViewInfo *mp3viewInfo)
{
    int i;
    char artist[mp3viewInfo->artist_size];
    for(i=0;i<(mp3viewInfo->artist_size-1);i++)
    {
        fread(&artist[i],1,1,mp3viewInfo->fptr_src_audio);
    }
    artist[i]='\0';
    printf("ARTIST  : %s\n",artist);
}

// Read and print the album tag
Status album_tag(Mp3ViewInfo *mp3viewInfo)
{
    char album_buffer[4];
        
    if(fread(album_buffer,1,4,mp3viewInfo->fptr_src_audio)!=4)
    {
        printf("Unable to read the tag");
        return e_failure;
    }
    if(album_size(mp3viewInfo)==e_failure)
    {
        return e_failure;
    }
    if(skip_flag(mp3viewInfo->fptr_src_audio)==e_failure)
    {
        return e_failure;
    }
    if(album(mp3viewInfo)==e_failure)
    {
        return e_failure;
    }
 return e_success;
}

Status album_size(Mp3ViewInfo *mp3viewInfo)
{
    unsigned char size_bytes[4];
    if (fread(size_bytes, 1, 4, mp3viewInfo->fptr_src_audio) != 4) 
    {
        perror("Unable to read tag size");
        return e_failure;
    }
    //convert to big endian
    int tag_size = (size_bytes[0] << 24) | (size_bytes[1] << 16) | (size_bytes[2] << 8) | size_bytes[3];
    mp3viewInfo->album_size=tag_size;
    return e_success;
}


Status album(Mp3ViewInfo *mp3viewInfo)
{
    int i;
    char album[mp3viewInfo->album_size];
    for(i=0;i<(mp3viewInfo->album_size-1);i++)
    {
        fread(&album[i],1,1,mp3viewInfo->fptr_src_audio);
    }
    album[i]='\0';
    printf("ALBUM   : %s\n",album);
}

Status year_tag(Mp3ViewInfo *mp3viewInfo)
{
    char year_buffer[4];
        
    if(fread(year_buffer,1,4,mp3viewInfo->fptr_src_audio)!=4)
    {
        printf("Unable to read the tag");
        return e_failure;
    }
    if(year_size(mp3viewInfo)==e_failure)
    {
        return e_failure;
    }
    if(skip_flag(mp3viewInfo->fptr_src_audio)==e_failure)
    {
        return e_failure;
    }
    if(year(mp3viewInfo)==e_failure)
    {
        return e_failure;
    }
 return e_success;
}

Status year_size(Mp3ViewInfo *mp3viewInfo)
{
    unsigned char size_bytes[4];
    if (fread(size_bytes, 1, 4, mp3viewInfo->fptr_src_audio) != 4) 
    {
        perror("Unable to read tag size");
        return e_failure;
    }
    //convert to big endian
    int tag_size = (size_bytes[0] << 24) | (size_bytes[1] << 16) | (size_bytes[2] << 8) | size_bytes[3];
    mp3viewInfo->year_size=tag_size;
    return e_success;
}


Status year(Mp3ViewInfo *mp3viewInfo)
{
    int i;
    char year[mp3viewInfo->year_size];
    for(i=0;i<(mp3viewInfo->year_size-1);i++)
    {
        fread(&year[i],1,1,mp3viewInfo->fptr_src_audio);
    }
    year[i]='\0';
    printf("YEAR    : %s\n",year);
}


Status music_tag(Mp3ViewInfo *mp3viewInfo)
{
    char music_buffer[4];
        
    if(fread(music_buffer,1,4,mp3viewInfo->fptr_src_audio)!=4)
    {
        printf("Unable to read the tag");
        return e_failure;
    }
    if(music_size(mp3viewInfo)==e_failure)
    {
        return e_failure;
    }
    if(skip_flag(mp3viewInfo->fptr_src_audio)==e_failure)
    {
        return e_failure;
    }
    if(music(mp3viewInfo)==e_failure)
    {
        return e_failure;
    }
 return e_success;
}

Status music_size(Mp3ViewInfo *mp3viewInfo)
{
    unsigned char size_bytes[4];
    if (fread(size_bytes, 1, 4, mp3viewInfo->fptr_src_audio) != 4) 
    {
        perror("Unable to read tag size");
        return e_failure;
    }
    //convert to big endian
    int tag_size = (size_bytes[0] << 24) | (size_bytes[1] << 16) | (size_bytes[2] << 8) | size_bytes[3];
    mp3viewInfo->music_size=tag_size;
    return e_success;
}


Status music(Mp3ViewInfo *mp3viewInfo)
{
    int i;
    char music[mp3viewInfo->music_size];
    for(i=0;i<(mp3viewInfo->music_size-1);i++)
    {
        fread(&music[i],1,1,mp3viewInfo->fptr_src_audio);
    }
    music[i]='\0';
    printf("MUSIC   : %s\n",music);
}


Status comment_tag(Mp3ViewInfo *mp3viewInfo)
{
    char comment_buffer[4];
        
    if(fread(comment_buffer,1,4,mp3viewInfo->fptr_src_audio)!=4)
    {
        printf("Unable to read the tag");
        return e_failure;
    }
    if(comment_size(mp3viewInfo)==e_failure)
    {
        return e_failure;
    }
    if(skip_flag(mp3viewInfo->fptr_src_audio)==e_failure)
    {
        return e_failure;
    }
    if(comment(mp3viewInfo)==e_failure)
    {
        return e_failure;
    }
 return e_success;
}

Status comment_size(Mp3ViewInfo *mp3viewInfo)
{
    unsigned char size_bytes[4];
    if (fread(size_bytes, 1, 4, mp3viewInfo->fptr_src_audio) != 4) 
    {
        perror("Unable to read tag size");
        return e_failure;
    }
    //convert to big endian
    int tag_size = (size_bytes[0] << 24) | (size_bytes[1] << 16) | (size_bytes[2] << 8) | size_bytes[3];
    mp3viewInfo->comment_size=tag_size;
    return e_success;
}

Status comment(Mp3ViewInfo *mp3viewInfo)
{
    int i;
    char comment[mp3viewInfo->comment_size];
    for(i=0;i<(mp3viewInfo->comment_size-1);i++)
    {
        fread(&comment[i],1,1,mp3viewInfo->fptr_src_audio);
    }
    comment[i]='\0';
    printf("COMMENT : %s\n",comment);
}
