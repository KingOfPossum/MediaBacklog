#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include "database.h"
#include "igdb_wrapper.h"

typedef struct APIGameEntry {
  int id;
  int igdb_id;
  char *name;
  char *howlongtobeat_cover_url;
  int main_story_length;
  int main_extra_length;
  int completionist_length;
  int min_price;
  int avg_price;
  int max_price;
} APIGameEntry;

typedef struct APIIGDBEntry {
  int igdb_id;
  char *cover_url;
  char *summary;
  char **genres;
  int num_genres;
  char **platforms;
  int num_platforms;
} APIIGDBEntry;

typedef struct APILibraryGameEntry {
  APIGameEntry game_infos;
  APIIGDBEntry igdb_infos;
  int library_id;
  int user_id;
  char **consoles;
  int num_consoles;
  char *status;
} APILibraryGameEntry;

int init_database(char *path);
void close_database();

int game_exists(sqlite3 *connection,char *name);
int add_game(sqlite3 *connection,char *name, IGDBEntry igdb_entry);

int igdb_game_exists(sqlite3 *connection, IGDBEntry igdb_entry);
void igdb_add_game(sqlite3 *connection, IGDBEntry igdb_entry);

int igdb_platform_exists(sqlite3 *connection, char *platform);
int igdb_platform_add(sqlite3 *connection, char *platform);

int igdb_games_platform_exists(sqlite3 *connection, int igdb_id, int platform_id);
void igdb_games_platform_add(sqlite3 *connection, int igdb_id, int platform_id);

int igdb_genre_exists(sqlite3 *connection, char *genre);
int igdb_genre_add(sqlite3 *connection, char *genre);

int igdb_games_genre_exists(sqlite3 *connection, int igdb_id, int genre_id);
void igdb_games_genre_add(sqlite3 *connection, int igdb_id, int genre_id);

int game_exists_in_library(sqlite3 *connection, int user_id, int game_id);
int library_add_game(sqlite3 *connection, int user_id, int game_id, char *status);

int library_console_entry_exists(sqlite3 *connection, int library_id, char *platform);
void add_library_console_entry(sqlite3 *connection, int library_id, char *platform);

APILibraryGameEntry add_game_to_library(char *name, char *platform, char *status, IGDBEntry game);

APIGameEntry get_game(sqlite3 *connection, int game_id, char *name);
APIIGDBEntry get_igdb_infos(sqlite3 *connection, int igdb_id);
APILibraryGameEntry get_game_from_library(sqlite3 *connection, int game_id, int library_id);

void print_APIGameEntry(APIGameEntry game);
void print_APILibraryGameEntry(APILibraryGameEntry library_game);
void print_APIIGDBEntry(APIIGDBEntry igdb_entry);

#endif