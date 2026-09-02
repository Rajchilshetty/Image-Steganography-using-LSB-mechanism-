#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "types.h"
#include "common.h"
#include "ui.h"

Status do_decoding(decodeInfo*decInfo)
{
    if(open_files2(decInfo)!=e_success)
    {
        printf(RED "✘ unable to open files\n"RESET);
    }
    else
    {
        printf(GREEN "\n✓ Files Opened Successfully\n" RESET);
    }
    
    if(check_magicstring(decInfo)!=e_success)
    {
        printf(RED "✘ Magic string not found,no data to encode in the given file\n" RESET);
        return e_failure;
    }
     else
    {
        printf(GREEN "✓ Magic string found,decoding data\n" RESET);
    }
    
    if(decode_secretfile_extn_size(decInfo)!=e_success)
    {
        printf(RED "✘ unable to decode secret file extn size\n" RESET);
        return e_failure;
    }
     else
    {
        printf(GREEN"✓ Secretfile extension size decoded Successfully\n" RESET);
    }
    
    if(decode_secretfile_extn(decInfo)!=e_success)
    {
        printf(RED "✘ unable to decode secret file extn\n" RESET);
        return e_failure;
    }
     else
    {
        printf(GREEN "✓ Secretfile extension decoded Successfully\n" RESET);
    }
    
    if(decode_secretfile_size(decInfo)!=e_success)
    {
        printf(RED "✘ unable to decode secret file size\n" RESET);
        return e_failure;
    }
     else
    {
        printf(GREEN "✓ Secretfile size decoded Successfully\n" RESET);
    }
    
    if(decode_secretfile_data(decInfo)!=e_success)
    {
        printf(RED "✘ unable to decode secret file data\n" RESET);
        return e_failure;
    }
     else
    {
        printf(GREEN "✓ Secretfile Data decoded Successfully\n" RESET);
    }
    

    return e_success;

}
Status open_files2(decodeInfo*decInfo)
{
    // Stego Image file
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    // Do Error handling
    if (decInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n",decInfo->stego_image_fname);

    	return e_failure;
    }

    // Secret file
    decInfo->fptr_secret = fopen(decInfo->secret_fname, "w");
    // Do Error handling
    if (decInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->secret_fname);

    	return e_failure;
    }

    

    // No failure return e_success
    return e_success;
}

//to check magic string is present or not
Status check_magicstring(decodeInfo*decInfo)
{   //set the pointer to 54 byte in order to bypass the header file 
    fseek(decInfo->fptr_stego_image,54,SEEK_SET);
    char buff[8];
    char ch[3];
    for(int i=0;i<2;i++)
    {
        if(fread(buff,8,1,decInfo->fptr_stego_image)!=1)
        {
            return e_failure;
        }
        ch[i]=decode_byte_from_lsb(buff);
    }
    ch[2]='\0';
    if(strcmp(ch,MAGIC_STRING))
    {
        return e_failure;
    }
    return e_success;
}

int decode_size_from_lsb(char * buffer)
{
    int x=0;
    for(int i=0;i<32;i++)
    {
        x|=((buffer[i] & 1) << i);

    }
    return x;
}
char decode_byte_from_lsb(char * buffer)
{
    unsigned char ch=0;
    for(int i=0;i<8;i++)
    {
        ch|=((buffer[i]&1)<<i);
    }
    return ch;
}
Status decode_secretfile_extn_size(decodeInfo*decInfo)
{
    char buff[32];
    int ch;
    if(fread(buff,32,1,decInfo->fptr_stego_image)!=1)
        {
            return e_failure;
        }
    ch=decode_size_from_lsb(buff);
    decInfo->extn_size=ch;
    printf(YELLOW "%d " RESET GREEN "is the size of secret file extension\n" RESET,ch);
    return e_success;
}

Status decode_secretfile_extn(decodeInfo*decInfo)
{   
    char buff[8];
    char ch[decInfo->extn_size+1];
    for(int i=0;i<(decInfo->extn_size);i++)
    {
        if(fread(buff,8,1,decInfo->fptr_stego_image)!=1)
        {
            return e_failure;
        }
        ch[i]=decode_byte_from_lsb(buff);
    }
    ch[decInfo->extn_size]='\0';
    printf(YELLOW "%s " RESET GREEN "is the name of secretfile extension\n" RESET,ch);
    return e_success;
}

Status decode_secretfile_size(decodeInfo*decInfo)
{
    char buff[32];
    int ch;
    if(fread(buff,32,1,decInfo->fptr_stego_image)!=1)
        {
            return e_failure;
        }
    ch=decode_size_from_lsb(buff);
    decInfo->size_secret_file=ch;
    printf(YELLOW "%d" RESET GREEN" is the size of secret file\n" RESET, ch);
    return e_success;
}

Status decode_secretfile_data(decodeInfo*decInfo)
{

    char buff[8];
    char ch[decInfo->size_secret_file+1];
    for(int i=0;i<(decInfo->size_secret_file);i++)
    {
        if(fread(buff,8,1,decInfo->fptr_stego_image)!=1)
        {
            return e_failure;
        }
        ch[i]=decode_byte_from_lsb(buff);
    }
    ch[decInfo->size_secret_file]='\0';
    fprintf(decInfo->fptr_secret,"%s",ch);
    return e_success;

}

