#include "databaseManager.h"
#include "api.h"
#include <stdio.h>
#include "igdb_wrapper.h"

void get_new_game(char *json);

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

  bind_post_request("/game",get_new_game);

  start_server(1234);

  printf("----------------------------------\n\n");

  close_database();
  return 0;
}

void get_new_game(char *json) {
  printf("JSON:\n%s\n",json);
  char game_name[128];
  sscanf(json,"{\"game\":\"%[^\"]\"}",game_name);
  printf("Game: %s\n",game_name);

  IGDBEntry new_game = getGame(game_name,"PS2");

  print_entry(new_game);

  free_entry(&new_game);
}