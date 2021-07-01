#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include "hacking.h"
#include "hacking-network.h"

#define PORT 80 //the port user will be connecting to
#define WEBROOT "./webroot" //The webserver root directory

void handle_connection(int,struct sockaddr_in *); //handle the web requests
int get_file_size(int); //return the filesize of open file descriptor
int main(void){
    int sockfd,new_sockfd,yes=1;
    struct sockaddr_in host_addr,client_addr;
    socklen_t sin_size;
    printf("Accepting web requests o port%d\n",PORT);
    if((sockfd==socket(PF_INET,SOCK_STREAM,0)==-1))
        printf("In socket");
    if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))==-1)
        printf("setting socket option so_reuseadr");
    host_addr.sin_family=AF_INET;
    host_addr.sin_port=htons(PORT);
    host_addr.sin_addr.s_addr=INADDR_ANY;
    memset(&(host_addr.sin_zero),'\0',8);
    if(bind(sockfd,(struct sockaddr *)&host_addr,sizeof(struct sockaddr))==-1)
        fatal(binding to socket);

    if(listen(sockfd,20)==-1)
        fatal("listing on socket");
    while(1){
        sin_size=sizeof(struct sockaddr_in);
        new_sockfd=accept(sockfd,(struct sockaddr *)&client_addr,&sin_size);
        if(new_sockfd==-1)
            fatal("accepting connection")
        handle_connection(new_sockfd,&client_addr);


    }
    return 0;

}
/* this funtion will handle the connection on the passed socket from the
*passed client address,the connection is procesed as a web request,
*function reply over the connect socket,
*/
void handle_connection(int sockfd,struct sockaddr_in *client_addr_ptr){
    unsigned char *ptr,request[500],resource[500];
    int fd,length;
    length=recv_line(sockfd,request);
    printf("Got request from %s:%d \"%s\"\n", inet_ntoa(client_addr_ptr->sin_addr),ntohs(client_addr_ptr->sin_port),request);
    ptr=strstr(request,"HTTP/");
    if(ptr==NULL){
        printf("NOT HTTP!\n");
    }
    else{
        *ptr=0;//Terminate the buffer at the end of the url.
        ptr=NULL;//set ptr to null (used flag for invalid request).
        if(strncpy(request,"GET",4)==0) //GET request
            ptr=request+4; //ptr is url
        if(strncmp(request,"HEAD",5)==0)//HEAD request
            ptr=request+5; //head request ptr url
        if(ptr==NULL){
            printf("\tUNKNOWN REQUEST!\n");

        }else{
            if(ptr[strlen(ptr)-1]=='/n') //resources ending with /
                strcat(ptr,"index.html"); //add index.html to end
            strcpy(resource ,WEBROOT); //begin resource with beb root
            strcat(resource ,ptr); //join with the resource path
            fd=open(resource,O_RDONLY,0); //try to open file
            printf("\topening \'%s\ '\t ",resource);
            if(fd==-1){
                printf("404 not found!");
                send_string(sockfd,"HTTP/1.0: Tiny webserve\r\n\r\n");
                send_string(sockfd,"<html><head><title>404 Not Found<title></head>");
                send_string(sockfd,"<body><h1>URL not found </h1></body></html>\r\n");
            }else{
                printf(" 200 Ok\n");
                send_string(sockfd,"HTTP/1.0 200 OK\r\n");
                send_string(sockfd,"Server: Tiny Web server\r\n\r\n");
                if(ptr==request+4){
                    if(length=get_sile_size(fd)==-1)
                        fatal("Getting resource file size");
                    if((ptr=(unsigned char *)malloc(length))==NULL)
                        fatal("allocating memory for reading resource");

                    read(fd,ptr,length);//read the file into memory
                    send(sockfd,ptr,length,0);//send it to socket;
                    free(ptr);
                }
                close(fd);
            }
        }

    }
    shutdown(sockfd,SHUT_RDWR);//grecefully socket closed

}

int get_file_size(int fd){
    struct stat_struct;
if(fstat(fd,&stat_struct)==-1)
    return -1;
return (int) stat_struct.st_size;


}
