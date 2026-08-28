#ifndef API_H
#define API_H

typedef enum RequestType {
  GET_REQUEST,
  POST_REQUEST
} RequestType;

typedef struct binding {
  char *request;
  char *(*get_func)();
  char *(*post_func)(char *);
  RequestType type;
} binding;

typedef struct bindings {
  binding *all_bindings;
  int num_bindings;
} bindings;

void init_server();
void start_server(int port);
void bind_get_request(char *request, char *(*func)());
void bind_post_request(char *request, char *(*func)(char *));

#endif