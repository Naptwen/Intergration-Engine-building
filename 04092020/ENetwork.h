#pragma once

#include <WinSock2.h>
#pragma comment (lib , "ws2_32.lib")

#include <stdio.h>
#include <string>
#include <bitset> 

#define BUFF_SIZE 1024;

void Server_create(std::string inet_addr, int host, int num_Wait);