#include "ENetwork.h"


void Server_create(std::string inet_addr, int host, int num_Wait)
{
	SOCKET SS; //server socket
	SS = socket(PF_INET, SOCK_STREAM, 0);
	//***************
	SOCKADDR_IN SA;
	memset(&SA, 0, sizeof(SOCKADDR_IN));
	//***************
	SA.sin_family = AF_INET;
	SA.sin_port = htons(host);
	unsigned long u = strtoul(inet_addr.c_str(), NULL, 0);
	SA.sin_addr.s_addr = htonl(u);
	printf("host %d, addr %d\n", SA.sin_port, u);
	//***************
	if (bind(SS, (const sockaddr*)&SA, sizeof(SA)) == SOCKET_ERROR)
		printf("Binding Failed\n");
	if (listen(SS, num_Wait) == SOCKET_ERROR)
		printf("Listening Failed\n");
	printf("WAIT CLIENT . . .\n");
	//***************
	int CS; //Client socket	
	SOCKADDR_IN CA;
	int CAS = sizeof(CA);
	CS = accept(SS, (sockaddr*)&CA, &CAS);
	if (CS == INVALID_SOCKET)
	{
		printf("Accepting Error\n");
	}
	else
	{
		printf("%s connecting Complete!\n", inet_ntoa(CA.sin_addr));
	}
	closesocket(SS); //close server socket

	int nRcv;
	char message[1024];
	while (1)
	{
		
		printf("Message Receives . . .\n");
		nRcv = recv(CS, message, sizeof(message) - 1, 0);

		if (nRcv == SOCKET_ERROR)
		{
			printf("Recieve Error ... \n");
			break;
		}
		message[nRcv] = '\0';

		if (strcmp(message, "exit") == 0)
		{
			printf("Close Server Connection . .\n");
			break;
		}

		printf("Recieve Message : %s\n", message);
	}
	closesocket(CS);
}