#include <stdio.h>
#include "databaseManager.h"
#include "server.h"

int main(int argc, char *argv[]) {
  printf("--------------Database------------\n");
  
  init_database("./data/database.db");
  
  printf("----------------------------------\n\n");
  
  printf("--------------Server--------------\n");
  
  server();
  
  printf("----------------------------------\n\n");
  
  close_database();

  return 0;
}