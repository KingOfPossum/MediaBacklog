#include <string.h>
#include "databaseManager.h"
#include <stdio.h>
#include <stdlib.h>

database_table games_table = {
  "games",
  (char *[]){"id INTEGER PRIMARY KEY","igdb_id INTEGER","name TEXT NOT NULL","howlongtobeat_cover_url TEXT","main_story_length INTEGER","main_extra_length INTEGER","completionist_length INTEGER","min_price INTEGER","avg_price INTEGER","max_price INTEGER"},
  10
};

database_table games_backlog_table = {
  "games_backlog",
  (char *[]){"id INTEGER PRIMARY KEY","user_id INTEGER NOT NULL","game_id INTEGER NOT NULL"},
  3
};

database_table games_library_table = {
  "games_library",
  (char *[]){"id INTEGER PRIMARY KEY","user_id INTEGER NOT NULL","game_id INTEGER NOT NULL","console TEXT NOT NULL","status TEXT NOT NULL","FOREIGN KEY (game_id) REFERENCES games(id)"},
  6
};

database_table games_library_console_table = {
  "games_library_console",
  (char *[]){"library_id INTEGER","console TEXT","PRIMARY KEY(library_id,console)"},
  3
};

database_table igdb_games_table = {
  "igdb_games",
  (char *[]){"id INTEGER PRIMARY KEY","game_name TEXT NOT NULL","cover_url TEXT","description TEXT"},
  4
};

database_table igdb_games_genre_table = {
  "igdb_games_genres",
  (char *[]){"game_id INTEGER NOT NULL","genre_id INTEGER NOT NULL","FOREIGN KEY (game_id) REFERENCES igdb_games(id)","FOREIGN KEY (genre_id) REFERENCES igdb_genres(id)"},
  4
};

database_table igdb_games_platforms_table = {
  "igdb_games_platforms",
  (char *[]){"game_id INTEGER NOT NULL","platform_id INTEGER NOT NULL","FOREIGN KEY (game_id) REFERENCES igdb_games(id)","FOREIGN KEY (platform_id) REFERENCES igdb_platforms(platform_id)"},
  4
};

database_table igdb_genres_table = {
  "igdb_genres",
  (char *[]){"id INTEGER PRIMARY KEY","genre_name TEXT NOT NULL"},
  2
};

database_table igdb_platforms_table = {
  "igdb_platforms",
  (char *[]){"id INTEGER PRIMARY KEY","platform_name TEXT NOT NULL"},
  2
};

database_table users_table = {
  "users",
  (char *[]){"id INTEGER PRIMARY KEY","user_name TEXT UNIQUE","password TEXT","profile_picture_url TEXT","is_admin INTEGER DEFAULT 0"},
  5
};

database db;

int init_database(char *path) {
  memset(&db,0,sizeof(database));

  database_table tables[10] = {
    games_table,
    games_backlog_table,
    games_library_table,
    games_library_console_table,
    igdb_games_table,
    igdb_games_genre_table,
    igdb_games_platforms_table,
    igdb_genres_table,
    igdb_platforms_table,
    users_table
  };

  db.file_name = strdup(path);
  db.num_tables = 10;
  db.tables = malloc(db.num_tables * sizeof(database_table));
  memcpy(db.tables,tables, db.num_tables * sizeof(database_table));

  sqlite3 *connection = open_connection(db.file_name);

  printf("Opened database\n");

  init_database_tables(connection,db);

  printf("Initialized database tables\n");

  sqlite3_close(connection);
  return 0;
}

void close_database() {
  free(db.tables);
}

int game_exists(sqlite3 *connection,char *name) {
  select_result *result = select_sql(connection,games_table,(char *[]){"id"},1,"name=?",(char *[]){name},1);
  
  int id = -1;
  if(result ->num_results > 0) {
    id = atoi(result->all_results[0].data[0]);
  }

  free_select_results(result);
  return id;
}

int add_game(sqlite3 *connection,char *name, IGDBEntry igdb_entry) {  
  char igdb_id_str[16];
  snprintf(igdb_id_str,sizeof(igdb_id_str),"%d",igdb_entry.igdb_id);

  insert_sql(connection, games_table, (char *[]){"name","igdb_id"}, 2, (char *[]){name, igdb_id_str},2);

  int game_id = game_exists(connection,name);

  if(igdb_game_exists(connection, igdb_entry)) {
    printf("=> IGDB game already in database!\n");
    return game_id;
  }
  printf("=> IGDB game not in database\nNow adding IGDB game to database...\n");
  igdb_add_game(connection, igdb_entry);

  return game_id;
}

int igdb_game_exists(sqlite3 *connection, IGDBEntry igdb_entry) {
  char igdb_id_str[16];
  snprintf(igdb_id_str, sizeof(igdb_id_str), "%d", igdb_entry.igdb_id);

  select_result *result = select_sql(connection, igdb_games_table, NULL, 0, "id=?", (char *[]){igdb_id_str},1);

  if(result->num_results > 0) {
    free_select_results(result);
    return 1;
  }
  free_select_results(result);
  return 0;
}

void igdb_add_game(sqlite3 *connection, IGDBEntry igdb_entry) {
  char igdb_id_str[16];
  snprintf(igdb_id_str, sizeof(igdb_id_str), "%d", igdb_entry.igdb_id);

  // Needs to add the real desciption of a igdb game
  insert_sql(connection, igdb_games_table, (char *[]){"id","game_name","cover_url","description"},4,(char *[]){igdb_id_str,igdb_entry.game_name,igdb_entry.cover_url,""},4);

  printf("Adding all platforms of the IGDB entry...\n");
  for(int i = 0;i < igdb_entry.num_platforms;i++){
    int platform_id = igdb_platform_exists(connection, igdb_entry.platforms[i]);
    if(platform_id == -1){
      printf("=> Platform %s was not in database\nNow adding Platform to database...\n", igdb_entry.platforms[i]);
      platform_id = igdb_platform_add(connection, igdb_entry.platforms[i]);
    }
    else {
      printf("=> Platform %s was already in database\n", igdb_entry.platforms[i]);
    }

    if(!igdb_games_platform_exists(connection, igdb_entry.igdb_id, platform_id)) {
      printf("=> Game-Platform entry was not in database!\nNow adding Game-Platform entry to database\n");
      igdb_games_platform_add(connection, igdb_entry.igdb_id, platform_id);
    }
    else {
      printf("=> Game-Platform entry was already in database\n");
    }
  }

  //TODO: Adding genres to the database
}

int igdb_platform_exists(sqlite3 *connection, char *platform) {
  select_result *result = select_sql(connection, igdb_platforms_table, (char *[]){"id"}, 1, "platform_name=?", (char *[]){platform}, 1);

  int id = -1;

  if(result->num_results > 0){
    id = atoi(result->all_results[0].data[0]);
  }

  free_select_results(result);
  return id;
}

int igdb_platform_add(sqlite3 *connection, char *platform) {
  insert_sql(connection, igdb_platforms_table, (char *[]){"platform_name"}, 1, (char *[]){platform}, 1);

  return igdb_platform_exists(connection, platform);
}

int igdb_games_platform_exists(sqlite3 *connection, int igdb_id, int platform_id) {
  char igdb_id_str[16];
  char platform_id_str[16];
  snprintf(igdb_id_str, sizeof(igdb_id_str), "%d", igdb_id);
  snprintf(platform_id_str, sizeof(platform_id_str), "%d", platform_id);

  select_result *result = select_sql(connection, igdb_games_platforms_table, NULL, 0, "game_id=? AND platform_id=?", (char *[]){igdb_id_str, platform_id_str}, 2);

  if(result->num_results > 0){
    free_select_results(result);
    return 1;
  }
  free_select_results(result);
  return 0;
}

void igdb_games_platform_add(sqlite3 *connection, int igdb_id, int platform_id) {
  char igdb_id_str[16];
  char platform_id_str[16];
  snprintf(igdb_id_str, sizeof(igdb_id_str), "%d", igdb_id);
  snprintf(platform_id_str, sizeof(platform_id_str), "%d", platform_id);

  insert_sql(connection, igdb_games_platforms_table, (char *[]){"game_id","platform_id"}, 2, (char *[]){igdb_id_str, platform_id_str}, 2);
}

int game_exists_in_library(sqlite3 *connection, int user_id, int game_id) {
  char user_id_str[16];
  char game_id_str[16];

  snprintf(user_id_str, sizeof(user_id_str), "%d", user_id);
  snprintf(game_id_str, sizeof(game_id_str), "%d", game_id);

  select_result *result = select_sql(connection, games_library_table, (char *[]){"id"}, 1, "user_id=? AND game_id=?", (char *[]){user_id_str, game_id_str},2);

  int id = -1;
  if(result->num_results > 0){
    id = atoi(result->all_results[0].data[0]);
  }

  free_select_results(result);
  return id;
}

int library_add_game(sqlite3 *connection, int user_id, int game_id, char *platform, char *status) {
  char user_id_str[16];
  char game_id_str[16];

  snprintf(user_id_str, sizeof(user_id_str), "%d", user_id);
  snprintf(game_id_str, sizeof(game_id_str), "%d", game_id);

  insert_sql(connection, games_library_table, (char *[]){"user_id","game_id","console","status"},4, (char *[]){user_id_str, game_id_str, platform, status},4);

  return game_exists_in_library(connection,user_id,game_id);
}

int library_console_entry_exists(sqlite3 *connection, int library_id, char *platform) {
  char library_id_str[16];
  snprintf(library_id_str, sizeof(library_id_str), "%d", library_id);

  select_result *result = select_sql(connection, games_library_console_table, NULL, 0, "library_id=? AND console=?",(char *[]){library_id_str,platform},2);
  if(result->num_results > 0){
    free_select_results(result);
    return 1;
  }
  free_select_results(result);
  return 0;
}

void add_library_console_entry(sqlite3 *connection, int library_id, char *platform) {
  char library_id_str[16];
  snprintf(library_id_str, sizeof(library_id_str), "%d", library_id);

  insert_sql(connection, games_library_console_table, (char *[]){"library_id","console"}, 2, (char *[]){library_id_str, platform}, 2);
}

APIGameEntry get_game(sqlite3 *connection, int game_id, int library_id) {
  
}

APIGameEntry add_game_to_library(char *name, char *platform, char *status, IGDBEntry game) {
  sqlite3 *connection = open_connection(db.file_name);

  int game_id = game_exists(connection,name);
  if(game_id == -1) {
    printf("=> Game not in database!\nNow adding game to database...\n");
    game_id = add_game(connection,name,game);    
  }
  else {
    printf("=> Game is already in database!\n");
  }

  int library_id = game_exists_in_library(connection, -1, game_id);
  if(library_id == -1){
    printf("=> Game not in library!\nNow adding to library...\n");
    library_id = library_add_game(connection, -1, game_id, platform, status);
  }
  else {
    printf("=> Game is already in library!\n");
  }

  if(!library_console_entry_exists(connection,library_id, platform)) {
    printf("=> Library-Console entry does not exists!\nNow adding entry...\n");
    add_library_console_entry(connection, library_id, platform);
  }
  else {
    printf("=> Library-Console entry does already exists!\n");
  }

  APIGameEntry entry;
  memset(&entry, 0, sizeof(APIGameEntry));

  entry.name = strdup(name);
  entry.library_id = library_id;
  entry.user_id = -1;
  //entry.consoles = 
  entry.cover_url = strdup(game.cover_url);
  //entry.genres = 
  entry.platforms = malloc(game.num_platforms * sizeof(char) * 128);
  memcpy(entry.platforms, game.platforms, sizeof(game.platforms));
  entry.status = strdup(status);

  sqlite3_close(connection);

  return entry;
}