#ifndef MYUT_H
#define MYUT_H


#include<stdio.h>
#include<iostream>
#include<string>
#include <cstdlib>
#include <sys/types.h>
#include<unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "myStructs.h"
using namespace std;

extern string commands;
extern struct conClient* peers;
extern struct conClient* Server_IP_List; 
extern int ccnt, peercnt;
extern char *lport;
extern int sockfd;
extern char type;
extern bool contd;
extern struct infoIP *serverInfo;
extern struct infoIP *Me;
void displayServerIP()
{
	printf("\n******************************** Server IP List ********************************\n");
	if(Server_IP_List!=NULL){
	struct conClient *temp2= Server_IP_List;
	while(temp2 !=NULL)
	{
		struct hostent *he;
		he = gethostbyaddr(&temp2->peer, sizeof temp2->peer, AF_INET);
		cout<<"\n Port :"<<temp2->port<<"\n Address :"<<inet_ntoa(temp2->peer)<<"\n Hostname :"<<he->h_name<<"\n";
		temp2 = temp2->next;
	}
	}
	else
		printf("\nNo Connections\n");
}
#endif

