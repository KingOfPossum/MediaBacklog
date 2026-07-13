from backend.GameLibraryEntry import GameLibraryEntry
from backend.tables.database import Database

class GamesLibraryTable(Database):
    def __init__(self):
        schema = """
        id INTEGER PRIMARY KEY,
        user_id INTEGER NOT NULL,
        game_id INTEGER NOT NULL,
        console TEXT NOT NULL,
        status TEXT NOT NULL,
        FOREIGN KEY (game_id) REFERENCES games(id)
        """

        super().__init__(table_name="games_library",schema=schema)

    def add_game(self, user_id:int, game_id:int, console:str, status:str):
        query = f"""
        INSERT INTO {self.table_name}(user_id,game_id,console,status)
        VALUES (?,?,?,?)
        RETURNING id
        """

        return self.sql_execute_fetchone(query,(user_id,game_id,console,status))[0]

    def get_library_id(self,user_id:int, game_id: int) -> int | None:
        query = f"""
        SELECT id
        FROM {self.table_name}
        WHERE user_id=? AND game_id=?
        """

        library_id = self.sql_execute_fetchone(query,(user_id,game_id))
        return library_id[0] if library_id else None

    def get_all_games(self) -> list[GameLibraryEntry]:
        query = f"""
        SELECT *
        FROM {self.table_name}
        """

        result = self.sql_execute_fetchall(query)
        return [GameLibraryEntry(*game) for game in result]