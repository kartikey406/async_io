//gcc -Wall -Werror -g -o a.out user_space_async_io.c -lrt
#include<stdio.h>
#include<stdlib.h>
#include<aio.h>
#include<string.h>
#include<errno.h>

struct aiocb *async_read(FILE *fp,char *buf,int bytes){
    struct aiocb *aio;
    aio=malloc(sizeof(struct aiocb));
    if(aio==NULL){
        printf("unable to allocate memory \n");
        return NULL;
    }
    memset(aio,0,sizeof(struct aiocb));
    aio->aio_buf=buf;
    aio->aio_fildes=fileno(fp);
    aio->aio_nbytes=bytes;
    aio->aio_offset=0;
    int result = aio_read(aio);
    if (result < 0){
        printf("aio result is less than 0 \n");
        free(aio);
        return NULL;
    }
    return aio;

}


#define BUFSIZE 25000
int main(int argc,char **argv){
int ret;
FILE *fp;
if(argc < 2){
    printf("file name is not provided \n");
    return EXIT_FAILURE;
}

fp=fopen(argv[1],"r");
if(fp==NULL){
    printf("file is not present \n");
    return EXIT_FAILURE;
}
char buf[BUFSIZE];
struct aiocb *aio=async_read(fp,buf,BUFSIZE);
int counter=0;
while(aio_error(aio) == EINPROGRESS){
    counter++;
}
ret=aio_return(aio);
printf("we have counted %d times \n",counter);
fclose(fp);
if(ret >0){
    printf("Read bytes %d \n",ret);
}
else{
    printf("unable to read bytes from the file \n");
}
return 0;

}