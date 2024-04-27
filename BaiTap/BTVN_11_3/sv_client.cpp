#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char* argv[]) 
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

    // Enter data
    int mssv;
    char hoten[64];
    unsigned char ngay, thang;
    unsigned short nam;
    float diemtb;

    printf("Nhap MSSV: ");
    scanf("%d%*c", &mssv);
    printf("Nhap ho ten: ");
    fgets(hoten, sizeof(hoten), stdin);
    hoten[strlen(hoten) - 1] = 0;
    printf("Nhap ngay, thang, nam sinh: ");
    scanf("%hhu%hhu%hu", &ngay, &thang, &nam);
    printf("Nhap diem trung binh: ");
    scanf("%f", &diemtb);

    //Packing data
    char buffer[256];
    int pos = 0;

    memcpy(buffer, &mssv, sizeof(mssv));
    pos += sizeof(mssv);
    buffer[pos++] = ngay;
    buffer[pos++] = thang;
    memcpy(buffer + pos, &nam, sizeof(nam));
    pos += sizeof(nam);
    memcpy(buffer + pos, &diemtb, sizeof(diemtb));
    pos += sizeof(diemtb);
    memcpy(buffer + pos, hoten, strlen(hoten));
    pos += strlen(hoten);

    //Connect to server
    int res = connect(client, (struct sockaddr *)&addr, sizeof(addr));
    if(res == -1)
    {
        perror("connect() failed");
        return 1;
    }

    //Send to server
    send(client, buffer, pos, 0);

    close(client);

    return 0;
}