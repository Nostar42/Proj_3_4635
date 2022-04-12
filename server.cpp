#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
//#include <linux/in.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string>
#include <cstring>
#include <iostream>


int SockList[10] = { };

struct connection_t
{
	int sock;
	struct sockaddr address;
	unsigned int addr_len;
};

void Exit(connection_t* conn);
void Help(connection_t* conn);
void Invalid(connection_t * conn);
void ListUsers(connection_t * conn);
void BroadCast(connection_t * conn, const char * ServerBuffer);
void Message(connection_t * conn, const char * ServerBuffer);

void * process(void * ptr)
{     

    int isLogged = 0;
    int CheckSend, CheckRead, len;
    const char * ServerMessage;
    char  ServerBuffer[4096];
	connection_t * conn;
    long addr = 0;

	if (!ptr) pthread_exit(0); 
	conn = (connection_t *)ptr;
	addr = (long)((struct sockaddr_in *)&conn->address)->sin_addr.s_addr;
    
    //Add User to ScckList
    for(int i = 0; i < 10; i++){
        if(SockList[i] == 0){
            SockList[i] = conn->sock;
            break;
        }
    }

    //Send Greeting
    std::cout << "Connected user:" << conn << std::endl;
    ServerMessage = "Server >Welcome to the TCP weather alert system. Type 'Help' for avaliable commands\n";
    CheckSend = send(conn->sock, ServerMessage, strlen(ServerMessage),0);
    std::cout << "sending client " << CheckSend << " Bytes" << std::endl;

    do{
		// Recieve Message From Client
        memset(ServerBuffer, 0, sizeof ServerBuffer);
	   	CheckRead = read(conn->sock, ServerBuffer, sizeof(ServerBuffer));
        if(CheckRead > 0){
	        std::cout << "Received " << CheckRead << " Bytes" << std::endl;
            std::cout << "Client >" << ServerBuffer << std::endl;
            if(strcmp("Help", ServerBuffer) == 0 && isLogged == 0){
                Help(conn);
            }
            else if(strcmp("Exit", ServerBuffer) == 0){
                Exit(conn);
            }
            else if(strcmp("List", ServerBuffer) == 0){
                ListUsers(conn);
            }
            else if(strstr(ServerBuffer, "Broadcast") != NULL){
                BroadCast(conn, ServerBuffer);
            }
            else if(strstr(ServerBuffer, "Message") != NULL){
                Message(conn, ServerBuffer);
            }
            else{
                Invalid(conn);
            }
            memset(ServerBuffer, 0, sizeof ServerBuffer);
        }
    }while(1);
}

int main(int argc, char ** argv)
{
	int sock = -1;
	struct sockaddr_in address;
	int port;
	connection_t * connection;
	pthread_t thread;

	/* check for command line arguments */
	if (argc != 2)
	{
		fprintf(stderr, "usage: %s port\n", argv[0]);
		return -1;
	}

	/* obtain port number */
	if (sscanf(argv[1], "%d", &port) <= 0)
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

	/* bind socket to port */
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
	if (bind(sock, (struct sockaddr *)&address, sizeof(struct sockaddr_in)) < 0)
	{
		fprintf(stderr, "%s: error: cannot bind socket to port %d\n", argv[0], port);
		return -4;
	}

	/* listen on port */
	if (listen(sock, 5) < 0)
	{
		fprintf(stderr, "%s: error: cannot listen on port\n", argv[0]);
		return -5;
	}

	printf("%s: ready and listening\n", argv[0]);
	
	while (1)
	{
		/* accept incoming connections */
		connection = (connection_t *)malloc(sizeof(connection_t));
		connection->sock = accept(sock, &connection->address, &connection->addr_len);
		if (connection->sock <= 0)
		{
			free(connection);
		}
		else
		{   
	        /* start a new thread but do not wait for it */
			pthread_create(&thread, 0, process, (void *)connection);
			pthread_detach(thread);
		}
	}
	
    return 0;
}

void Exit(connection_t * conn){
    std::cout << "User has exited server" << std::endl;
	close(conn->sock);
	free(conn);
	pthread_exit(0);
}
void Help(connection_t * conn){
    std::cout << "An unknown user has requested commands" << std::endl;
    const char * ServerBuffer = "List of commands\n Help\n List\n Broadcast \"Message\"\n Message UserID \"text\"\n Exit\n";
    int CheckSend = send(conn->sock, ServerBuffer, strlen(ServerBuffer), 0);
    std::cout << "Server >" << ServerBuffer << std::endl; 
    std::cout << "Sending Client " << CheckSend << " Bytes" << std::endl;
}

void ListUsers(connection_t * conn){
    std::cout << "User has requested a list of active users" << std::endl;
    for(int i = 0; i < 10; i++){
        if(SockList[i] != 0){ 
            std::string ServerBuffer = std::to_string(SockList[i]);
            int CheckSend = write(conn->sock, ServerBuffer.c_str(), ServerBuffer.size());
            std::cout << "Server >" << ServerBuffer << std::endl;
            std::cout << "Sending Client " << CheckSend << " Bytes" << std::endl;
        }
    }
}
void Message(connection_t * conn, const char * ServerBuffer){
    char * pch = strtok(const_cast<char*>(ServerBuffer), " ");
    for(int i = 0; i < 1; i++){
        pch = strtok (NULL, " ");
    }
    std::cout << pch << std::endl;
    int CheckSend = write(SockList[atoi(pch)], ServerBuffer, strlen(ServerBuffer));
    std::cout << "Server >" << ServerBuffer << std::endl;
    std::cout << "Sending Client " << CheckSend << " Bytes" << std::endl;

}

void BroadCast(connection_t * conn, const char * ServerBuffer){
    for(int i = 0; i < 10;i++){
        if(SockList[i] != 0){
            int CheckSend = write(SockList[i],ServerBuffer,strlen(ServerBuffer));
            std::cout << "Server >" << ServerBuffer << std::endl; 
            std::cout << "Sending Client " << CheckSend << " Bytes" << std::endl;
        }
    }
}

void Invalid(connection_t * conn){
    std::cout << "User has entered an invalid command" << std::endl;
    const char * ServerBuffer = "Invalid Command\n";
    int CheckSend = write(conn->sock, ServerBuffer, strlen(ServerBuffer));
    std::cout << "Server >" << ServerBuffer << std::endl; 
    std::cout << "Sending Client " << CheckSend << " Bytes" << std::endl;
}
