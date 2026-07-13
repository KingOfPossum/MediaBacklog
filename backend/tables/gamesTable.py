from backend.GameEntry import GameEntry
from backend.tables.database import Database

class GamesTable(Database):
    def __init__(self):
        schema  = """
        id INTEGER PRIMARY KEY,
        igdb_id INTEGER,
        name TEXT NOT NULL,
        game_length INTEGER,
        min_price INTEGER,
        avg_price INTEGER,
        max_price INTEGER
        """

        super().__init__(table_name="games",schema=schema)

    def add_game(self,game_name: str, igdb_id: int) -> int:
        query = f"""
        INSERT INTO {self.table_name}(igdb_id, name)
        VALUES (?,?)
        RETURNING id
        """

        return self.sql_execute_fetchone(query,(igdb_id,game_name))[0]

    def get_game_id(self,game_name:str) -> int | None:
        query = f"""
        SELECT id 
        FROM {self.table_name}
        WHERE name=?
        """

        id = self.sql_execute_fetchone(query,(game_name,))
        return id[0] if id else None

    def get_game(self,game_id: int) -> GameEntry:
        query = f"""
        SELECT *
        FROM {self.table_name}
        WHERE id=?
        """

        return GameEntry(*self.sql_execute_fetchone(query,(game_id,)))