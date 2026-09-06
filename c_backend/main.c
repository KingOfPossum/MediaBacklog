#include "databaseManager.h"
#include "api.h"
#include <stdio.h>
#include "igdb_wrapper.h"
#include <stdlib.h>
#include <string.h>

char *get_new_game(char *json);
char *get_game_func(char *request);
char *get_library_func(char *request);

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

  bind_get_request("/getGame", get_game_func);
  bind_get_request("/getLibrary", get_library_func);
  bind_post_request("/game",get_new_game);

  start_server(1234);

  printf("----------------------------------\n\n");

  close_database();
  return 0;
}

char *get_new_game(char *json) {
  printf("-----------Adding new Game--------\n");

  char game_name[128];
  char platform[128];
  char status[128];
  sscanf(json,"{\"game\":\"%[^\"]\",\"platform\":\"%[^\"]\",\"status\":\"%[^\"]\"}",game_name,platform,status);
  
  printf("Searching for game: %s (%s) ...\n",game_name,platform);

  IGDBEntry new_game = getGame(game_name,platform);
  //print_entry(new_game);

  // if(new_game.igdb_id == -1) {
  //   printf("Game not found by IGDB :(\n");
  //   printf("----------------------------------\n\n");
  //   return strdup("NO GAME FOUND");
  // }

  APILibraryGameEntry library_game = add_game_to_library(game_name,platform,status,new_game);
  print_APILibraryGameEntry(library_game);  

  char *returned_json = APILibraryGameEntry_to_json(library_game);

  free_entry(&new_game);
  free_APILibraryGameEntry(&library_game);

  printf("----------------------------------\n\n");
  
  return returned_json;
}

char *get_game_func(char *request) {
  char *game_name = get_query_param(request, "name");
  char *game_platform = get_query_param(request, "platform");
  printf("Lookup for game %s (%s) in database...\n",game_name, game_platform);

  APILibraryGameEntry game = get_game_from_library_by_name(game_name);

  char *json = APILibraryGameEntry_to_json(game);

  free_APILibraryGameEntry(&game);

  printf("%s\n",json);

  return json;
}

char *get_library_func(char *request) {
  APILibrary library = get_library();

  char *json = APILibrary_to_json(library);

  free_APILibrary(&library);

  return json;

}