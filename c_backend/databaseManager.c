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
  (char *[]){"id INTEGER PRIMARY KEY","user_id INTEGER NOT NULL","game_id INTEGER NOT NULL","status TEXT NOT NULL","FOREIGN KEY (game_id) REFERENCES games(id)"},
  5
};

database_table games_library_console_table = {
  "games_library_console",
  (char *[]){"library_id INTEGER","console TEXT","PRIMARY KEY(library_id,console)"},
  3
};

database_table igdb_games_table = {
  "igdb_games",
  (char *[]){"id INTEGER PRIMARY KEY","game_name TEXT NOT NULL","cover_url TEXT","summary TEXT"},
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

  insert_sql(connection, igdb_games_table, (char *[]){"id","game_name","cover_url","summary"},4,(char *[]){igdb_id_str,igdb_entry.game_name,igdb_entry.cover_url,igdb_entry.summary},4);

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

  printf("Adding all genres of the IGDB entry...\n");
  for(int i = 0;i < igdb_entry.num_genres;i++) {
    int genre_id = igdb_genre_exists(connection,igdb_entry.genres[i]);
    if(genre_id == -1) {
      printf("=> Genre %s was not in database!\nNow adding Genre to database...\n",igdb_entry.genres[i]);
      genre_id = igdb_genre_add(connection, igdb_entry.genres[i]);
    }
    else {
      printf("=> Genre %s was already in database\n",igdb_entry.genres[i]);
    }

    if(!igdb_games_genre_exists(connection, igdb_entry.igdb_id, genre_id)) {
      printf("=> Game-Genre entry was not in database!\nNow adding Game-Genre entry to database\n");
      igdb_games_genre_add(connection, igdb_entry.igdb_id, genre_id);
    }
    else {
      printf("=> Game-Genre entry was already in database\n");
    }
  }
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

int igdb_genre_exists(sqlite3 *connection, char *genre) {
  select_result *result = select_sql(connection, igdb_genres_table, (char *[]){"id"}, 1, "genre_name=?", (char *[]){genre}, 1);

  int id = -1;

  if(result->num_results > 0){
    id = atoi(result->all_results[0].data[0]);
  }

  free_select_results(result);
  return id;
}

int igdb_genre_add(sqlite3 *connection, char *genre) {
  insert_sql(connection, igdb_genres_table, (char *[]){"genre_name"}, 1, (char *[]){genre}, 1);

  return igdb_genre_exists(connection, genre);
}

int igdb_games_genre_exists(sqlite3 *connection, int igdb_id, int genre_id) {
  char igdb_id_str[16];
  char genre_id_str[16];
  snprintf(igdb_id_str, sizeof(igdb_id_str), "%d", igdb_id);
  snprintf(genre_id_str, sizeof(genre_id_str), "%d", genre_id);

  select_result *result = select_sql(connection, igdb_games_genre_table, NULL, 0, "game_id=? AND genre_id=?", (char *[]){igdb_id_str, genre_id_str}, 2);

  if(result->num_results > 0){
    free_select_results(result);
    return 1;
  }
  free_select_results(result);
  return 0;
}

void igdb_games_genre_add(sqlite3 *connection, int igdb_id, int genre_id) {
  char igdb_id_str[16];
  char genre_id_str[16];
  snprintf(igdb_id_str, sizeof(igdb_id_str), "%d", igdb_id);
  snprintf(genre_id_str, sizeof(genre_id_str), "%d", genre_id);

  insert_sql(connection, igdb_games_genre_table, (char *[]){"game_id","genre_id"}, 2, (char *[]){igdb_id_str, genre_id_str}, 2);
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

int library_add_game(sqlite3 *connection, int user_id, int game_id, char *status) {
  char user_id_str[16];
  char game_id_str[16];

  snprintf(user_id_str, sizeof(user_id_str), "%d", user_id);
  snprintf(game_id_str, sizeof(game_id_str), "%d", game_id);

  insert_sql(connection, games_library_table, (char *[]){"user_id","game_id","status"},3, (char *[]){user_id_str, game_id_str, status},3);

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

APIGameEntry get_game(sqlite3 *connection, int game_id, char *name) {
  select_result *result;

  if(game_id != -1) {
    char id_str[16];
    snprintf(id_str, sizeof(id_str), "%d", game_id);

    result = select_sql(connection, games_table, NULL, 0, "id=?", (char *[]){id_str}, 1);
  }
  else {
    result = select_sql(connection, games_table, NULL, 0, "name=?", (char *[]){name}, 1);
  }

  APIGameEntry game;  
  memset(&game, 0, sizeof(APIGameEntry));
  game.id = -1;

  if(result->num_results > 0) {
    game.id = atoi(result->all_results[0].data[0]);
    game.igdb_id = atoi(result->all_results[0].data[1]);
    game.name = strdup(result->all_results[0].data[2]);
    if(result->all_results[0].data[3] != NULL) {
      game.howlongtobeat_cover_url = strdup(result->all_results[0].data[3]);
    }    
    game.main_story_length = atoi(result->all_results[0].data[4]);
    game.main_extra_length = atoi(result->all_results[0].data[5]);
    game.completionist_length = atoi(result->all_results[0].data[6]);
    game.min_price = atoi(result->all_results[0].data[7]);
    game.avg_price = atoi(result->all_results[0].data[8]);
    game.max_price = atoi(result->all_results[0].data[9]);
  }

  free_select_results(result);
  
  return game;
}

APIIGDBEntry get_igdb_infos(sqlite3 *connection, int igdb_id) {
  char igdb_id_str[16];
  snprintf(igdb_id_str, sizeof(igdb_id_str), "%d", igdb_id);
  
  APIIGDBEntry igdb_infos;
  memset(&igdb_infos, 0, sizeof(APIIGDBEntry));
  igdb_infos.igdb_id = igdb_id;

  //Get the basic IGDB infos
  select_result *result = select_sql(connection, igdb_games_table, (char *[]){"cover_url","summary"}, 2, "id=?", (char *[]){igdb_id_str}, 1);

  if(result->num_results > 0) {
    igdb_infos.cover_url = strdup(result->all_results[0].data[0]);
    igdb_infos.summary = strdup(result->all_results[0].data[1]);
  }

  //Get all the genres for the game
  result = select_sql(connection, igdb_games_genre_table, (char *[]){"genre_id"}, 1, "game_id=?", (char *[]){igdb_id_str}, 1);
  
  if(result->num_results > 0){
    igdb_infos.num_genres = result->num_results;
    igdb_infos.genres = malloc(igdb_infos.num_genres * sizeof(char *));

    for(int i = 0;i < igdb_infos.num_genres;i++) {
      select_result *genre_result = select_sql(connection, igdb_genres_table, (char *[]){"genre_name"}, 1, "id=?", (char *[]){result->all_results[i].data[0]}, 1);
    
      if(genre_result->num_results > 0){
        igdb_infos.genres[i] = strdup(genre_result->all_results[0].data[0]);
      }

      free_select_results(genre_result);
    }
  }

  //Get all the platforms for the game
  result = select_sql(connection, igdb_games_platforms_table, (char *[]){"platform_id"}, 1, "game_id=?", (char *[]){igdb_id_str}, 1);
  
  if(result->num_results > 0){
    igdb_infos.num_platforms = result->num_results;
    igdb_infos.platforms = malloc(igdb_infos.num_platforms * sizeof(char *));

    for(int i = 0;i < igdb_infos.num_platforms;i++) {
      select_result *platform_result = select_sql(connection, igdb_platforms_table, (char *[]){"platform_name"}, 1, "id=?", (char *[]){result->all_results[i].data[0]}, 1);
    
      if(platform_result->num_results > 0){
        igdb_infos.platforms[i] = strdup(platform_result->all_results[0].data[0]);
      }

      free_select_results(platform_result);
    }
  }

  free_select_results(result);

  return igdb_infos;
}

APILibraryGameEntry get_game_from_library(sqlite3 *connection, int game_id, int library_id) {
  //Get general informations for the game
  APIGameEntry game_infos = get_game(connection, game_id, "");

  //Get IGDB informations for the game
  APIIGDBEntry igdb_infos;
  memset(&igdb_infos, 0, sizeof(APIIGDBEntry));
  igdb_infos.igdb_id = -1;

  if(game_infos.igdb_id != -1) {
    igdb_infos = get_igdb_infos(connection, game_infos.igdb_id);
  }

  //Get Library game infos
  APILibraryGameEntry entry;
  memset(&entry, 0, sizeof(APILibraryGameEntry));

  memcpy(&entry.game_infos, &game_infos, sizeof(APIGameEntry));
  memcpy(&entry.igdb_infos, &igdb_infos, sizeof(APIIGDBEntry));
  entry.library_id = library_id;

  char library_id_str[16];
  snprintf(library_id_str, sizeof(library_id_str), "%d", library_id);

  select_result *result = select_sql(connection, games_library_table, (char *[]){"user_id","status"}, 2, "id=?", (char *[]){library_id_str}, 1);
  if(result->num_results > 0){
    entry.user_id = atoi(result->all_results[0].data[0]);
    entry.status = strdup(result->all_results[0].data[1]);
  }

  //Get all consoles that game was added in the library
  result = select_sql(connection, games_library_console_table, (char *[]){"console"}, 1, "library_id=?", (char *[]){library_id_str},1);
  if(result->num_results > 0){
    entry.num_consoles = result->num_results;
    entry.consoles = malloc(result->num_results * sizeof(char *));

    for(int i = 0;i < result->num_results;i++) {
      entry.consoles[i] = strdup(result->all_results[i].data[0]);
    }
  }

  free_select_results(result);

  return entry;
}

void print_APIGameEntry(APIGameEntry game) {
  printf("\nAPIGameEntry:\n");
  printf("  ID: %d\n",game.id);
  printf("  IGDB ID: %d\n",game.igdb_id);
  printf("  NAME: %s\n",game.name ? game.name : "N/A");
  printf("  HOWLONGTOBEAT COVER URL: %s\n",game.howlongtobeat_cover_url ? game.howlongtobeat_cover_url : "N/A");
  printf("  MAIN STORY LENGTH: %d\n", game.main_story_length);
  printf("  MAIN EXTRA LENGTH: %d\n", game.main_extra_length);
  printf("  COMPLETIONIST LENGTH: %d\n", game.completionist_length);
  printf("  MIN PRICE: %d\n", game.min_price);
  printf("  AVG PRICE: %d\n", game.avg_price);
  printf("  MAX_PRICE: %d\n", game.max_price);
  printf("\n");
}

void print_APIIGDBEntry(APIIGDBEntry igdb_entry) {
  printf("\nAPIIGDBEntry:\n");
  printf("  ID: %d\n",igdb_entry.igdb_id);
  printf("  COVER URL: %s\n",igdb_entry.cover_url);
  printf("  SUMMARY: %.150s...\n", igdb_entry.summary);
  printf("  GENRES:\n");
  for(int i = 0;i < igdb_entry.num_genres;i++){
    printf("    -%s\n", igdb_entry.genres[i]);
  }
  printf("  PLATFORMS:\n");
  for(int i = 0;i < igdb_entry.num_platforms;i++) {
    printf("    -%s\n", igdb_entry.platforms[i]);
  }
  printf("\n");
}

void print_APILibraryGameEntry(APILibraryGameEntry library_game) {
  printf("\n#############################\n");
  printf("APILibraryGameEntry:\n");
  print_APIGameEntry(library_game.game_infos);
  print_APIIGDBEntry(library_game.igdb_infos);
  printf("  ID: %d\n", library_game.library_id);
  printf("  USER ID: %d\n", library_game.user_id);
  printf("  CONSOLES:\n");
  for(int i = 0;i < library_game.num_consoles;i++) {
    printf("    -%s\n",library_game.consoles[i]);
  }
  printf("  STATUS: %s\n", library_game.status);
  printf("#############################\n\n");
}

char *APIGameEntry_to_json(APIGameEntry game) {
  char *json = malloc(2048);

  snprintf(json, 2048,
     "{"
      "\"id\":%d,"
      "\"igdb_id\":%d,"
      "\"name\":\"%s\","
      "\"howlongtobeat_cover_url\":\"%s\","
      "\"main_story_length\":%d,"
      "\"main_extra_length\":%d,"
      "\"completionist_length\":%d,"
      "\"min_price\":%d,"
      "\"avg_price\":%d,"
      "\"max_price\":%d"
    "}",
    game.id,
    game.igdb_id,
    game.name,
    game.howlongtobeat_cover_url,
    game.main_story_length,
    game.main_extra_length,
    game.completionist_length,
    game.min_price,
    game.avg_price,
    game.max_price
  );

  return json;
}

char *APIIGDBEntry_to_json(APIIGDBEntry igdb_infos) {
  char *json = calloc(2048,sizeof(char));
  char *genres = calloc(2048,sizeof(char)); 
  char *platforms = calloc(2048,sizeof(char));
  
  if(igdb_infos.num_genres > 0){
    for(int i = 0;i < igdb_infos.num_genres-1;i++){
      strcat(genres, "\"");
      strcat(genres, igdb_infos.genres[i]);
      strcat(genres, "\",");
    }
    strcat(genres, "\"");
    strcat(genres, igdb_infos.genres[igdb_infos.num_genres-1]);
    strcat(genres, "\"");
  }

  if(igdb_infos.num_platforms > 0) {
    for(int i = 0;i < igdb_infos.num_platforms-1;i++){
      strcat(platforms, "\"");
      strcat(platforms, igdb_infos.platforms[i]);
      strcat(platforms, "\",");
    }
    strcat(platforms, "\"");
    strcat(platforms, igdb_infos.platforms[igdb_infos.num_platforms-1]);
    strcat(platforms, "\"");
  }

  snprintf(json, 2048,
    "{"
      "\"igdb_id\":%d,"
      "\"cover_url\":\"%s\","
      "\"summary\":\"%s\","
      "\"genres\":[%s],"
      "\"platforms\":[%s]"
    "}",
    igdb_infos.igdb_id,
    igdb_infos.cover_url,
    igdb_infos.summary,
    genres,
    platforms
  );

  free(genres);
  free(platforms);

  printf("%s\n\n",json);

  return json;
}

char *APILibraryGameEntry_to_json(APILibraryGameEntry library_entry) {
  char *json = calloc(4096, sizeof(char));
  char *consoles = calloc(2048, sizeof(char));

  if(library_entry.num_consoles > 0){
    for(int i = 0;i < library_entry.num_consoles-1;i++){
      strcat(consoles, "\"");
      strcat(consoles, library_entry.consoles[i]);
      strcat(consoles, "\",");
    }
    strcat(consoles, "\"");
    strcat(consoles, library_entry.consoles[library_entry.num_consoles-1]);
    strcat(consoles, "\"");
  }

  char *game_infos_json = APIGameEntry_to_json(library_entry.game_infos);
  char *igdb_infos_json = APIIGDBEntry_to_json(library_entry.igdb_infos);

  snprintf(json, 4096,
    "{"
      "\"game_infos\":%s,"
      "\"igdb_infos\":%s,"
      "\"library_id\":%d,"
      "\"user_id\":%d,"
      "\"consoles\":[%s],"
      "\"status\":\"%s\""
    "}",
    game_infos_json,
    igdb_infos_json,
    library_entry.library_id,
    library_entry.user_id,
    consoles,
    library_entry.status
  );

  free(consoles);
  free(game_infos_json);
  free(igdb_infos_json);

  return json;
}

APILibraryGameEntry add_game_to_library(char *name, char *platform, char *status, IGDBEntry game) {
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
    library_id = library_add_game(connection, -1, game_id, status);
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

  APILibraryGameEntry entry = get_game_from_library(connection, game_id, library_id);

  sqlite3_close(connection);
  
  return entry;
}