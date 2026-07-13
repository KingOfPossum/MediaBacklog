from backend.tables.database import Database

class IgdbGamesPlatformsTable(Database):
    def __init__(self):
        schema = """
        game_id INTEGER NOT NULL,
        platform_id INTEGER NOT NULL,
        FOREIGN KEY (game_id) REFERENCES igdb_games(game_id),
        FOREIGN KEY (platform_id) REFERENCES igdb_platforms(platform_id)
        """

        super().__init__(table_name="igdb_games_platforms", schema=schema)

    def add_entry(self,game_id: int, platform_id: int):
        query = f"""
        INSERT INTO {self.table_name}
        VALUES (?,?)
        """

        self.sql_execute(query,(game_id,platform_id))

    def get_platforms_for_game(self,game_id: int) -> list[int]:
        query = f"""
        SELECT platform_id
        FROM {self.table_name}
        WHERE game_id=?
        """

        return [platform[0] for platform in self.sql_execute_fetchall(query,(game_id,))]