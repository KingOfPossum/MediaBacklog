#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api.h"

#ifdef _WIN32
  #include <winsock2.h>
#elif defined (__linux__)
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <unistd.h>
#endif

#define BUFFER_SIZE 2048

bindings api_bindings;

void init_server() {
  memset(&api_bindings,0,sizeof(bindings));
}

void start_server(int port) {
  struct sockaddr_in service;
  char buffer[BUFFER_SIZE] = {0};

  #ifdef _WIN32
    int addrlen = sizeof(service);

    WSADATA wsadata;
    SOCKET server_socket, client_socket;
    int wsaerr;
    
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
  #elif defined(__linux__)
    int server_socket, client_socket;
    socklen_t addrlen = sizeof(service);
  #endif

  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  
  #ifdef _WIN32
    if(server_socket == INVALID_SOCKET) {
      printf("Socket failed with error: %d\n",WSAGetLastError());
      WSACleanup();
      return;
    }
    else{
      printf("Socket is OK\n");
    }
  #elif defined(__linux__)
    if(server_socket < 0) {
      printf("Socket failed with error");
      return;
    }
    else {
      printf("Socket is OK\n");
    }

    int opt = 1;
    if(setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
      printf("Error: setsockopt\n");
      close(server_socket);
      return;
    }
  #endif

  service.sin_family = AF_INET;
  service.sin_addr.s_addr = INADDR_ANY;
  service.sin_port = htons(port);

  #ifdef _WIN32
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
  #elif defined(__linux__)
    if(bind(server_socket, (struct sockaddr *)&service, sizeof(service)) < 0) {
      printf("Binding failed with error\n");
      close(server_socket);
      return;
    }

    if(listen(server_socket, 3) < 0){
      printf("Listening failed with error");
      close(server_socket);
      return;
    }

    printf("Linux-Server running on http://localhost:%d\n",port);
  #endif

  printf("Waiting for connections ...\n\n");

  while(1) {
    client_socket = accept(server_socket, (struct sockaddr*)&service,&addrlen);
    
    #ifdef _WIN32
      if(client_socket == INVALID_SOCKET) {
        printf("Accept failed!\n");
        continue;
      }
      
      int bytes_read = recv(client_socket, buffer, BUFFER_SIZE - 1,0);
      if(bytes_read <= 0) {
        closesocket(client_socket);
        continue;
      }
    #elif defined(__linux__)
      if(client_socket < 0) {
        printf("Accept failed!\n");
        continue;
      }

      ssize_t bytes_read = recv(client_socket, buffer, BUFFER_SIZE - 1,0);
      if(bytes_read <= 0) {
        close(client_socket);
        continue;
      }
    #endif

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
      
      #ifdef _WIN32
        closesocket(client_socket);
      #elif defined(__linux__)
        close(client_socket);
      #endif

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
      
      //printf("Checking for binding: %d (%s)\n",i,binding_request);
      if(strncmp(buffer,binding_request,strlen(binding_request)) == 0){
        if(api_bindings.all_bindings[i].type == GET_REQUEST) {
          char *header = 
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json\r\n"
            "Access-Control-Allow-Origin: *\r\n"
            "Connection: close\r\n"
            "\r\n";
          
          char *end_of_line = strstr(buffer, "\r\n");
          if(end_of_line != NULL) {
            size_t first_line_len = end_of_line - buffer;

            char *first_line_cpy = calloc(first_line_len + 1, sizeof(char));
            strncpy(first_line_cpy, buffer, first_line_len);

            char *response = api_bindings.all_bindings[i].get_func(first_line_cpy);
            free(first_line_cpy);

            send(client_socket, header, strlen(header), 0);

            if(response != NULL) { 
              send(client_socket,response,strlen(response),0);
            }
          }
        }
        else if(api_bindings.all_bindings[i].type == POST_REQUEST) {
          char *header =
            "HTTP/1.1 200 OK\r\n"
            "Access-Control-Allow-Origin: *\r\n"
            "Content-Type: application/json\r\n"
            "Connection: close\r\n"
            "\r\n";

          char *body_start = strstr(buffer,"\r\n\r\n");
          char *response_content = NULL;
          
          if(body_start != NULL) {
            char *json = body_start + 4;    
            response_content = api_bindings.all_bindings[i].post_func(json);
          } 
          else {
            printf("ERROR: Couldn't find body\n");
            response_content = strdup("{\"error\":\"No body found\"}");
          }

          send(client_socket,header,strlen(header),0);

          if(response_content != NULL) {
            send(client_socket, response_content, strlen(response_content), 0);
            free(response_content);
          }
        }
        
        break;
      }
    }

    #ifdef _WIN32
      closesocket(client_socket);
    #elif defined(__linux__)
      close(client_socket);
    #endif
    
    memset(buffer,0,BUFFER_SIZE);
  }

  #ifdef _WIN32
    closesocket(server_socket);
  #elif defined(__linux__)
    close(server_socket);
  #endif

  free(api_bindings.all_bindings);

  #ifdef _WIN32
    WSACleanup();
  #endif
}

void bind_get_request(char *request, char *(*func)(char *)) {
  api_bindings.num_bindings++;
  api_bindings.all_bindings = realloc(api_bindings.all_bindings,api_bindings.num_bindings * sizeof(binding));

  api_bindings.all_bindings[api_bindings.num_bindings-1].request = request;
  api_bindings.all_bindings[api_bindings.num_bindings-1].get_func = func;
  api_bindings.all_bindings[api_bindings.num_bindings-1].type = GET_REQUEST;
}

void bind_post_request(char *request, char *(*func)(char *)) {
  api_bindings.num_bindings++;
  api_bindings.all_bindings = realloc(api_bindings.all_bindings,api_bindings.num_bindings * sizeof(binding));

  api_bindings.all_bindings[api_bindings.num_bindings-1].request = request;
  api_bindings.all_bindings[api_bindings.num_bindings-1].post_func = func;
  api_bindings.all_bindings[api_bindings.num_bindings-1].type = POST_REQUEST;
}

void decode_url(char *url) {
  for(int i = 0; url[i] != '\0';i++){
    if(url[i] == '+'){
      url[i] = ' ';
    }
  }
}

char *get_query_param(const char *buffer, const char *key) {
  const char *query_start = strchr(buffer, '?');
  if (!query_start) {
    return NULL;
  }

  char search_key[128];
  snprintf(search_key, sizeof(search_key), "%s=", key);

  const char *key_pos = strstr(query_start, search_key);
  if(!key_pos) {
    return NULL;
  }

  const char *val_start = key_pos + strlen(search_key);
  const char *val_end = val_start;

  while(*val_end != '&' && *val_end != ' ' && *val_end != '\0'){
    val_end++;
  }

  size_t val_len = val_end - val_start;
  char *result = calloc(val_len + 1, sizeof(char));
  strncpy(result, val_start, val_len);
  result[val_len] = '\0';

  decode_url(result);

  return result;
}