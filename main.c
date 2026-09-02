#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include "ui.h"

OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1],"-e")==0)
    {
        return e_encode;
    }
    else if(strcmp(argv[1],"-d")==0)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{   
    char*ch1,*ch2,*ch3;
    ch1=strstr(argv[2],".bmp");
    ch2=strstr(argv[3],".txt");
    
    //validate source file
    if((ch1!=NULL)&&(*(ch1+4)=='\0'))
    {
        encInfo->src_image_fname=argv[2];
    }
    else
    {
        return e_failure;
    }
    //validate secret text file
    if((ch2!=NULL)&&(*(ch2+4)=='\0'))
    {
        encInfo->secret_fname=argv[3];
    }
    else
    {
        return e_failure;
    }
    //validate stego file
    if(argv[4]!=NULL)
    {
        ch3=strstr(argv[4],".bmp");
        if(ch3!=NULL && *(ch3+4)=='\0')
        {
            encInfo->stego_image_fname=argv[4];
        }
        else
        {
            return e_failure;
        }
    }
    else
    {
        encInfo->stego_image_fname="Stego.bmp";
    }

    return e_success;
}


Status read_and_validate_decode_args(char *argv[], decodeInfo *decInfo)
{   
    char*ch1,*ch2;
    ch1=strstr(argv[2],".bmp");
    
    //validate stego file
    if((ch1!=NULL)&&(*(ch1+4)=='\0'))
    {
        decInfo->stego_image_fname=argv[2];
    }
    else
    {
        return e_failure;
    }
    //validate secret text file
    if(argv[3]!=NULL)
    {
        ch2=strstr(argv[3],".txt");
        if((ch2!=NULL)&&(*(ch2+4)=='\0'))
        {
            decInfo->secret_fname=argv[3];
        }
        else
        {
            return e_failure;
        }
    }
    else
    {
        decInfo->secret_fname="decode.txt";
    }

    return e_success;
}


int main(int argc,char *argv[])
{
    EncodeInfo encInfo;
    decodeInfo decInfo;
    
    if (argc < 3)
    {
        printf(RED "Invalid arguments\n" RESET);
        printf(YELLOW "Usage:\n" RESET);
        printf("./a.out -e <src.bmp> <secret.txt> [stego.bmp]\n");
        printf("./a.out -d <stego.bmp> [dec.txt]\n");
        return 0;
    }

    if(check_operation_type(argv)==e_encode)
    {
        printf(YELLOW "\nINFO : ENCODING SELECTED\n" RESET);
        printf("\n");
        if(read_and_validate_encode_args(argv,&encInfo)==e_success)
        {
            printf(GREEN "read_and_validate_encode_args is success\n" RESET);
            if(do_encoding(&encInfo)==e_success)
            {
                printf(YELLOW "\n✓ ENCODING SUCCESFULLY COMPLETED\n" RESET);
                printf("\n");
            }
        }
        else
        {
            printf(RED "read_and_validate_encode_args is failure\n" RESET);
        }
    }
    else if(check_operation_type(argv)==e_decode)
    {
      printf(YELLOW "\nINFO : DECODING SELECTED\n" RESET);
      if(read_and_validate_decode_args(argv,&decInfo)==e_success)
      {
        printf(GREEN "\nread_and_validate_decode_args is success\n" RESET);
            if(do_decoding(&decInfo)==e_success)
            {
                printf(YELLOW "\n✓ DECODING SUCCESFULLY COMPLETED\n" RESET);
                printf(YELLOW "CHECK FOR THE DATA IN %s\n" RESET,decInfo.secret_fname);
                printf("\n");
            }
      }
    }

    return 0;
}
