#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>  // ssize_t
#include <sys/socket.h> // send(),recv()
#include <netdb.h>      // gethostbyname()
#include <ctype.h>

/**
* Client code
* 1. Create a socket and connect to the server specified in the command arugments.
* 2. Prompt the user for input and send that input as a message to the server.
* 3. Print the message received from the server and exit the program.
*/
char *send_to_serv(char *keyText, char *messageText) {
  char *combined;
  combined = calloc(strlen(messageText) + 1 + strlen(keyText), sizeof(char));
  strcpy(combined, messageText);
  //printf("combined -%s-\n", combined);
  strcat(combined, " ");
  //printf("combined -%s-\n", combined);
  strcat(combined, keyText);
  //printf("combined -%s-\n", combined);

  return combined;
}


char *readFile(char *filePath){
  char *currLine = NULL;
  size_t len = 0;
  FILE *inputFile = fopen(filePath, "r");
  fseek(inputFile, 0, SEEK_END);
  long size = ftell(inputFile);
  rewind(inputFile);
  char *readFileText = malloc(size +1);
  size_t readInBytes = fread(readFileText, 1, size, inputFile);
  readFileText[readInBytes] = '\0';
  fclose(inputFile);
  //printf("Read file: %s\n", readFileText);
  return readFileText;
};

// Error function used for reporting issues
void error(const char *msg) { 
  perror(msg); 
  exit(0); 
} 

// Set up the address struct
void setupAddressStruct(struct sockaddr_in* address, 
                        int portNumber, 
                        char* hostname){
 
  // Clear out the address struct
  memset((char*) address, '\0', sizeof(*address)); 

  // The address should be network capable
  address->sin_family = AF_INET;
  // Store the port number
  address->sin_port = htons(portNumber);

  //printf("portNumber: %d hostname: %s\n", portNumber, hostname);
  fflush(stdout);
  // Get the DNS entry for this host name
  struct hostent* hostInfo = gethostbyname(hostname); 
  if (hostInfo == NULL) { 
    fprintf(stderr, "CLIENT: ERROR, no such host\n"); 
    exit(0); 
  }
  // Copy the first IP address from the DNS entry to sin_addr.s_addr
  memcpy((char*) &address->sin_addr.s_addr, 
        hostInfo->h_addr_list[0],
        hostInfo->h_length);
}

int main(int argc, char *argv[]) {
  int socketFD, charsWritten, charsRead;
  struct sockaddr_in serverAddress;
  char buffer[80000];
  // Check usage & args

  for (int i = 0; i < argc; i++) {
    //printf("argv[%d]: %s\n", i, argv[i]);
}

// assign args to be 0-input file 1-key 2-port and set hostname to local host every time!!!!!!!!!!!!!!!!!!
  char* key = malloc(strlen(argv[2]));
  key = strcpy(key, argv[2]);
  char* message = malloc(strlen(argv[1]));
  message = strcpy(message, argv[1]);
  
  int portNumber  = atoi(argv[3]);
  char* hostname= malloc(10);
  hostname = strcpy(hostname, "localhost");

  //printf("hostname %s port %d\n", hostname, portNumber);
  fflush(stdout);

  if (argc < 3) { 
    fprintf(stderr,"USAGE: %s %d hostname port\n", hostname, portNumber); 
    exit(0); 
  } 
 // Get input message from user
  //printf("CLIENT: Enter text to send to the server, and then hit enter: ");
  char *keyText;
  keyText = malloc(strlen(readFile(key)));
  keyText = readFile(key);
  char *messageText;
  messageText = malloc(strlen(readFile(message)));
  messageText = readFile(message);
  int j;
  for (j=0 ; j < strlen(messageText); j++) {
    if (!((messageText[j] >= 'A' && messageText[j] <= 'Z') || (messageText[j] >= 'a' && messageText[j] <= 'z') || messageText[j] == ' '||  messageText[j] == '\n')){
      //printf("message: %s\n", messageText);
      fprintf(stderr, "enc_client error: input contains bad characters\n");
      exit(1);
      return 1;
    }
  }
  //printf("key: -%s-\n", keyText);
  //printf("message: -%s-\n", messageText); 
  if ((strlen(keyText)) < strlen(messageText)-1){
    //printf("length of key %ld\n", strlen(keyText));
    //printf("length of message %ld\n", strlen(messageText));

    fprintf(stderr,"Error key '%s' is too short\n", argv[2]);
    exit(1);
    return 1;

  }

  // Create a socket
  socketFD = socket(AF_INET, SOCK_STREAM, 0); 
  if (socketFD < 0){
    error("CLIENT: ERROR opening socket");
  }

   // Set up the server address struct
  setupAddressStruct(&serverAddress, portNumber, hostname);

  // Connect to server
  if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0){
    error("CLIENT: ERROR connecting");
  }

  // Clear out the buffer array
  memset(buffer, '\0', sizeof(buffer)+1);
  
  char *encryption_message = send_to_serv(keyText,messageText);
  //printf("concatenated message: %s\n", encryption_message);
  //strncpy(buffer, encryption_message, sizeof(buffer)-1);
  // Remove the trailing \n that fgets adds
  //buffer[strcspn(buffer, "\n")] = '\0'; 

  // Send message to server
  // Write to the server
  size_t sent = 0;
  size_t length = strlen(encryption_message);
  while (sent < length){
    charsWritten = send(socketFD, encryption_message + sent, length - sent, 0); 
    if (charsWritten < 0){
      error("CLIENT: ERROR writing to socket");
      exit(1);
    }
    if (charsWritten < strlen(encryption_message)){
      printf("CLIENT: WARNING: Not all data written to socket!\n");
    }
    if (charsWritten == 0) {
        fprintf(stderr, "CLIENT: Connection closed unexpectedly\n");
        exit(1);
    }
    sent += charsWritten;
  }

  

  // Get return message from server
  // Clear out the buffer again for reuse
  memset(buffer, '\0', sizeof(buffer));
  // Read data from the socket, leaving \0 at end
  charsRead = recv(socketFD, buffer, sizeof(buffer) - 1, 0); 
  if (charsRead < 0){
    error("CLIENT: ERROR reading from socket");
  }
  printf("%s\n", buffer);
  memset(buffer, '\0', sizeof(buffer));

  //printf("CLIENT: I received this from the server: \"%s\"\n", buffer);
  //printf("last char is... -%d-\n",buffer[strlen(buffer)]);

  // Close the socket
  close(socketFD); 
  return 0;
}