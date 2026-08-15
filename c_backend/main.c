#include "database.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  database_table games_table = {
    "games",
    (char *[]){"id INTEGER PRIMARY KEY","igdb_id INTEGER","name TEXT NOT NULL","howlongtobeat_cover_url TEXT","main_story_length INTEGER","main_extra_length INTEGER","completionist_length INTEGER","min_price INTEGER","avg_price INTEGER","max_price INTEGER"},
    10
  };
  
  database_table games_backlog_table = {
    "games_backlog",
    (char *[]){"id INTGER PRIMARY KEY","user_id INTEGER NOT NULL","game_id INTEGER NOT NULL"},
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

  database_table igdb_games_genre_table = {
    "igdb_games_genres",
    (char *[]){"game_id INTEGER NOT NULL","genre_id INTEGER NOT NULL","FOREIGN KEY (game_id) REFERENCES igdb_games(id)","FOREIGN KEY (genre_id) REFERENCES igdb_genres(id)"},
    4
  };

  database_table igdb_games_platforms_table = {
    "igdb_games_platforms",
    (char *[]){"game_id INTEGER NOT NULL","platform_id INTEGER NOT NULL","FOREIGN KEY (game_id) REFERENCES igdb_games(game_id)","FOREIGN KEY (platform_id) REFERENCES igdb_platforms(platform_id)"},
    4
  };

  database_table igdb_games_table = {
    "igdb_games",
    (char *[]){"id INTEGER PRIMARY KEY","game_name TEXT NOT NULL","cover_url TEXT","description TEXT"},
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

  database db = {"./data/database.db",(database_table []){users_table,games_table,games_backlog_table,games_library_table,games_library_console_table,igdb_games_table,igdb_genres_table,igdb_platforms_table,igdb_games_genre_table,igdb_games_platforms_table},10};
  
  sqlite3 *connection;
  int x = sqlite3_open(db.file_name,&connection);

  init_database(connection,db);

  if(x != SQLITE_OK) {
    printf("ERROR OPENING database!\n");
    sqlite3_close(connection);
    return 0;
  }

  sqlite3_close(connection);

  printf("Closed database!\n");
  
  return 0;
}