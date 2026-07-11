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