#include "database.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void create_table(sqlite3 *db, database_table table) {
  // Create the command for creating the table
  char create_cmd[512] = "CREATE TABLE IF NOT EXISTS ";
  strcat(create_cmd, table.table_name);
  strcat(create_cmd, " (");

  for(int i = 0;i < table.num_params; i++) {
    strcat(create_cmd, table.params[i]);

    if(i < table.num_params - 1) {
      strcat(create_cmd, ", ");
    }
  }

  strcat(create_cmd, ");");

  printf("Created Table %s!\n",table.table_name);

  // Execute the create table command
  char *err_msg;
  int result = sqlite3_exec(db, create_cmd, 0, 0, &err_msg);

  if(result != 0) {
    printf("ERROR CREATING TABLE: %s\n", err_msg);
    sqlite3_free(err_msg);
  }
}

void insert(sqlite3 *db, database_table table, char **columns, int num_columns, char **values, int num_values) {
  // Check if the number of values given is a multiple of the number of columns we want to insert
  if(num_values % num_columns != 0) {
    printf("ERROR: Incorrect number of values\n");
    return;
  }

  // Pack all the individual insertions into an transaction for better performance
  sqlite3_exec(db,"BEGIN TRANSACTION;",0,0,0);

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
  int x = sqlite3_prepare_v2(db,insert_cmd,-1,&stmt,0);

  if(x != SQLITE_OK){
    printf("ERROR PREPARE: %s\n",sqlite3_errmsg(db));
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
      printf("ERROR EXECUTE (row : %d): %s\n",row,sqlite3_errmsg(db));
    }

    // Reset statement for next row
    sqlite3_reset(stmt);
  }

  // End transaction
  sqlite3_exec(db,"COMMIT;",0,0,0);

  // Free memory of statement
  sqlite3_finalize(stmt);
}

static int callback(void *data, int argc, char **argv, char **azColName) {
  select_result *results = (select_result *)data;
  results->num_results++;

  // Allocate memory for the current row
  results->all_results = realloc(results->all_results,sizeof(single_result) * results->num_results);

  results->all_results[results->num_results-1].num_values = argc;
  // Allocate memory for all values received for the current row
  results->all_results[results->num_results-1].data = malloc(sizeof(char *) * argc);

  for(int i = 0; i < argc; i++) {
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    
    if(argv[i] != NULL) {
      results->all_results[results->num_results-1].data[i] = strdup(argv[i]);
    }
    else {
      results->all_results[results->num_results-1].data[i] = strdup("NULL");
    }
  }
  printf("\n");
  return 0;
}

select_result *select_sql(sqlite3 *db, database_table table) {
  char select_cmd[512] = "SELECT * FROM ";
  strcat(select_cmd,table.table_name);
  strcat(select_cmd,";");
  
  select_result *results = malloc(sizeof(select_result));
  results->num_results = 0;
  results->all_results = malloc(sizeof(single_result));

  sqlite3_exec(db,select_cmd,callback,results,0);

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