#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#define MAXSIZE 50

struct Patient {
    char name[50];
    char id[5];
    char contact[15];
    int pulse;
    int bp;
    char blood_group[5];
};

struct Request {
    int choice;
    struct Patient patient;
    char id[5];
};

int main()
{
	char buff[MAXSIZE];
	int sockfd,retval,i;
	int recedbytes,sentbytes;
	struct sockaddr_in serveraddr;
    struct Request req;
    struct Patient pat;

	//sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd==-1)
	{
		printf("\nSocket Creation Error");
		return 0;
	}

	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(3388);
	serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	//retval=connect(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	if(retval==-1)
	{
		printf("Connection error");
		return 0;
	}

	while(1)
    {   
        sockfd=socket(AF_INET,SOCK_STREAM,0);
        retval=connect(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));

        printf("1.Insert\n2.Delete\n3.Update\n4.Exit\n");
        scanf("%d", &req.choice);

        if (req.choice==4)
        {
            break;
        }

        else if (req.choice==1||req.choice==3)
        {
            printf("Name: ");scanf("%s", req.patient.name);
            printf("\nPatient ID: ");scanf("%s", req.patient.id);
            printf("\nContact Number: ");scanf("%s", req.patient.contact);
            printf("\nPulse: "); scanf("%d", &req.patient.pulse);
            printf("\nBP: "); scanf("%d", &req.patient.bp);
            printf("\nBlood Group: "); scanf("%s", req.patient.blood_group);
        }
        else if (req.choice==2)
        {
            printf("Enter patient ID to delete:");
            scanf("%s", req.id);
        }

        send(sockfd, &req, sizeof(req), 0);

        recedbytes = recv(sockfd, buff, sizeof(buff), 0);
        buff[recedbytes] = '\0';

        printf("Server Response: \n%s\n", buff);
    }
	close(sockfd);
}
