#ifndef SERVER_H
#define SERVER_H

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

void remS_IP_L(int fd);

void server(char *portNo)
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
	char buf[1024];
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
/*                Me->fd=sockfd;
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
	//	printf("\n Select routine");
		read_fds = master; // copy it
		if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1 ) 
		{
			perror("select");
			exit(4);
		}		
		// run through the existing connections looking for data to read
		for(i = 0; i <= fdmax; i++) 
		{
			if (FD_ISSET(i, &read_fds)) 
			{ // we got one!!
			//	
				if (i == sockfd)
				{
	//				printf("\n Found new connection\n");
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
				//		send(newfd,"\nHi\n",4,0);
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
						flag=false;
						
					}
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
						else
						{
							printf("\n Wrong command \n");
						}	
					}
					else
					{
						printf("\n Wrong command \n");
                                                 		
					}
				}	
					// handle data from a client
				else if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0) 
				{
						// got error or connection closed by client
						if (nbytes == 0) 
						{
							// connection closed
								remS_IP_L(i);
								printf("Connection disconnected on socket %d hung up\n", i);
							/*********************************************Send updated Server IP List************************/
							
						}
						else 
						{
							perror("Connecction Terminated");
						}
						close(i); // bye!
						FD_CLR(i, &master); // remove from master set
						char serverbuff[]="Start :";
						int lbuf=7;
						struct conClient *temp2= Server_IP_List;
						while(temp2!=NULL)
						{
								
								char n1[10];
								sprintf(n1, "%d", temp2->fd);
								int tlen = strlen(n1);
								strcat(serverbuff,n1);
								lbuf = tlen +lbuf;

								strcat(serverbuff,"%");
								lbuf++;
								char n2[16];
								sprintf(n2, "%lu",(unsigned long) temp2->peer.s_addr); 
								int tlen1 = strlen(n2);
								strcat(serverbuff,n2);
								lbuf = lbuf+tlen1;
								strcat(serverbuff,"%");
								lbuf++;	
								char n3[10];
								sprintf(n3, "%d", temp2->port);
								 int tlen2 = strlen(n3);
								strcat(serverbuff,n3);
								lbuf=lbuf+tlen2;
								strcat(serverbuff,"%");
								lbuf++;
								strcat(serverbuff,"\n");
								lbuf++;
								temp2 = temp2->next;
						}
						strcat(serverbuff,"#");
                                                lbuf++;
						int j;
						for(j=0;j<=fdmax;j++)
						{
							if(FD_ISSET(j,&master) && j!=sockfd && j!=1)
							{
								send(j,serverbuff,lbuf,0);
								cout<<"\n Sent : "<<serverbuff<<"\n"<<"to:"<<j<<" Size"<<lbuf;
							}
						}
						displayServerIP();
						
				} 
				else 
				{
						cout<<"\n Data received by server:"<<buf<<"\n";
					//	send(i,"RECEIVED",8,0);
						struct sockaddr_in peer;
   						int peer_len;
						      /* We must put the length in a variable.              */
   						peer_len = sizeof(peer);
     							 /* Ask getpeername to fill in peer's socket address.  */
   						if (getpeername(i,(sockaddr*) &peer,(socklen_t*) &peer_len) == -1) 
						{
						      perror("getpeername() failed");
						      //return -1;
						}
						if(Server_IP_List == NULL)
						{
							Server_IP_List = (struct conClient *)malloc(sizeof (struct conClient));	
							Server_IP_List->fd = i;
							Server_IP_List->port=atoi(buf);
							Server_IP_List->peer.s_addr=peer.sin_addr.s_addr;
							Server_IP_List->next = NULL;
						}
						else
						{
							struct conClient *temp2= Server_IP_List;
							struct conClient *prev=peers;
							while(temp2 !=NULL)
							{
								prev=temp2;
								temp2 = temp2->next;
							}
							prev->next = (struct conClient *)malloc(sizeof (struct conClient));	
							prev->next->fd = i;
							prev->next->port=atoi(buf);
							prev->next->peer.s_addr=peer.sin_addr.s_addr;
							prev->next->next = NULL;
							//cout<<"\n Connected to : \n"<<inet_ntoa(temp2->peer.s_addr);
						}
						char serverbuff[]="Start :";
						int lbuf=7;
						struct conClient *temp2= Server_IP_List;
						while(temp2!=NULL)
						{
								
								/*char n1[10];
								sprintf(n1, "%d", temp2->fd);
								strcat(serverbuff,n1);

								strcat(serverbuff,"%");

								char n2[16];
								sprintf(n2, "%lu",(unsigned long) temp2->peer.s_addr); 
								strcat(serverbuff,n2);

								strcat(serverbuff,"%");

								char n3[10];
								sprintf(n3, "%d", temp2->port);
								strcat(serverbuff,n3);
								strcat(serverbuff,"%");
                                                                strcat(serverbuff,"\n");

								temp2 = temp2->next;*/
								char n1[10];
                                                                sprintf(n1, "%d", temp2->fd);
                                                                int tlen = strlen(n1);
                                                                strcat(serverbuff,n1);
                                                                lbuf =lbuf+ tlen;

                                                                strcat(serverbuff,"%");
                                                                lbuf++;
                                                                char n2[16];
                                                                sprintf(n2, "%lu",(unsigned long) temp2->peer.s_addr);
                                                                int tlen1 = strlen(n2);
                                                                strcat(serverbuff,n2);
                                                                lbuf =lbuf+ tlen1;
                                                                strcat(serverbuff,"%");
                                                                lbuf++;
                                                                char n3[10];
                                                                sprintf(n3, "%d", temp2->port);
                                                                 int tlen2 = strlen(n3);
                                                                strcat(serverbuff,n3);
                                                                lbuf=lbuf+tlen2;
                                                                strcat(serverbuff,"%");
                                                                lbuf++;
                                                                strcat(serverbuff,"\n");
                                                                lbuf++;
                                                                temp2 = temp2->next;

						}
						strcat(serverbuff,"#");
						lbuf++;
						int j;
						for(j=0;j<=fdmax;j++)
						{
							if(FD_ISSET(j,&master) && j!=sockfd && j!=0)
							{
								send(j,serverbuff,lbuf,0);
				//				cout<<"\n Sent : "<<serverbuff<<"\n"<<"to:"<<j<<" Size :"<<lbuf;
							}
						}
						displayServerIP();
						// we got some data from a client
						//update server IP List
						
				}	
				FD_CLR(i,&read_fds);
			} // END handle data from client

		} // END looping through file descriptors
	} // END for(;;)--and you thought it would never end!


}


void remS_IP_L(int fd)
{
	struct conClient *temp2= Server_IP_List;
	struct conClient *prev= Server_IP_List;
	
	while(temp2!=NULL)
	{
		if(temp2->fd == fd)
		{
			if(temp2==Server_IP_List)
			{
				Server_IP_List=temp2->next;
	//			free(temp2);
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

#endif
