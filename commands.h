#ifndef COMND_H
#define COMND_H

#include<stdio.h>
#include<iostream>
#include<string>
#include <cstdlib>
#include<unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "myUtil.h"

string commands;
struct conClient* peers;
struct conClient* Server_IP_List; 
int ccnt, peercnt;
char *lport;
int sockfd;
char type;
bool contd;
struct infoIP *serverInfo;
struct infoIP *Me;
void initMe();
void help();
void creator();
void display();
int registerS(char *vals);
int connectC(char *vals);
void list();
int terminate(char *vals);
void quit();
void put(char *vals);
void get(char *vals);
bool peerAlreadyCon(char *IP,char *nport);
bool isMe(char *IP,char *nport);
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
	return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

bool checkServerList(char *IP,char *nport);
void help()
{
	cout<<commands;	
}
void creator()
{
	cout<<"\n Name: Ashwini Abhay Patil \n UBIT Name: ashwinia \n UB Email: ashwinia@buffalo.edu \n";
}
void initMe()
{
	char ac[80];
if (gethostname(ac, sizeof(ac))<0 ) {
        cout<<"\nError\n";
        return;
    }
    //cout <<"\nHost name is :" << ac << "." << endl;

    struct hostent *phe = gethostbyname(ac);
    if (phe == 0) {
  //      cout << "Yow! Bad host lookup." << endl;
        return;
    }

    for (int i = 0; phe->h_addr_list[i] != 0; ++i) {
        struct in_addr addr;

        memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
         Me->fd=sockfd;
         Me->peer.s_addr= addr.s_addr;
         Me->port =atoi(lport);

//        cout << "IP Address :" << i << ": " << inet_ntoa(addr) << endl;
    }
	
}
void display()
{
	
//	cout<<"\n IP address :"<<inet_ntoa(Me->peer)<<"\n";
	char ac[80];
if (gethostname(ac, sizeof(ac))<0 ) {
        cout<<"\nError\n"; 
        return;
    }
    cout <<"\nHost name is :" << ac << "." << endl;
    cout << "IP Address : " << inet_ntoa(Me->peer) << endl;
    		
		
}
int registerS(char *vals)
{
	int len = strlen(vals);
	if(contd)
	{
		cout<<"\n Already connected to a server\n";
		return -1;
	}
	int ind=0;
	while(vals[ind]!='\0')
	{
		if(vals[ind]==' ')
		{
			break;		
		}
		ind++;
	}
	char IP[ind+1];
	char nport[len-ind+1];
	if(vals[ind]=='\0')
	{
		cout<<"\n Wrong arguments passed\n";
		return -1;
	}
	else
	{
		strncpy(IP,vals,ind);
		int index=ind+1,vi=0;
		while(vals[index]!='\0')
		{
			nport[vi]=vals[index];			
			index++;
			vi++;
		}
		IP[ind]= '\0';	
		nport[vi]='\0';
	}

	struct addrinfo thints, *tres;
	int tsockfd;

	// first, load up address structs with getaddrinfo():
	memset(&thints, 0, sizeof thints);
	thints.ai_family = AF_UNSPEC;
	thints.ai_socktype = SOCK_STREAM;
	getaddrinfo(IP, nport, &thints, &tres);
	// make a socket:
	tsockfd = socket(tres->ai_family, tres->ai_socktype, tres->ai_protocol);
	if(tsockfd<0)
	{
		cout<<"\n Cannot connect\n";
		return -1;
	}
	// connect!
	if(connect(tsockfd, tres->ai_addr, tres->ai_addrlen)==-1)
	{
		cout<<"\nCannot connect. Pls check arguments";
		return -1;
	}
	
	struct sockaddr_in* temp = (struct sockaddr_in *) tres->ai_addr; 
	serverInfo = (struct infoIP *)malloc(sizeof (struct infoIP));
	serverInfo->fd = tsockfd;
	serverInfo->port=temp->sin_port;
	serverInfo->peer.s_addr=temp->sin_addr.s_addr;
	if (send(tsockfd, lport, strlen(lport), 0) == -1) 
	{
		perror("send");
		return -1;
	}
	cout<<"\n Sent:"<<lport<<"\n";	
	cout<<"\n Connected to : \n"<<inet_ntoa(temp->sin_addr);
	contd = true;
	return tsockfd;
}
int connectC(char *vals)
{
	if(!contd)
	{
		printf("\n Cannot connect unless connected to Server\n");
		return -1;
	}
	int len = strlen(vals);
	if(ccnt == 3)
	{
		cout<<"\n Host has 3 connections and cannot connect to more\n";
		return -1;
	}
	int ind=0;
	while(vals[ind]!='\0')
	{
		if(vals[ind]==' ')
		{
			break;		
		}
		ind++;
	}
	char IP[ind+1];
	char nport[len-ind+1];
	if(vals[ind]=='\0')
	{
		cout<<"\n Wrong arguments passed\n";
		return -1;
	}
	else
	{
		strncpy(IP,vals,ind);
		int index=ind+1,vi=0;
		while(vals[index]!='\0')
		{
			nport[vi]=vals[index];			
			index++;
			vi++;
		}
		IP[ind]= '\0';	
		nport[vi]='\0';
	}
	if(!checkServerList(IP,nport))
	{
		cout<<"\n No such connection in Server_IP_List \n";
		return -1;
	}
	if(peerAlreadyCon(IP,nport))
	{
		printf("\n Already connected to %s \n",IP);
		return -1;	
	}
	if(isMe(IP,nport))
	{
		printf("\n Cannot connect to yourself %s \n",IP);
                return -1;
	}
	struct addrinfo thints, *tres;
	int tsockfd;
	// first, load up address structs with getaddrinfo():
	memset(&thints, 0, sizeof thints);
	thints.ai_family = AF_UNSPEC;
	thints.ai_socktype = SOCK_STREAM;
	getaddrinfo(IP, nport, &thints, &tres);
	// make a socket:
	tsockfd = socket(tres->ai_family, tres->ai_socktype, tres->ai_protocol);
	if(tsockfd<0)
	{
		cout<<"\n Cannot connect\n";
		return -1;
	}
	// connect!

	if(connect(tsockfd, tres->ai_addr, tres->ai_addrlen)==-1)
	{
		return -1;
	}
	struct sockaddr_in* temp = (struct sockaddr_in *) tres->ai_addr; 
	if(peers == NULL)
	{
		peers = (struct conClient *)malloc(sizeof (struct conClient));	
		peers->fd = tsockfd;
		peers->port=temp->sin_port;
		peers->peer.s_addr=temp->sin_addr.s_addr;
		peers->next = NULL;
	}
	else
	{
		struct conClient *temp2= peers;
		//struct conClient *prev=peers;
		while(temp2 !=NULL)
		{
		//	prev=temp2;
			temp2 = temp2->next;
		}
		temp2 = (struct conClient *)malloc(sizeof (struct conClient));	
		temp2->fd = tsockfd;
		temp2->port=temp->sin_port;
		temp2->peer.s_addr=temp->sin_addr.s_addr;
		temp2->next = NULL;
		cout<<"\n Connected to : \n"<<inet_ntoa(temp->sin_addr);
	}
	
	ccnt++;	
	return tsockfd;


}
void list()
{
	printf("\n******************************** List of Connections ********************************\n");
	std::cout.flush();
	int i=1;
	if(serverInfo!=NULL)
	{
		struct hostent *he1;
		he1 = gethostbyaddr(&serverInfo->peer, sizeof serverInfo->peer, AF_INET);
		//printf("\n%d. %s \t %s \t %d",i,he1->h_name,inet_ntoa(serverInfo->peer),ntohs(serverInfo->port));
		cout<<"\n"<<i<<".	"<<he1->h_name<<"	"<<inet_ntoa(serverInfo->peer)<<"	"<<ntohs(serverInfo->port);
		i++;
	}	
	if(ccnt!=0){
	struct conClient *temp= peers;
	while(temp !=NULL)
	{
		struct hostent *he;
		he = gethostbyaddr(&temp->peer, sizeof temp->peer, AF_INET);
		cout<<"\n"<<i<<".	"<<he->h_name<<"	"<<inet_ntoa(temp->peer)<<"	"<<ntohs(temp->port);
		temp = temp->next;
		i++;
	}}
	if(ccnt==0 && contd==false)
	{
		cout<<"\nNo connections yet\n";
	}
	cout<<endl;
	std::cout.flush();
	
}
int terminate(char *vals)
{
	int index=-1,fd;
	try
	{	
		index = atoi(vals);
	}
	catch(const char* msg)
	{
		return -1;
	}
	if(index>0)
	{
		if(index==1 && contd)
		{
			contd=false;
			fd = serverInfo->fd;
		//	free(serverInfo);
			close(fd);
			return fd;	
		}
		else
		{
			struct conClient *temp= peers;
			struct conClient *prev= peers;
			int i=1;
			while(i!=index-1 && temp!=NULL)
			{
				prev = temp;
				temp = temp->next;
				i++;
			}
			if(temp==NULL)
			{
				return -1;
			}
			if(temp==peers)
			{
				fd = peers->fd;
				peers = temp->next;
	//			free(temp);
				close(fd);
			//	ccnt--;
				return fd;
				
			}	
			fd = temp->fd;
			prev->next = temp->next;
		}
	}
//	ccnt--;
	close(fd);
	cout<<"\n Terminated connection with :"<<fd<<endl;	
	return fd;
		
}
void quit()
{
	exit(0);
}
void put(char *vals)
{
	int len = strlen(vals);
 
        int ind=0;
        while(vals[ind]!='\0')
        {
                if(vals[ind]==' ')
                {
                        break;
                }
                ind++;
        }
        char IP[ind+1];
        char nport[len-ind+1];
        if(vals[ind]=='\0')
        {
                cout<<"\n Wrong arguments passed\n";
                return;
        }
        else
        {
                strncpy(IP,vals,ind);
                int index=ind+1,vi=0;
                while(vals[index]!='\0')
                {
                        nport[vi]=vals[index];
                        index++;
                        vi++;
                }
                IP[ind]= '\0'; 
                nport[vi]='\0';
        }
	
	int index=-1,fd;
	struct infoIP sendto;
        try
        {
                index = atoi(IP);
        }
        catch(const char* msg)
        {
                return;
        }
        if(index>0)
        {
                if(index==1 && contd)
                {
                        printf("\n Cannot send it to Server\n");
                        return;
                }
                else
                {
                        struct conClient *temp= peers;
                        struct conClient *prev= peers;
                        int i=1;
                        while(i!=index-1 && temp!=NULL)
                        {
                                prev = temp;
                                temp = temp->next;
                                i++;
                        }
                        if(temp==NULL)
                        {
				printf("\n Wrong Connection ID\n");
                                return;
                        }
                        if(temp==peers)
                        {
                                sendto.fd = peers->fd;
                                sendto.peer.s_addr = peers->peer.s_addr;
                                sendto.port=peers->port;

                        }
                        else
			{
				sendto.fd = temp->fd;
                                sendto.peer.s_addr = temp->peer.s_addr;
                                sendto.port=temp->port;

			}
                }
        }
        
        

}
void get(char *vals)
{
	int len = strlen(vals);

        int ind=0;
        while(vals[ind]!='\0')
        {
                if(vals[ind]==' ')
                {
                        break;
                }
                ind++;
        }
        char IP[ind+1];
        char nport[len-ind+1];
        if(vals[ind]=='\0')
        {
                cout<<"\n Wrong arguments passed\n";
                return;
        }
        else
        {
                strncpy(IP,vals,ind);
                int index=ind+1,vi=0;
                while(vals[index]!='\0')
                {
                        nport[vi]=vals[index];
                        index++;
                        vi++;
                }
                IP[ind]= '\0';
                nport[vi]='\0';
        }

        int index=-1,fd;
        struct infoIP sendto;
        try
        {
                index = atoi(IP);
        }
        catch(const char* msg)
        {
                return;
        }
        if(index>0)
        {
                if(index==1 && contd)
                {
                        printf("\n Cannot send it to Server\n");
			return;	
                }
		else
                {
                        struct conClient *temp= peers;
                        struct conClient *prev= peers;
                        int i=1;
                        while(i!=index-1 && temp!=NULL)
                        {
                                prev = temp;
                                temp = temp->next;
                                i++;
                        }
                        if(temp==NULL)
                        {
                                printf("\n Wrong Connection ID\n");
                                return;
                        }

                        if(temp==peers)
                        {
                                sendto.fd = peers->fd;
                                sendto.peer.s_addr = peers->peer.s_addr;
                                sendto.port=peers->port;

                        }
                        else
                        {
                                sendto.fd = temp->fd;
                                sendto.peer.s_addr = temp->peer.s_addr;
                                sendto.port=temp->port;

                        }
                }
        }
        

}
bool checkServerList(char *IP,char *nport)
{
	int port;
	try
	{	
		port = atoi(nport);
	}
	catch(const char* msg)
	{
		return false;
	}
	struct conClient *temp2= Server_IP_List;
	while(temp2 !=NULL)
	{
		struct hostent *he;
		he = gethostbyaddr(&temp2->peer, sizeof temp2->peer, AF_INET);
		if((temp2->port==port && strcmp(IP,inet_ntoa(temp2->peer))) || (temp2->port==port && strcmp(IP,he->h_name)))
			return true;
		temp2 = temp2->next;
	}
	return false;
}

bool peerAlreadyCon(char *IP,char *nport)	
{
	int port;
        try
        {
                port = atoi(nport);
        }
        catch(const char* msg)
        {
                return false;
        }
        struct conClient *temp2= peers;
        while(temp2 !=NULL)
        {
                struct hostent *he;
                he = gethostbyaddr(&temp2->peer, sizeof temp2->peer, AF_INET);
                if((temp2->port==port && strcmp(IP,inet_ntoa(temp2->peer))) || (temp2->port==port && strcmp(IP,he->h_name)))
                        return true;
                temp2 = temp2->next;
        }
        return false;
	
}

bool isMe(char *IP,char *nport)
{

	int port;
        try
        {
                port = atoi(nport);
        }
        catch(const char* msg)
        {
                return false;
        }
        	 struct hostent *he;
                he = gethostbyaddr(&Me->peer, sizeof Me->peer, AF_INET);

                if((Me->port==port && strcmp(IP,inet_ntoa(Me->peer))) || (Me->port==port && strcmp(IP,he->h_name)))
                        return true;
       
       return false;



}

/*void checkClientnServerIP()
{
	int fd[10];
	int cnt=0;
	struct conClient *temp2= peers;
        while(temp2 !=NULL)
        {
                
		char port[12];
		sprintf(port,"%d",temp22->port);
		if(!checkServerList(inet_ntoa(temp2->peer), port))
		{
			fd[cnt]=temp2->fd;
			cnt++;
		}
                temp2 = temp2->next;
        }
	int fo=0;
	for(fo=0;fo<cnt;fo++)
	{
		rem_Peer(fo);
	}
        
}*/
#endif
