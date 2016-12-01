#include<stdio.h>
#include<iostream>
#include<string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "commands.h"
#include "myStructs.h"
#include "myUtil.h"
#include "server.h"
#include "client.h"
void intialiseS();
void initialiseC();

int main(int argc, char *argv[])
{

 	if(argc==3)
	{
		lport = argv[2];
	}
	else
	{
		cout<<"Wrong Arguments!!";
		return 0;
	}
	if(strcmp(argv[1],"s")==0 || strcmp(argv[1],"S")==0)
	{
		intialiseS();
		server(argv[2]);
	}
	else if(strcmp(argv[1],"c")==0 || strcmp(argv[1],"C")==0)
	{
		initialiseC();
		client(argv[2]);
	}
	else
		cout<<"\n Wrong arguments";		
		
	  
        return 0;
}

void intialiseS()
{
	commands = "\n Commands: \n 1.Help \t 2.Creator \t 3.Display \n";
	peers =NULL;
	Server_IP_List = NULL; 
	ccnt=0;
	type = 's';
	Me = (struct infoIP *)malloc(sizeof (struct infoIP));
}

void initialiseC()
{
	commands = "\n Commands: \n 1.Help \t 2.Creator \n 3.Display \t 4.Register \n 5.Connect \t 6.List \n 7.Terminate \t 8.Quit \n 9.Get \t\t 10.Put \n";
	peers = NULL;
	Server_IP_List = NULL; 
	ccnt=0;
	peercnt=0;
	type='c';
	contd = false;
	serverInfo = NULL;
	Me = (struct infoIP *)malloc(sizeof (struct infoIP));
}
