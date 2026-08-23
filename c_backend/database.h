#ifndef DATABASE_H
#define DATABASE_H

#include "sqlite3.h"

typedef struct database_table {
  char *table_name;
  char **columns;
  int num_columns;
} database_table;

typedef struct database {
  char *file_name;
  database_table *tables;
  int num_tables;
} database;

typedef struct {
  char **data;
  int num_values;
} single_result;

typedef struct {
  single_result *all_results;
  int num_results;
  char **column_names;
} select_result;

void init_database_tables(sqlite3 *connection, database db);
void create_table(sqlite3 *connection, database_table table);
void insert(sqlite3 *connection, database_table table, char **columns, int num_columns, char **values, int num_values);
select_result *select_sql(sqlite3 *connection, database_table table,char **columns,int num_columns,char *where, char **params, int num_params);
void free_select_results(select_result *results);
void print_select_results(select_result results);

#endif
