#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/wait.h>
#define MAXSIZE 90

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
	int sockfd,newsockfd,retval,i;
	socklen_t actuallen;
	int recedbytes,sentbytes;
	struct sockaddr_in serveraddr,clientaddr;
	char buff[MAXSIZE], response[200];
    struct Request req;
    struct Patient pat;
	
	int a=0;
	sockfd=socket(AF_INET,SOCK_STREAM,0);

	if(sockfd==-1)
	{
	printf("\nSocket creation error");
	}

	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(3388);
	serveraddr.sin_addr.s_addr=htons(INADDR_ANY);
	retval=bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	if(retval==1)
	{
		printf("Binding error");
		close(sockfd);
	}

	retval=listen(sockfd,1);
	if(retval==-1)
	{
	close(sockfd);
	}
	actuallen=sizeof(clientaddr);
	
	while(1)
	{
		newsockfd=accept(sockfd,(struct sockaddr*)&clientaddr,&actuallen);
	    if(newsockfd==-1)
	    {
	    	close(sockfd);
	    }
        
        if (fork()==0)
        {
            recedbytes = recv(newsockfd, &req, sizeof(req), 0);
            memset(response, 0, sizeof(response));

            if (req.choice==1)
            {
                FILE *fp = fopen("patients.txt", "a");
                fprintf(fp, "%s %s %s %d %d %s\n", req.patient.name,
                        req.patient.contact,
                        req.patient.id,
                        req.patient.bp,
                        req.patient.pulse,
                        req.patient.blood_group);
                fclose(fp);
                sprintf(response, "Inserted Successfully");
            }
            else if(req.choice==2)
            {   
                FILE *fp = fopen("patients.txt", "r");
                FILE *temp = fopen("temp.txt", "w");
                int found = 0;

                memset(response, 0, sizeof(response));
                while(fscanf(fp, "%s %s %s %d %d %s", pat.name, 
                    pat.contact, pat.id , &pat.bp, &pat.pulse, pat.blood_group)!=EOF)
                    {
                        if (strcmp(pat.id, req.id)==0)
                            found = 1;
                        else
                        {
                            fprintf(temp, "%s %s %s %d %d %s\n", pat.name, 
                        pat.contact, pat.id , pat.bp, pat.pulse, pat.blood_group);
                        }
                    }
                    fclose(fp);
                    fclose(temp);
                    remove("patients.txt");
                    rename("temp.txt", "patients.txt");
                    if (found)
                    {
                        strcpy(response, "Found and deleted\n");
                    }
                    else
                    {
                        strcpy(response, "Record not found\n");
                    }
            }
            else if(req.choice==3)
            {
                FILE *fp = fopen("patients.txt", "r");
                FILE *temp = fopen("temp.txt", "w");
                int found = 0;

                memset(response, 0, sizeof(response));
                while(fscanf(fp, "%s %s %s %d %d %s", pat.name, 
                    pat.contact, pat.id , &pat.bp, &pat.pulse, pat.blood_group)!=EOF)
                    {
                        if (strcmp(pat.id, req.id)==0)
                        {
                            found = 1;
                            fprintf(temp, "%s %s %s %d %d %s\n", req.patient.name, 
                        req.patient.contact, req.patient.id , req.patient.bp, req.patient.pulse, req.patient.blood_group);
                        }

                        else
                        {
                            fprintf(temp, "%s %s %s %d %d %s\n", pat.name, 
                        pat.contact, pat.id , pat.bp, pat.pulse, pat.blood_group);
                        }
                    }
                    fclose(fp);
                    fclose(temp);
                    remove("patients.txt");
                    rename("temp.txt", "patients.txt");

                    if(found)
                    strcpy(response, "Record Updated.\n");
                    else
                    strcpy(response, "Patient ID not found.");
            }    
            send(newsockfd, response, strlen(response)+1, 0);
            close(newsockfd);
            exit(0);
        }
        close(newsockfd);
        waitpid(-1, NULL, WNOHANG);
		
	}
	close(sockfd);
	close(newsockfd);
}

