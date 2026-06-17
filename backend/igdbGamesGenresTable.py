from database import Database

class IgdbGamesGenresTable(Database):
    def __init__(self):
        schema = """
        game_id INTEGER NOT NULL,
        genre_id INTEGER NOT NULL,
        FOREIGN KEY (game_id) REFERENCES igdb_games(id),
        FOREIGN KEY (genre_id) REFERENCES igdb_genres(id)
        """

        super().__init__(table_name="igdb_games_genres",schema=schema)