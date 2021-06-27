#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "hacking.h"
struct hostnet
{
    char *h_name;
    char **h_aliases;
    int h_addrtype;
    int h_length;
    char **h_addr_list;
    #define h_addr h_addr_list[0]
};

int main(int argc,char *argv[]){
    struct hostent *host_info;
    struct in_addr *address;
    if(argc<2){
        printf("Usage:%s <hostname>\n",argv[0]);
        exit(1);
    }
    host_info=gethostbyname(argv[1]);
    if(host_info==NULL){
        printf("Couldnt lookup %s\n",argv[1]);

    }
    else{
        address=(struct in_addr*)(host_info->h_addr);
        printf("%s has address %s\n",argv[1],inet_ntoa(*address));
    }
}