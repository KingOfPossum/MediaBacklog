from backend.tables.database import Database

class GamesLibraryTable(Database):
    def __init__(self):
        schema = """
        id INTEGER PRIMARY KEY,
        user_id INTEGER NOT NULL,
        game_id INTEGER NOT NULL,
        status TEXT NOT NULL
        """

        super().__init__(table_name="games_library",schema=schema)