#include "database.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

sqlite3 *open_connection(char *path) {
  sqlite3 *connection;
  int rc = sqlite3_open(path,&connection);

  if(rc != SQLITE_OK) {
    printf("Error while opening database\n");
    sqlite3_close(connection);
    return NULL;
  }
  
  return connection;
}

void init_database_tables(sqlite3 *connection, database db) {
  for(int i = 0;i < db.num_tables;i++) {
    create_table(connection,db.tables[i]);
  }
}

void create_table(sqlite3 *connection, database_table table) {
  // Create the command for creating the table
  char create_cmd[512] = "CREATE TABLE IF NOT EXISTS ";
  strcat(create_cmd, table.table_name);
  strcat(create_cmd, " (");

  for(int i = 0;i < table.num_columns; i++) {
    strcat(create_cmd, table.columns[i]);

    if(i < table.num_columns - 1) {
      strcat(create_cmd, ", ");
    }
  }

  strcat(create_cmd, ");");

  printf("Created create cmd: %s\n",create_cmd);

  // Execute the create table command
  char *err_msg;
  int result = sqlite3_exec(connection, create_cmd, 0, 0, &err_msg);

  if(result != 0) {
    printf("ERROR CREATING TABLE: %s\n", err_msg);
    sqlite3_free(err_msg);
  }
}

void insert_sql(sqlite3 *connection, database_table table, char **columns, int num_columns, char **values, int num_values) {
  // Check if the number of values given is a multiple of the number of columns we want to insert
  if(num_values % num_columns != 0) {
    printf("ERROR: Incorrect number of values\n");
    return;
  }

  // Pack all the individual insertions into an transaction for better performance
  sqlite3_exec(connection,"BEGIN TRANSACTION;",0,0,0);

  // Create the insert cmd
  char insert_cmd[512] = "INSERT INTO ";
  strcat(insert_cmd, table.table_name);
  
  strcat(insert_cmd, " (");
  for(int i = 0;i < num_columns; i++) {
    strcat(insert_cmd,columns[i]);
    if(i < num_columns - 1) {
      strcat(insert_cmd, ", ");
    }
  }
  strcat(insert_cmd,") VALUES (");

  for(int i = 0; i < num_columns; i++) {
    strcat(insert_cmd,"?");
    if(i < num_columns - 1) {
      strcat(insert_cmd, ", ");
    }
  }
  strcat(insert_cmd, ");");

  printf("Compiled statement: %s\n", insert_cmd);

  // Compile the cmd
  sqlite3_stmt *stmt;
  int x = sqlite3_prepare_v2(connection,insert_cmd,-1,&stmt,0);

  if(x != SQLITE_OK){
    printf("ERROR PREPARE: %s\n",sqlite3_errmsg(connection));
    return;
  }

  int num_rows = num_values / num_columns;

  // Insert all rows individually
  for(int row = 0; row < num_rows; row++) {
    for(int col = 0; col < num_columns; col++) {
      int index = (row * num_columns) + col;

      // Fill the ?'s in the cmd with actual value
      sqlite3_bind_text(stmt,col + 1,values[index],-1,SQLITE_TRANSIENT);
    }

    // Execute the insert cmd for current row
    x = sqlite3_step(stmt);
    if(x != SQLITE_DONE) {
      printf("ERROR EXECUTE (row : %d): %s\n",row,sqlite3_errmsg(connection));
    }

    // Reset statement for next row
    sqlite3_reset(stmt);
  }

  // End transaction
  sqlite3_exec(connection,"COMMIT;",0,0,0);

  // Free memory of statement
  sqlite3_finalize(stmt);
}

select_result *select_sql(sqlite3 *connection, database_table table,char **columns,int num_columns,char *where, char **params, int num_params) {
  // build the select cmd
  char select_cmd[512] = "SELECT ";
  
  // if columns is NULL or num_columns is 0 we select *
  if(columns == NULL || num_columns == 0) {
    strcat(select_cmd,"*");
  }
  // else select the given colums
  else {
    for(int i = 0;i < num_columns;i++) {
      strcat(select_cmd,columns[i]);
      if(i < num_columns-1) {
        strcat(select_cmd,",");
      }
    }
  }

  strcat(select_cmd," FROM ");

  strcat(select_cmd,table.table_name);

  // insert the where clause if given
  if(where != NULL) {
    strcat(select_cmd," WHERE ");
    strcat(select_cmd,where);
  }

  strcat(select_cmd,";");

  // compile the command
  sqlite3_stmt *stmt;
  int x = sqlite3_prepare_v2(connection,select_cmd,-1,&stmt,0);
  
  printf("Compiled statement: %s\n",select_cmd);

  if(x != SQLITE_OK){
    printf("ERROR PREPARE: %s\n",sqlite3_errmsg(connection));
    return NULL;
  }

  // fill the ?'s in the where clause
  for(int i = 0; i < num_params;i++) {
    sqlite3_bind_text(stmt,i+1,params[i],-1,SQLITE_TRANSIENT);
  }

  // initialize results
  select_result *results = malloc(sizeof(select_result));
  results->num_results = 0;
  results->all_results = NULL;
  results->column_names = columns;

  // number of columns returned
  int returned_cols = sqlite3_column_count(stmt);

  // parse the results into the select_results struct
  while(sqlite3_step(stmt) == SQLITE_ROW) {
    results->num_results++;
    results->all_results = realloc(results->all_results,results->num_results * sizeof(single_result));

    results->all_results[results->num_results-1].num_values = returned_cols;
    results->all_results[results->num_results-1].data = malloc(sizeof(char *) * returned_cols);

    for(int i = 0; i < returned_cols;i++) {
      const char *val = (const char *)sqlite3_column_text(stmt,i);

      if(val != NULL) {
        results->all_results[results->num_results-1].data[i] = strdup(val);
      }
      else{
        results->all_results[results->num_results-1].data[i] = strdup("NULL");
      }
    }
  }

  // free memory of the statement
  sqlite3_finalize(stmt);

  return results;
}

void free_select_results(select_result *results) {
  if(results == NULL) {
    return;
  }

  for(int i = 0;i < results->num_results;i++) {
    for(int j = 0;j < results->all_results[i].num_values;j++) {
      free(results->all_results[i].data[j]);
    }
    free(results->all_results[i].data);
  }
  free(results->all_results);
  free(results);
}

void print_select_results(select_result results) {
  printf("Results:\n");
  printf("  Number of Results: %d\n",results.num_results);
  
  for(int i = 0;i < results.num_results;i++) {
    printf("  [");
    for(int j = 0;j < results.all_results[i].num_values;j++){
      printf("%s=%s",results.column_names[j],results.all_results[i].data[j]);
      if(j < results.all_results[i].num_values-1) {
        printf(", ");
      }
    }
    printf("]\n");
  }
}