#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include "common.h"
#include "ui.h"

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
    printf("\nwidth = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    rewind(fptr_image);
    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}
Status check_capacity(EncodeInfo *encInfo)
{
    encInfo->image_capacity=get_image_size_for_bmp(encInfo->fptr_src_image);
    encInfo->size_secret_file=get_file_size(encInfo->fptr_secret);
    if((encInfo->image_capacity)>(54+16+32+32+32+(encInfo->size_secret_file*8)))
    {
        return e_success;
    }
    else
    {
        printf("no capactity to encode\n");
        return e_failure;
    }
}

Status do_encoding(EncodeInfo *encInfo)
{
    if((open_files(encInfo))==e_success)
    {
        if(check_capacity(encInfo)==e_success)
       { 
            printf(GREEN "\n✓ File opened successfully\n" RESET);
            if((copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image))==e_success)
            {
                printf(GREEN "✓ Header copied successfully\n" RESET);
                if(encode_magic_string(MAGIC_STRING,encInfo)==e_success)
                {
                    printf(GREEN "✓ Magic string is Encoded successfully\n" RESET);
                    if(encode_secret_file_extn_size(encInfo)==e_success)
                    {
                        printf(GREEN "✓ Secret file extn size Encoded successfully\n" RESET);
                        if(encode_secret_file_extn(".txt",encInfo)==e_success)
                        {
                            printf(GREEN "✓ Secret file extn  Encoded successfully\n" RESET);
                            if(encode_secret_file_size(encInfo->size_secret_file,encInfo)==e_success)
                            {
                                printf(GREEN "✓ Secret file size Encoded successfully\n" RESET );
                                if(encode_secret_file_data(encInfo)==e_success)
                                {
                                    printf(GREEN "✓ Secret file data Encoded successfully\n" RESET);
                                    if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
                                    {
                                        printf(GREEN "✓ remaining data copied successfully\n" RESET);
                                        return e_success;
                                    }
                                    else
                                    {
                                        printf(RED "✘ Failed to copy remaining to stego file\n" RESET);
                                        return e_failure;
                                    }

                                }
                                else
                                {
                                    printf(RED "✘ failed to encode secret file data\n" RESET);
                                    return e_failure;
                                }

                            }
                            else
                            {
                                printf(RED "✘ Failed to encode secret file size \n" RESET);
                                return e_failure;
                            }
                        }
                        else
                        {
                            printf("✘ failed to encode secret file extension\n");
                            return e_failure;

                        }
                    }
                    else
                    {
                        printf("✘ failed to encode secret file extension size \n");
                        return e_failure;

                    }
                    
                }
                else
                {
                    printf("✘ Failed to encode magic string \n");
                    return e_failure;
                }
            }
            else
            {
                printf("✘ Failed to copy BMP header\n");
                return e_failure;
            }
        }
    }
    else
    {
        printf("✘ failed to open files\n");
        return e_failure;
    }
}

uint get_file_size(FILE *fptr)
{
    fseek(fptr,0,SEEK_END);
    uint size=ftell(fptr);
    fseek(fptr,0,SEEK_SET);
    return size;

}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    fseek(fptr_src_image,0,SEEK_SET);
    char buff[54];
    fseek(fptr_src_image,0,SEEK_SET);
    if(fread(buff,54,1,fptr_src_image)!=1)
    {
        return e_failure;
    }
    if(fwrite(buff,54,1,fptr_dest_image)!=1)
    {
        return e_failure;
    }
    return e_success;
}

/* Store Magic String */
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    char buff[8];
    for(int i=0;magic_string[i]!='\0';i++)
    {
        if(fread(buff,8,1,encInfo->fptr_src_image)!=1)
        {
            return e_failure;
        }
        encode_byte_to_lsb(magic_string[i], buff);

        if(fwrite(buff,8,1,encInfo->fptr_stego_image)!=1)
        {
            return e_failure;
        }
    }

    return e_success;
}

/* Encode a byte into LSB of image data array */
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for(int i=0;i<8;i++)
    {
        image_buffer[i]=((image_buffer[i] & 0xFE) | (data >> i) & 1);
    }
}

Status encode_secret_file_extn_size(EncodeInfo *encInfo)
{
    char buff[32];
    if(fread(buff,32,1,encInfo->fptr_src_image)!=1)
    {
        return e_failure;
    }
    for(int i=0;i<32;i++)
    {
        buff[i]=((buff[i] & 0xFE) | (4>>i) & 1);
    }
    if(fwrite(buff,32,1,encInfo->fptr_stego_image)!=1)
    {
        return e_failure;
    }
    return e_success;
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    char buff[8];
    
    for(int i=0;file_extn[i]!='\0';i++)
    {
        if(fread(buff,8,1,encInfo->fptr_src_image)!=1)
        {
            return e_failure;
        }
        encode_byte_to_lsb(file_extn[i], buff);

        if(fwrite(buff,8,1,encInfo->fptr_stego_image)!=1)
        {
            return e_failure;
        }
    }

    return e_success;
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char buff[32];
    if(fread(buff,32,1,encInfo->fptr_src_image)!=1)
    {
        return e_failure;
    }
    for(int i=0;i<32;i++)
    {
        buff[i]=((buff[i]&0xFE)|(file_size>>i)&1);
    }
    if(fwrite(buff,32,1,encInfo->fptr_stego_image)!=1)
    {
        return e_failure;
    }
    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char buff[8];
    
    for(int i=0;i<(encInfo->size_secret_file);i++)
    {
        if(fread(buff,8,1,encInfo->fptr_src_image)!=1)
        {
            return e_failure;
        }
        encode_byte_to_lsb(getc(encInfo->fptr_secret), buff);

        if(fwrite(buff,8,1,encInfo->fptr_stego_image)!=1)
        {
            return e_failure;
        }
    }

    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    int ch;
    char buff;
    while((ch=fgetc(fptr_src))!=EOF)
    {
        if(fputc(ch,fptr_dest)==EOF)
        {
            return e_failure;

        }

    }
    return e_success;
}






