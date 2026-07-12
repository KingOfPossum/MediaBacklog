from Game import Game
from backend.tables.database import Database

class IgdbGamesTable(Database):
    def __init__(self):
        schema = """
        id INTEGER PRIMARY KEY,
        game_name TEXT NOT NULL,
        cover_url TEXT,
        description TEXT
        """

        super().__init__(table_name="igdb_games",schema=schema)

    def add_game(self,game:Game):
        query = f"""
        INSERT INTO {self.table_name}
        VALUES (?,?,?,?)
        """

        self.sql_execute(query,(game.id,game.name,game.cover,game.summary[0]))

    def check_if_game_exists(self,igdb_id: int) -> int:
        query = f"""
        SELECT *
        FROM {self.table_name}
        WHERE id=?
        """

        id = self.sql_execute_fetchone(query,(igdb_id,))

        if id is None:
            return False
        return True