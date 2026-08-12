#include "database.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  database_table games_table = {"games", (char *[]){"id INTEGER PRIMARY KEY","name TEXT NOT NULL","cover_url TEXT"}, 3};
  database_table genres_table = {"genres", (char *[]){"id INTEGER PRIMARY KEY","genre_name TEXT NOT NULL"}, 2};
  
  database db = {"database.db",(database_table []){games_table,genres_table},2};
  
  sqlite3 *connection;

  int x = sqlite3_open(db.file_name,&connection);

  if(x != SQLITE_OK) {
    printf("ERROR OPENING database!\n");
    sqlite3_close(connection);
    return 0;
  }

  printf("Opened database!\n");

  for(int i = 0; i < db.num_tables; i++) {
    create_table(connection, db.tables[i]);
  }
  
  insert(connection, games_table, (char *[]){"name","cover_url"}, 2, (char *[]) {"Metroid Prime","'asdsad'","Metroid Fusion","'sdad'","Mario 64","'asdadsadasda'"},6);

  printf("Inserted data!\n");

  select_result *results = select_sql(connection,games_table);

  for(int i = 0;i < results->num_results; i++) {
    printf("[");
    for(int j = 0;j < results->all_results[i].num_values; j++) {
      printf(results->all_results[i].data[j]);

      if(j < results->all_results[i].num_values-1) {
        printf(",");
      }
    }
    printf("]\n");
    if(i < results->num_results-1) {
      printf(",");
    }
  }

  free_select_results(results);

  sqlite3_close(connection);

  printf("Closed database!\n");
  
  getchar();

  return 0;
}