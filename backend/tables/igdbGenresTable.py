from backend.tables.database import Database

class IgdbGenresTable(Database):
    def __init__(self):
        schema = """
        id INTEGER PRIMARY KEY,
        genre_name TEXT NOT NULL
        """

        super().__init__(table_name="igdb_genres",schema=schema)