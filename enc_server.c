#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>


// Error function used for reporting issues
void error(const char *msg) {
  perror(msg);
  exit(1);
} 

char *encryption(char *buffer) {
  char *messageText;
  char *keyText;
  messageText = strtok(buffer, "\n");
  keyText = strtok(NULL," ");
  char *enc_string;
  enc_string = calloc(strlen(messageText), sizeof(char));
  printf("str len of messageText %ld\n", strlen(messageText));
  printf("message to check last char -%s-\n", messageText);
  int i;
  for (int i = 0; i < strlen(messageText); i++) {
    messageText[i] = toupper(messageText[i]);
  }
  for (int i=0; i<strlen(messageText); i++) {
    printf("i: %d\n", i);
    int conv;
    printf("messageText[i]: %d\n", messageText[i]-65);
    printf("keyText[i]: %d\n", keyText[i]-65);
    conv = (((messageText[i]-65) + (keyText[i]-65)));
    if (conv == 27) {
      conv = 32;
    } else if (conv <= 26) {
      conv = (conv + 65);
    } else if (conv > 27 ) {
      conv = (conv - 26) + 65;
    }
    printf("number conversion: %d\n", conv);

    if (((messageText[i] + keyText[i])) == 91) {
      enc_string[i] = ' ';
    } else {
      enc_string[i] = conv ;
    }
  }

  printf("encryption string: -%s-\n", enc_string);
  //enc_string[strlen(messageText)] = '\0';
  
  return enc_string;
}

// Set up the address struct for the server socket
void setupAddressStruct(struct sockaddr_in* address, 
                        int portNumber){
 
  // Clear out the address struct
  memset((char*) address, '\0', sizeof(*address)); 

  // The address should be network capable
  address->sin_family = AF_INET;
  // Store the port number
  address->sin_port = htons(portNumber);
  // Allow a client at any address to connect to this server
  address->sin_addr.s_addr = INADDR_ANY;
}

int main(int argc, char *argv[]){
  int connectionSocket, charsRead;
  char buffer[256];
  struct sockaddr_in serverAddress, clientAddress;
  socklen_t sizeOfClientInfo = sizeof(clientAddress);

  // Check usage & args
  if (argc < 2) { 
    fprintf(stderr,"USAGE: %s port\n", argv[0]); 
    exit(1);
  } 
  
  // Create the socket that will listen for connections
  int listenSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (listenSocket < 0) {
    error("ERROR opening socket");
  }

  // Set up the address struct for the server socket
  setupAddressStruct(&serverAddress, atoi(argv[1]));

  // Associate the socket to the port
  if (bind(listenSocket, 
          (struct sockaddr *)&serverAddress, 
          sizeof(serverAddress)) < 0){
    error("ERROR on binding");
  }

  // Start listening for connetions. Allow up to 5 connections to queue up
  listen(listenSocket, 5); 
  
  // Accept a connection, blocking if one is not available until one connects
  while(1){
    // Accept the connection request which creates a connection socket
    connectionSocket = accept(listenSocket, 
                (struct sockaddr *)&clientAddress, 
                &sizeOfClientInfo); 
    if (connectionSocket < 0){
      error("ERROR on accept");
    }

    printf("SERVER: Connected to client running at host %d port %d\n", 
                          ntohs(clientAddress.sin_addr.s_addr),
                          ntohs(clientAddress.sin_port));

    // Get the message from the client and display it
    memset(buffer, '\0', 256);
    printf("message: %s", buffer);
    //printf("After encryption... %s\n", encryption(buffer));
    // Read the client's message from the socket
    charsRead = recv(connectionSocket, buffer, 255, 0); 
    if (charsRead < 0){
      error("ERROR reading from socket");
    }
    // ENCODE THE MESSAGE HERE!!!!!!!!!!
    printf("SERVER: I received this from the client: \"%s\"\n", buffer);

    char *returnMessage = encryption(buffer);
    // Send a Success message back to the client
    charsRead = send(connectionSocket, 
                    returnMessage, strlen(returnMessage), 0); 
    if (charsRead < 0){
      error("ERROR writing to socket");
    }
    // Close the connection socket for this client
    free(returnMessage);
    close(connectionSocket); 
  }
  // Close the listening socket
  close(listenSocket); 
  return 0;
}
