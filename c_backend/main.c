#include "databaseManager.h"
#include "api.h"
#include <stdio.h>
#include "igdb_wrapper.h"

void get_new_game(char *json);
char *get_test();

int main(int argc, char *argv[]) {
  printf("--------------Database------------\n");

  init_database("./data/database.db");

  printf("----------------------------------\n\n");


  printf("--------------IGDB TEST-----------\n");

  IGDBEntry game = getGame("Skyrim","PC");
  print_entry(game);
  free_entry(&game);

  printf("----------------------------------\n\n");


  printf("--------------Server--------------\n");

  bind_get_request("/test",get_test);
  bind_post_request("/game",get_new_game);

  start_server(1234);

  printf("----------------------------------\n\n");

  close_database();
  return 0;
}

void get_new_game(char *json) {
  printf("-----------Adding new Game--------\n");

  char game_name[128];
  char platform[128];
  char status[128];
  sscanf(json,"{\"game\":\"%[^\"]\",\"platform\":\"%[^\"]\",\"status\":\"%[^\"]\"}",game_name,platform,status);
  
  printf("Searching for game: %s (%s) ...\n",game_name,platform);

  IGDBEntry new_game = getGame(game_name,platform);
  print_entry(new_game);

  if(new_game.igdb_id == -1) {
    printf("Game not found by IGDB :(\n");
    return;
  }

  add_game_to_library(game_name,platform,status,new_game);

  free_entry(&new_game);

  printf("----------------------------------\n\n");
}

char *get_test() {
  printf("TEST!!!\n\n");
  return "{\"msg\": \"This is another test ;)\"}";
}