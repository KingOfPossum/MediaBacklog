from backend.tables.database import Database

class GamesLibraryConsoleTable(Database):
    def __init__(self):
        schema = """
        library_id INTEGER KEY,
        console TEXT KEY,
        PRIMARY KEY(library_id,console)
        """

        super().__init__(table_name="games_library_console",schema=schema)

    def add_entry(self,library_id: int, console:str):
        query = f"""
        INSERT INTO {self.table_name}
        VALUES (?,?)
        """

        self.sql_execute(query,(library_id,console))

    def get_all_consoles_for_game(self,library_id: int):
        query = f"""
        SELECT console
        FROM {self.table_name}
        WHERE library_id=?
        """

        return self.sql_execute_fetchall(query,(library_id,))

    def check_if_entry_already_exists(self,library_id: int, console: str) -> bool:
        query = f"""
        SELECT *
        FROM {self.table_name}
        WHERE library_id=? AND console=?
        """

        return self.sql_execute_fetchone(query,(library_id,console)) is not None