#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include "database.h"
#include "igdb_wrapper.h"

typedef struct APIGameEntry {
  char *name;
  int library_id;
  int user_id;
  char **consoles;
  char *cover_url;
  char **genres;
  char **platforms;
  int main_story_length;
  int main_extra_length;
  int completionist_length;
  int min_price;
  int max_price;
  int avg_price;
  char *status;
} APIGameEntry;

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

int game_exists_in_library(sqlite3 *connection, int user_id, int game_id);
int library_add_game(sqlite3 *connection, int user_id, int game_id, char *platform, char *status);

int library_console_entry_exists(sqlite3 *connection, int library_id, char *platform);
void add_library_console_entry(sqlite3 *connection, int library_id, char *platform);

APIGameEntry add_game_to_library(char *name, char *platform, char *status, IGDBEntry game);

#endif