//dumps raw memory in hex bytes

void dump(const unsigned char *data_buffer,const unsigned int length){
    unsigned char byte;
    unsigned int i,j;
    for(i=0;i<length;i++){
        byte=data_buffer[i];
        printf("%0x ",data_buffer[i]);//Display bytes in hex.
        if(((i%16)==15) || (i==length-1)){
            for(j=0;j<15-(i%16);j++)
                printf(" ");
            printf("| ");
             for(j=(i-(i%16));j<=i;j++){//Display printable bytes from line.
                if((byte>31) && (byte<127))//outside printable char range
                    printf("%c",byte);
                else
                    printf(".");


             }

            printf("\n"); //end of the dump line (each line is 16 bytes)
            
        }//end if
    }//end for 
}