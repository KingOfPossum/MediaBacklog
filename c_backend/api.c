#include <winsock2.h>
#include <stdio.h>
#include "api.h"

#define BUFFER_SIZE 2048

bindings api_bindings;

void init_server() {
  memset(&api_bindings,0,sizeof(bindings));
}

void start_server(int port) {
  WSADATA wsadata;
  SOCKET server_socket, client_socket;
  int wsaerr;
  struct sockaddr_in service;
  int addrlen = sizeof(service);
  char buffer[BUFFER_SIZE] = {0};

  WORD wVersionRequested = MAKEWORD(2,2);

  wsaerr = WSAStartup(wVersionRequested,&wsadata);
  if(wsaerr != 0) {
    printf("Winsock dll not found!\n");
  }
  else {
    printf("Winsock dll found!\n");
    printf("Status: %s\n",wsadata.szSystemStatus);
  }

  server_socket = INVALID_SOCKET;
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if(server_socket == INVALID_SOCKET) {
    printf("Socket failed with error: %d\n",WSAGetLastError());
    WSACleanup();
    return;
  }
  else{
    printf("Socket is OK\n");
  }

  service.sin_family = AF_INET;
  service.sin_addr.s_addr = INADDR_ANY;
  service.sin_port = htons(port);

  if(bind(server_socket, (struct sockaddr *)&service, sizeof(service)) == SOCKET_ERROR) {
    printf("Binding failed with error: %d\n",WSAGetLastError());
    closesocket(server_socket);
    WSACleanup();
    return;
  }

  if(listen(server_socket, 3) == SOCKET_ERROR) {
    printf("Listening failed with error: %d\n",WSAGetLastError());
    closesocket(server_socket);
    WSACleanup();
    return;
  }

  printf("Windows-Server running on http://localhost:%d\n",port);
  printf("Waiting for connections ...\n\n");

  while(1) {
    client_socket = accept(server_socket, (struct sockaddr*)&service,&addrlen);
    if(client_socket == INVALID_SOCKET) {
      printf("Accept failed!\n");
      continue;
    }

    recv(client_socket, buffer, BUFFER_SIZE - 1,0);
    printf("Request received:\n\n%s\n\n",buffer);
    
    // CORS Preflight header
    if(strncmp(buffer, "OPTIONS",7) == 0) {
      char *cors_response = 
        "HTTP/1.1 204 No Content\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Access-Control-Allow-Methods: POST, GET, OPTIONS\r\n"
        "Access-Control-Allow-Headers: Content-Type\r\n"
        "Connection: close\r\n"
        "\r\n";

      send(client_socket,cors_response, strlen(cors_response),0);
      printf("Responded to CORS Preflight!\n\n");
      
      closesocket(client_socket);
      memset(buffer,0,BUFFER_SIZE);
      continue;
    }

    for(int i = 0;i < api_bindings.num_bindings;i++) {
      char binding_request[128] = "\0";
      
      if(api_bindings.all_bindings[i].type == GET_REQUEST) {
        strcat(binding_request,"GET ");
      }
      else if(api_bindings.all_bindings[i].type == POST_REQUEST) {
        strcat(binding_request,"POST ");
      }
      strcat(binding_request,api_bindings.all_bindings[i].request);
      
      printf("Checking for binding: %d (%s)\n",i,binding_request);
      if(strncmp(buffer,binding_request,strlen(binding_request)) == 0){
        if(api_bindings.all_bindings[i].type == GET_REQUEST) {
          char *response = api_bindings.all_bindings[i].get_func();
          send(client_socket,response,strlen(response),0);
        }
        else if(api_bindings.all_bindings[i].type == POST_REQUEST) {
          char *post_response = 
            "HTTP/1.1 204 No Content\r\n"
            "Access-Control-Allow-Origin: *\r\n"
            "Connection: close\r\n"
            "\r\n";
          
          send(client_socket,post_response,strlen(post_response),0);

          char *body_start = strstr(buffer,"\r\n\r\n");

          if(body_start != NULL) {
            char *json = body_start + 4;    
            api_bindings.all_bindings[i].post_func(json);
          } 
          else {
            printf("ERROR: Couldn't find body\n");
          }
        }
        
        closesocket(client_socket);
        memset(buffer,0,BUFFER_SIZE);
        break;
      }
    }

    closesocket(client_socket);
    memset(buffer,0,BUFFER_SIZE);
  }

  closesocket(server_socket);

  free(api_bindings.all_bindings);

  WSACleanup();
}

void bind_get_request(char *request, char *(*func)()) {
  api_bindings.num_bindings++;
  api_bindings.all_bindings = realloc(api_bindings.all_bindings,api_bindings.num_bindings * sizeof(binding));

  api_bindings.all_bindings[api_bindings.num_bindings-1].request = request;
  api_bindings.all_bindings[api_bindings.num_bindings-1].get_func = func;
  api_bindings.all_bindings[api_bindings.num_bindings-1].type = GET_REQUEST;
}

void bind_post_request(char *request, void (*func)(char *)) {
  api_bindings.num_bindings++;
  api_bindings.all_bindings = realloc(api_bindings.all_bindings,api_bindings.num_bindings * sizeof(binding));

  api_bindings.all_bindings[api_bindings.num_bindings-1].request = request;
  api_bindings.all_bindings[api_bindings.num_bindings-1].post_func = func;
  api_bindings.all_bindings[api_bindings.num_bindings-1].type = POST_REQUEST;
}