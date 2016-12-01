#ifndef MYST_H
#define MYST_H


#include<stdio.h>
#include<iostream>
#include<string>
#include<unistd.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
using namespace std;


struct conClient
{
	int fd;
	struct in_addr peer;
	unsigned short int port;
	struct conClient* next;
};

struct infoIP
{
	int fd;
	struct in_addr peer;
	unsigned short int port;
};

#endif
