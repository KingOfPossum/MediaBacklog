import sqlite3

class Database:
    def __init__(self,table_name:str,schema:str):
        self.table_name = table_name
        self.path = f"data/database.db"

        self.sql_execute(f'CREATE TABLE IF NOT EXISTS {table_name} ({schema})')

    def sql_execute(self, query:str, params:tuple = ()):
        with sqlite3.connect(self.path) as connection:
            cursor = connection.cursor()
            cursor.execute(query, params)

    def sql_execute_fetchall(self, query:str, params:tuple = ()) -> list:
        with sqlite3.connect(self.path) as connection:
            cursor = connection.cursor()
            cursor.execute(query, params)
            data = cursor.fetchall()
        return data

    def sql_execute_fetchone(self, query:str, params:tuple = ()) -> tuple:
        with sqlite3.connect(self.path) as connection:
            cursor = connection.cursor()
            cursor.execute(query, params)
            data = cursor.fetchone()
        return data