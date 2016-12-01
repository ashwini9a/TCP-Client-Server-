#ifndef CLIENT_H
#define CLIENT_H





#include<stdio.h>
#include<iostream>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>

#include "commands.h"
void rem_Peer(int fd);
void checkClientnServerIP(fd_set *master);
void printMaster(int fdmax, fd_set *master)
{
	int i=0;
	for(i=0;i<fdmax+1;i++)
	{
		FD_ISSET(i,master);
		cout<<"\n FD:"<<i;
	}	
}
void client(char *portNo)
{

	help();	
	fd_set master;
	fd_set read_fds;
	int fdmax;
	int newfd;
	struct addrinfo hints, *ai, *p;
	// newly accept()ed socket descriptor
	struct sockaddr_storage remoteaddr; // client address
	socklen_t addrlen;
//	char buf[1024];
	int nbytes;
	// buffer for client data
	char remoteIP[INET6_ADDRSTRLEN];
	int yes=1;
	int i, j, rv;
	// for setsockopt() SO_REUSEADDR, below
	FD_ZERO(&master);
	FD_ZERO(&read_fds);
	// clear the master and temp sets
	// get us a socket and bind it
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if ((rv = getaddrinfo(NULL, portNo, &hints, &ai)) != 0) 
	{
		fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
		exit(1);
	}
	for(p = ai; p != NULL; p = p->ai_next) 
	{
		sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (sockfd < 0) 
		{
			continue;
		}
		// lose the pesky "address already in use" error message
		setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) < 0) 
		{
			close(sockfd);
			continue;
		}
		struct sockaddr_in *tad = (struct sockaddr_in*)p->ai_addr;
	/*	Me->fd=sockfd;
	        Me->peer.s_addr= tad->sin_addr.s_addr;
       		Me->port =tad->sin_port;
*/
		break;
	}
	// if we got here, it means we didn't get bound
	if (p == NULL) 
	{
		fprintf(stderr, "selectserver: failed to bind\n");
		exit(2);
	}
//	Me->fd=sockfd;
//	Me->peer.s_addr= ;
//	Me->port =;
	freeaddrinfo(ai); // all done with this
	// listen
	if (listen(sockfd, 10) == -1) 
	{
		perror("listen");
		exit(3);
	}
	// add the listener to the master set
	FD_SET(sockfd, &master);
	FD_SET(0,&master);
	// keep track of the biggest file descriptor
	fdmax = sockfd; // so far, it's this one
	initMe();
	// main loop
	while(1) 
	{
//		printMaster(fdmax,&master);
//		printf("\n Select routine");
		read_fds = master; // copy it
		if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1 ) 
		{
			perror("select");
			exit(4);
		}		
		// run through the existing connections looking for data to read
		for(i = 0; i <= fdmax; i++) 
		{
			char buf[1024];
			if (FD_ISSET(i, &read_fds)) 
			{ // we got one!!
			//	
				if (i == sockfd)
				{ if(ccnt <3){

					// handle new connections
					addrlen = sizeof remoteaddr;
					newfd = accept(sockfd,(struct sockaddr *)&remoteaddr,&addrlen);
					if (newfd == -1) 
					{
						perror("accept");
					} 
					else 
					{
						FD_SET(newfd, &master); // add to master set
						if (newfd > fdmax) 
						{
							// keep track of the max
							fdmax = newfd;
						}
						
						printf("\nNew connection from %s on ""socket %d\n",	inet_ntop(remoteaddr.ss_family,	get_in_addr((struct sockaddr*)&remoteaddr),remoteIP, INET6_ADDRSTRLEN),	newfd);

						/*if(!checkServerList(char *IP,char *nport))
						{
							close
						}	*/
					/*************************************Update peer list****************************/
					struct sockaddr_in* temp = (struct sockaddr_in *) &remoteaddr;
					/*char nport[10];
					sprintf(nport,"%d",temp->sin_port);
					cout<<" \nIP :"<<inet_ntoa(temp->sin_addr)<<"port";  
					if(!checkServerList(inet_ntoa(temp->sin_addr),nport))
                                        {
							FD_CLR(newfd,&master);
                                                        close(newfd);
							printf("\n New Connection rejected since not in Server IP List \n");
                                        }
					else{*/
					if(peers == NULL)
					{
						peers = (struct conClient *)malloc(sizeof (struct conClient));	
						peers->fd = newfd;
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
						temp2->fd = newfd;
						temp2->port=temp->sin_port;
						temp2->peer.s_addr=temp->sin_addr.s_addr;
						temp2->next = NULL;
						cout<<"\n Connected to : \n"<<inet_ntoa(temp->sin_addr);
					}
	
					ccnt++;	
						
					}
				}
				else
				{
					cout<<"\n Cannot connect since 3 connections already set up";
				}
				} 
			
				else if(i==0)
				{
				//	printf("\n Keyboard input detected");
					fgets(buf, 1024,stdin);
					 //e trailing newline character from the input buffer if needed. */
				        int len = strlen(buf) - 1;
				        if (buf[len] == '\n' )
					{
				            buf[len] = '\0';
				//	    printf("\n new line included\n");
					}
                                	int ind=0;
					
					while(buf[ind]!='\0')
					{
						if(buf[ind]==' ')
						{
							break;		
						}
						ind++;
					}
					char comnd[ind+1];
					char vals[len-ind+1];
					bool flag =true;
					if(buf[ind]=='\0')
					{
						strcpy(comnd,buf);
					}
					else
					{
						strncpy(comnd,buf,ind);
						int index=ind+1,vi=0;
						while(buf[index]!='\0')
						{
							vals[vi]=buf[index];			
							index++;
							vi++;
						}
						comnd[ind]= '\0';	
						vals[vi]='\0';
						//cout<<"\n"<<comnd<<"\n"<<vals<<"\n";
						flag=false;
						
					}
				//	cout<<"\n"<<comnd;
				//	cout<<"\n"<<vals;
					if(flag)
					{
						if(strcmp(comnd,"HELP")==0)
						{
							help();
						}
						else if(strcmp(comnd,"CREATOR")==0)
						{
							creator();
						}
						else if(strcmp(comnd,"DISPLAY")==0)
						{
							display();
						}
						else if(strcmp(comnd,"LIST")==0)
                                                {
							list();
                                                }
						else if(strcmp(comnd,"QUIT")==0)
                                                {
							quit();
                                                }	
						else
						{
							printf("\n Wrong command \n");
						}	
					}
					else
					{
						if(strcmp(comnd,"REGISTER")==0)
                                                {
							int newfdR = registerS(vals);
				//			cout<<"\nNew Register fd:"<<newfdR;
							if(newfdR >0)
							{
								FD_SET(newfdR, &master);
								if (newfdR > fdmax)
                                                		{
		                                                        // keep track of the max
                                                                	fdmax = newfdR;
                                                                }
				//				cout<<"\nRegister fd:"<<newfdR;
//								printMaster(fdmax,&master);
                                                        
							}
                                                }
                                                else if(strcmp(comnd,"CONNECT")==0)
                                                {
							int newfdC = connectC(vals);
							if(newfdC>0)
							{
								FD_SET(newfdC, &master);
								if (newfdC > fdmax)
                                                                {
                                                                        // keep track of the max
                                                                        fdmax = newfdC;
                                                            	}
                                                                        
							}
                                                }
                                                else if(strcmp(comnd,"TERMINATE")==0)
                                                {
							int remfd = terminate(vals);
							if(remfd>0)
							{							
								FD_CLR(remfd, &master);
								rem_Peer(remfd);
							}
                                                }
                                                else
                                                {
                                                        printf("\n Wrong command \n");
                                                } 		
					}
				//	cout<<"\n"<<comnd<<"\n"<<vals<<"\n";	
				}	// handle data from a client
				else if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0) 
				{
						// got error or connection closed by client
						if (nbytes == 0) 
						{
							// connection closed
								rem_Peer(i);
								printf("Connection on socket %d hung up", i);
						}
						else 
						{
							perror("\nConnection Terminated");
						}
						close(i); // bye!
						FD_CLR(i, &master); // remove from master set
						
				} 
				else 
				{
					if(contd && serverInfo->fd==i)
					{
				//		cout<<"\n Server IP List 4m client :END\n"<<buf<<"\n size :"<<nbytes;
						Server_IP_List = (struct conClient *)malloc(sizeof (struct conClient));
				//		cout<<"\n1";
						struct conClient *temp2= Server_IP_List;
				//		cout<<"\n2";
						int index = 7;
						bool al=false;
						while(buf[index]!='#')
						{
							if(al)
							{
								temp2->next = (struct conClient *)malloc(sizeof (struct conClient));
								temp2 = temp2->next;	
							}
							else
								al=true;
							int j=0;
							while(buf[index]!='\n')
							{	
				//				cout<<"\n3";					
								
								if(j==0)
								{
									char n1[10];
									int index2=0;
									while(buf[index]!='%')
									{	
										n1[index2] = buf[index];
										index2++;
										index++;
									}
									
									n1[index2]='\0';
									int fdS = atoi(n1);
									temp2->fd = fdS;
									temp2->next = NULL;
									j++;
								}
								else if(j==1)
								{
									char n1[16];
									int index2=0;
									while(buf[index]!='%')
									{	
										n1[index2] = buf[index];
										index2++;
										index++;
									}
																									
									n1[index2]='\0';
									uint32_t s_addr1 = atoi(n1);
									temp2->peer.s_addr=s_addr1;
									j++;
								}
								else if(j==2)
								{	
									char n1[10];
									int index2=0;
									while(buf[index]!='%')
									{	
										n1[index2] = buf[index];
										index2++;
										index++;
									}
									
									n1[index2]='\0';
									unsigned short int port1 = atoi(n1);
									temp2->port=port1;
									j++;
								}
								index++;
							} 
							index++;
							temp2->next = NULL;
						}
						displayServerIP();
						checkClientnServerIP(&master); 
						//Server : Update server IP List 	
					}
					else
					{
						cout<<"\n Got some data at client end\n"<<buf<<"\n";
						// we got some data from a client 
						
					}
						
						
						
				}	
				FD_CLR(i,&read_fds);
			} // END handle data from client

		} // END looping through file descriptors
//		printMaster(fdmax,&master);
	} // END for(;;)--and you thought it would never end!



}

void rem_Peer(int fd)
{
//	printf("\n******************************** List of Connections ********************************\n");
	struct conClient *temp2= peers;
        struct conClient *prev= peers;

        while(temp2!=NULL)
        {
                if(temp2->fd == fd)
                {
			ccnt--;
                        if(temp2==peers)
                        {
                                peers=temp2->next;
                         //       free(temp2);
                                return;
                        }
                        prev->next = temp2->next;
                        return;
                }
                prev = temp2;
                temp2 = temp2->next;

        }
        if(temp2==NULL)
        {
                return;
        }

}

void checkClientnServerIP(fd_set *master)
{
        int fd[10];
        int cnt=0;
        struct conClient *temp2= peers;
        while(temp2 !=NULL)
        {

                char port[12];
                sprintf(port,"%d",temp2->port);
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
                rem_Peer(fd[fo]);
		FD_CLR(fd[fo],master);	
        }

}

#endif
