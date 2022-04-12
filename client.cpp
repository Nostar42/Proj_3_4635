#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <string>

int main(int argc, char ** argv)
{  
    std::string TempString; 
    int CheckRead,CheckSend;
    char ClientBuffer[4096];
    char buffer[4096];
	int port;
	int sock = -1;
	struct sockaddr_in address;
	struct hostent * host;
	
	
	/* checking commandline parameter */
	if (argc != 3)
	{
		printf("usage: %s hostname port\n", argv[0]);
		return -1;
	}

	/* obtain port number */
	if (sscanf(argv[2], "%d", &port) <= 0)
	{
		fprintf(stderr, "%s: error: wrong parameter: port\n", argv[0]);
		return -2;
	}

	/* create socket */
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock <= 0)
	{
		fprintf(stderr, "%s: error: cannot create socket\n", argv[0]);
		return -3;
	}

	/* connect to server */
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	host = gethostbyname(argv[1]);
	if (!host)
	{
		fprintf(stderr, "%s: error: unknown host %s\n", argv[0], argv[1]);
		return -4;
	}
	memcpy(&address.sin_addr, host->h_addr_list[0], host->h_length);
	if (connect(sock, (struct sockaddr *)&address, sizeof(address)))
	{
		fprintf(stderr, "%s: error: cannot connect to host %s\n", argv[0], argv[1]);
		return -5;
	}

    //Read server message
    CheckRead = read(sock,buffer, sizeof(buffer));
    std::cout << "Received " << CheckRead << " bytes" << std::endl;
    std::cout << "Server> " << buffer << std::endl; 

    //Do while receiving data; recv||buffer!=exit
    
    //Server Client Communication
    do{
        //Send Message To Server
        std::cin.getline(ClientBuffer,4096);
        CheckSend = write(sock, &ClientBuffer, strlen(ClientBuffer));
        if(CheckSend > 0){
            //std::cout << "Client >" << ClientBuffer << std::endl; 
            //std::cout << "Sending Server " << CheckSend << " Bytes" << std::endl;
            memset(ClientBuffer, 0, sizeof ClientBuffer);
        }
        
        //Recieve Message From Server 
            CheckRead = read(sock, ClientBuffer,sizeof(ClientBuffer));
        if(CheckRead > 0){
            std::cout << "Server >" << ClientBuffer << std::endl;
            //std::cout << "Received " << CheckRead << " Bytes" << std::endl;
            memset(ClientBuffer, 0, sizeof ClientBuffer);
        }

    }while(strcmp("Exit", ClientBuffer) != 0);
	close(sock);

	return 0;
}
