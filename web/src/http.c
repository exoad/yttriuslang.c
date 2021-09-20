#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 1024
#define DEFAULT "Web Scraper by Liguo Jiao"

int create_socket(void) {
	int Socket_fd = socket(AF_INET, SOCK_STREAM, 0);	// make a socket
	if (Socket_fd == -1){
		perror(" |ERROR| Socket creation failed ");
		exit(EXIT_FAILURE);
	}
	return Socket_fd;
}   //line 7

char* receive_query(int socket_fd) {
	int bytesum = 0;
	int current_received;
	int Size = (BUF_SIZE)*sizeof(char);
	char* byte_received = (char*)malloc(Size); byte_received[0] = '\0';
	char* buffer = (char*) malloc(BUF_SIZE);
	memset(buffer, 0, BUF_SIZE);
	while((current_received = recv(socket_fd, buffer, BUF_SIZE, 0)) > 0){
		// If received any bytes current_received will not equal to 0
		bytesum += current_received;
		if (bytesum >= Size){
			Size = Size * 2; // Extend twice size
			byte_received = realloc(byte_received, Size);
			if (byte_received == '\0'){
				perror(" |ERROR| Error receiving data"); return NULL;
			}
		}
		strncat(byte_received, buffer, current_received);
		memset(buffer, 0, current_received);
	}
	if(current_received < 0){
		perror(" |ERROR| Error receiving data"); return NULL;
	}
	return byte_received;
}   //lines 26

char* http_query(char *host, char *page, int port) {
	int Current_byte_received;   // temporary saving for received bytes
	int Socket_fd = create_socket();
	int IP_Leanth = 15; int IP_Leanth_Memory = 16;   // The length of IP address is 15, so give it 16 memory allocations
	int Requires_to_Server = 0;
	char* IP_Address = (char*)malloc(IP_Leanth_Memory); 
	char* Byte_received; char* Extract_pages = page;   //Extract page path
	char* Message = "GET /%s HTTP/1.0\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n";
	char* Requires = (char*)malloc(strlen(host)+strlen(Extract_pages)+strlen(DEFAULT)+strlen(Message)-5);// -5 is to consider the %s %s %s in Message and the ending \0;
	struct sockaddr_in* Socket;  // Extract the socket for network
	struct hostent *Host_Address;  // Extract host address
	
	memset(IP_Address, 0, IP_Leanth_Memory);
	if ((Host_Address = gethostbyname(host)) == NULL){
		perror(" |ERROR| IP Connection failed "); return NULL;}
	if (inet_ntop(AF_INET, (void*)Host_Address -> h_addr_list[0], IP_Address, IP_Leanth) == '\0'){
		perror(" |ERROR| Network address converting failed"); return NULL;}
	Socket = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in*));
	Socket->sin_family = AF_INET;
	Current_byte_received = inet_pton(AF_INET, IP_Address, (void *)(&(Socket->sin_addr.s_addr))); //converts the character string ip into a network address
	if (Current_byte_received < 0){
		perror(" |ERROR| Initialization of Socket is failed"); return NULL;}
	if (Current_byte_received == 0 && !Current_byte_received < 0){
		fprintf(stderr, " |Warning| %s is not a valid IP address\n", IP_Address); return NULL;}
	Socket->sin_port = htons(port);/* htons handles byte ordering differences between computer architectures and network protocols*/
	bcopy(Host_Address -> h_addr_list[0], (void *)&(Socket -> sin_addr.s_addr), Host_Address -> h_length);
	if (connect(Socket_fd, (struct sockaddr*)Socket, sizeof(struct sockaddr)) < 0){
		perror(" |ERROR| Connection faild "); return NULL;	}
	sprintf(Requires, Message, Extract_pages, host, DEFAULT);
	fprintf(stderr, "\n**************************\n\nExtraction:\n%s**************************\n", Requires);
	while(Requires_to_Server < strlen(Requires)){
		Current_byte_received = send(Socket_fd, Requires + Requires_to_Server, strlen(Requires) - Requires_to_Server, 0);
		if(Current_byte_received == -1){
			perror(" |ERROR| Requires sending failed "); return NULL;}
		Requires_to_Server += Current_byte_received;// Current_byte_received is the number of characters sent
	}
	Byte_received = receive_query(Socket_fd);
	free(Socket); free(IP_Address); free(Requires); close(Socket_fd);
	return Byte_received;
}   // Lines 39

// split http content from the response string
char* http_split_content(char *response) {
    char* all_contents = strstr(response, "\r\n\r\n"); // Terminate Header
    *(all_contents+2) = '\0';
    all_contents+=4;
    return all_contents;    // Jump over two newlines.
}   //Lines 6
