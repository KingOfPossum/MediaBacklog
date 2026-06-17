from database import Database

class IgdbGamesTable(Database):
    def __init__(self):
        schema = """
        id INTEGER PRIMARY KEY,
        cover_url TEXT,
        description TEXT
        """

        super().__init__(table_name="igdb_games",schema=schema)