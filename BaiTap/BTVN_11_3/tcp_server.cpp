#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h> 
#include <fcntl.h> 

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    if(argc !=  4)
    {
        printf("usage: %s <port> <greeting_file> <client_data_file>\n", argv[0]);
        return 1;
    }

    //Create socket waiting connect 
    int server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server == -1)
    {
        perror("socket() failed");
        return 1;
    }

    //Declare server address
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(atoi(argv[1]));

    //Attach socket
    if (bind(server, (struct sockaddr *)&addr, sizeof(addr))) {
        perror("bind() failed");
        return 1;
    }

    //Waiting connect
    if (listen(server, 5)) 
    {
        perror("listen() failed");
        return 1;
    }
    printf("waiting for a new client ...\n");

    //Accpet connect to client
    int client = accept(server, NULL, NULL);
    if (client == -1)
    {
        perror("accept() failed");
        return 1;
    }
    printf("new client connected: %d\n", client);

    char buffer[BUFFER_SIZE];
    
    //Send greeting
    int greeting;
    greeting = open(argv[2], O_RDONLY, 0644);
    if(greeting  == -1)
    {
        printf("open() hello.txt failed\n");
    }
    read(greeting, buffer, sizeof(buffer));
    close(greeting);
    send(client, buffer, strlen(buffer), 0);
    
    //Write data to file
    int client_data;
    client_data = open(argv[3], O_WRONLY, 0664);
    while(true)
    {
        int bytes_received = recv(client, buffer, sizeof(buffer), 0);
        if(bytes_received <= 0)
        {
            break;
        }
        if (bytes_received < sizeof(buffer))
        {
            buffer[bytes_received] = 0;  
        }
        write(client_data, buffer, bytes_received);
        puts(buffer);
    }
    close(client_data);

    //Disconnect
    close(client);
    close(server);

    return 0;
}