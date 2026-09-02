#ifndef DECODE_H
#define DECODE_HCODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * DEcoding secret file FROM stego Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _decodeInfo
{
   
     /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

    /* Secret File Info */
    char *secret_fname;
    FILE *fptr_secret;
    char extn_secret_file[MAX_FILE_SUFFIX];
    char secret_data[MAX_SECRET_BUF_SIZE];
    long size_secret_file;
    int extn_size;

   

} decodeInfo;

Status do_decoding(decodeInfo*decInfo);
Status open_files2(decodeInfo*decInfo);
Status check_magicstring(decodeInfo*decInfo);
int decode_size_from_lsb(char * buffer);
char decode_byte_from_lsb(char * buffer);
Status decode_secretfile_extn_size(decodeInfo*decInfo);
Status decode_secretfile_extn(decodeInfo*decInfo);
Status decode_secretfile_size(decodeInfo*decInfo);
Status decode_secretfile_data(decodeInfo*decInfo);

#endif