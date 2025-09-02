#ifndef MP3VIEW_H
#define MP3VIEW_H

#include "mp3types.h"

typedef struct _Mp3ViewInfo
{
    /*audio information*/
    char *src_audio_fname;
    FILE *fptr_src_audio;
    int title_size;
    int artist_size;
    int album_size;
    int year_size;
    int music_size;
    int comment_size;
}Mp3ViewInfo;


/* Check operation type */
OperationType check_operation_type(char *argv);

/* Read and validate Encode args from argv */
Status read_and_validate_view_args(char *argv[], Mp3ViewInfo *mp3viewInfo);

/* Perform the encoding */
Status do_view(Mp3ViewInfo *mp3viewInfo);

/* Get File pointers for i/p and o/p files */
Status open_files(Mp3ViewInfo *mp3viewInfo);

/* Skip 10 bytes of audio header */
Status skip_audio_header(FILE *fptr_src_audio);

Status title_tag(Mp3ViewInfo *mp3viewInfo);
Status title_size(Mp3ViewInfo *mp3viewInfo);
Status skip_flag(FILE* fptr_src_audio);

/*Function to get the title of the song */
Status title(Mp3ViewInfo *mp3viewInfo);

Status artist_tag(Mp3ViewInfo *mp3viewInfo);
Status artist_size(Mp3ViewInfo *mp3viewInfo);
/*Function to get the artist name */
Status artist(Mp3ViewInfo *mp3viewInfo);


Status album_tag(Mp3ViewInfo *mp3viewInfo);
Status album_size(Mp3ViewInfo *mp3viewInfo);
/*Function to get the album */
Status album(Mp3ViewInfo *mp3viewInfo);


Status year_tag(Mp3ViewInfo *mp3viewInfo);
Status year_size(Mp3ViewInfo *mp3viewInfo);
/*Function to get the year */
Status year(Mp3ViewInfo *mp3viewInfo);


Status music_tag(Mp3ViewInfo *mp3viewInfo);
Status music_size(Mp3ViewInfo *mp3viewInfo);

/*Function to get the music */
Status music(Mp3ViewInfo *mp3viewInfo);

Status comment_tag(Mp3ViewInfo *mp3viewInfo);
Status comment_size(Mp3ViewInfo *mp3viewInfo);

/*Function to get the comment */
Status comment(Mp3ViewInfo *mp3viewInfo);


#endif







