#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if(argc !=  2)
    {
        printf("usage: %s <port>\n", argv[0]);
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

    char buf[256];

    int mssv;
    char hoten[64];
    unsigned char ngay, thang;
    unsigned short nam;
    float diemtb;

    while (1) {
        int client = accept(server, NULL, NULL);
        int ret = recv(client, buf, sizeof(buf), 0);

        if (client == -1)
        {
            perror("accept() failed");
            return 1;
        }
        printf("new client connected: %d\n", client);

        int pos = 0;
        memcpy(&mssv, buf, sizeof(mssv));
        pos += sizeof(mssv);
        ngay = buf[pos++];
        thang = buf[pos++];
        memcpy(&nam, buf + pos, sizeof(nam));
        pos += sizeof(nam);
        memcpy(&diemtb, buf + pos, sizeof(diemtb));
        pos += sizeof(diemtb);
        
        int len = ret - pos;
        memcpy(hoten, buf + pos, len);
        hoten[len] = 0;

        printf("%d %s %d-%d-%d %.2f\n", mssv, hoten, nam, thang, ngay, diemtb);

        close(client);
    }
    
    close(server);

    return 1;
}