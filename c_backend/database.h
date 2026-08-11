#ifndef DATABASE_H
#define DATABASE_H

#include "sqlite3.h"

typedef struct database_table {
  char *table_name;
  char **params;
  int num_params;
} database_table;

typedef struct database {
  char *file_name;
  database_table *tables;
  int num_tables;
} database;

void create_table(sqlite3 *db, database_table table);
void insert(sqlite3 *db, database_table table, char **columns, int num_columns, char **values, int num_values);
void select(sqlite3 *db, database_table table);

#endif
