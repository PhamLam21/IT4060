#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        printf("Usage: %s <IP_address> <port>\n", argv[0]);
        return 1;
    }

    //create socket
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client == -1)
    {
        perror("socket() failed");
        return 1;
    }

    //Declare server address
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));
    
    //Connect to server
    int res = connect(client, (struct sockaddr *)&addr, sizeof(addr));
    if(res == -1)
    {
        perror("connect() failed");
        return 1;
    }

    //Receive data from server
    char buffer[BUFFER_SIZE];
    int ret = recv(client, buffer, sizeof(buffer), 0);
    buffer[ret] = 0;
    puts(buffer);

    //Enter&send data to server
    
    while(1)
    {
        printf("Enter message to send (type 'exit' to exit): \n");
        fgets(buffer, sizeof(buffer), stdin);

        send(client, buffer, strlen(buffer), 0);

        if(strncmp(buffer, "exit", 4) == 0)
        {
            break;
        }
    }

    close(client);

    return 0;
}