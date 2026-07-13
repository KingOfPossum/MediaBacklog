from backend.tables.database import Database

class IgdbGamesGenresTable(Database):
    def __init__(self):
        schema = """
        game_id INTEGER NOT NULL,
        genre_id INTEGER NOT NULL,
        FOREIGN KEY (game_id) REFERENCES igdb_games(id),
        FOREIGN KEY (genre_id) REFERENCES igdb_genres(id)
        """

        super().__init__(table_name="igdb_games_genres",schema=schema)

    def add_entry(self,game_id: int, genre_id: int):
        query = f"""
        INSERT INTO {self.table_name}
        VALUES (?,?)
        """

        self.sql_execute(query,(game_id,genre_id))

    def get_genres_for_game(self,game_id: int) -> list[int]:
        query = f"""
        SELECT genre_id
        FROM {self.table_name}
        WHERE game_id=?
        """

        return [genre[0] for genre in self.sql_execute_fetchall(query,(game_id,))]