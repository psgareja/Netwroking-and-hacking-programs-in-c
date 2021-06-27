 /*this fucntion accepts socket fd and ptr to the null terminated string to send 
 *function sure bytes will send 
 *return 1 on success and 0 on failure .
 */

int send_string(int sockfd,unsigned char *buffer){
    int sent_bytes,bytes_to_send;
    bytes_to_send=strlen(buffer);
    while(bytes_to_send>0){
        sent_bytes=send(sockfd,buffer,bytes_to_send,0);
        if(sent_bytes==-1)
            return 0;
        bytes_to_send-=sent_bytes;
        buffer+=sent_bytes;
    }
    return -1;
    }
/* this function accept socket fd abd ptr to destination
*buffer,it will recieve from the socket until EOL
*sequence in seen. EOL read from socket
*destination buffer terminated before these bytes.
*return sieze of the read line
*/

int recv_line(int sockfd,unsigned char *dest_buffer){

#define EOL "\r\n" //end the line 
#define EOL_SIZE 2
    unsigned chat *ptr;
    int eol_matched =0;
    ptr=dest_buffer;
    while(recv(sockfd,ptr,1,0)==1)//read a single bytes.
        {
            if(*ptr==EOL[eol_matched]){
                eol_matched++;
                if(eol_matched==EOL_SIZE){
                    *(ptr+1-EOL_SIZE)='\0';
                    return strlen(dest_buffer);

                }
            }
            else{
                eol_matched=0;
            }
            ptr++;

        }
        return 0;
}